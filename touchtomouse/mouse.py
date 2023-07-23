import socket
import struct
from pynput.mouse import Button, Controller

def split_4byte_integer(number):
    number = number & 0xFFFFFFFF
    first_2_bytes = (number >> 16) & 0xFFFF
    last_2_bytes = number & 0xFFFF
    return first_2_bytes, last_2_bytes


# socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = 7078
serversocket.bind(("", port))
message, addr = serversocket.recvfrom(4)
serversocket.sendto(b"Smosh", addr)

print(message)


# response interpret
ustruct = struct.Struct('I')

# mouse setup
mouse = Controller()
print("starting mouse controller")

while True:
    # touchx = serversocket.recvfrom(2)
    # touchy = serversocket.recvfrom(2)
    # print(touchx, touchy)
    _touch, addr = serversocket.recvfrom(4)
    touch = ustruct.unpack(_touch)[0]
    touchy, touchx = split_4byte_integer(touch)
    xpos = round((touchx/320)*1920)
    ypos = round((touchy/240)*1080)
    mouse.position = (xpos, ypos)
    print({xpos, ypos, addr})

print("Exiting...")
serversocket.close()