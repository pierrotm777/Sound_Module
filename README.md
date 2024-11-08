# Sound_Module
This Sound Module use handset channels for all commandes.  
The first version accept only PWM and CPPM output.  
The second version will accept also SBUS, IBUS (Flysky), SUMD (Graupner), SRXL (Multiplex), JETIEX and XANY [(o)](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/README.md#o-xany) / BURC [(oo)](https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/README.md#oo-burc) on theses inputs modes.  
It's possible to command the **Volume**, **Previous sound**, **Next Sound**, **Play/Stop sound** and **Play same sound**.  
For command these fonctions, you need to define 3 or 5 channels.  
See paragraph **Commandes**.  

## Two PCB versions
<table cellspacing=0>
  <tr>
    <td align=center width=400><a href="https://github.com/pierrotm777/Sound_Module/blob/main/Hardware1/README.md"><img src="https://github.com/pierrotm777/Sound_Module/blob/main/Sound_Christian_Top.jpg" border="0" name="submit" title="Sound Module NORMAL (V1.0)" alt="Sound Module NORMAL (V1.0)"/></a><br><b>Version NORMAL</b></td>
	<td align=center width=400><a href="https://github.com/pierrotm777/Sound_Module/blob/main/Hardware2/README.md"><img src="https://github.com/pierrotm777/Sound_Module/blob/main/Sound_Module_Xany_Top.jpg" border="0" name="submit" title="Sound Module XANY (V1.1)" alt="Sound Module XANY (V1.1)"/></a><br><b>Version XANY</b></td>

  </tr>
</table> 

### First PCB options
This board accept two modes:  
  * mode Next/Previous sounds (This mode use two switchs on the Handset for select each sound  
  * mode 8 Buttons (This mode need a [8 buttons keyboard](https://p-loussouarn-free-fr.translate.goog/projet/MS8-V2/MS8-V2.html?_x_tr_sch=http&_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en#Keyboard) that replace a proportionnal channel into the handset  

### Second PCB options
This board accept Only the Xany feature and need a BURC module connected on your handset or an OpenAVRc handset.    

## User manual
You can find a documentation in French [here](https://github.com/pierrotm777/Sound_Module/blob/main/Module_Son_Manuel_Utilisateur.pdf).  

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

 
