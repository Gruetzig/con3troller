import socket
import struct
from pynput.mouse import Button, Controller

# socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 7078
serversocket.bind(("", port))
serversocket.listen(5)
print(f"Waiting for connection on port {port}")
clientsocket, address = serversocket.accept()
print(f"connected to {address} on port {port}")

# response interpret
ustruct = struct.Struct('H')

# mouse setup
mouse = Controller()
print("starting mouse controller")

while True:
    touchx = ustruct.unpack(clientsocket.recv(2))[0]
    touchy = ustruct.unpack(clientsocket.recv(2))[0]
    xpos = round((touchx/320)*1920)
    ypos = round((touchy/240)*1080)
    mouse.position = (xpos, ypos)
    print({xpos, ypos})

print("Exiting...")
clientsocket.close()
serversocket.close()