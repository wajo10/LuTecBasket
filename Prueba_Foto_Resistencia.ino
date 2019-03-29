#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define pinSD 4     //define el pin para seleccionar la tarjeta SD
#define clk 8
#define AB 9
byte num2Byte(int num) {
  byte numBin = B1111111;
  switch (num) {
    case 0:
      //abcdefg.
      numBin = B11111010; //
      break;
    case 2:
      //abcdefg.
      numBin = B11011101; //B00100101
      break;
    case 3:
      //abcdefg.
      numBin = B11110101; //B00001101
      break;
    case 4:
      numBin = B01100110; //B10011001
      break;
    case 5:
      numBin = B10110110; //B01001001
      break;
    case 6:
      numBin = B10111110; //B01000001
      break;
    case 7:
      //abcdefg.
      numBin = B11100000;
      break;
    case 8:
      //abcdefg.
      numBin = B11111111;
      break;
    case 9:
      //abcdefg.
      numBin = B11100110;
      break;
    case 1:
      //abcdefg.
      numBin = B01100001;
      break;
    case '-':
      numBin = B00000101;
  }
  return numBin;
}

TMRpcm tmrpcm;   //Se crea un objeto de la librería TMRpcm

const int ResPin = A5;
int Res = 0;
int Contador = 0;
const int buttonPin = 7;
int buttonState = 0;
bool Play = false;
bool flag = true;
unsigned long Segundos;
long anterior = 0;
int trans = 0;
int luz_inicial = 0;
void setup() {
  tmrpcm.speakerPin = 10; //Se define el pin en el que está conectada la bocina
  pinMode (ResPin, INPUT);
  pinMode(clk, OUTPUT);
  pinMode(AB, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  if (!SD.begin(pinSD)) {  // see if the card is present and can be initialized:
    Serial.println("Fallo en la tarjeta SD");  //Aviso de que algo no anda bien
    return;   //No hacer nada si no se pudo leer la tarjeta
  }

}

void loop() {
  Res = analogRead(ResPin);
  buttonState = digitalRead(buttonPin);
  //Serial.println(trans);

  if (buttonState == HIGH || Play) {
    unsigned long actual = (millis() / 1000) - (trans / 1000);
    Serial.print(luz_inicial);
    Serial.print("//");
    Serial.println(Res);
    Play = true;
    if(flag){
      Contador=0;
      flag = false;
    }
    Segundos = 30 - (actual - anterior);
      if (luz_inicial == 0 or (abs(Res-luz_inicial>00))){
      luz_inicial = Res;
      Serial.println("Cambio");
    }
    if ( (luz_inicial - Res) >  30){
      tmrpcm.play("canasta.wav", 4);
      Contador++;
      delay(1000);
      Serial.println(Contador);
    }
    else {
      tmrpcm.stopPlayback();
    }
    if (Contador >= 100) {
      Contador = 0;
    }
    else {
      if ((unsigned long)actual - anterior >= 30 ) {
        Play = false;
        anterior = actual;
        Segundos = 30;
        Serial.println("Reiniciando");
      }
      else {
        if ((Segundos) % 3 != 0) {
          shiftOut(AB, clk, LSBFIRST, num2Byte(Contador / 10));
          shiftOut(AB, clk, LSBFIRST, num2Byte(Contador % 10));
        }
        else {
          shiftOut(AB, clk, LSBFIRST, num2Byte((Segundos) / 10));
          shiftOut(AB, clk, LSBFIRST, num2Byte((Segundos) % 10));
        }
      }
    }
     if (Segundos == 3){
        tmrpcm.play("buzzer.wav");
        delay(1000);
     }
    delay(250);
  }
  else {
    //Serial.println("NO");
    Serial.println(Res);
    flag = true;
    if (trans/500 % 2 ==0){
     shiftOut(AB, clk, LSBFIRST, num2Byte(Contador / 10));
     shiftOut(AB, clk, LSBFIRST, num2Byte(Contador % 10)); 
    }
    else {
     shiftOut(AB, clk, LSBFIRST, num2Byte('-'));
     shiftOut(AB, clk, LSBFIRST, num2Byte('-')); 
    }
    
    trans = millis();
  }
}
