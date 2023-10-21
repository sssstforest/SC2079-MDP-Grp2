import multiprocessing
import time
import threading

from bluetoothapi import BluetoothAPI
from ipsocketapi import IPSocketAPI
from serialapi import SerialAPI
from imageapi import ImageAPI
import RPi.GPIO as GPIO
import atexit
import queue

'''Format sent from Algo E.g S___6 (direction, pad,pad,pad,index)'''
'''Initial Android format E.g ALG:8,1,S,0;14,1,W,1;1,2,S,2;'''
#ALG:START

class Multithreader:
    
    def __init__(self):
        self.bluetoothapi = BluetoothAPI()
        self.ipsocketapi = IPSocketAPI()
        self.serialapi = SerialAPI()
        self.imageClientapi = ImageAPI()
        self.write_message_queue = multiprocessing.Queue()
        self.obstacle_id = None
        
    #Function to start all the threads
    def initialize_processes(self):
        global takePictureNow
        global imageQueue
        global imageProcess
        print("[Main] Attempting to initialize multithreader...")
        self.serialapi.connect()
        #Connect the different components
        self.ipsocketapi.connect()
        self.bluetoothapi.connect()

        #Run the multithreading
        self.read_bluetooth_process = threading.Thread(target=self.read_bluetooth)
        self.read_ipsocket_process = threading.Thread(target=self.read_ipsocket)
        self.read_image_process = threading.Thread(target=self.takePicture)
        self.write_process = threading.Thread(target=self.write)
        self.handleIQ_process = threading.Thread(target=self.handleImageQueue)

        self.read_ipsocket_process.start()
        self.read_bluetooth_process.start()
        self.read_image_process.start()
        self.write_process.start()
        self.handleIQ_process.start()
        print("[Main] Initialized multithreader successfully")

        self.read_ipsocket_process.join()
        self.read_bluetooth_process.join()
        self.read_image_process.join()
        self.write_process.join()
        self.handleIQ_process.join()

    #Function to take picture and add to the imageQueue
    def takePicture(self):
        global running
        global takePictureNow
        global imageProcess
        global imageQueue

        while running:
            if takePictureNow == True and imageProcess == True:
                obstacle_id = self.obstacle_id
                print(f"[Image] Taking the picture for {obstacle_id}")
                takenPicture = self.imageClientapi.rpiTakePicture()
                print(f"[Image] Successfully taken the photo for {obstacle_id}")
                imageQueue.put([takenPicture,obstacle_id])
                takePictureNow = False
    #disconnect all/RPI end
    def disconnectall(self):
        global reccedImages
        global numObstacle
        while True:
                    if len(reccedImages) == numObstacle:
                        time.sleep(5)
                        self.imageClientapi.sendEmptyImage()
                        print("[Image] Closing camera")
                        self.imageClientapi.imageClose()
                        print("[Main] Disconnecting from IP Socket")
                        self.ipsocketapi.server.close()
                        print("[Main] Disconnecting from Bluetooth")
                        self.bluetoothapi.server.shutdown(2)
                        self.bluetoothapi.server.close()
                        running = False
                        exit()
                    break
        return

    #Function to send the images to the image server and update the result when it comes back
    def handleImageQueue(self):
        global obstacleCounter
        global reccedImages
        global running
        global imageProcess
        while running and imageProcess==True:
            if not imageQueue.empty():
                print(f"[Main] Current Queue: {imageQueue}")
                currentQ = imageQueue.get()
                takenPicture = currentQ[0]
                obstacle_id = currentQ[1]
                count = 0
                print("[Main] Sending Image to Server")
                image_id = self.imageClientapi.sendPictureToServer(takenPicture)
                image_id = str(image_id)
                print("[Main] Image ID received from server:", image_id)
                iMsg = image_id.encode('utf-8')
                obs=str(obstacle_id)
                try:
                    if (image_id =='N'and count==0): #if the message is invalid, send results to ipsocket
                        print("[Main] Sending the invalid message to ipsocket")
                        iError = (image_id+obs).encode('utf-8')
                        self.ipsocketapi.write(iError)
                        #time.sleep(5)
                        msg=self.ipsocketapi.read()
                        print("[Main] Retrieve instruction from ipsocket")
                        print("message from algo:"+ msg.decode('utf-8'))+"send directly to STM"
                        self.serialapi.write(msg)
                        count+=1
                        continue
                    elif (image_id != '00' and image_id !='N'): #if the message is valid, send results to android
                        print("[Bluetooth] Sending the image results to android")
                        bMsg = "TARGET,"+obs+","+str(image_id)
                        print("[Bluetooth] Message sent to android:",bMsg)
                        # bMsg = self.convert_to_dict('B', bMsg)
                        # self.write_message_queue.put(bMsg)
                        #tell android immediately
                        self.bluetoothapi.write(bMsg)
                        #after recognise image
                        imageProcess=False
                        reccedImages.append(image_id)
                        obstacleCounter -=1
                        print(f"[Main] Number of obstacles left {obstacleCounter}")
                    else:
                        print("Error faced but ignore")
                        imageProcess=False
                except Exception as mistake:
                    print("image recognition error:")
                    print(mistake)
        

    #Function to read messages from the Android tablet
    def read_bluetooth(self):
        global obstacleCounter
        global numObstacle
        global running
        global bluetoothOn
        global firstTime
        global firstMessage

        while running and bluetoothOn:
            message = self.bluetoothapi.read()
            if message is not None and len(message) > 0:               
                print("[Main] Message received from bluetooth", message)
                try:
                  if b'START' in message:
                      firstTime = False
                      #message = firstMessage.decode('utf-8')
                      print("[Main] Starting to dequeue")
                      self.write()

                  elif b'ALG' in message:
                      obstacles = message.decode('utf-8').split(';')
                      print(obstacles)
                      x=0
                      while x<len(obstacles):
                          if len(obstacles[x])<4:
                              obstacles.remove(obstacles[x])
                          else:
                              x+=1
                      #get rid of empty obstacles
                      filteredObstacles= ";".join(obstacles)     
                      filteredObstacles= filteredObstacles+";"            
                      numObstacle = len(obstacles)
                      obstacleCounter=len(obstacles)
                      print(f"[Main] The total number of obstacles is {obstacleCounter}")
                      #message = self.convert_to_dict('I', filteredObstacles.encode('utf-8')) #Forwarding entire string to algo
                      #self.write_message_queue.put(message)
                      print("Sending filtered obstacles directly to algo:" + filteredObstacles)
                      self.ipsocketapi.write(filteredObstacles.encode('utf-8'))
                except Exception as exception:
                      print("[ERROR] Invalid message from bluetooth")
                      print(str(exception))
                
    #Function to read messages from Algorithm
    def read_ipsocket(self):
        global running
        global firstMessage

        while running and firstTime:
            message = self.ipsocketapi.read()
            if message is not None and len(message) > 0:
                n=5
                instr=[message[i:i+n]for i in range(0,len(message),n)]
                for r in instr:
                    if b'P' in r :
                        image_message = self.convert_to_dict('P', r)
                        print("[Main] Queued ", image_message, "to image server")
                        self.write_message_queue.put(image_message)

                    else:
                    #message = message.decode()
                        print("[Main] Queueing message to be sent to STM:",r)
                        stm_message = self.convert_to_dict('S', r)
                        print("[Main] Queued ", stm_message, "to STM")
                        self.write_message_queue.put(stm_message)
                        andr = "COMMAND,"+ (r.decode('utf-8'))
                        andr = andr.encode('utf-8')
                        and_message = self.convert_to_dict('B',andr) 
                        print("[Main] Queued", and_message, "to Android")
                        self.write_message_queue.put(and_message)
            else:
                print("[Main] Invalid command", message ,"read from Algo")
        while running and firstTime==False:
            message = self.ipsocketapi.read()
            #send error correction instruction to STM
            if message is not None and len(message) > 0:
                n=5
                instr=[message[i:i+n]for i in range(0,len(message),n)]
                for r in instr:
                    if b'P' in r :
                        r=r.decode('utf-8')
                        obstacle_id = int(r[-1])-48
                        print("[Main] Obstacle ID:", obstacle_id)
                        self.obstacle_id = obstacle_id
                        print("[Main] Setting take picture now to be true")
                        self.serialapi.write(str(r))
                        takePictureNow = True
                        print("Going to take picture for" + obstacle_id)
                    else:
                        try:
                            print("[Error Correction] Sending ",r," to STM")
                            self.serialapi.write(r)
                            andmsg= "COMMAND,".encode('utf-8')+r
                            self.bluetoothapi.write(andmsg)
                            ack = None
                            while ack is None:
                                ack = self.serialapi.read()
                                print("Received from STM", ack)
                                if  b'A' not in ack:
                                    ack = None
                        except Exception as wrong: 
                            #to show what is being sent
                            print("[Error Correction Error] Sending STM",r)
                            print(wrong)
        

    #Protocol to reconnect with Android tablet
    def reconnect_android(self):
        print("[Main] Attempting to reconnect")
        self.bluetoothapi.disconnect()

        global writeOn
        global bluetoothOn
        writeOn = False
        bluetoothOn = False

        self.bluetoothapi.connect()
        print("[Bluetooth] BT successfully reconnected")

        writeOn = True
        bluetoothOn = True


        # call multiprocess and start
        self.read_android_process = threading.Thread(target=self.read_bluetooth)
        self.read_android_process.start()
        self.read_android_process.join()
        self.write_process = threading.Thread(target=self.write)
        self.write_process.start()
        self.write_process.join()

        print("Reconnected to android...")

    #Function for writing messages to the different components                    
    def write(self):
        global running
        global writeOn
        global firstTime
        global takePictureNow
        global imageProcess
        while running and writeOn and firstTime == False and imageProcess==False:
            try:
                if self.write_message_queue.empty():
                    continue
                if takePictureNow==False:
                    message = self.write_message_queue.get()
                    print(message)
                    header = message["header"]
                    body = message["body"]
                    if header == "B": #Android
                        print("[Main] Sending ",body," to Android")
                        failed = self.bluetoothapi.write(body)
                        if failed:
                            print("[Bluetooth] Attempting to reconnect bluetooth")
                            self.reconnect_android(self)

                    elif header == "I": #Algo
                        print("[Main] Sending", body, "to IpSocket")
                        self.ipsocketapi.write(body)

                    elif header == "P": #imageserver
                        obstacle_id = int(body[-1])-48
                        print("[Main] Obstacle ID:", obstacle_id)
                        self.obstacle_id = obstacle_id
                        print("[Main] Setting take picture now to be true")
                        self.serialapi.write(body)
                        takePictureNow = True
                        print("Going to take picture for" + obstacle_id)

                    elif header == "S": #STM
                        print(f"[Main] STM processing started with {body}")
                        try:
                            print("[Main] Sending ",body," to STM")
                            self.serialapi.write(body)
                            ack = None
                            while ack is None:
                                ack = self.serialapi.read()
                                print("Received from STM", ack)
                                if  b'A' not in ack:
                                    ack = None
                        except Exception as wrong: 
                            #to show what is being sent
                            print("Sending STM", body)
                            print(wrong)
                            
                    else:
                        print("[Main] Invalid header " + str(header))
                    
                    print("[Main] Message sent")
            except Exception as exception:
                print("[Main] Error occurred in write: " + str(exception))
                time.sleep(1)
                
    #Converting to a dictionary to store in the queue
    def convert_to_dict(self, header, body):
        return {"header": header, "body": body}

    #Clean up operation for when the programme is closed midway
    def clean_up(self):
        GPIO.cleanup()


if __name__ == "__main__":
    #Defining global variables
    takePictureNow = False
    imageProcess = False
    imageQueue = queue.Queue(6)
    obstacleCounter = None
    numObstacle = None
    reccedImages = []
    bluetoothOn = True
    writeOn = True
    running = True
    firstTime = True
    firstMessage = ""

    #Running the programme
    mt = Multithreader()
    mt.disconnectall()
    atexit.register(mt.clean_up)

    time.sleep(2)

    mt.initialize_processes()


