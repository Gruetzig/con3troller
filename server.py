import socket
import struct
from pynput.keyboard import Key, Controller

def BIT(n):
    return (1<<n)

# button defines
KEY_A = BIT(0)
KEY_B = BIT(1)
KEY_SELECT = BIT(2)
KEY_START = BIT(3)
KEY_DRIGHT = BIT(4)
KEY_DLEFT = BIT(5)
KEY_DUP = BIT(6)
KEY_DDOWN = BIT(7)
KEY_R = BIT(8)
KEY_L = BIT(9)
KEY_X = BIT(10)
KEY_Y = BIT(11)
KEY_ZL = BIT(14)
KEY_ZR = BIT(15)
KEY_TOUCH = BIT(20)
KEY_CSTICK_RIGHT = BIT(24)
KEY_CSTICK_LEFT = BIT(25)
KEY_CSTICK_UP = BIT(26)
KEY_CSTICK_DOWN = BIT(27)
KEY_CPAD_RIGHT = BIT(28)
KEY_CPAD_LEFT = BIT(29)
KEY_CPAD_UP = BIT(30)
KEY_CPAD_DOWN = BIT(31)
# socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 7078
serversocket.bind(("", port))
serversocket.listen(5)
print(f"Waiting for connection on port {port}")
clientsocket, adress = serversocket.accept()
# keyboard
print("Initializing keyboard...")
keyboard = Controller()




# response interpret
ustruct = struct.Struct('I')

# loop
while True:
    _buttons = clientsocket.recv(4)
    buttons = ustruct.unpack(_buttons)[0]
    print(buttons)
    if (KEY_A & buttons):
        keyboard.press(Key.down)
    else:
        keyboard.release(Key.down)
    
    
print("Exiting...")
clientsocket.close()
serversocket.close()