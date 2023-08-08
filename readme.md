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
2. Place `kconfig.json` next to `server.py` 
3. Execute the `server.py` script in the command line with `python server.py` (python may also only work as `python3` or `py -3`)
4. Place `con3troller.3dsx` into `/3ds/` on the 3DS SD card
5. Launch `con3troller` in the Homebrew Launcher 

## 3DS side usage

Exit with the START button, begin with A button. Select the IP by using B and the D-Pad, open the settings(WIP) with the Y button. 
IP is saved in sdmc:/con3troller/ip.txt

## Compiling

### Linux(tested) / macOS(untested) / Windows(untested)

Install [devkitPro](https://devkitpro.org/wiki/devkitPro_pacman), then the `3ds-dev` package. Then run `./build.sh`. To standalone compile the 3DS side, run `make` in the `con3troller` folder. Quick testing can be done with 3dslink, in case you don't know about it.