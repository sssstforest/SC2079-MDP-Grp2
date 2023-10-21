import time
import imagezmq
from picamera2 import Picamera2, Preview
import numpy as np
import cv2
import socket
from imutils.video import VideoStream

count=0
sender = imagezmq.ImageSender(connect_to="tcp://192.168.17.15:5555")
rpi_name = socket.gethostname()
cam = Picamera2()
config = cam.create_preview_configuration(main={"size":(1000,1000)})
cam.configure(config)
cam.start()
print("[Image] Start Taking Photo")
time.sleep(1.0)
while count<2:
        time.sleep(2)
        start = time.time()
        image = cam.capture_array()
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        end = time.time()
        print(f"[Image] Finished taking picture and sending photo... time took: {end - start}")
        start = time.time()
        result = sender.send_image(rpi_name, image)
        end = time.time()
        print(f"[Main] Received result: {result}, time took: {end - start}")
        if b'38' in result:
            print("Result is R")
            count += 1
        elif b'39' in result:
            print("Result is L")
            count += 1
        else:
            print("Nothing Detected!")
cam.close()


