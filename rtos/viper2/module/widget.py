###############################################################################
# IMPORTS
###############################################################################
from const import *

###############################################################################
# WIDGET CLASS
###############################################################################
class Widget(object):
  """
  Container of all the widgets (and their dimensions in the box list)
  """
  ecu_number = 0
  ecu = 0
  
  def __init__(self):
    """
    Constructor : Initialize pygame and widgets.
    """
    self._widget_box	    = {}
    self._widgets			= {}
    self._actual_widget 	= None
    self._focused_widget  = 0
    self._eventpos = {}
    
    pygame.init()
    from config import dispatch_display
    screen = pygame.display.set_mode((dispatch_display._screen[0], dispatch_display._screen[1]))
    pygame.display.set_caption('ViPER2')
    background = background_color
    screen.fill(background)
    pygame.display.flip()
        
  def event(self, ev):
    """
    * Receiving an event, search if it occurs to a widget and launch widget's action if OK
    @ param ev event received
    """ 
    # TODO : if mousebuttonup.pos == mousebuttondown.pos
    #           -> launch event
    # It will launch the event at the up position unlike now (at down position)
    if (ev.type == MOUSEBUTTONDOWN):
      self._focused_widget = 0
      for id in self._widget_box:
        if (ev.pos[0] > self._widget_box[id][0][0]):
          if (ev.pos[0] < self._widget_box[id][1][0]):
            if (ev.pos[1] > self._widget_box[id][0][1]):
              if (ev.pos[1] < self._widget_box[id][1][1]):
                self._focused_widget = id      
                self._widgets[id].event(ev)
    elif (ev.type == MOUSEBUTTONUP):
      if(self._focused_widget != 0):
        self._widgets[self._focused_widget].event(ev)
    elif (ev.type == KEYDOWN):
      """self._widgets[self._focused_widget].event(ev)"""
                    
  def add(self, widg, device, id, box):
    """
    * Add widget in widget list, increment ecu number
      (because it could happen that two widgets have the same id
      if they are not in the same Ecu. In this case, the widget's
      id is multiply by the Ecu's id)  
    @param widg widget to add
    @param device device to know the widget's Ecu
    @param id widget's id to store the idget in the widget list
    @param box widgets list
    """
    if (self._focused_widget == None):
      self._focused_widget = id
    if (self.ecu != device._ecu):
      self.ecu = device._ecu
      self.ecu_number = self.ecu_number + 1
    
    self._widget_box[id+32*(self.ecu_number-1)] = box
    self._widgets[id+32*(self.ecu_number-1)] 	 = widg
  
  def quit_pygame(self):
    import pygame
    pygame.quit()
    
