###############################################################################
# IMPORTS
##########
# You shall modify devices section only
###############################################################################
#import device # device.SIGUSR1, device.SIGUSR2 and device.SIGALRM
from ecu import Ecu
from scheduler import Scheduler

##
# Devices
##
import timer
from timer import Timer
from motor import Motor
from power import Power
from display import Display

###############################################################################
# DISPLAY
##########
# You shall modify pygame to True or False if you want or not to use pygame (True by default)
# You shall modify screen if you want to change screen dimensions ([800, 600] by default)
###############################################################################
dispatch_display = Display(pygame = False)

###############################################################################
# SCHEDULER
##########
# You shall modify speedCoeff only
###############################################################################
scheduler = Scheduler(speedCoeff = 1)

###############################################################################
# REGISTERS
##########
# Add registers like that : reg = Register("NAME")
###############################################################################
#sensor = Register("SENSOR")
#control = Register("CONTROL")

###############################################################################
# ECUS 
##########
# You should always add registers in same order. Else there is an overlap.
# TODO I want to find out how to add registers with no order constraint
###############################################################################
allEcus = [
  Ecu(
    "../examples/viper2/App-vp2/trampoline",
    scheduler,
    [
      Timer("TIMER0", 7, type = timer.AUTO, delay = 200),
      Motor("MOTOR0", 2),
      Power("POWER", 9)
    ]
  )
]

