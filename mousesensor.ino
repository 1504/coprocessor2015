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
  if(Serial.available() > 2){
    //First mouse thingy
    mouse1Val = mouse1.read(DELTA_X_REG);
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
    Serial.write(raw.ZAxis);
  }
delay(8); 
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
*/
