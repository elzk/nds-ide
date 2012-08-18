###############################################################################
# IMPORTS
###############################################################################
import re#, sys#, tty#, termios
import threading, time #ReadingThread
from errors import IPCError

import ipc
from device import Device
from scheduler import Event 
from const import *
from signal import SIGALRM, SIGUSR1, SIGUSR2

###############################################################################
# READING THREAD CLASS
###############################################################################
class ReadingThread(threading.Thread):
  """
  Reading thread block on fifo access while fifo is empty.
  When fifo is not empty, reading thread add an event to the
  ecu. Next, ecu add an event to the good device one, and scheduler
  wake up the device. The scheduler give modifiedRegisters mask to
  event too.
  """

  def __init__(self, ipc, ecu):
    """
    Constructor.
    @param ipc ipc structure to access fifo with ipc_mod module
    @ecu ecu to add event.
    """
    threading.Thread.__init__(self)
    self.__ipc = ipc
    self.__ecu = ecu

    self.__run = True

  def run(self):
    """
    The threaded function.
    Wait that fifo is not empty.
    """
    
    """ Get modified device and reg """
    modified = ipc.tpl_ipc_pop_fifo(self.__ipc);
    
    while self.__run:
      """ Call Ecu event handler """
      self.__ecu.event(int(modified.dev), modified.reg_mask, modified.time)
      
      """ Get modified device and reg """
      modified = ipc.tpl_ipc_pop_fifo(self.__ipc);
            
  def kill(self):
    """
    Call to stop the main loop of the reading thread.
    """
    self.__run = False
    """ Set the fifo semaphore free to end the reading thread loop """
    ipc.tpl_sem_post_fifo_full_sem(self.__ipc)

