#!/usr/bin/env python3

import paho.mqtt.client as mqtt
from datetime import datetime
import time
import RPi.GPIO as GPIO
from config import *
from mfrc522 import MFRC522
from work import *

executing = True
isUnlocked = True
scan_counter = 0

terminal_id = "T0"
broker = "localhost"

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
    time.sleep(1)
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
                    buzzer()
                    blink()

                    if prev_card != None and prev_card == num:
                        if isWorking:
                            punishment()
                        else:
                            call_worker(num, dt)
                            last_scan = datetime.timestamp(datetime.now())
                            isUnlocked = False
                            prev_card = None
                    elif prev_card == None:
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


if __name__ == "__main__":
    GPIO.add_event_detect(buttonRed, GPIO.FALLING, callback=buttonPressedCallback, bouncetime=200)
    run_sender()
    GPIO.cleanup() 