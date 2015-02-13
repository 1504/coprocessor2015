#include <Boards.h>
#include <Firmata.h>
#include <HMC5883L.h>
#include <Wire.h>
#include <adns2620.h>
ADNS2620 mouse1(3,4);
ADNS2620 mouse2(8,5);
char mouse1Val;
char mouse2Val;
char surface;
char magX, magY, magZ;
HMC5883L compass;
int error = 0;
//Bits and bytes here
byte byte1 = B11111111;
byte byte2 = B01000000;
bool alliance;
int station;
bool gameMode;
int roboX;
int roboY;
int rotation;
int elevatorLevel;
int ledElevatorPin = 11;
int ledElevator;
int ledTimePin = 13;
const int ledPin =  13;      // the number of the LED pin
int allianceLed = 7;
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
int ledX;
int ledY;
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000; 
void setup() {
    mouse1.begin();
    delay(100);
    mouse1.sync();
    mouse1.write(CONFIGURATION_REG, 0x01);
    mouse2.begin();
    delay(100);
    mouse2.sync();
    mouse2.write(CONFIGURATION_REG, 0x01);
    Serial.begin(9600);
    Wire.begin();
    compass = HMC5883L();
    compass.SetMeasurementMode(Measurement_Continuous);
    pinMode(ledElevatorPin, OUTPUT);
    pinMode(allianceLed, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(10, OUTPUT);
}

void loop() {
  if(Serial.available() > -1){
    bool alliance = bitRead(byte1, 7);
    if (alliance){
      Serial.print("Blue Team");
      Serial.print("\t");
      digitalWrite(allianceLed, HIGH);
    }
    if(!alliance){
      Serial.print("Red Team");
      Serial.print("\t");
    }
    
    switch(getStation(byte1)){
      case 1:
      Serial.print("Station 1");
      Serial.print("\t");
      digitalWrite(9, HIGH);
      break;
      case 2:
      Serial.print("Station 2");
      Serial.print("\t");
      digitalWrite(9, HIGH);
      digitalWrite(1, HIGH);
      break;
      case 3:
      Serial.print("Station 3");
      Serial.print("\t");
      digitalWrite(9, HIGH);
      digitalWrite(1, HIGH);
      digitalWrite(12, HIGH);
      break;
    
    }
    bool gameMode = bitRead(byte1, 4);
    if(gameMode){
      Serial.print("Auton");
      Serial.print("\t");
       unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    interval = interval-35;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
    }
    else{
      Serial.print("Teleop");
      Serial.print("\t");
    }
    roboX = getDirection(byte1,3);
    switch (roboX){
      case 0:
      Serial.print("Neutral");
      Serial.print("\t");
      ledX = 127;
      analogWrite(10,ledX);
      break;
      case 1:
      //Going right
      Serial.print("Right");
      Serial.print("\t");
      ledX = 255;
      analogWrite(10,ledX);
      break;
      case -1:
      //Going left
      Serial.print("Left");
      Serial.print("\t");
      ledX = 0;
      analogWrite(10,ledX);
      break;  
       }
    roboY = getDirection(byte1,1);
    switch (roboY){
      case 0:
      Serial.print("Neutral");
      Serial.print("\t");
      ledY = 127;
      analogWrite(10,ledY);
      break;
      case 1:
      //Going up
      Serial.print("Upward");
      Serial.print("\t");
      ledY = 255;
      analogWrite(10,ledY);
      break;
      case -1:
      //Going down
      Serial.print("Downward");
      Serial.print("\t");
      ledY = 0;
      analogWrite(10,ledY);
      break;  
       }
    rotation = getDirection(byte2,7);
    switch (rotation){
      case 0:
      Serial.print("Neutral");
      Serial.print("\t");
      break;
      case 1:
      //Going CW
      Serial.print("CW");
      Serial.print("\t");
      break;
      case -1:
      //Going CCW
      Serial.print("CCW");
      Serial.print("\t");
      break;  
       }
    switch (getElevatorLevel(byte2, 5)) {
      case 0:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");
      ledElevator = 0;
      break;
      case 1:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");
      ledElevator = 42.5;
      break;
      case 2:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");
      ledElevator = 85;
      break;
      case 3:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      ledElevator = 127.5;
      break;
      case 4:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      ledElevator = 170;
      break;
      case 5:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      ledElevator = 212.5;
      break;
      case 6:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      ledElevator = 255;
      break;
      
  }
  switch (getCurrentTime(byte2, 2)) {
      case 0:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");
 
      break;
      case 15:
      Serial.print(getCurrentTime(byte2, 2));
      digitalWrite(ledTimePin, HIGH);
      Serial.print("\t");
      break;
      case 35:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes
      digitalWrite(ledTimePin, LOW);
      break;
      case 55:
      Serial.print(getCurrentTime(byte2, 2));
      digitalWrite(ledTimePin, HIGH);
      Serial.print("\t");  //do yes
      break;
      case 75:
      Serial.print(getCurrentTime(byte2, 2));  //do yes
      digitalWrite(ledTimePin, LOW);
      break;
      case 90:
      Serial.print(getCurrentTime(byte2, 2));
      digitalWrite(ledTimePin, HIGH);
      Serial.print("\t");  //do yes
      break;
      case 105:
      Serial.print(getCurrentTime(byte2, 2));
      digitalWrite(ledTimePin, LOW);
      Serial.print("\t");  //do yes
      break;
      case 120:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes

      //There are some fates worse than death 
      unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    interval = interval-35;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
      break;  
  }
    //First mouse thingy
    /*mouse1Val = mouse1.read(DELTA_X_REG);
    Serial.write(mouse1Val);
    mouse1Val = mouse1.read(DELTA_Y_REG);
    Serial.write(mouse1Val);
    surface = mouse1.read(SQUAL_REG);
    Serial.write(surface);
    //Second mouse thingy
    mouse2Val = mouse2.read(DELTA_X_REG);
    Serial.print(mouse2Val, HEX);
    mouse2Val = mouse2.read(DELTA_Y_REG);
    Serial.print(mouse2Val, HEX);
    surface = mouse2.read(SQUAL_REG);
    Serial.println(surface);
    //Magnetometer, reads and write
    MagnetometerRaw raw = compass.ReadRawAxis();
    Serial.write(raw.XAxis);
    Serial.write(raw.YAxis);
    Serial.write(raw.ZAxis);*/
    Serial.println("");
    //Starts the writing to leds
    analogWrite(ledElevatorPin, ledElevator);
  }
delay(8); 
}
int getStation(byte a){
   bool a1 = bitRead(a, 6);
   bool b1 = bitRead(a, 5);
   if(a1&&b1){return 1;}
   if(a1&&!b1){return 2;}
   if(b1&&!a1){return 3;}
}
int getDirection(byte a, int bitNum){
   bool a1 = bitRead(a, bitNum);
   bool b1 = bitRead(a, bitNum-1);
   if(a1&&b1){return 1;}
   if(a1&&!b1){return 0;}
   if(b1&&!a1){return -1;}
}
int getElevatorLevel(byte a, int bitNum){
  bool a1 = bitRead(a, bitNum);
  bool a2 = bitRead(a, bitNum-1);
  bool a3 = bitRead(a, bitNum-2);
  if(a1&&!a2&&!a3){
    return 0;
  } 
  if(!a1&&a2&&!a3){
    return 1;
  } 
  if(!a1&&a2&&a3){
    return 2;
  } 
  if(a1&&a2&&!a3){
    return 3;
  } 
  if(a1&&a2&&a3){
    return 4;
  } 
  if(!a1&&!a2&&a3){
    return 5;
  } 
  if(!a1&&!a2&&!a3){
    return 6;
  } 
}
int getCurrentTime(byte a, int bitNum){
  bool a1 = bitRead(a, bitNum);
  bool a2 = bitRead(a, bitNum-1);
  bool a3 = bitRead(a, bitNum-2);
  if(!a1&&!a2&&!a3){
    return 0;
  } 
  if(!a1&&a2&&!a3){
    return 15;
  } 
  if(!a1&&a2&&a3){
    return 35;
  } 
  if(a1&&a2&&!a3){
    return 55;
  } 
  if(a1&&a2&&a3){
    return 75;
  } 
  if(!a1&&!a2&&a3){
    return 90;
  } 
  if(a1&&!a2&&!a3){
    return 105;
  }
  if(a1&&!a2&&a3){
    return 120;
  }
}
/*
mousel
x
y
squal
mouser
x
y
squal
mag
x
y
z
Byte 1 Bit 7 - Alliance
  1 = Blue Team
  0 = Red Team
Byte 1 Bit 6-5 - Station
  11 = Station 1
  10 = Station 2
  01 = Station 3
Byte 1 Bit 4 - Game Mode
  1 = Auton
  0 = Teleop
Byte 1 Bit 3-2 - X-Direction
  11 = Right
  10 = Neutral
  01 = Left
Byte 1 Bit 1-0 - Y-Direction
  11 = Forward
  10 = Neutral
  01 = Backward
Byte 2 Bit 7-6 - Rotation
  11 = CW
  10 = Neutral
  01 = CCW
Byte 2 Bit 5-3 - Elevator Level
  100 = 0
  010 = 1
  011 = 2
  110 = 3
  111 = 4
  001 = 5
  000 = 6
Byte 2 Bit 2-0 - Current Time
  000 = 0
  010 = 15
  011 = 35
  110 = 55
  111 = 75
  001 = 90
  100 = 105
  101 = 120
//Pin Usage
13 - Timing Pin
12 - Driver Station
11 - Elevator
10 - X
9 - Driver Station
8 - Mouse
7 - Alliance
6 - Y
5 - Mouse
4 - Mouse
3 - Mouse
2 - 
1 - Driver Station
*/
