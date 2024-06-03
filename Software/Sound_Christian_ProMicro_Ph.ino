/***************************************************
 DFPlayer - Mini MP3 Player used with RC Handset
 ***************************************************/

#include <Misclib.h>
#include <EEPROM.h>
#include <Rcul.h>
#include <TinyPinChange.h>

#include <TinyCppmReader.h> //Cppm reader
#include <SoftRcPulseIn.h>  //Pwm reader


static SoftRcPulseIn PwmRcVolume;
static SoftRcPulseIn PwmRcSoundChangeUp;
static SoftRcPulseIn PwmRcSoundChangeDown;
static SoftRcPulseIn PwmRcPlayPause;
TinyCppmReader TinyCppmReader;

/* Possible values to compute a shifting average fin order to smooth the recieved pulse witdh */
#define AVG_WITH_1_VALUE        0
#define AVG_WITH_2_VALUES       1
#define AVG_WITH_4_VALUES       2
#define AVG_WITH_8_VALUES       3
#define AVG_WITH_16_VALUES      4

#define AVERAGE_LEVEL          AVG_WITH_4_VALUES  /* Choose here the average level among the above listed values */
                                                  /* Higher is the average level, more the system is stable (jitter suppression), but lesser is the reaction */
/* Macro for average */
#define AVERAGE(ValueToAverage,LastReceivedValue,AverageLevelInPowerOf2)  ValueToAverage=(((ValueToAverage)*((1<<(AverageLevelInPowerOf2))-1)+(LastReceivedValue))/(1<<(AverageLevelInPowerOf2)))

#define PROJECT_NAME    SOUND_MODULE
#define FW_VERSION      0
#define FW_REVISION     1
#define SOUND_MODULE_REV     PRJ_VER_REV(PROJECT_NAME, FW_VERSION, FW_REVISION) /* ProjectName VVersion.Revision */

#define PRINT_BUF_SIZE   100
static char PrintBuf[PRINT_BUF_SIZE + 1];
#define PRINTF(fmt, ...)    do{if(Serial){snprintf_P(PrintBuf, PRINT_BUF_SIZE, PSTR(fmt) ,##__VA_ARGS__);Serial.print(PrintBuf);}}while(0)
#define PRINT_P(FlashStr)   do{if(Serial){Serial.print(FlashStr);}}while(0)

#define CARRIAGE_RETURN     0x0D /* '\r' = 0x0D (code ASCII) */
#define RUB_OUT             0x08
#define CFG_MSG_MAX_LENGTH  22 /* Longest Rx or Tx Message */
static char                 CfgMessage[CFG_MSG_MAX_LENGTH + 1];/* + 1 pour fin de chaine */


#define INPUT_PIN_A 2   //SBUS or Sound Volume (PWM)
#define INPUT_PIN_B 3   //Sound Change sound Up(PWM)
#define INPUT_PIN_C 10  //Sound Change sound Down(PWM) (6 non reconnue)
#define INPUT_PIN_D 6   //Play Pause
#define DF_BUSY     4

uint8_t LED_STATE   =   5;
uint32_t LedStartMs=millis();
boolean  LedState=HIGH;
long rememberTime=0;
#define LED_HALF_PERIOD_MS      250
boolean InputisSynchro = false;
bool MiniMaxiUs = false;

#include <DFPlayer_Mini_Mp3.h>
bool DFPlayerAvailable = false;
uint8_t BusyIsOn;
#define DFPLAYER_EQ_NORMAL 0
#define DFPLAYER_EQ_POP 1
#define DFPLAYER_EQ_ROCK 2
#define DFPLAYER_EQ_JAZZ 3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASS 5

#define DFPLAYER_DEVICE_SD 2

uint8_t Inputs_Alarms[]={18,19,20,21,15,14};

