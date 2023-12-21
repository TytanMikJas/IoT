#!/usr/bin/env python3
from config import *
import RPi.GPIO as GPIO
import time

intensity = 0
execute = True

encoderLeftPrevoiusState = GPIO.input(encoderLeft)
encoderRightPrevoiusState = GPIO.input(encoderRight)

def buttonPressedCallback(channel):
    global execute
    execute = False
    print("\nButton connected to GPIO " + str(channel) + " pressed.")

def increaseIntensity():
    global intensity
    intensity += 10
    print(f"\nEncoder increased intensity to {intensity}")

def descreateIntensity():
    global intensity
    intensity -= 10
    print(f"\nEncoder decreased intensity to {intensity}")

def encoderTurned(channel):
    global encoderLeftPrevoiusState
    global encoderRightPrevoiusState
    global intensity
    encoderLeftCurrentState = GPIO.input(encoderLeft)
    encoderRightCurrentState = GPIO.input(encoderRight)

    if(encoderLeftPrevoiusState == 1 and encoderLeftCurrentState == 0 and intensity < 100):
        increaseIntensity()
    if(encoderRightPrevoiusState == 1 and encoderRightCurrentState == 0 and intensity > 0):
        descreateIntensity()
    diode1.ChangeDutyCycle(intensity)

    encoderLeftPrevoiusState = encoderLeftCurrentState
    encoderRightPrevoiusState = encoderRightCurrentState


def main():
    while execute:
        time.sleep(0.1)
        #pass
        
    diode1.stop()
    GPIO.cleanup()


if __name__ == '__main__':
    print('Script started...')
    GPIO.add_event_detect(buttonRed, GPIO.FALLING, callback=buttonPressedCallback, bouncetime=200)
    GPIO.add_event_detect(encoderLeft, GPIO.FALLING, callback=encoderTurned, bouncetime=200)
    GPIO.add_event_detect(encoderRight, GPIO.FALLING, callback=encoderTurned, bouncetime=200)

    diode1 = GPIO.PWM(led1, 50)
    diode1.start(0)

    main()
    print('Script finished.')
