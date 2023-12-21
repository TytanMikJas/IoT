import neopixel
import board
from config import *


pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0/32, auto_write=False)
active_pixels = [False for _ in len(pixels)]
current_pixel = 1 

pixel_on = (255, 50, 50)
pixel_off = (0, 0, 0)

def work_setup():
    ...

def restore_pixel_illumination(pixel):
    if (active_pixels[previous_pixel]):
        pixels[previous_pixel] = pixel_on
    else:
        pixels[previous_pixel] = pixel_off
    

def encoderTurnedCallback(channel):
    if(encoderRight == 0):
        current_pixel = (current_pixel+1)%len(pixels)
        if (current_pixel > 0):
             restore_pixel_illumination( current_pixel - 1)
    else:
        current_pixel = (current_pixel-1)%len(pixels)
        if (current_pixel < len(pixels) ):
            restore_pixel_illumination( current_pixel + 1)

    if (active_pixels[current_pixel]):
        pixels[current_pixel] = pixel_off
    else:
        pixels[current_pixel] = pixel_on
    

def greenButtonPressedCallback(channel):
    active_pixels[current_pixel] != active_pixels[current_pixel]        



def work():
    GPIO.add_event_detect(buttonGreen, GPIO.FALLING, callback=greenButtonPressedCallback, bouncetime=200)
    GPIO.add_event_detect(encoderLeft, GPIO.FALLING, callback=encoderTurnedCallback, bouncetime=200)
    