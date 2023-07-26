# con3troller

WIP 3DS controller via local WIFI. 

Whenever the touch position is updated, python script will update mouse position, current ratio: \
(touchx / 320) * 1920 \
(touchy / 240) * 1080 \
(end result is rounded) \
(movement should be stable)

Buttons are mappable to other keyboard letters via kconfig.json. Special buttons including the arrow keys are currently WIP

## Setup 

1. Install pynput with
```python -3 -m pip install pynput``` 
2. Place kconfig.json next to server.py 
3. Execute the server.py script in the command line 
4. n the 3DS SD card, create a folder named `con3troller`
5. Inside, place a file named ip.txt
6. In this file, you place your PCs LOCAL IP address, e. g. ```192.168.178.61```, just that, no newline
+ How do I get my local IP... - Google it
7. Place con3troller.3dsx into `/3ds/` on the 3DS SD card
8. Launch `con3troller` in the Homebrew Launcher 

Profit

You don't have to do all that every time, basically just ensure IP, launch python script, launch 3DS application, profit.


## 3DS side usage

Press START to exit the app

## Compiling

### Linux(tested) / macOS(untested) / Windows(untested)

Install [devkitPro](https://devkitpro.org/wiki/devkitPro_pacman), then the `3ds-dev` package. Then run `./build.sh`. To standalone compile the 3DS side, run `make` in the `con3troller` folder. Quick testing can be done with 3dslink, in case you don't know about it.