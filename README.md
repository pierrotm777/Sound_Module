# Sound_Module
Sound Module witch use handset channel for all commandes


![here](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/Sound%26SmokeModule/Sound%26Smoke_Module.jpg)

## Sound&Smoke_Module PCB:
![here](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/Sound%26SmokeModule/S%26SM1.png)  

![here](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/Sound%26SmokeModule/S%26SM2.png)  

## Videos
[![Video 1](https://img.youtube.com/vi/RUjrtYIM5t8/0.jpg)](https://www.youtube.com/watch?v=RUjrtYIM5t8 "Video 1")  
Video of engine start, acceleration/deceleration, smoke generation in proportion to throttle and automatic shutdown after 15 seconds at idle.

[![Video 2](https://img.youtube.com/vi/LdH3JfI2tWg/0.jpg)](https://www.youtube.com/watch?v=LdH3JfI2tWg "Video 2")  
Video of engine start, acceleration, generation of smoke in proportion to throttle, launch of auxiliary sounds from transmitter,  
volume adjustment of auxiliary sound and engine sound from transmitter and automatic shutdown after 15 idle.  
The use of the X-Any/RCUL system of the OpenAVRc transmitter makes everything very ergonomic.

## More details on the realization and configuration of the module
[Rc-Navy Sound&Smoke](http://p.loussouarn.free.fr/projet/Sound&SmokeModule/Sound&SmokeModule.html)

## User manual
[Configuration of the Sound&Smoke Module](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/Sound%26SmokeModule/RC_Sound%26Smoke_Manuel_Utilisateur.pdf)

## Upload the HEX file
We don't give sources, because X-Any libraries are not Open Source for the receive side.   
You can found binary file [Sound&Smoke Module](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Sound%26SmokeModule/Firmware_Sound%26Smoke_Module)  

## Sounds files 
You will find under these folders the sounds files to copy on the SD cards.  
[Motor's Sounds](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Sound%26SmokeModule/Firmware_Sound%26Smoke_Module/SD_Arduino_Pro_Micro)  
[Auxiliary's Sounds](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Sound%26SmokeModule/Firmware_Sound%26Smoke_Module/SD_DF_Player_mini)  

The **Arduino Pro Micro** board use a particular method for upload a code.  
You can use the program [32u4_hexuploader](https://github.com/uriba107/32u4_hexuploader).

 

## Commandes
```RC_SOUND_SMOKE V0.3 [RCUL/Xany] (C) RC Navy 2022

CONF?               Retourne la configuration courante complète

RC.FILTER?          Retourne si le filtre RC est actif ou pas
RC.FILTER=ON        Définit si le filtre RC est actif ou pas (Pour récepteur filtrant les grandes variation : Récepteurs PCM, DSP, Hold)

THR.ZERO?           Retourne la valeur en μS du point milieu des Gaz moteur (par exemple THR.ZERO=1500)
THR.ZERO=CAL        Définit la valeur en μS du point milieu des Gaz moteur (placer le manche moteur au centre avant de valider la commande)

ENG.LIST?           Retourne la liste des fichiers moteur stockés sur la carte SD Arduino, par exemple :
ENG.LIST=DSL-LTL, DSL-V12, VAPEUR, DSL-OLD, DSL-120, DSL-TURB, DSL-TUG, SCAN-V12, DSL-BIG, DSL-180, DIESEL7, SCAN-250, CAT-C32
ENG.SEL?            Retourne le fichier moteur sélectionné, par exemple : ENG.SEL=CAT-C32
ENG.SEL=CAT-C32     Définit le moteur sélectionné (CAT-C32 : Caterpillar C32 Marine)
ENG.ATTEN?          Retourne l’attenuation du son moteur (0 à 3, si différent de 0, l'inter 8 est utiliser pour rendre actif ou pas l'atténuation)
ENG.ATTEN=0         Définit l’attenuation du son moteur (0 à 3, si différent de 0, l'inter 8 est utiliser pour rendre actif ou pas l'atténuation)

SMOKE.MIN?          Retourne l’index min de la fréquence de fumée (de 0 à 11 : valeur par défaut=0)
SMOKE.MIN=00        Définit l’index min de la fréquence de fumée (de 0 à 11 : valeur par défaut=0)
SMOKE.MAX?          Retourne l’index max de la fréquence de fumée (de 1 à 12 : valeur par défaut=12)
SMOKE.MAX=12        Définit l’index max de la fréquence de fumée (de 1 à 12 : valeur par défaut=12)
SMOKE.TEST?         Retourne l’index courant de la fréquence de fumée (de SMOKE.MIN à SMOKE.MAX : par défaut de 0 à 12)
SMOKE.TEST=x        Définit l’index courant de la fréquence de fumée (de SMOKE.MIN à SMOKE.MAX : par défaut de 0 à 12)
SMOKE.TEST=OFF      Stoppe le test du générateur de fumée
SMOKE.MODE?         Retourne le mode de fonctionnement du générateur de fumée
SMOKE.MODE=OFF      Définit le mode de fonctionnement du générateur de fumée : utilisé (pas de fumée générée)
SMOKE.MODE=PROP     Définit le mode de fonctionnement du générateur de fumée : proportionnelle aux Gaz (plus réaliste pour moteur à vapeur)
SMOKE.MODE=ACCEL    Définit le mode de fonctionnement du générateur de fumée : proportionnelle aux Accélérations (plus réaliste pour moteur Diesel)

AUX.NB?             Retourne le nombre de pistes (fichier son MP3) présents sur la carte SD du DF Player
AUX.TRACK?          Retourne le numéro de la piste en cours (piste en cours de lecture ou dernière piste lue)
AUX.TRACK=x         Définit le numéro de la piste et la lance (x de 1 à AUX.NB)
AUX.EQ?             Retourne la valeur de l'EQualizer
AUX.EQ=x            Définit la valeur de l'EQualizer: de 0 à 5. Permet de régler le type de sonorité:
                    0 – Normal, 1 – Pop, 2 – Rock, 3 – Jazz, 4 – Classic, 5 – Bass
AUX.VOL?            Retourne le volume du DF Player
AUX.VOL=x           Définit le volume des sons auxiliaires du DF Player de 0 à 30 (Le réglage du volume du son moteur se fait par le pot RV1)
AUX.HWPRIO?         Retourne la priorité des 6 sons externes
AUX.HWPRIO=PNNNNN   Définit la priorité des 6 sons externes (N pour Nomale ou P pour Prioritaire)
AUX.NEXT            Définit et joue la piste suivante
AUX.PREV            Définit et joue la piste précédente
AUX.PAUSE           Stoppe la lecture de la piste courante
AUX.PLAY            Relance la lecture de la piste courante
AUX.RAND            Joue les pistes de manière aléatoire

>Les sons 1 à 8 sont pilotés par les contacts Sw (sauf le 8e si une atténuation du son moteur est définie. Dans ce cas, le Sw N°8 permet d’atténuer le
son du moteur).
>Les sons de 9 à 14 sont pilotés en HW sur front descendant par les entrées A1 à A6 du connecteur P4.
>Les sons au-dessus de 14 ne sont pas utilisés par radio-commande, mais peuvent être lancés par l’interface USB à l’aide de la commande AUX.TRACK=x. 
```

