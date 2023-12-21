#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import time
import RPi.GPIO as GPIO
from config import *

executing = True

broker = "localhost"
client = mqtt.Client()

def buttonPressedCallback(channel):
    global executing
    executing = False

def process_message(client, userdata, message):
    message_decoded = (str(message.payload.decode("utf-8"))).split(".")

    if message_decoded[0] == "Client connected" or message_decoded[0] == "Client disconnected":
        print(message_decoded[0] + " at: " + message_decoded[1] + " on terminal " + message_decoded[2])
    else:
        print("Card " + message_decoded[0] + " has been read at: " + message_decoded[1] + " on terminal " + message_decoded[2])

def connect_to_broker():
    client.connect(broker)
    client.on_message = process_message
    client.loop_start()
    client.subscribe("card_used/client")


def disconnect_from_broker():
    while executing:
        time.sleep(0.1)
    client.loop_stop()
    client.disconnect()


def run_receiver():
    connect_to_broker()
    disconnect_from_broker()


if __name__ == "__main__":
    GPIO.add_event_detect(buttonRed, GPIO.FALLING, callback=buttonPressedCallback, bouncetime=200)
    run_receiver()
    GPIO.cleanup()