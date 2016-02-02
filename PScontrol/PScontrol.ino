/*
  Patrick Thomas, Izzy Cuasay, Ryan Greenlee - Robunny Rumble Project
  Date Created: January 29, 2016
  Due: February 2, 2016
  
  This program interprets input from a playstation controller.
*/

#include <Servo.h>
#include <Psx.h>                                          // Includes the Psx Library 

// Any pins can be used since it is done in software
Servo gripper;
Servo left_wheel;
Servo right_wheel;

#define dataPin 8
#define cmndPin 9
#define attPin 11
#define clockPin 10

Psx Psx;

unsigned int data = 0;

void setup() {
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 10);  // Defines what each pin is used
                                                          // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
                                                          // Delay measures how long the clock remains at each state,
                                                          // measured in microseconds.
                                                          // too small delay may not work (under 5)
  Serial.begin(9600);
  gripper.attach(11);
  left_wheel.attach(13);
  right_wheel.attach(12);
}

void loop() {
  PSX_control();
}

void PSX_control() {
  data = Psx.read();
  Serial.println(data);
  if (data & psxR2) {
    left_wheel.writeMicroseconds(1680);
    right_wheel.writeMicroseconds(1300);
  }
  else
  {
    left_wheel.writeMicroseconds(1490);
    right_wheel.writeMicroseconds(1490);
  }
  if (data & psxL2) {
    left_wheel.writeMicroseconds(1300);
    right_wheel.writeMicroseconds(1680);
  }
  else
  {
    left_wheel.writeMicroseconds(1490);
    right_wheel.writeMicroseconds(1490);
  }
  if (data & psxR1){
    gripper.write(180);
  }
  if (data & psxL1){
    gripper.write(45);
  }
  delay(20);
}
