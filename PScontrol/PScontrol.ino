/*
  Patrick Thomas, Izzy Cuasay, Ryan Greenlee - Robunny Rumble Project
  Date Created: January 29, 2016
  Due: February 22, 2016
  
  This program interprets input from a playstation controller.
*/

// Include appropriate libraries
#include <Servo.h>
//#include <Psx_analog.h>
#include <Psx.h>
// Includes the Psx Library

// Any pins can be used since it is done in software
Servo gripper;
Servo left_wheel;
Servo right_wheel;

#define dataPin 13 //brown
#define cmndPin 11 //orangw
#define attPin 10 //yellow
#define clockPin 12 //blue

Psx Psx;

unsigned int data = 0;

int delay_amount = 0;

void setup() {
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 20);  // Defines what each pin is used
  // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
  // Delay measures how long the clock remains at each state,
  // measured in microseconds.
  // too small delay may not work (under 5)
  Serial.begin(9600);
  gripper.attach(4);
  left_wheel.attach(5);
  right_wheel.attach(6);
}

void loop() {
  PSX_control();
}

void PSX_control() {
  // Read the controls from the PlayStation controller and sends
  // commands to the robot.
  data = Psx.read();
  Serial.println("Yes");
  if (data & psxR2) {
    forward();
  }
  if (data & psxL2) {
    backward();
  }
  if (data & psxL1) {
    left();
  }
  if (data & psxR1) {
    right();
  }
  if (data & psxRight) {
    spin_left();
    delay_amount = 0;
  }
  if (data & psxLeft) {
    spin_right();
    delay_amount = 0;
  }
  if (data & psxX) {
    gripper.write(180);
  }
  if (data & psxO) {
    gripper.write(45);
  }
  if (data & psxStrt) {
    spin_left();
    delay_amount = 20;
  }
  if (data & psxSlct) {
    spin_right();
    delay_amount = 20;
  }

  // If no data is being sent, stop the robot
  if (!data) {
    stop_bot();
  }
  delay(delay_amount);
}
void forward() {
  // Move the robot forward
  left_wheel.writeMicroseconds(1280);
  right_wheel.writeMicroseconds(1680);
  delay_amount = 0;
}
void backward() {
  // Move the robot backward
  left_wheel.writeMicroseconds(1680);
  right_wheel.writeMicroseconds(1280);
  delay_amount = 0;
}
void stop_bot() {
  // Stop the robot
  left_wheel.writeMicroseconds(1490);
  right_wheel.writeMicroseconds(1490);
  delay_amount = 0;
}
void left() {
  // Make the robot turn left
  left_wheel.writeMicroseconds(1300);
  right_wheel.writeMicroseconds(1530);
  delay_amount = 0;
}
void right() {
  // Make the robot turn right
  left_wheel.writeMicroseconds(1450);
  right_wheel.writeMicroseconds(1680);
  delay_amount = 0;
}
void spin_left() {
  // Make the robot spin left
  left_wheel.writeMicroseconds(1680);
  right_wheel.writeMicroseconds(1680);
}

void spin_right() {
  // Make the robot spin right
  left_wheel.writeMicroseconds(1300);
  right_wheel.writeMicroseconds(1300);
}
