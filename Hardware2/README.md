# Sound_Module XANY

This Sound Module use handset channels for all commandes.  
The first version accept only PWM and CPPM output.  
The second version will accept also SBUS, IBUS (Flysky), SUMD (Graupner), SRXL (Multiplex), JETIEX and XANY [(o)](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/README.md#o-xany) / BURC [(oo)](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/README.md#oo-burc) on theses inputs modes.  
It's possible to command the **Volume**, **Previous sound**, **Next Sound**, **Play/Stop sound** and **Play same sound**.  
For command these fonctions, you need to define 3 or 5 channels.  
See paragraph **Commandes**. 

![Top View](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/Sound_Module_Xany_Top.jpg)  

## Sound Module PCB XANY:
![Schematic](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/Sound_Module_v1.1.jpg)

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
S=W Write Default Settings
S? Read Settings
S=I X X=1 to 5 Input mode, 1=PWM, 2=CPPM, 3=SBUS, 4=IBUS, 5=SUMD
S=Y Z Y=1 to 4 1=Vol, 2=Prev, 3=Next, 4=PlayPause,5=PlaySame / Z=1 to 12 (Channel Nb)
V=X X=1 to 30 Default Volume
M Maxi/Centre/Mini Channel in uS
P? Read DFPlayer config
+ OR - Next/Prev Sound
X /Play/Pause
U Debug mode ON/OFF
Y=X Z X=0 to 16 Xany mode 0=OFF 1 to 16=Xany Channel / Z=1 to 3 Xany mode
  
R Erase EEPROM and all configuration

```

## (o) XANY
X-Any is the direct integration into [OpenAVRc](https://github.com/Ingwie/OpenAVRc_Dev) of the RCUL functionality developed for arduino targets.  
X-Any allows you to transmit in digital form, via a single proportional channel, any information between the transmitter and the receiver (e.g. the position of switch contacts) independently of the type of HF module: operation in 2.4 GHz is therefore guaranteed.  
With OpenAVRc, it is not necessary to add an external RCUL encoder to benefit from the Multiswitch encoder functionality: the X-Any encoder is integrated into the transmitter in software.  

## (oo) BURC
As X-Any, [BURC](http://p.loussouarn.free.fr/arduino/exemple/BURC/BURC.html#encoder) use also the RCUL functionality developed for arduino targets.  
BURC is usable with all handset witch has a PPM or SBUS input with the same features than X-Any, but need an external coder.  

