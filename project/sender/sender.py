sender.py
#!/usr/bin/env python3

from cmath import log
import paho.mqtt.client as mqtt
from datetime import datetime
import time
import RPi.GPIO as GPIO
from config import *
from mfrc522 import MFRC522
import neopixel
import board
from config import *
import time
from PIL import Image, ImageDraw, ImageFont
import lib.oled.SSD1331 as SSD1331


pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0/32, auto_write=False)
active_pixels = [False for _ in range(0, len(pixels))]

current_pixel = 0
disp = SSD1331.SSD1331()
disp.Init()
disp.clear()

pixel_on = (75, 255, 100)
pixel_off = (0, 0, 0)
pixel_highlighted = (100, 200, 255)

isWorking = True




executing = True
isUnlocked = True
scan_counter = 0

terminal_id = "T0"
broker = "10.108.33.129"

client = mqtt.Client()

def call_worker(card_number, time):
    client.publish("card_used/client", str(card_number) + "." + str(time) + "." + str(terminal_id))

def connect_to_broker():
    client.connect(broker)
    call_worker("Client connected", str(datetime.now()))

def disconnect_from_broker():
    call_worker("Client disconnected", str(datetime.now()))
    client.disconnect()


def buttonPressedCallback(channel):
    global executing
    executing = False

def buzzer_state(state):
    GPIO.output(buzzerPin, state) 

def buzzer():
    buzzer_state(GPIO.LOW)
    time.sleep(0.25)
    buzzer_state(GPIO.HIGH)

def blink():
    GPIO.output(led1, GPIO.HIGH)
    time.sleep(0.25)
    GPIO.output(led1, GPIO.LOW)

def rfidRead():
    global executing
    global isWorking
    global isUnlocked
    global scan_counter
    MIFAREReader = MFRC522()
    last_scan = datetime.timestamp(datetime.now()) 
    prev_card = None

    while executing:
        
        scan_counter += 1
        
        if datetime.timestamp(datetime.now()) - last_scan > 1.0:
            (status, TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)
            if status == MIFAREReader.MI_OK:
                scan_counter = 0
                (status, uid) = MIFAREReader.MFRC522_Anticoll()
                if status == MIFAREReader.MI_OK and isUnlocked:
                    dt = datetime.now()
                    num = 0
                    for i in range(0, len(uid)):
                        num += uid[i] << (i*8)

                    # call_worker(num, dt)
                    if prev_card != None and prev_card == num:
                        if isWorking:
                            punishment()
                        else:
                            buzzer()
                            call_worker(num, dt)
                            last_scan = datetime.timestamp(datetime.now())
                            isUnlocked = False
                            prev_card = None
                            end_work()
                    elif prev_card == None:
                        buzzer()
                        blink()
                        call_worker(num, dt)
                        last_scan = datetime.timestamp(datetime.now())
                        isUnlocked = False
                        prev_card = num
                        work()

            elif scan_counter%2 == 0:
                isUnlocked = True

def run_sender():
    connect_to_broker()
    rfidRead()
    disconnect_from_broker()
       
def restore_pixel_illumination(pixel):
    global active_pixels
    global pixel_off
    global pixel_on
    
    if (active_pixels[pixel]):
        pixels[pixel] = pixel_on
    else:
        pixels[pixel] = pixel_off
    pixels.show()

def encoderTurnedCallback(channel):
    global current_pixel
    global pixels
    global pixel_highlighted

    if(GPIO.input(encoderRight) == 0):
        if (current_pixel + 1 < len(active_pixels)):
            restore_pixel_illumination(current_pixel)
            current_pixel += 1
    else:
        if(current_pixel > 0):
            restore_pixel_illumination(current_pixel)
            current_pixel -= 1
    
    pixels[current_pixel] = pixel_highlighted    
    pixels.show()

def greenButtonPressedCallback(channel):
    global active_pixels
    global current_pixel
    global disp
    global pixels
    global pixel_highlighted
    global isWorking

    active_pixels[current_pixel] = not active_pixels[current_pixel]        
    restore_pixel_illumination(current_pixel)
    current_pixel = 0
    pixels[current_pixel] = pixel_highlighted
    pixels.show()
    work_end = True
    for pixel_is_on in active_pixels:
        if (not pixel_is_on):
            work_end = False
    
    if (work_end):
        isWorking = False
        disp.ShowImage(Image.open('happyman.jpg').resize((disp.width, disp.height)), 0, 0)

def punishment():
    global disp
    image = Image.new("RGB", (disp.width, disp.height), "WHITE")
    draw = ImageDraw.Draw(image)
    gunman = Image.open('gunman.jpg').resize((disp.width, disp.height))
    image.paste(gunman, (0, 0))
    disp.ShowImage(image, 0, 0)

    GPIO.output(buzzerPin, GPIO.LOW)
    time.sleep(3)
    GPIO.output(buzzerPin, GPIO.HIGH)

def work():
    global pixels
    global isWorking

    isWorking = True
    pixels[current_pixel] = pixel_highlighted
    pixels.show()
 
def end_work():
    global active_pixels
    global disp

    active_pixels = [False for _ in range(0, len(pixels))]
    disp.clear()
    pixels.fill(pixel_off)
    pixels.show()

GPIO.add_event_detect(buttonGreen, GPIO.FALLING, callback=greenButtonPressedCallback, bouncetime=200)
GPIO.add_event_detect(encoderLeft, GPIO.FALLING, callback=encoderTurnedCallback, bouncetime=40)



if __name__ == "__main__":
    GPIO.add_event_detect(buttonRed, GPIO.FALLING, callback=buttonPressedCallback, bouncetime=200)
    run_sender()
    GPIO.cleanup() 