uint8_t SoundNumber = 0;
uint16_t NewVolumeUs, NewSoundUpUs, NewSoundDwUs, Play_StopUs;
uint16_t OldVolumeUs, OldSoundUs;
uint8_t NewVolume, OldVolume;
uint8_t NewSound, OldSound;
uint8_t TrackNumber,Equalizer,FolderNumber;
bool WaitForChangePrev = false;
bool WaitForChangeNext = false;
uint8_t SoundNextLow = 1;
uint8_t SoundPrevLow = 1;
unsigned long started1s = millis();
bool DebugMode = false;

// const int foldersCount = 5;          // the volume level
// int tracks[foldersCount] = {0,0,0,0,0};   // tracks in microSD


struct MaStructure {
  /* Valeurs par defaut dans EEprom */
  byte ID;
  uint8_t VOLUME_CH;
  uint8_t CHANGEUP_CH;
  uint8_t CHANGEDW_CH;
  uint8_t PLAYPAUSE_CH;
  uint8_t VOLUME;
  uint16_t CENTER_CH;
  uint16_t MAXI_CH;
  uint16_t MINI_CH;
  uint8_t INPUT_TYPE;
  
}; // Ne pas oublier le point virgule !

MaStructure ms;

void setup()
{ 
  pinMode(DF_BUSY,INPUT);
  pinMode(LED_STATE, OUTPUT);  // Set LED as an output
  digitalWrite(LED_STATE,HIGH);
  EEPROM.get(0,ms);
/* try // Play mp3 file in selected folder
       // void mp3_play_file_in_folder (uint8_t folder, uint32_t num);
*/
  for (uint8_t i = 0; i<8; i++)
  {
    pinMode(Inputs_Alarms[i], INPUT_PULLUP); // 6 Inputs pullup setup
  }

  Serial1.begin(9600);
  Serial.begin(115200);// Usb connection
  //while(!Serial){delay(0);}
  delay(1000);
  //USE CR ONLY WITH ARDUINO IDE
  PRINT_P("\r\nInitializing DFPlayer ...\r\n");
  mp3_begin(Serial1);  //set softSerial for DFPlayer-mini mp3 module 
  
  mp3_set_device(DFPLAYER_DEVICE_SD);//SD
  mp3_set_EQ(DFPLAYER_EQ_NORMAL);//

  mp3_set_volume(ms.VOLUME);delay(500);
  mp3_set_volume(ms.VOLUME);delay(500);
  mp3_get_volume();
  OldVolume = mp3_wait_volume();
  
  mp3_get_tf_sum();
  TrackNumber = mp3_wait_tf_sum();

  // mp3_get_folder_nb();
  // uint8_t NbFd = mp3_wait_folder_nb();
  // Serial.print("NbFolder:"); Serial.println(NbFd);

  // mp3_get_folder_sum(1);
  // uint8_t FileInFolder = mp3_wait_folder_sum();
  // Serial.print("NbFileInFolder:"); Serial.println(FileInFolder);

  // for (int i=0; i < foldersCount; i++) {
  // // Query the total number of microSD card files
  //   mp3_get_folder_sum(i + 1);
  //   tracks[i] = mp3_wait_folder_sum();

  //   Serial.print("Find ");
  //   Serial.print(tracks[i]);
  //   Serial.print(" tracks in folder 0");
  //   Serial.print(i);
  //   Serial.println(".");
  // }
  
  DFPlayerAvailable = true;
  digitalWrite(LED_STATE,LOW);							  
  Serial.println();
  Serial.println(F("****************"));
  Serial.println(F("* Sound Module *"));
  Serial.println(F("****************"));
  Serial.println();

  //Serial.print("Volume:");Serial.println(OldVolume);
  if (OldVolume != 0)
    PRINT_P("DFPlayer Mini is online.\r\n");
  else
    PRINT_P("DFPlayer Mini is offline.\r\n");
  
  PRINT_P("Hit Enter or H + Enter for Help\r\n");
  readAllEEprom();

  
  if (ms.INPUT_TYPE == 1){
    //Serial.println(F("Pwm is used"));
    PwmRcVolume.attach(INPUT_PIN_A);
    PwmRcSoundChangeUp.attach(INPUT_PIN_B);
    PwmRcSoundChangeDown.attach(INPUT_PIN_C);
    PwmRcPlayPause.attach(INPUT_PIN_D);
  }
  if (ms.INPUT_TYPE == 2){
  //Serial.println(F("Cppm is used"));
  TinyCppmReader.attach(INPUT_PIN_A); /* Attach TinyCppmReader to INPUT_PIN_A pin */
  }
  
  SoundNumber = 1;
  //mp3_play_physical(SoundNumber);
}

