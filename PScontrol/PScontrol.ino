// Include the proper libraries
// Make sure these libraries are in the proper directory
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo gripper;
Servo left_wheel;
Servo right_wheel;
// Setting up the wireless transceiver
#define CE_PIN   9
#define CSN_PIN 10

// Defining the transmit pipe
// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

// Creating a radio object
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

// Creating an array to store the button data coming from the transceiver
int buttonState[6];
int oldAngle = 512;
bool oldbuttonState1;
bool oldbuttonState2;

void setup() {
  radio.begin();
  radio.setChannel(19);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  gripper.attach(4);
  left_wheel.attach(6);
  right_wheel.attach(5);
  Serial.begin(9600);
}

void loop() {
  reciever();
}

// reads in an array from the wireless transiever and use the function movement_control()
void reciever() {
  int servo_direction;
  if (radio.available()) {
    bool done = false;
    while (!done) {
      done = radio.read(buttonState, sizeof(buttonState));
      Serial.print(buttonState[0]);
      Serial.print("   ");
      Serial.print(buttonState[1]);
      Serial.print("   ");
      Serial.print(buttonState[2]);
      Serial.print("   ");
      Serial.print(buttonState[3]);
      Serial.print("   ");
      Serial.print(buttonState[4]);
      Serial.print("   ");
      Serial.println(buttonState[5]);
      movement_control();
    }
  }
}

// uses input and movement functions to move the robot and to control the grippers
void movement_control() {
  if (buttonState[0] == LOW && oldbuttonState1 == HIGH) {
    gripper_close();
    oldbuttonState1 = LOW;
  }
  if (buttonState[0] == HIGH) {
    oldbuttonState1 = HIGH;
  }
  if (buttonState[1] == LOW && oldbuttonState2 == HIGH) {
    gripper_open();
    oldbuttonState2 = LOW;
  }
  if (buttonState[1] == HIGH) {
    oldbuttonState2 = HIGH;
  }
  if (abs((buttonState[3] - 512)) > 60 || abs((buttonState[2] - 512)) > 60) {
    robot_movement(buttonState[2], buttonState[3]);
  }
  else if (abs((buttonState[5] - 512)) > 60) {
    robot_spin(buttonState[5]);
  }
  else {
    stop_bot();
  }
  if (abs((buttonState[4] - oldAngle)) > 50){         // detects changes in order to use gripper by analog
    gripper_control(buttonState[4]);
    oldAngle = buttonState[4];
  }
}

// opens the gripper claw
void gripper_close() {
  gripper.write(180);
}

// closes gripper claw around the egg
void gripper_open() {
  gripper.write(45);
}

// spins the robot
void  robot_spin(int val) {
  int rw, lw, difference;
  difference = map(val, 0, 1023, -200, 200);
  lw = 1490 + difference;
  rw = 1490 + difference;
  left_wheel.writeMicroseconds(lw);
  right_wheel.writeMicroseconds(rw);
}

// moves the robot forward, backwards and turns
void robot_movement(int xVal, int yVal) {
  int speed_bot = map(xVal, 0, 1023, 1680, 1280);
  int difference = map(yVal, 0, 1023, -2*abs((speed_bot - 1490)), 2*abs((speed_bot - 1490)));
  int lw = speed_bot;
  int rw = 1490 + (1490 - speed_bot);
  if (difference > 0 && speed_bot < 1490) {
    rw += difference;
  }
  else if (difference < 0 && speed_bot < 1490) {
    lw += difference;
  }
  else if (difference > 0 && speed_bot > 1490) {
    rw -= difference;
  }
  else if (difference < 0 && speed_bot > 1490) {
    lw -= difference;
  }
  else{
    lw = 1490;
    rw = 1490;
  }
  left_wheel.writeMicroseconds(lw);
  right_wheel.writeMicroseconds(rw);
}

void stop_bot() {
  left_wheel.writeMicroseconds(1490);
  right_wheel.writeMicroseconds(1490);
}

void gripper_control(int xVal){
  int angle = map (xVal, 0, 1023, 45, 180);
  gripper.write(angle);
}

