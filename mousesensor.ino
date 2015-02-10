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
byte byte1 = 11101111;
byte byte2 = 01001110;
bool alliance;
int station;
bool gameMode;
int roboX;
int roboY;
int rotation;
int elevatorLevel;
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

}

void loop() {
  if(Serial.available() > -1){
    bool alliance = bitRead(byte1, 7);
    if (alliance){
      Serial.print("Blue Team");
      Serial.print("\t");
    }
    if(!alliance){
      Serial.print("Red Team");
      Serial.print("\t");
    }
    
    switch(getStation(byte1)){
      case 1:
      Serial.print("Station 1");
      Serial.print("\t");
      break;
      case 2:
      Serial.print("Station 2");
      Serial.print("\t");
      break;
      case 3:
      Serial.print("Station 3");
      Serial.print("\t");
      break;
    
    }
    bool gameMode = bitRead(byte1, 4);
    if(gameMode){
      Serial.print("Auton");
      Serial.print("\t");
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
      break;
      case 1:
      //Going right
      Serial.print("Right");
      Serial.print("\t");
      break;
      case -1:
      //Going left
      Serial.print("Left");
      Serial.print("\t");
      break;  
       }
    roboY = getDirection(byte1,1);
    switch (roboY){
      case 0:
      Serial.print("Neutral");
      Serial.print("\t");
      break;
      case 1:
      //Going up
      Serial.print("Upward");
      Serial.print("\t");
      break;
      case -1:
      //Going down
      Serial.print("Downward");
      Serial.print("\t");
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
      break;
      case 1:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");
      break;
      case 2:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");
      break;
      case 3:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      break;
      case 4:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      break;
      case 5:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      break;
      case 6:
      Serial.print(getElevatorLevel(byte2, 5));
      Serial.print("\t");  //do yes
      break;
      
  }
  switch (getCurrentTime(byte2, 2)) {
      case 0:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");
      break;
      case 15:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");
      break;
      case 35:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes
      break;
      case 55:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes
      break;
      case 75:
      Serial.print(getCurrentTime(byte2, 2));  //do yes
      break;
      case 90:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes
      break;
      case 105:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes
      break;
      case 120:
      Serial.print(getCurrentTime(byte2, 2));
      Serial.print("\t");  //do yes
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
  }
delay(30); 
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

*/
/* Processing code for this example
 // Dimmer - sends bytes over a serial port
 // by David A. Mellis
 //This example code is in the public domain.
 
 import processing.serial.*;
 Serial port;
 
 void setup() {
 size(256, 150);
 
 println("Available serial ports:");
 println(Serial.list());
 
 // Uses the first port in this list (number 0).  Change this to
 // select the port corresponding to your Arduino board.  The last
 // parameter (e.g. 9600) is the speed of the communication.  It
 // has to correspond to the value passed to Serial.begin() in your
 // Arduino sketch.
 port = new Serial(this, Serial.list()[0], 9600);  
 
 // If you know the name of the port used by the Arduino board, you
 // can specify it directly like this.
 //port = new Serial(this, "COM1", 9600);
 }
 
 void draw() {
 // draw a gradient from black to white
 for (int i = 0; i < 256; i++) {
 stroke(i);
 line(i, 0, i, 150);
 }
 
 // write the current X-position of the mouse to the serial port as
 // a single byte
 port.write(byte1);
 }
 */

/* Max/MSP v5 patch for this example
 
----------begin_max5_patcher----------
1008.3ocuXszaiaCD9r8uhA5rqAeHIa0aAMaAVf1S6hdoYQAsDiL6JQZHQ2M
YWr+2KeX4vjnjXKKkKhhiGQ9MeyCNz+X9rnMp63sQvuB+MLa1OlOalSjUvrC
ymEUytKuh05TKJWUWyk5nE9eSyuS6jesvHu4F4MxOuUzB6X57sPKWVzBLXiP
xZtGj6q2vafaaT0.BzJfjj.p8ZPukazsQvpfcpFs8mXR3plh8BoBxURIOWyK
rxspZ0YI.eTCEh5Vqp+wGtFXZMKe6CZc3yWZwTdCmYW.BBkdiby8v0r+ST.W
sD9SdUkn8FYspPbqvnBNFtZWiUyLmleJWo0vuKzeuj2vpJLaWA7YiE7wREui
FpDFDp1KcbAFcP5sJoVxp4NB5Jq40ougIDxJt1wo3GDZHiNocKhiIExx+owv
AdOEAksDs.RRrOoww1Arc.9RvN2J9tamwjkcqknvAE0l+8WnjHqreNet8whK
z6mukIK4d+Xknv3jstvJs8EirMMhxsZIusET25jXbX8xczIl5xPVxhPcTGFu
xNDu9rXtUCg37g9Q8Yc+EuofIYmg8QdkPCrOnXsaHwYs3rWx9PGsO+pqueG2
uNQBqWFh1X7qQG+3.VHcHrfO1nyR2TlqpTM9MDsLKNCQVz6KO.+Sfc5j1Ykj
jzkn2jwNDRP7LVb3d9LtoWBAOnvB92Le6yRmZ4UF7YpQhiFi7A5Ka8zXhKdA
4r9TRGG7V4COiSbAJKdXrWNhhF0hNUh7uBa4Mba0l7JUK+omjDMwkSn95Izr
TOwkdp7W.oPRmNRQsiKeu4j3CkfVgt.NYPEYqMGvvJ48vIlPiyzrIuZskWIS
xGJPcmPiWOfLodybH3wjPbMYwlbFIMNHPHFOtLBNaLSa9sGk1TxMzCX5KTa6
WIH2ocxSdngM0QPqFRxyPHFsprrhGc9Gy9xoBjz0NWdR2yW9DUa2F85jG2v9
FgTO4Q8qiC7fzzQNpmNpsY3BrYPVJBMJQ1uVmoItRhw9NrVGO3NMNzYZ+zS7
3WTvTOnUydG5kHMKLqAOjTe7fN2bGSxOZDkMrBrGQ9J1gONBEy0k4gVo8qHc
cxmfxVihWz6a3yqY9NazzUYkua9UnynadOtogW.JfsVGRVNEbWF8I+eHtcwJ
+wLXqZeSdWLo+FQF6731Tva0BISKTx.cLwmgJsUTTvkg1YsnXmxDge.CDR7x
D6YmX6fMznaF7kdczmJXwm.XSOOrdoHhNA7GMiZYLZZR.+4lconMaJP6JOZ8
ftCs1YWHZI3o.sIXezX5ihMSuXzZtk3ai1mXRSczoCS32hAydeyXNEu5SHyS
xqZqbd3ZLdera1iPqYxOm++v7SUSz
-----------end_max5_patcher-----------
 */
