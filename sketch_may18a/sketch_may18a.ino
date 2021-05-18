#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <SoftwareSerial.h>

#define PIN_RXBT        2
#define PIN_TXBT        3
#define PIN_SD          10
#define PIN_SPEAKER     9

#define SONUN           0
#define SONDEUX         1
#define SONTROIS        2

#define ALLUME          1
#define PAUSE           0

SoftwareSerial BTSerial(PIN_RXBT, PIN_TXBT); // RX | TX
TMRpcm tmrpcm;

int etatSon[2] = {SONUN, PAUSE};
int state = 0;
boolean NL = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BTSerial.begin(38400);
  pinMode(PIN_RXBT, INPUT);
  pinMode(PIN_TXBT, OUTPUT);
  tmrpcm.speakerPin = PIN_SPEAKER;
  delay(500);
  if (!SD.begin(PIN_SD)){
    Serial.println("SD fail");
    return;
  }
  
  tmrpcm.setVolume(5); //
  tmrpcm.play("son1.wav");
  BTSerial.print("AT+VERSION?");
  BTSerial.print("\r\n");
  Serial.println(BTSerial.read());
}

void loop() {
  if(receptionBT());
  else Serial.println('Error At : receptionBT');
}

int receptionBT(){
  if (BTSerial.available()){
        state = (int)BTSerial.read();
        Serial.println(state);
        if(controller());
        else {
          Serial.println('Error At : controller');
          return 0;
        }
  }
  return 1;
}

int controller(){
  int sonPrev = etatSon[0];
  int etatSonPrev = etatSon[1];
  int toChange = 0;
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
      case 8:
        etatSon[1] = PAUSE;
        break;
      case 10:
        etatSon[1] = ALLUME;
        break;
  }
  if(sonPrev != etatSon[0]) toChange = 1;
  if(etatSonPrev != etatSon[1]) toChange = 2;
  delay(500);
  if(majSon(toChange));
  return 1;
}

int majSon(int toChange){
  switch(toChange){
    case 1:
      switch(etatSon[0]){
        case SONUN:
          tmrpcm.play("son1.wav");
          Serial.println("Son 1");
          break;
        case SONDEUX:
          tmrpcm.play("son2.wav");
          Serial.println("Son 2");
          break;
        case SONTROIS:
          tmrpcm.play("son3.wav");
          Serial.println("Son 3");
          break;
      }
      break;
    case 2:
      etatSon[1] ? tmrpcm.play("son1.wav") : tmrpcm.pause();
      Serial.println(etatSon[1]);
      break;
    default:
      break; 
  }
  return 1;
}
