import socket
import struct
import json
from pynput.mouse import Button, Controller
from pynput.keyboard import Controller as kController
import threading

def BIT(n):
    return (1<<n)

def buttonstuff(buttons):
    for key in pbuttons:
        if (bits[key] & buttons):
            keyboard.press(pbuttons[key])
        else:
            keyboard.release(pbuttons[key])

def mousestuff(touchx, touchy):
    # touch
    if not touchx + touchy == 0:
        xpos = round((touchx/320)*1920)
        ypos = round((touchy/240)*1080)
        mouse.position = (xpos, ypos)


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
    "KEY_CSTICK_RIGHT": BIT(24),
    "KEY_CSTICK_LEFT": BIT(25),
    "KEY_CSTICK_UP": BIT(26),
    "KEY_CSTICK_DOWN": BIT(27),
    "KEY_CPAD_RIGHT": BIT(28),
    "KEY_CPAD_LEFT": BIT(29),
    "KEY_CPAD_UP": BIT(30),
    "KEY_CPAD_DOWN": BIT(31)
}

config = open("kconfig.json", "r")
jsonf = config.read()
config.close()

pbuttons = json.loads(jsonf)
print("config loaded")

print("Waiting for connection...")
# socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = 7078
serversocket.bind(("", port))
message, addr = serversocket.recvfrom(4)
serversocket.sendto(b"Smosh", addr)
print(f"{str(message, 'UTF-8')} from a connecting 3DS")

print("Initializing mouse...")
mouse = Controller()
print("Done. have fun")
# response interpret
ustruct = struct.Struct('HHI')
print("Initializing keyboard...")
keyboard = kController()
# mouse setup

print("Done! Have fun")
while True:
    resp, addr = serversocket.recvfrom(8)
    touchx, touchy, buttons = ustruct.unpack(resp)
    btt = threading.Thread(target=buttonstuff, args=(buttons,))
    mouset = threading.Thread(target=mousestuff, args=(touchx, touchy))
    btt.start()
    mouset.start()
    btt.join()
    mouset.join()
    


print("Exiting...")
serversocket.close()