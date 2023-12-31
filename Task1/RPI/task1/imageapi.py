import socket
from picamera2 import Picamera2, Preview
import imagezmq
import time
import sys
import queue
import numpy as np
import cv2

# to test send id to bluetooth
from bluetoothapi import BluetoothAPI


class ImageAPI:
    HOST = '192.168.17.15'  # jie kai laptop
    # HOST = '192.168.17.30'#sishi laptop
    PORT = '50000'
    READ_BUFFER_SIZE = 1024

    def __init__(self):
        self.client = None
        self.count = 0
        self.camera = Picamera2()
        config = self.camera.create_preview_configuration(
            main={"size": (1000, 1000)})
        self.camera.configure(config)
        self.camera.start()

    def sendImage(self, image):
        print("[Image] Attempting to connect to Image Server...")
        sender = imagezmq.ImageSender(
            connect_to="tcp://"+self.HOST+":"+self.PORT)
        print("[Image] Successfully connected to Image Server: " + str(self.HOST))

        rpi_name = socket.gethostname()
        print('[Image] Sending image to server...')
        reply = sender.send_image(rpi_name, image)
        self.count += 1
        print(f"[Image] We sent picture {self.count}.")
        print(f"[Image] Reply for picture {self.count}: ", reply)
        print(
            f'[Image] Connection with image server closed after picture {self.count}')
        reply = reply.decode('utf-8')
        return reply

    def rpiTakePicture(self):
        while True:
            try:
                print('[Image] Initializing Camera.')
                print('[Image] Taking Picture')
                # self.camera.start_preview(Preview.QTGL)

                time.sleep(2)
                image = self.camera.capture_array()
                image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                print('[Image] Finished taking picture')
                break

            except Exception as exception:
                print("[Image] Sending image to the server failed: " + str(exception))
                time.sleep(1)

        return image

    def imageClose(self):
        self.camera.close()

    def read(self):
        print("[Image] Attempting to read from image server via Wi-Fi...")
        try:
            message = self.client.recv(self.READ_BUFFER_SIZE)
        except Exception as exception:
            print(
                "[Image] Failed to read from image server via Wi-Fi: " + str(exception))
        else:
            if message is not None and len(message) > 0:
                print("[Image] Message read from image server via Wi-Fi:")
                message = message.decode()
                print('[Image] Received: ' + str(message))
                return message


if __name__ == '__main__':
    ic = ImageAPI()
    time.sleep(2)
    while True:
        command = input("Execute Image Capturing: ")
        if command == "yes":
            image = ic.rpiTakePicture()
            imageID = ic.sendImage(image)
            print("Image ID:", imageID)
            if imageID == "N":
                print("no detection result")
        elif command == "exit":
            ic.camera.close()
            print("Exiting")
            exit()
        elif command == "end":
            ic.sendEmptyImage()
            print()

# to test send image id to android
# if __name__ == '__main__':
#     ic = ImageAPI()
#     bt = BluetoothAPI()
#     time.sleep(2)
#     bt.connect()
#     while True:
#         bt.connect()
#         command = input("Execute Image Capturing: ")
#         if command == "yes":
#             image = ic.rpiTakePicture()
#             imageID = ic.sendPictureToServer(image)
#             print("Image ID:", imageID)
#             while (imageID == b"N"):
#                 print("no detection result")
#                 imageID = ic.image()
#             bMsg= "TARGET,1,"+imageID
#             print("[Main] Sending ",bMsg," to Android")
#             failed=bt.write(bMsg.encode('utf-8'))
#             if failed:
#                         print("[Bluetooth] Attempting to reconnect bluetooth")
#                         bt.reconnect_android(bt)
#         elif command == "exit":
#             ic.camera.close()
#             print("exiting")
#             exit()
#         elif command == "end":
#             ic.sendEmptyImage()
#             print()
