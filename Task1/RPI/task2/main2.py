import time
import threading

from bluetoothapi import BluetoothAPI
from serialapi import SerialAPI
import RPi.GPIO as GPIO
import atexit
import socket
import imagezmq
from picamera2 import Picamera2, Preview
import cv2
import numpy as np

class Multithreader:
    
    def __init__(self):
        self.bluetoothapi = BluetoothAPI()
        self.serialapi = SerialAPI()
    
    #Function to start all the threads
    def initialize_processes(self):
        print("[Main] Attempting to initialize multithreader...")
        #Connect the different components
        self.serialapi.connect()
        self.bluetoothapi.connect()

        #Run the multithreading
        self.read_bluetooth_process = threading.Thread(target=self.read_bluetooth)
        self.read_image_process = threading.Thread(target=self.read_image)

        self.read_bluetooth_process.start()
        self.read_image_process.start()
        print("[Main] Initialized multithreader successfully")

        self.read_bluetooth_process.join()
        self.read_image_process.join()

    #Function to take picture, send to the image server and handle result
    def read_image(self):
        global takePic
        global running
        global start
        global setup
        global count
        try:
            if (setup):
                self.cam = Picamera2()
                config = self.cam.create_preview_configuration(main={"size":(1000,1000)})
                self.cam.configure(config)
                self.cam.start()
                print("[Image] Start Camera")
                setup=False
            while start==False and takePic==True:
                    print("[Image] Start taking photo...")
                    time.sleep(2)
                    image = self.cam.capture_array()
                    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                    print("[Image] Finished taking picture and sending photo...")
                    if count == 0:
                        print("STM starting...")
                        self.serialapi.write("S".encode('utf-8'))
                        count += 1
                    else:
                        print("STM continuing...")
                        self.serialapi.write("D".encode('utf-8'))
                    sender = imagezmq.ImageSender(connect_to="tcp://192.168.17.15:5555")#jie kai laptop
                    #sender = imagezmq.ImageSender(connect_to="tcp://192.168.17.30:50000")#sishi laptop
                    rpi_name = socket.gethostname()
                    print("[Image] Connected to Image server...")
                    result = sender.send_image(rpi_name, image)
                    print("[Image] Received result:", result)
                    if b'38' or b'39' in result:
                        takePic = False
                        return result
                    elif b'00' in result:
                        print("It is a bullseye!")
                        continue
                    else: 
                        print("Unable to recognise!")
                        continue
        except Exception as error:
            print("Image Recognition Error!")
            print(error)


    #Function to read messages for bluetooth and stop the function after start is read
    def read_bluetooth(self):
        global takePic
        global running
        global start
        while start:
            message = self.bluetoothapi.read()
            start=False
            if message is not None and len(message) > 0:               
                print("[Main] Message recieved from bluetooth", message)
                try:
                  if b'sp' in message:
                      #start taking image
                      takePic=True
                      print("[Main] Going to Image...")
                      result = self.read_image()
                      #Tell STM to move
                      #self.serialapi.write(("S").encode("utf-8"))
                      time.sleep(2)
                      if b'38' in result:
                        print("Sending R to STM")
                        self.serialapi.write("R".encode("utf-8"))
                      elif b'39' in result:
                        print("Sending L in STM")
                        self.serialapi.write("L".encode("utf-8"))
                      #wait for STM acknowledgement
                      ack = None
                      while ack is None:
                        ack = self.serialapi.read()
                        print("[Main] Received from STM:")
                        print(ack)
                        if b'A' in ack:
                            #start taking image
                            takePic=True
                            print("[Main] Going to Image...")
                            result = self.read_image()
                            #self.serialapi.write(("D").encode("utf-8"))
                            time.sleep(1)
                            if b'38' in result:
                                print("Sending R to STM")
                                self.serialapi.write("R".encode("utf-8"))
                                break
                            elif b'39' in result:
                                print("Sending L in STM")
                                self.serialapi.write("L".encode("utf-8"))
                                break

                except:
                      print("[ERROR] Invalid message from bluetooth")
        exit()
                     
    #Clean up operation after we exit the programme
    def clean_up(self):
        GPIO.cleanup()
        self.cam.close()


if __name__ == "__main__":
    takePic = False
    running = True
    start = True
    setup = True
    count = 0
    currentObs = 1

    #Running the programme
    mt = Multithreader()
    atexit.register(mt.clean_up)

    time.sleep(1)

    mt.initialize_processes()


