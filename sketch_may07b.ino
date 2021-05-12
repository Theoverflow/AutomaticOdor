//#include <btAudio.h>
*//#include <DRC.h>


#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Stepper.h>

#define PIN_HUMIDIFICATEUR  4 //Faire enum des pins
#define PIN_BCK             5 //Construction Pin I2S
#define PIN_WS              6
#define PIN_DOUT            7

#define FRAISE          0 //Faire enum des odeurs
#define POMME           1
#define VANILLE         2
#define CHOCOLAT        3
#define CERISE          4
#define NBODEUR         5
#define SONUN           0 //Faire enum des sons
#define SONDEUX         1
#define SONTROIS        2
#define STEPS           32
#define ALLUME          1
#define ETEINT          0

SoftwareSerial BTSerial(2, 3); // RX | TX
Stepper myStepper(STEPS, 8, 9, 10, 11);
//btAudio audio = btAudio("ESP_Speaker");

int etatOdeur[2] = [FRAISE, ETEINT];
int etatSon[2]   = [SONUN, ETEINT];
int state = 0;
boolean NL = true;
int presentStep = 0;
int stepVal = 2048/NBODEUR;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BTSerial.begin(115200);  
  //audio.begin();
  //audio.I2S(PIN_BCK, PIN_DOUT, PIN_WS);
  myStepper.setSpeed(200);
  pinMode(PIN_HUMIDIFICATEUR, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(receptionBT());
  etatOdeur[1] == ALLUME ? digitalWrite(PIN_HUMIDIFICATEUR, HIGH) : digitalWrite(PIN_HUMIDIFICATEUR, LOW);// On humidifie
}

int receptionBT(){
  if (BTSerial.available()){
        state = (int)BTSerial.read();
        Serial.write(state);
        if(controller());
  }
  return 1;
}

int controller(){
  int etatOdeurPrev = etatOdeur[0];
  int etatSonPrev = etatSon[0];
  if(state != 0 && state%2 == 0){
    switch(state){
      case 2:
        etatSon[0] = SONUN;
        etatSon[1] = ALLUME;
        break;
      case 4:
        etatSon[0] = SONDEUX;
        etatSon[1] = ALLUME;
        break;
      case 6:
        etatSon[0] = SONTROIS;
        etatSon[1] = ALLUME;
        break;
      case 100:
        etatSon[1] = ETEINT;
        break;
    }
  }
  if(state%2 == 1){
    switch(state){
      case 1:
        etatOdeur[0] = FRAISE;
        etatOdeur[1] = ALLUME;
        break;
      case 3:
        etatOdeur[0] = POMME;
        etatOdeur[1] = ALLUME;
        break;
      case 5:
        etatOdeur[0] = VANILLE;
        etatOdeur[1] = ALLUME;
        break;
      case 7:
        etatOdeur[0] = CHOCOLAT;
        etatOdeur[1] = ALLUME;
        break;
      case 9:
        etatOdeur[0] = CERISE;
        etatOdeur[1] = ALLUME;
        break;
      case 99:
        etatOdeur[1] = ETEINT;
        break;
    }
    if(etatOdeurPrev != etatOdeur[0]){
      if(majMoteur());
    }
  }
  return 1;
}

int majMoteur(){
  presentStep = (presentStep + stepVal)%2048;
  myStepper.step(stepVal);
  return 1;
}
