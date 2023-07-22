# con3troller

WIP 3DS controller via local WIFI. 

## touch to mouse

Whenever the touch position is updated, python script will update mouse position, current ratio:
(touchx / 320) * 1920
(touchy / 240) * 1080
(end result is rounded)
Yes, movement is unstable (right now, maybe I can fix)

#### Setup & Usage

install pynput with
```python -3 -m pip install pynput``` \
execute the script in the command line \
on the 3DS SD card, create a folder named `con3troller`, inside, place a file named ip.txt. In this file, you place your PCs LOCAL IP address, e. g. ```192.168.178.61```, just that, no newline. \
How do I get my local IP... - Google it \
Place mouselistener.3dsx into `/3ds/`
Launch `mouselistener` in the Homebrew Launcher \
Press START to exit

## keyboard

WIP