###############################################################################
# ECU CLASS
###############################################################################
class Ecu(object):
  """
  Ecu class.
  Ecu (Engine control unit) represent devices and an operating system.
  """  
  def __init__(self, osPath, scheduler, devices = None):
    """
    Constructor.
    @osPath trampoline os executable to use with this Ecu.
    @param devices (optionnal) devices list to use in Ecu.
    """
    """ Copy """
    self.__osPath = osPath
    self.__scheduler = scheduler
    self.__dir = re.match(r'.*/+', osPath).group() #get dir from osPath
    self._dir = self.__dir
        
    """ Init """
    self._devices        = {} # Dict
    self.__offset        = ipc.REGISTER_ID_BITS # Last bits are used by registers
    self.__ipc           = None
    self.__readingThread = None # No reading thread if we only generate
    
    self.__id_list          = {}
    self.__id_list[SIGUSR1] = 0
    self.__id_list[SIGUSR2] = 0
    self.__id_list[SIGALRM] = 0
        
    """ Add devices """
    if devices != None:
      self.add(devices)       
        
  def add(self, devices):
    """
    Add devices to the ecu.
    @param devices the devices list used by the Ecu (class Device)
    """
    for device in devices:
      self._devices[device.id] = device
      
  def start(self):
    """
    * Run trampoline process, run reading thread and devices.
    * Initialize Pygame if needed.
    """
    
    """ IPC """
    self.__ipc = ipc.tpl_ipc_create_instance(self.__osPath)
    if not self.__ipc:
      raise IPCError, "You must compile trampoline before run viper 2"  

    """ Init devices """ 
    for name, device in self._devices.iteritems():
      device.setEcu(self)
      device.setScheduler(self.__scheduler)
      device.genLongID(self.__offset)
      device.start()

    """ Init and start reading thread """
    self.__readingThread = ReadingThread(self.__ipc, self)
    self.__readingThread.setDaemon(True) # Can stop script even if thread is running
    self.__readingThread.start()
    
    """ Tell Trampoline, Viper2 initialization is done """
    ipc.tpl_ipc_ready(self.__ipc);
    
    """ Draw Ecu if Display mode == pygame (here and not in __init__ because devices are not yet created in __init__) """
    from config import dispatch_display
    dispatch_display.ecu(self)
  	
  def generate(self):
    """
    Generate the header file use to compile trampolin with the same
    identifiers than viper 2
    """

    """ Open header """
    try:
      cfile = open(self.__dir + "vp_ipc_devices.c", "w")
      header = open(self.__dir + "vp_ipc_devices.h", "w")
      oilFile = open(self.__dir + "target.cfg", "w")
    except IOError:
      print "Can't access to " + self.__dir + "vp_ipc_devices.h"
      print " or " + self.__dir + "vp_ipc_devices.c"
      print " or " + self.__dir + "target.cfg"
      raise IOError, "You should verify dir \""+ self.__dir + "\" exists and it can be writable"

    """ Generate header """
    header.write("#ifndef __VP_DEVICES_H__\n#define __VP_DEVICES_H__\n")
    header.write('\n#include "com.h" /* reg_id_t, dev_id_t */\n')
    cfile.write("/* vp_ipc_devices.c */\n")
    cfile.write('\n#include "com.h" /* reg_id_t, dev_id_t */\n')
    cfile.write('\n#include "vp_ipc_devices.h" /* registers constants */\n')
    oilFile.write("interrupts[32]{\n")
    
    """ Generate device identifier """
    index = 0
    header.write("\n/* Devices */\n")
    cfile.write("\n/* Devices */\n")
    for name, device in self._devices.iteritems(): 
      index += 1
      header.write("#define " + device.name + "_val ((reg_id_t)" + hex(device.id) +  ") << " + str(self.__offset) + "\n")

    header.write("\n");

    for name, device in self._devices.iteritems():
      header.write("extern const reg_id_t " + device.name + ";\n")
      cfile.write("const reg_id_t " + device.name + " = " + device.name + "_val;\n")
      oilFile.write("  " + device.irq + " = " + str(device.callbackIndex) + ";\n")

    """ Generate register identifier """
    header.write("\n/* Registers */\n")
    cfile.write("\n/* Registers */\n")
    for name, device in self._devices.iteritems():
      device.generateRegisters(header, cfile)

    """ Generate matchless registers identifiers """
    header.write("\n/* Completes registers */\n")
    cfile.write("\n/* Completes registers */\n")
    for name, device in self._devices.iteritems():
      device.generate(header, cfile)
     
    """ Generate footer """
    header.write("\n#endif /* __VP_DEVICES_H__ */\n")
    cfile.write("\n/* End of file vp_ipc_devices.c */\n")
    oilFile.write("};\n")

    header.close()
    oilFile.close()

  def sendIt(self, signum, id):
    """
    Send an UNIX interruption (kill).
    @pararm signum must be Device.SIGUSR1, Device.SIGUSR2 or Device.SIGALRM
    @param id the device id
    """
    # TODO : check if id already in the list ?    
    self.__id_list[signum] += id
   
  def launchIt(self):
    """
    Launch interrupts
    Reset lists
    """
    for sig, id in self.__id_list.iteritems():
      if (id != 0):
        ipc.tpl_ipc_send_it(self.__ipc, sig, id, self.__scheduler._verbose)
            
    self.__id_list[SIGUSR1] = 0
    self.__id_list[SIGUSR2] = 0
    self.__id_list[SIGALRM] = 0
     
  def kill(self):
    """
    Stop Reading thread, trampoline and Ecu
    """
        
    """Stop reading thread"""
    if self.__readingThread:
      self.__readingThread.kill()
      # TODO : isAlive useful ???
      if self.__readingThread.isAlive():
        if (self.__scheduler._verbose == True):
          print "Waiting reading thread 0.1 seconde..."
          threading.Event().wait(0.1)
        if self.__readingThread.isAlive():
          if (self.__scheduler._verbose == True):
            print "Reading thread stop not clearly."
          
    """ Stop IPC """
    if self.__ipc:
      ipc.tpl_ipc_destroy_instance(self.__ipc)

  def getIPC(self):
    return self.__ipc

  def getDir(self):
    return self.__dir

  def event(self, deviceID, registersMask, deviceTime):
    """
    Add an event to the scheduler list and wakes up it.
    @param deviceID device id
    @param registerMask register mask (reg_id_t)
    """
    
    deviceID = deviceID >> self.__offset
    if deviceID not in self._devices:
      raise IPCError, str(deviceID) + " is not in devices list !" 

    else:
      """ Find index of modified registers """
      mask  = 1 << self.__offset
      index = self.__offset + 1
      reg = []

      while mask != 0:
        if registersMask & mask != 0:
          reg.append(index)

        mask >>= 1
        index -= 1
        
      
      """ Add event to scheduler """
      self.__scheduler.addEvent(Event(self._devices[deviceID], deviceTime, 0, reg))

  def send(self, sub):
    print "ecu.send() - sub:" + str(sub)
    
  
