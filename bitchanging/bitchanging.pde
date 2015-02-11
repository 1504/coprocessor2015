// Need G4P library
import g4p_controls.*;
import processing.serial.*;
Serial port;
int total;
public void setup(){
  size(480, 320, JAVA2D);
  createGUI();
  customGUI();
  port = new Serial(this, Serial.list()[0], 9600);
  // Place your setup code here
  
}

public void draw(){
  background(230);
  String val;
  val = port.readStringUntil('\n'); 
  println(val);
  port.write(total);
  delay(30);
}

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){

}
