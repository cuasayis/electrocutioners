/*Joy_Stick_PS2.ino
   Authors: Patrick Thomas, Izzy Cuasay, and Ryan Greenlee
   Created: February 10, 2016

   This program controls a BOEBOT first by autonomous control and then wirelessly using a PS2 controller and bluetooth dongle on the 
   bot. The autonomous program runs once and then stays put until the user presses the start button on the PS2 controller allowing
   user control of the bot. When controlled by the user, the bot moves based on the button pressed or direction the joystick is moved
   on the PS2 controller.
   
   The hardware includes the Boebot, three servos, gripper, two sonar sensors, flex sensor, joystick, wireless transceiver, Playstation
   controller, and Bluetooth dongle.
   
   The program includes the following functions:
   autonomous():        commands for autonomous control of the robot
   back_up():           backs up robot for 1.5 seconds
   bump_wall():         nudges bot towards the wall to the left
   bump_left():         moves bot to the left
   turn_around():       turns robot around
   turn_right():        turns robot 90 degrees right
   PS2_control():       reads values from PS controller and then desginates actions for the robot to do
   gripper_open():      opens the gripper claw
   gripper_close():     closes gripper claw
   stop_bot():          stops the robot
   robot_spin(int val): spins the robot based on input value from the joystick
   backward():          moves bot backwards
   forward():           moves bot forwards
   forward_tap():       moves the bot forward for pad delay milliseconds then stops
   backward_tap():      moves the bot backward for pad delay milliseconds then stops
   left_tap():          spins the bot left for pad delay milliseconds then stops
   right_tap():         spins the bot right for pad delay milliseconds then stops
   veer_right():        moves the bot forward while turning to the right
   veer_left():         moves the bot forward while turning to the left

*/


#include <PS2X_lib.h>  // include the library to use PS2 controller and commands
#include <Servo.h>     // include the library to use servo motors
Servo gripper;         // creates a servo object for the gripper
Servo left_wheel;      // creates a servo object for the left wheel
Servo right_wheel;     // creates a servo object for the right wheel

/******************************************************************
   set pins connected to PS2 controller:
 ******************************************************************/
#define PS2_DAT        6  //    data pin number
#define PS2_CMD        4  //    comand pin number
#define PS2_SEL        3  //    attention pin number
#define PS2_CLK        5  //    clock pin number

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

int error = 0;
byte type = 0;
byte vibrate = 0;

const int pingPinLF = 8;      // pin number for the sonar sensor on the side of the bot
const int pingPinLB = 7;      // pin number for the sonar sensor on the front of the bot

const int flexpin = A0;       // assigns analog pin number A0 for the flex sensor input

const int pad_delay = 20;     // delay time in milliseconds for d_pad buttons

void setup() {
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  gripper.attach(11);           // attachs pin number 3 to the gripper servo
  left_wheel.attach(12);        // attachs pin number 6 to the left wheel servo
  right_wheel.attach(13);       // attachs pin number 6 to the right wheel servo
}

void loop() {
  autonomous();         // begins autonomous control mode for the robot
}

