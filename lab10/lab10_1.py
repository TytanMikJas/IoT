#!/usr/bin/env python3

from config import *
import board
import busio
import adafruit_bme280.advanced as adafruit_bme280
from PIL import Image, ImageDraw, ImageFont
import lib.oled.SSD1331 as SSD1331

TEMP_IMAGE_PATH = 'temp.png'
HUMID_IMAGE_PATH = 'humid.png'
PRESS_IMAGE_PATH = 'press.png'
FONT_PATH = './lib/oled/Font.ttf'

def bme280_config():
    i2c = busio.I2C(board.SCL, board.SDA)
    bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, 0x76)

    bme280.sea_level_pressure = 1013.25
    bme280.standby_period = adafruit_bme280.STANDBY_TC_500
    bme280.iir_filter = adafruit_bme280.IIR_FILTER_X16
    return bme280

def bme280_read(bme):
    bme.overscan_pressure = adafruit_bme280.OVERSCAN_X16
    bme.overscan_humidity = adafruit_bme280.OVERSCAN_X1
    bme.overscan_temperature = adafruit_bme280.OVERSCAN_X2
    return {"temperature" : round(bme.temperature, 4), "humidity" : round(bme.humidity, 4), "pressure" : round(bme.pressure, 4)}

def display_parameters(parameters):
    image1 = Image.new("RGB", (disp.width, disp.height), "WHITE")
    draw = ImageDraw.Draw(image1)

    text_positions = [(0, 0), (0, 25), (0, 50)]
    images = [image_temperature, image_humidity, image_pressure]
    labels = ['Temperature', 'Humidity', 'Pressure']

    for pos, img, label in zip(text_positions, images, labels):
        image1.paste(img, pos)
        draw.text((17, pos[1]), f'{label}: {parameters[label.lower()]}', font=font_small, fill="BLACK")

    disp.ShowImage(image1, 0, 0)


if __name__ == "__main__":
    print('SCRIPT STARTS...')
    bme1 = bme280_config()

    disp = SSD1331.SSD1331()
    disp.Init()
    disp.clear()

    font_small = ImageFont.truetype(FONT_PATH, 8)
    image_temperature = Image.open(TEMP_IMAGE_PATH).resize((15, 10))
    image_humidity = Image.open(HUMID_IMAGE_PATH).resize((15, 10))
    image_pressure = Image.open(PRESS_IMAGE_PATH).resize((15, 10))
   
    try:
        while True:
            parameters = bme280_read(bme1)
            display_parameters(parameters)
    except KeyboardInterrupt:
        print('Script terminated by user')