void loop()
{

  if(CfgMessageAvailable() >= 0)
  {
    InterpreteCfgAndExecute();
  }

//  ms.VOLUME_CH = 5;
//  ms.CHANGEDW_CH = 6;
//  ms.CHANGEUP_CH = 7;
//  ms.VOLUME    = 10;
//  ms.CENTER_CH = 1500;
//  ms.MINI_CH = 1000;
//  ms.MAXI_CH = 2000;
    /*
     Channel Pwm 6 ON/OFF
     Channel Pwm 7 and 8 NEXT/PREV
     Channel Pwm 9 Volume
     */

  if (ms.INPUT_TYPE == 1){/* Partie PWM Input on pin 2, 3 and 9*/
    if(PwmRcVolume.available())
    {
      InputisSynchro = true;
      AVERAGE(NewVolumeUs,PwmRcVolume.width_us(),AVERAGE_LEVEL);//channel
    }
    else
    {
      InputisSynchro = false;
    }
    if(PwmRcSoundChangeUp.available())
      AVERAGE(NewSoundUpUs,PwmRcSoundChangeUp.width_us(),AVERAGE_LEVEL);
    if(PwmRcSoundChangeDown.available())
      AVERAGE(NewSoundDwUs,PwmRcSoundChangeDown.width_us(),AVERAGE_LEVEL);
    if(PwmRcPlayPause.available())
      AVERAGE(Play_StopUs,PwmRcPlayPause.width_us(),AVERAGE_LEVEL);
  }

  if (ms.INPUT_TYPE == 2){/* Partie CPPM Input on pin 2 */
  /* set TINY_CPPM_READER_SINGLE_CHANNEL 0 into TinyCppmReader.h for have all channels */
    if ((TinyCppmReader::detectedChannelNb() > 0) && TinyCppmReader::isSynchro())
    {
      InputisSynchro = true;
      NewVolumeUs = TinyCppmReader::width_us(ms.VOLUME_CH);//channel 5 by default
      NewSoundUpUs = TinyCppmReader::width_us(ms.CHANGEUP_CH);//channel 7 by default
      NewSoundDwUs= TinyCppmReader::width_us(ms.CHANGEDW_CH);//channel 6 by default
      Play_StopUs= TinyCppmReader::width_us(ms.PLAYPAUSE_CH);//channel 8 by default
    }
    else
    {
      InputisSynchro = false;
    }
  }

  /* Blink LED Management */
  if (InputisSynchro == false)//Led is On if signal is found or blinking if not found
  {
    if(millis()-LedStartMs>=LED_HALF_PERIOD_MS)
    {
      digitalWrite(LED_STATE, LedState);
      LedState=!LedState;
      LedStartMs=millis();
    }
  }
  else
  {
    digitalWrite(LED_STATE, HIGH);
  }

  if(MiniMaxiUs == true)
  {
    if (NewVolumeUs !=0)
    {
      if ((NewVolumeUs < 1500) && (NewVolumeUs < ms.MINI_CH))
      {
        ms.MINI_CH = NewVolumeUs;
      }
      if ((NewVolumeUs > 1500) && (NewVolumeUs > ms.MAXI_CH))
      {
        ms.MAXI_CH = NewVolumeUs;
      }
      ms.CENTER_CH = (ms.MAXI_CH + ms.MINI_CH)/2;
      Serial.print(NewVolumeUs);Serial.print("\t");
      Serial.print(ms.MINI_CH);Serial.print("\t");
      Serial.print(ms.CENTER_CH);Serial.print("\t");
      Serial.println(ms.MAXI_CH);
    }
    else
    {
      Serial.print("! ");
    }
  }  

  /* Change Volume */
  static unsigned long timer = millis();
  if (millis() - timer > 500) 
  {
    timer = millis();
if (DebugMode == true){
    PRINT_P(NewVolumeUs);PRINT_P("\t");//PRINT_P("\t");
    PRINT_P(NewSoundUpUs);PRINT_P("\t");
    PRINT_P(NewSoundDwUs);PRINT_P("\t");
    PRINT_P(Play_StopUs);PRINT_P("\t");
    PRINTF("SoundNumber: %d\r\n",SoundNumber);
}
    if (NewVolumeUs != 0 ) 
      NewVolume = map(NewVolumeUs, ms.MINI_CH, ms.MAXI_CH, 1, 30);
    else 
      NewVolume = OldVolume;
    //Serial.print(F("Volume : "));Serial.println(NewVolume);

    if (NewVolume != OldVolume)
      mp3_set_volume(NewVolume);
    OldVolume = NewVolume;

    BusyIsOn = digitalRead(DF_BUSY);
    //Serial.print(F("Busy in "));Serial.println(BusyIsOn);
    
  /* Change Sound */
    // inter 2 positions (1 for PREVIOUS and 1 for NEXT)
    if (NewSoundDwUs != 0){
      ((NewSoundDwUs < (ms.CENTER_CH - 200))? SoundPrevLow = 0: SoundPrevLow = 1);//Previous Sound CH=7
      if (SoundPrevLow == 1){
        WaitForChangePrev = true;
      }
      if ((SoundPrevLow == 0) && (WaitForChangePrev == true)){
        //mp3_prev();PRINT_P("PREV");
        SoundNumber = SoundNumber -1;
        mp3_play_physical(SoundNumber);
        WaitForChangePrev = false;
      }
    } //else
      //  Serial.println("No Previous Channel found !");
    if (NewSoundDwUs != 0){
      ((NewSoundUpUs > (ms.CENTER_CH + 200))? SoundNextLow = 0: SoundNextLow = 1);//Next Sound CH=8    
      if (SoundNextLow == 1){
        WaitForChangeNext = true;
      }
      if ((SoundNextLow == 0) && (WaitForChangeNext == true)){
        //mp3_next();PRINT_P("NEXT");
        SoundNumber = SoundNumber +1;
        mp3_play_physical(SoundNumber);
        WaitForChangeNext = false;
      }
    } //else
      //  Serial.println("No Next Channel found !");
    
    /* Play Stop */
    if (Play_StopUs != 0) {
      if (Play_StopUs < (ms.CENTER_CH - 200)) mp3_pause();
      if (Play_StopUs > (ms.CENTER_CH + 200)) mp3_play_physical(SoundNumber);
    }

    /* Read 6 alarms inputs */
    for (uint8_t i = 0; i<6; i++)
    {
      if(digitalRead(Inputs_Alarms[i]) == LOW)
      {
        //mp3_play_file_in_folder(1,i);
        mp3_play(i+1);//Need a folder named MP3
        // if (DebugMode == true)
        // {
           PRINTF("Alarm: %d started !\r\n",i+1);
        // }
      }
    }
    /* Read 6 alarms inputs */
  }//end timer

}