// commands for autonomous control of the robot
void autonomous() {
  bool starter = true;                    // creates boolean object for when to start the bot
  stop_bot();                             // makes sure bot is not moving
  delay(500);                             // waits 0.5 seconds
  while (starter) {                       // runs a while loop til the right joystick is pushed down
    ps2x.read_gamepad(false, vibrate);    // reads in values from the PS2 controller
    if (ps2x.Button(PSB_R3)) {            // waits till the right joystick button is pressed down to start autonomous
      starter = false;                    // sets starter to false ending while loop
    }
  }
  gripper.write(45);                      // sets the gripper position to open
  bool done = false;                      // creates boolean variable to run bot until flex sensor is bent
  forward();                              // sets bot to drive forward
  while (!done) {                         // run in a loop until the bot is done
    int d_t_w = flexer(flexpin);          // calls flexer to read the value of the flex sensor
    if (d_t_w > 925 || d_t_w < 850) {     // moves forward until the flex sensor on the front of the bot is bent
      done = true;                        // sets done to true to exit the while loop
    }
  }
  gripper.write(110);    // closes girpper
  delay(300);            // waits 300 milliseconds
  turn_right();          // turns the bot to the right
  forward();             // moves bot forward
  delay(400);            // waits 400 milliseconds
  done = false;          // creates boolean variable to run loop until flex sensor is bent again
  while (!done) {                           // moves forward until the flex sensor is bent
    forward();
    if (distance_to(pingPinLF) > 4 && distance_to(pingPinLB) > 4) {               // if the front and back of the bot is greater than 4 centimeters away from the ball, the bot moves toward the wall
      bump_wall();
    }
    else if (distance_to(pingPinLF) > 4 && distance_to(pingPinLB) < 4) {          // if the front is more than 4 centimeters away and the back is less than 4 centimeters from the wall, the bot moves toward the wall
      bump_wall();
    }
    else if (distance_to(pingPinLF) < 4 && distance_to(pingPinLB) > 4) {          // if the front is less than 4 centimeters from the wall and the back is more than 4 centimeters, teh bot moves way from the wall
      bump_left();
    }
    else if (distance_to(pingPinLF) < 4 && distance_to(pingPinLB) < 4) {          // if the front and back of the bot are less than 4 centimeters from the wall, the bot moves away from the wall
      bump_left();
    }
    int d_t_w = flexer(flexpin);
    if (d_t_w > 925 || d_t_w < 850) {     // moves forward until the flex sensor on the front of the bot is bent
      done = true;                        // sets done to true to exit the loop
    }
  }
  gripper.write(45);             // opens girpper
  delay(50);                     // waits 50 milliseconds
  back_up();                     // backs up bot
  turn_around();                 // turns the bot around
  forward();                     // bot moves forward for 8 seconds
  delay(4.5e3);                  // waits 4.5 seconds
  stop_bot();                    // stops the bot
  while (true) {                 // waits till start button is pressed to begin manual control
    ps2x.read_gamepad(false, vibrate);  // reads in values from the PS controller
    if (ps2x.Button(PSB_START)) {       // if the start button is pressed, the bot begins remote control mode
      PS2_control();                    // calls the function PS2_control
    }
  }
}

// backs up the robot for 1.5 seconds
void back_up() {
  backward();
  delay(1500);
}

// reads the flex sensor's value
int flexer (int flexpin) {
  int flexposition;    // Input value from the analog pin.
  int servoposition;   // Output value to the servo.

  // Read the position of the flex sensor (0 to 1023):

  return flexposition = analogRead(flexpin);   // returns flex sensor value
}

// nudges bot towards the wall to the left
void bump_wall() {
  right_wheel.writeMicroseconds(1300);
  left_wheel.writeMicroseconds(1560);
  delay(30);                               // waits 30 milliseconds
}

void bump_left() {
  left_wheel.writeMicroseconds(1700);
  right_wheel.writeMicroseconds(1440);
  delay(30);                               // waits 30 milliseconds
}

// turns robot around
void turn_around() {
  // spins bot to the right
  right_wheel.writeMicroseconds(1700);
  left_wheel.writeMicroseconds(1700);
  delay(1100);                            // waits 1.1 seconds
  stop_bot();                             // calls function to stop bot movement
}

// turns robot 90 degrees right
void turn_right() {
  // spins bot to the right
  right_wheel.writeMicroseconds(1700);
  left_wheel.writeMicroseconds(1700);
  delay(500);                             // waits 0.5 seconds
  stop_bot()                              // calls function to stop bot movement
}

// uses the sonar sensor to detect how far away objects are and returns the distance in inches.
int distance_to(int pingPin) {
  long duration, centimeters;                 // creates variables to hold time it takes for sonar to ping back and variable for distance to object

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);          // sets pingPin mode to output
  digitalWrite(pingPin, LOW);        // Sets pingPin to low
  delayMicroseconds(2);              // waits 2 microseconds
  digitalWrite(pingPin, HIGH);       // sets pingPin to High
  delayMicroseconds(5);              // waits 5 microseconds
  digitalWrite(pingPin, LOW);        // sets pinPin to low

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);             // changes pingPin to input
  duration = pulseIn(pingPin, HIGH);   // reads in number of microseconds to 

  // convert the time into a distance
  centimeters = microsecondsToCentimeters(duration);
  return centimeters;                  // returns the distance to object in centimeters
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

