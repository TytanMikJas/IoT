import time
import RPi.GPIO as GPIO
from config import *
from mfrc522 import MFRC522
from datetime import datetime

executing = True

def buttonPressedCallback(channel):
    global executing
    executing = False

def buzzer_state(state):
    GPIO.output(buzzerPin, state) 

def buzzer():
    buzzer_state(GPIO.HIGH)
    time.sleep(0.25)
    buzzer_state(GPIO.LOW)

def blink():
    GPIO.output(led1, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(led1, GPIO.LOW)

def rfidRead():
    global executing
    MIFAREReader = MFRC522()
    last_scan = datetime.timestamp(datetime.now()) 
    while executing:
        
        if datetime.timestamp(datetime.now()) - last_scan > 2.0:
            (status, TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)
            if status == MIFAREReader.MI_OK:
                (status, uid) = MIFAREReader.MFRC522_Anticoll()
                if status == MIFAREReader.MI_OK:
                    dt = datetime.now()
                    num = 0
                    for i in range(0, len(uid)):
                        num += uid[i] << (i*8)
                    print(f"Card read UID: {num}")
                    print(f"Date and time of scanning: {dt}")
                    buzzer()
                    blink()
                    last_scan = datetime.timestamp(datetime.now())


if __name__ == "__main__":
    GPIO.add_event_detect(buttonRed, GPIO.FALLING, callback=buttonPressedCallback, bouncetime=200)
    rfidRead()
    GPIO.cleanup() 