void writeMiniMaxi()
{
  MiniMaxiUs = !MiniMaxiUs;
  if (MiniMaxiUs == true) 
    PRINT_P("Start Mini Maxi search:\r\n");
  else{
    PRINT_P("Stop Mini Maxi search:\r\n");
    EEPROM.put(0, ms);
  }
}

void readAllEEprom()
{
  EEPROM.get(0,ms);// Read all EEPROM settings in one time
  if ( ms.ID != 0x99)
  {
    SettingsWriteDefault();
    delay(500);
    readAllEEprom();
  }
  else
  {
    //EEPROM Ok!
    PRINT_P("\r\n");
    PRINT_P("*************************\r\n");
    PRINT_P("* oO EEPROM Settings Oo *\r\n");
    PRINT_P("*************************\r\n");
    if (ms.INPUT_TYPE == 1) PRINT_P("PWM mode in use\r\n");
    if (ms.INPUT_TYPE == 2) PRINT_P("CPPM mode in use\r\n");
    PRINTF("Vol:       %d\r\n",ms.VOLUME);
    PRINTF("VolCh:     %d\r\n",ms.VOLUME_CH);
    PRINTF("Prev:      %d\r\n",ms.CHANGEDW_CH);
    PRINTF("Next:      %d\r\n",ms.CHANGEUP_CH);
    PRINTF("Play Stop: %d\r\n",ms.PLAYPAUSE_CH);
    PRINTF("CenterCh:  %d\r\n",ms.CENTER_CH);
    PRINTF("MaxiCh:    %d\r\n",ms.MAXI_CH);
    PRINTF("MiniCh:    %d\r\n",ms.MINI_CH);
    PRINT_P("\r\n");
  }
}