// reads values from PS controller and then designates actions for the robot to do
void PS2_control() {
  while (true) {
    ps2x.read_gamepad(false, vibrate);
    int LJx = ps2x.Analog(PSS_RX);
    Serial.println(LJx);
    if (ps2x.Button(PSB_CROSS)) {          // opens gripper
      gripper_open();
    }
    if (ps2x.Button(PSB_CIRCLE)) {         // closes gripper
      gripper_close();
    }
    if (abs((LJx - 128)) > 10) {           // spins robot based on left joystick
      robot_spin(LJx);
      Serial.println("1");
    }
    if (ps2x.Button(PSB_R2)) {             // moves bot forward
      forward();
      Serial.println("2");
    }
    if (ps2x.Button(PSB_L2)) {             // moves bot backwards
      backward();
      Serial.println("3");
    }
    if (ps2x.Button(PSB_PAD_UP)) {         // moves bot slightly forward
      forward_tap();
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {       // moves bot slightly backward
      backward_tap();
    }
    if (ps2x.Button(PSB_PAD_LEFT)) {       // spins bot slightly to the left
      left_tap();
    }
    if (ps2x.Button(PSB_PAD_RIGHT)) {      // spins bot slightly to the right
      right_tap();
    }
    if (ps2x.Button(PSB_R1)) {             // the bot moves forward turning to the right
      veer_right();
    }
    if (ps2x.Button(PSB_L1)) {             // the bot moves backward turning to the left
      veer_left();
    }
    if (abs((LJx - 128)) < 10 && !ps2x.Button(PSB_R2) && !ps2x.Button(PSB_L2) && !ps2x.Button(PSB_L1) && !ps2x.Button(PSB_R1)) {
      stop_bot();                          // if nothing happens on controller, the bot is stopped.
    }
  }
}


// opens the gripper claw
void gripper_open() {
  gripper.write(180);
}

// closes gripper claw around the egg
void gripper_close() {
  gripper.write(45);
}

// stops the robot
void stop_bot() {
  left_wheel.writeMicroseconds(1490);
  right_wheel.writeMicroseconds(1490);
}

// spins the robot based on input value from the joystick
void  robot_spin(int val) {
  int rw, lw, difference;
  difference = map(val, 0, 250, -200, 200);  // converts joystick values to differences to spin bot based on amount joystick is moved
  lw = 1490 + difference;              // converts difference into microseconds in order to move the left wheel
  rw = 1490 + difference;              // converts difference into microseconds in order to move the right wheel
  left_wheel.writeMicroseconds(lw);
  right_wheel.writeMicroseconds(rw);
}
// moves bot backwards
void backward() {
  left_wheel.writeMicroseconds(1280);
  right_wheel.writeMicroseconds(1680);
}
// moves bot forwards
void forward() {
  left_wheel.writeMicroseconds(1620);
  right_wheel.writeMicroseconds(1280);
}

// moves the bot forward for pad_delay milliseconds then stops
void forward_tap() {
  left_wheel.writeMicroseconds(1680);
  right_wheel.writeMicroseconds(1360);
  delay(pad_delay);
  stop_bot();
}

// moves the bot backward for pad_delay milliseconds then stops
void backward_tap() {
  left_wheel.writeMicroseconds(1280);
  right_wheel.writeMicroseconds(1680);
  delay(pad_delay);
  stop_bot();
}

// spins the bot left for pad_delay milliseconds then stops
void left_tap() {
  left_wheel.writeMicroseconds(1300);
  right_wheel.writeMicroseconds(1300);
  delay(pad_delay);
  stop_bot();
}

// spins the bot right for pad_delay milliseconds then stops
void right_tap() {
  left_wheel.writeMicroseconds(1700);
  right_wheel.writeMicroseconds(1700);
  delay(pad_delay);
  stop_bot();
}

// moves the bot forward while turning to the right
void veer_right() {
  left_wheel.writeMicroseconds(1700);
  right_wheel.writeMicroseconds(1450);
}
 // moves the bot forward while turning to the left
void veer_left() {
  left_wheel.writeMicroseconds(1550);
  right_wheel.writeMicroseconds(1300);
}
