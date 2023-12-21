#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import time
import sqlite3
import tkinter
import RPi.GPIO as GPIO
from datetime import datetime
from config import *
from project.receiver.create_database import *

executing = True

broker = "localhost"
client = mqtt.Client()

window = tkinter.Tk()

def buttonPressedCallback(channel):
    global executing
    executing = False

def process_message(message):
    message_decoded = (str(message.payload.decode("utf-8"))).split(".")

    if message_decoded[0] == "Client connected" or message_decoded[0] == "Client disconnected":
        print(message_decoded[0] + " at: " + message_decoded[1] + " on terminal " + message_decoded[2])
    else:
        print("Card " + message_decoded[0] + " has been read at: " + message_decoded[1] + " on terminal " + message_decoded[2])
        connention = sqlite3.connect("workers.db")
        cursor = connention.cursor()

        #inserting the log into the database
        cursor.execute("INSERT INTO workers_log VALUES (?,?,?)",
        (message_decoded[1], message_decoded[0], message_decoded[2]))

        #printing the log to the window
        cursor.execute("SELECT * FROM workers_log")
        log_entries = cursor.fetchall()
        labels_log_entry = []
        print_log_window = tkinter.Tk()

        prev_entry = None

        for log_entry in log_entries:

            if prev_entry != None and prev_entry[1] == log_entry[1]:
                time_worked = str(datetime.strptime(log_entry[0], "%Y-%m-%d %H:%M:%S.%f") - datetime.strptime(prev_entry[0], "%Y-%m-%d %H:%M:%S.%f"))
                labels_log_entry.append(tkinter.Label(print_log_window, text=(
                    "Card %s, logged out on the terminal %s at time %s and worked for %s" % (log_entry[1], log_entry[2], log_entry[0], time_worked))))
                prev_entry = None
            else:
                labels_log_entry.append(tkinter.Label(print_log_window, text=(
                    "Card %s, logged on the terminal %s at time %s" % (log_entry[1], log_entry[2], log_entry[0]))))
                prev_entry = log_entry
            
        for label in labels_log_entry:
            label.pack(side="top")

        connention.commit()
        connention.close()

        print_log_window.mainloop()

def create_main_window():
    window.geometry("250x100")
    window.title("Workers logs")
    label = tkinter.Label(window, text="Listening to the MQTT")
    exit_button = tkinter.Button(window, text="Stop", command=window.quit)

    label.pack()
    exit_button.pack(side="right")

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