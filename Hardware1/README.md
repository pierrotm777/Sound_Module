# Sound_Module

This Sound Module use handset channels for all commandes.  
The first version accept only PWM and CPPM output.  
The second version will accept also SBUS, IBUS (Flysky), SUMD (Graupner), SRXL (Multiplex) and JETIEX.  
It's possible to command the **Volume**, **Previous sound**, **Next Sound**, **Play/Stop sound** and **Play same sound**.  
For command these fonctions, you need to define 3 or 5 channels.  
See paragraph **Commandes**. 

<table cellspacing=0>
  <tr>
    <td align=center width=200><a href="https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/User's%20OpenAVRc%20Transmitters/pierrotm777/Fc16/README.md"><img src="https://github.com/pierrotm777/Sound_Module/blob/main/Sound_ChristianTop.jpg" border="0" name="submit" title="Fc16 Transmitter" alt="Fc16 Transmitter"/></a><br><b>Version NORMAL</b></td>
	<td align=center width=200><a href="https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/User's%20OpenAVRc%20Transmitters/pierrotm777/My_Prototypes/README.md"><img src="My_Prototypes/Protos_Pierrot.jpg" border="0" name="submit" title="Prototypes" alt="Prototypes"/></a><br><b>Prototypes</b></td>

  </tr>
</table> 

![Top View](https://github.com/pierrotm777/Sound_Module/blob/main/Sound_ChristianTop.jpg)  

## Sound Module PCB:
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

