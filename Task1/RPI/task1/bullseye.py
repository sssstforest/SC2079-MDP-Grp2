import socket
from picamera2 import Picamera2, Preview
import imagezmq
import time
import sys
import queue
import numpy as np
import cv2

from serialapi import SerialAPI
from imageapi import ImageAPI

#code for moving around the bullseye
if __name__ == '__main__':
    ic = ImageAPI()
    time.sleep(2)
    serialapi = SerialAPI()
    serialapi.connect()
    while True:
        command = input("Execute Image Capturing: ")
        if command == "yes":
            image = ic.rpiTakePicture()
            imageID = ic.sendPictureToServer(image)
            print("Image ID:", imageID)
            if imageID == "N":
                print("no detection result")
            elif imageID == "00":
                serialapi.write("a".encode('utf-8'))
                print("Sent", command)
            
        elif command == "exit":
            ic.camera.close()
            print("Exiting taking picture")
            print("Closing Serial Connection")
            serialapi.serial_connection.close()
            exit()
        elif command == "end":
            ic.sendEmptyImage()
            print()