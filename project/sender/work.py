import neopixel
import board
from config import *
import time
from PIL import Image, ImageDraw, ImageFont

import lib.oled.SSD1331 as SSD1331

pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0/32, auto_write=False)
active_pixels = [False for _ in len(pixels)]

current_pixel = 0
disp = SSD1331.SSD1331()
disp.Init()
disp.clear()

pixel_on = (75, 255, 100)
pixel_off = (0, 0, 0)
pixel_highlighted = (100, 200, 255)

isWorking = False

def restore_pixel_illumination(pixel):
    global active_pixels
    global pixel_off
    global pixel_on
    
    if (active_pixels[pixel]):
        pixels[pixel] = pixel_on
    else:
        pixels[pixel] = pixel_off
    

def encoderTurnedCallback(channel):
    global current_pixel
    global pixels
    global active_pixels
    global pixel_on
    global pixel_off
    
    if(encoderRight == 0):
        if (current_pixel + 1 < len(pixels)):
            restore_pixel_illumination(current_pixel)
            current_pixel += 1
    else:
        if(current_pixel > 0):
            restore_pixel_illumination(current_pixel)
            current_pixel -= 1
    

    # if (active_pixels[current_pixel]):
    #     pixels[current_pixel] = pixel_off
    # else:
    #     pixels[current_pixel] = pixel_highlighted
    pixels[current_pixel] = pixel_highlighted    

def greenButtonPressedCallback(channel):
    active_pixels[current_pixel] != active_pixels[current_pixel]        
    restore_pixel_illumination(current_pixel)
    current_pixel = 0
    pixels[current_pixel] = pixel_highlighted
    work_end = True
    for pixel_is_on in active_pixels:
        if (not pixel_is_on):
            work_end = False
    
    if (work_end):
        end_work()
        disp.ShowImage(Image.open('happyman.jpg').resize((disp.width, disp.height)), 0, 0)

def punishment():
    disp.ShowImage(Image.open('gunman.jpg').resize((disp.width, disp.height)), 0, 0)

    GPIO.output(buzzerPin, GPIO.LOW)
    time.sleep(3)
    GPIO.output(buzzerPin, GPIO.HIGH)

def work():
    global isWorking
    isWorking = True
    GPIO.add_event_detect(buttonGreen, GPIO.FALLING, callback=greenButtonPressedCallback, bouncetime=200)
    GPIO.add_event_detect(encoderLeft, GPIO.FALLING, callback=encoderTurnedCallback, bouncetime=200)
    pixels[current_pixel] = pixel_highlighted
        
def end_work():
    global isWorking
    isWorking = False
    GPIO.remove_event_detect(buttonGreen)
    GPIO.remove_event_detect(encoderLeft)