void SettingsWriteDefault()
{
  ms.ID        = 0x99;//write the ID to indicate valid data
  ms.VOLUME_CH = 5;
  ms.CHANGEDW_CH = 6;//6=prev et 8=next
  ms.CHANGEUP_CH = 7;//6=prev et 8=next
  ms.PLAYPAUSE_CH = 8;//
  ms.VOLUME    = 10;
  ms.CENTER_CH = 1500;
  ms.MINI_CH = 1000;
  ms.MAXI_CH = 2000;
  ms.INPUT_TYPE = 1;//1=PWM, 2=CPPM, 3=SBUS
  //blinkNTime(1,1000,500);
  PRINTF("Defaults Value Saved");
  EEPROM.put(0, ms);
}

void readSettingsDFP()
{
  PRINT_P("\r\n");
  PRINT_P("***************************\r\n");
  PRINT_P("* oO DFPlayer Settings Oo *\r\n");
  PRINT_P("***************************\r\n");
  mp3_get_state();PRINTF("State       : %d\r\n",mp3_wait_state()); //read mp3 state
  mp3_get_volume();PRINTF("Volume      : %d\r\n",mp3_wait_volume()); //read current volume
  mp3_get_EQ();Equalizer = mp3_wait_EQ(); //read EQ setting
  PRINT_P("Equalizer: ");
  switch(Equalizer)
  {
    case 0:PRINT_P("EQ_NORMAL\r\n");break;
    case 1:PRINT_P("EQ_POP\r\n");break;
    case 2:PRINT_P("EQ_ROCK\r\n");break;
    case 3:PRINT_P("EQ_JAZZ\r\n");break;
    case 4:PRINT_P("EQ_CLASSIC\r\n");break;
    case 5:PRINT_P("EQ_BASS\r\n");break;
  }
  mp3_get_tf_sum();PRINTF("Files found: %d\r\n",mp3_wait_tf_sum());//read number of files
  mp3_get_tf_current();PRINTF("Current sound: %d\r\n",mp3_wait_tf_current());
}


static char CfgMessageAvailable(void)
{
  char Ret = -1;
  char RxChar;
  static uint8_t Idx = 0;

  if(Serial.available() > 0)
  {
    RxChar = Serial.read();
    switch(RxChar)
    {
      case CARRIAGE_RETURN: /* Si retour chariot: fin de message */
        CfgMessage[Idx] = 0;/* Remplace CR character par fin de chaine */
        Ret = Idx;
        Idx = 0; /* Re-positionne index pour prochain message */
        break;
      case RUB_OUT:
        if(Idx) Idx--;
        break;
      default:
        if(Idx < CFG_MSG_MAX_LENGTH)
        {
          CfgMessage[Idx] = RxChar;
          Idx++;
        }
        else Idx = 0; /* Re-positionne index pour prochain message */
        break;
    }
  }
  return(Ret); 
}

