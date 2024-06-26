# Sound_Module NORMAL
This Sound Module use handset channels for all commandes.  
This version accept only PWM and CPPM output. 
It's possible to command the **Volume**, **Previous sound**, **Next Sound**, **Play/Stop sound** and **Play same sound**.  
For command these fonctions, you need to define 3 or 5 channels.  
See paragraph **Commandes**.  

![Top View](https://github.com/pierrotm777/Sound_Module/blob/main/Sound_ChristianTop.jpg)  

## Sound Module PCB NORMAL:
![Schematic](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware1/Sound_Module_v1.0.png)

## User manual


## Upload the HEX file
Two firmware are possibles, NORMAL or XANY.  
They are downloadable [here](https://github.com/pierrotm777/Sound_Module/tree/main/Firmware).  

## How to upload your firmware
The **Arduino Pro Micro** board use a particular method for upload a code.  
You can use the program [32u4_hexuploader](https://github.com/uriba107/32u4_hexuploader).


## Commandes
```SOUND_MODULE V1.0 [Use RC_NANY libs] (C) pierrotm777
H Help
C Module Connections
S=W Write Default Settings
S? Read Settings
S=I X X=1 to 5 Input mode, 1=PWM, 2=CPPM, 3=SBUS, 4=IBUS, 5=SUMD
S=Y Z Y=1 to 4 1=Vol, 2=Prev, 3=Next, 4=PlayPause,5=PlaySame / Z=1 to 12 (Channel Nb)
V=X X=1 to 30 Default Volume
M Maxi/Centre/Mini Channel in uS
P? Read DFPlayer config
+ OR - Next/Prev Sound
X /Play/Pause
D Debug mode ON/OFF
Y=X Z X=0 to 16 Xany mode 0=OFF or 1 to 16=Xany Channel / Z=1 to 4 Xany mode
  
R Erase EEPROM and all configuration
```
