import socket
import time


class IPSocketAPI:
    READ_BUFFER_SIZE = 2048

    def __init__(self):
        self.client = None
        self.server = None
        self.client_address = None

    def check_connection(self):
        return self.client is None or self.server is None

    def connect(self):
        while True:
            print("[Algo] Connecting to Algo...", end=" ")
            try:
                self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.server.setsockopt(
                    socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                self.server.bind(('192.168.17.1', 6000))
                self.server.listen()
                print("Listening...", end=" ")
                self.client, self.client_address = self.server.accept()
                #self.server= socket.socket()
                # print("Connecting")
                # self.server.connect(('192.168.17.20',6000))
                print("Connected "+str(self.client_address))
                # self.server.listen(1)

            except Exception as exception:
                print("[Algo] Connection failed: " + str(exception))

                time.sleep(3)
            else:
                print("[Algo] Connected successfully")
                print("[Algo] Client address is " +
                      str(self.client_address))

                break

    def write(self, message):
        print("[Algo] Sending message:")
        print(message)

        try:
            self.client.send(message)
        except Exception as exception:
            print("[Algo] Failed to send: " + str(exception))

    def read(self):
        print("")
        print("[Algo] Reading message from Algo...")

        try:
            message = self.client.recv(self.READ_BUFFER_SIZE)
        except Exception as exception:
            print("[Algo] Failed to read: " + str(exception))
        else:
            if message is not None and len(message) > 0:
                print("[Algo] Message read:", message)
                return message


if __name__ == '__main__':
    ipsocketapi = IPSocketAPI()
    try:
        ipsocketapi.server.close()
    except:
        print("not connected")
    ipsocketapi.connect()

    #sample obstacle location
    message = "ALG:0,18,E,0;18,19,S,1;18,0,W,2;5,0,E,3;10,10,E,4;9,10,W,5;"
    ipsocketapi.write(message.encode('utf-8'))
    while True:
        msg = input("Enter command:")
        if msg == 'r':
            algo = ipsocketapi.read()
            n = 5
            instr = [algo[i:i+n]for i in range(0, len(algo), n)]
            print(instr)
        else:
            ipsocketapi.write(msg.encode('utf-8'))
