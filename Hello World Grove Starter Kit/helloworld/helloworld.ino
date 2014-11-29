/*
*  Hugo Andrade de Oliveira
*  Intel Iot RoadShow - SP
*  Teste de sensores Grove StarterKit
*  28/11/14
*
* Schematic - Base Shield
* Buzzer             -> D6
* Sound Sensor       -> A3
* Potentiometer      -> A2
* Touch Sensor       -> D2
* Light Sensor       -> A1
* Temperature Sensor -> A0
* Relay              -> D5  
* LED                -> D8
* Servo              -> D3
* Button             -> D4
* LCD                -> I2C
*
*/

#include <Servo.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

// Declaração pinos digitais GPIO
const int pinButtonT = 2;  // botao de touch
const int pinServo   = 3;  // servo
const int pinButton  = 4;  // botao digital (Seletor Menu)
const int pinRelay   = 5;  // Rele
const int pinBuzzer  = 6;  // buzzer
const int pinLed     = 8;  // led

// Declaração pinos analogicos
const int pinTemp       = A0;  // sensor de temperatura
const int pinLight      = A1;  // sensor de luminosidade
const int potentiometer = A2;  // potenciometro
const int pinSound      = A3;  // sensor sonoro

// Variaveis globais temperatura
float temperature, resistance;
int   B=3975;  // B value of the thermistor

int sensorValue, shaft;
int opMenu=9;
Servo groveServo; //create a object

//Buzzer
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
int posNote = 0;

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(pinBuzzer, HIGH);
        delayMicroseconds(tone);
        digitalWrite(pinBuzzer, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}

// Menu
int menu(){
  
  if(digitalRead(pinButton))
        opMenu=++opMenu%7;     
  lcd.setCursor(0, 0);
  
  return opMenu;
}

// Startando Galileo
void setup() 
{
    // Definição dos pinos
    pinMode(pinButtonT,    INPUT);
    pinMode(pinButton,     INPUT);
    pinMode(potentiometer, INPUT);
    pinMode(pinBuzzer,     OUTPUT);
    pinMode(pinLed,        OUTPUT);
    pinMode(pinRelay,      OUTPUT);
    
    // Servo motor
    groveServo.attach(pinServo);
    
    // Inicializando lcd
    lcd.begin(16, 2);
    lcd.setRGB(255,255,255);
    
    // Mensagem inicial
    lcd.print("     Estacao    ");
    lcd.setCursor(0, 1);
    lcd.print("Multi-Sensorial");
    
    delay(500);
}


void loop(){
  
    switch(menu()){
      // Temperatura Ambiente
      case 0:
         lcd.setRGB(255,0,0);
         lcd.print("Temperatura Amb:");
         sensorValue = analogRead(pinTemp);                       // get analog value
         resistance=(float)(1023-sensorValue)*10000/sensorValue;  // get resistance
         temperature=1/(log(resistance/10000)/B+1/298.15)-273.15; // calc temperature
         lcd.setCursor(0,1);
         lcd.print("     ");
         lcd.print(temperature);
         lcd.print(" C    ");
         
         break;
      // Iluminação Ambiente   
      case 1:
         lcd.setRGB(0,255,0);
         lcd.print("Iluminacao:     ");
         sensorValue = analogRead(pinLight);
         lcd.setCursor(0, 1);
         lcd.print("      ");
         sensorValue = (sensorValue*100)/780; 
         lcd.print(sensorValue);
         lcd.print(" %     ");
         break;
      // Buzzer   
      case 2:
         lcd.setRGB(0,0,255);
         lcd.print("Buzzer:         ");
         lcd.setCursor(0, 1);
         if(digitalRead(pinButtonT)){
           lcd.print("      Tocou     ");
           digitalWrite(pinLed, HIGH);
           while(digitalRead(pinButtonT)){
             posNote = ++posNote%length;    
             if (notes[posNote] == ' ')
               delay(beats[posNote] * tempo); // rest
             else
               playNote(notes[posNote], beats[posNote] * tempo);
             delay(tempo / 2);
           }
         }
         else{
           lcd.print("    Nao Tocou   ");
           digitalWrite(pinLed, LOW);
           digitalWrite(pinBuzzer, LOW);
         }
         break;
      // Leitura de Potenciometro   
      case 3:
         lcd.setRGB(255,255,255);
         lcd.print("Potenciometro:     ");
         sensorValue = analogRead(pinLight);
         lcd.setCursor(0, 1);
         lcd.print("     ");
         lcd.print(analogRead(potentiometer));
         lcd.print("          ");
         break;
      // Sensor sonoro   
      case 4:
         lcd.setRGB(0,155,155);
         lcd.print("Sensor Sonoro: ");
         sensorValue = analogRead(pinLight);
         lcd.setCursor(0, 1);
         lcd.print("     ");
         lcd.print(analogRead(pinSound));
         lcd.print("        ");
         break;
       // Servo motor
       case 5:
         lcd.setRGB(111,15,155);
         lcd.print("Servo motor     ");
         shaft = analogRead(potentiometer);
         shaft = map(shaft, 0, 1023, 0, 179);
         //analog input data range from 1~1023, but servo
         groveServo.write(shaft);             //only reflects to data ranging from 1~179.
         lcd.setCursor(0, 1);
         lcd.print("     ");
         lcd.print(shaft); 
         lcd.print("        ");
         break;
      // Acionamento rele   
      case 6:
         lcd.setRGB(155,155,0);
         lcd.print("Rele:           ");
         lcd.setCursor(0, 1);
         if(digitalRead(pinButtonT)){
           lcd.print("      acionado     ");
           digitalWrite(pinLed, HIGH);
           digitalWrite(pinRelay, HIGH);
         }
         else{
           lcd.print("                ");
           digitalWrite(pinLed, LOW);
           digitalWrite(pinRelay, LOW);
         }
         break;
         
      default:
         break;
    }
    delay(200);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
