#############################
# Constant for pygame
#############################

# Colors
black = (0, 0, 0)
blue = (37, 44, 255)
green = (0, 152, 6)
orange = (255, 101, 39)
red = (248, 34, 0)
white = (255, 255, 255)
yellow = (255, 227, 24)
background_color = 231, 229, 210

# Devices
border_line = 5

# Button
button_width = 100
button_height = 80
button_height_only = 60

# DAC
dac_width = 360
dac_height = 320
max_volt = 140
max_dac = 2
max_time = 350

# LCD
lcd_width = 200
lcd_height = 120

# Server
lcdserver_width = 400
lcdserver_height = 400
robot_size = 4

try:
  import pygame
  from pygame.locals import QUIT, MOUSEBUTTONDOWN, MOUSEBUTTONUP, MOUSEMOTION, KEYDOWN, USEREVENT
  #ENTER = pygame.locals.USEREVENT + 0
  #EXIT = pygame.locals.USEREVENT + 1
  #BLUR = pygame.locals.USEREVENT + 2
  #FOCUS = pygame.locals.USEREVENT + 3
  #CLICK = pygame.locals.USEREVENT + 4
  #CHANGE = pygame.locals.USEREVENT + 5
  #OPEN = pygame.locals.USEREVENT + 6
  #CLOSE = pygame.locals.USEREVENT + 7
except:
  """"""