#define COMMAND       ( CfgMessage[0] )
#define ACTION        ( CfgMessage[1] )
#define ARG           ( CfgMessage[2] )
//#define ARG2          ( CfgMessage[4] )//Use a space between first (CfgMessage[2]) and second (CfgMessage[4]) argument
#define REQUEST       '?'
#define ORDER         '='

#define EMPTY_CMD      0
#define HELP_CMD      'H'

#define MS_SETTINGS   'S'// Read settings from EEPROM
#define MS_VOLUME     'V'// Read or Define volume
#define MS_MINMAX     'M'
#define MS_DFPLAYER   'P'//Read DFPlayer config
#define MS_PLAYNEXT   '+'
#define MS_PLAYPREV   '-'
#define MS_PLAYPAUSE  'X'

#define QUIT_CMD      'Q'
#define DBG_CMD       'U'
#define MS_RESET      'R'


//  ms.VOLUME_CH = 9;
//  ms.CHANGEUP_CH = 8;
//  ms.PLAY_STOP = 7;
//  ms.VOLUME    = 10;
//  ms.CENTER_CH = 1500;
//  ms.MINI_CH = 1000;
//  ms.MAXI_CH = 2000;

String myString = String(CfgMessage[3]);
enum {ACTION_ANSWER_WITH_REPONSE = 0, ACTION_ANSWER_ERROR};

static void InterpreteCfgAndExecute(void)
{
  uint8_t Val8,Val8b;
  uint8_t Action = ACTION_ANSWER_ERROR;
  char *CharPtr;

  switch(COMMAND)
  {
    case EMPTY_CMD: /* No break: Normal */
    case HELP_CMD:
      DisplayHelp();
      CfgMessage[1] = 0; /* Echo */
      Action = ACTION_ANSWER_WITH_REPONSE;
      break;
    case MS_MINMAX://Mx Mini Maxi channel in uS
        writeMiniMaxi();
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;
      break;
    case MS_VOLUME://Ax Volume channel
      if(ACTION == ORDER)
      {
        Val8 = (uint8_t)atoi(&ARG);
        if ((Val8 >= 1) && (Val8 <= 30))
        {
          Serial.print("Vol:");Serial.println(Val8);
          ms.VOLUME = Val8;
          mp3_set_volume(ms.VOLUME);
          EEPROM.put(0,ms);
          CfgMessage[1] = 0;
        }    
        Action = ACTION_ANSWER_WITH_REPONSE;
      }
      break;
    case MS_SETTINGS://S
      if(ACTION == ORDER)
      {
        Val8 = (uint8_t)atoi(&ARG);
        if(ARG == 'W')//S=W Writedefault config
        {
          // Serial.println(CfgMessage[2]);
          // Serial.println(CfgMessage[3]);
          // Serial.println(CfgMessage[4]);
          SettingsWriteDefault();
        }
        else if (ARG == 'I')//S=I X X=1 to 2 
        {
          CharPtr = (char*)(&ARG + 2);
          ms.INPUT_TYPE = atoi(CharPtr);;
          if (ms.INPUT_TYPE == 1) Serial.println("PWM");
          if (ms.INPUT_TYPE == 2) Serial.println("CPPM");
        }
        else if ((ARG == '1') && (*(&ARG + 1) == ' '))
        {
          CharPtr = (char*)(&ARG + 2);
          ms.VOLUME_CH = atoi(CharPtr);
          PRINTF("VolCh: %d\r\n",ms.VOLUME_CH);
        }
        else if ((ARG == '2') && (*(&ARG + 1) == ' '))
        {
          CharPtr = (char*)(&ARG + 2);
          ms.CHANGEDW_CH = atoi(CharPtr);
          PRINTF("NextCh: %d\r\n",ms.CHANGEDW_CH);
        }
        else if ((ARG == '3') && (*(&ARG + 1) == ' '))
        {
          CharPtr = (char*)(&ARG + 2);
          ms.CHANGEUP_CH = atoi(CharPtr);
          PRINTF("PrevCh: %d\r\n",ms.CHANGEUP_CH);
        }
        else if ((ARG == '4') && (*(&ARG + 1) == ' '))
        {
          CharPtr = (char*)(&ARG + 2);
          ms.PLAYPAUSE_CH = atoi(CharPtr);
          PRINTF("PlayPauseCh: %d\r\n",ms.PLAYPAUSE_CH);
        }     
        EEPROM.put(0,ms);
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;
      }
      else if(ACTION == REQUEST)
      {
        readAllEEprom();
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;
      }
      break;
    case MS_DFPLAYER://P
      if(ACTION == REQUEST)
      {
        if (DFPlayerAvailable == true)
        {
          readSettingsDFP();
        }
        else
        {
          Serial.print(F("DFPlayer not found"));
        }
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;
      }
      break;
    case MS_PLAYNEXT://+
        PRINT_P("\r\nNEXT\r\n");
        mp3_next();
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;
      break;
    case MS_PLAYPREV://-
        PRINT_P("\r\nPREV\r\n");
        mp3_prev();
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;
      break;
    case MS_PLAYPAUSE://X
        if (BusyIsOn == 0) {mp3_pause();PRINT_P("\r\nPAUSE\r\n");}
        if (BusyIsOn == 1) {mp3_play();PRINT_P("\r\nPLAY\r\n");}
        CfgMessage[1] = 0;          
        Action = ACTION_ANSWER_WITH_REPONSE;
      break;
    case DBG_CMD://U
        DebugMode = !DebugMode;
        CfgMessage[1] = 0; 
        Action = ACTION_ANSWER_WITH_REPONSE;
      break;
    case MS_RESET://R
        PRINT_P("\r\nWait...\r\n");
        for (uint16_t i = 0 ; i < EEPROM.length() ; i++) {
          EEPROM.write(i, 0);
        }
        //blinkNTime(2,1000,500);
        PRINT_P("\r\nReset EEPROM Done\r\n");
        CfgMessage[1] = 0;
        Action = ACTION_ANSWER_WITH_REPONSE;    
    break;

  }
  if(Action != ACTION_ANSWER_WITH_REPONSE)
  {
    strcpy_P(CfgMessage, PSTR("ERR"));
  }
  Serial.println(CfgMessage);

}

