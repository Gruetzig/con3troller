import socket
import struct
import json
from pynput.keyboard import Key, Controller

def BIT(n):
    return (1<<n)

# button defines
bits = {    
    "KEY_A": BIT(0),
    "KEY_B": BIT(1),
    "KEY_SELECT": BIT(2),
    "KEY_START": BIT(3),
    "KEY_DRIGHT": BIT(4),
    "KEY_DLEFT": BIT(5),
    "KEY_DUP": BIT(6),
    "KEY_DDOWN": BIT(7),
    "KEY_R": BIT(8),
    "KEY_L": BIT(9),
    "KEY_X": BIT(10),
    "KEY_Y": BIT(11),
    "KEY_ZL": BIT(14),
    "KEY_ZR": BIT(15),
    "KEY_TOUCH": BIT(20),
    "KEY_CSTICK_RIGHT": BIT(24),
    "KEY_CSTICK_LEFT": BIT(25),
    "KEY_CSTICK_UP": BIT(26),
    "KEY_CSTICK_DOWN": BIT(27),
    "KEY_CPAD_RIGHT": BIT(28),
    "KEY_CPAD_LEFT": BIT(29),
    "KEY_CPAD_UP": BIT(30),
    "KEY_CPAD_DOWN": BIT(31)
}

config = open("config.json", "r")
jsonf = config.read()
config.close()

pbuttons = json.loads(jsonf)
print("config loaded")
# socket
print("Waiting for connection...")
serversocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = 7078
serversocket.bind(("", port))
message, addr = serversocket.recvfrom(4)
serversocket.sendto(b"Smosh", addr)
print(f"{str(message, 'UTF-8')} from a connecting 3DS")
# keyboard
print("Initializing keyboard...")
keyboard = Controller()

# response interpret
ustruct = struct.Struct('I')
print("Done. have fun")
# loop
while True:
    _buttons = serversocket.recv(4)
    buttons = ustruct.unpack(_buttons)[0]
    print(buttons)
    for key in pbuttons:
        if (bits[key] & buttons):
            keyboard.press(pbuttons[key])
        else:
            keyboard.release(pbuttons[key])
    
print("Exiting...")
serversocket.close()