void DisplayHelp(void)
{
  PRINT_P("\r\nH Help\r\n");
  PRINT_P("S=W Write config\r\n");
  PRINT_P("S? Read Settings\r\n");
  PRINT_P("S=I X x=1 to 2 Input mode\r\n");//1=PWM, 2=CPPM
  PRINT_P("S=Y Z Y=1 to 4 1=Vol, 2=Prev, 3=Next, 4=PlayPause / Z=1 to 12 (Channel Nb)\r\n");
  PRINT_P("V=x x=1 to 30 Default Volume\r\n");
  PRINT_P("M Maxi/Centre/Mini Channel in uS\r\n");
  PRINT_P("P? Read DFPlayer config\r\n");
  PRINT_P("+ OR - Next/Prev Sound\r\n");
  PRINT_P("X /Play/Pause\r\n");
  PRINT_P("U Debug mode ON/OFF\r\n");
  
  PRINT_P("R Write Default Settings\r\n");
  PRINT_P("\r\n");
}

// used for flashing a pin
void blinkNTime(uint8_t count, uint16_t onDuration, uint16_t offDuration)
{
  for (uint8_t i = 0; i < count;i++)
  {
    if( LedState ==HIGH )
    {
      if( (millis()- rememberTime) >= onDuration){   
      LedState = LOW;// change the state of LED
      rememberTime=millis();// remember Current millis() time
      }
    }
    else
    {   
      if( (millis()- rememberTime) >= offDuration){     
      LedState = HIGH;// change the state of LED
      rememberTime=millis();// remember Current millis() time
      }
    }    
  }
}
