/*
  Patrick Thomas, Izzy Cuasay, Ryan Greenlee - Robunny Rumble Project
  Date Created: Feburary 8, 2016
  Due: February 9, 2016
  
  This program interprets input from a playstation controller.
*/

#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>
Servo gripper;
Servo left_wheel;
Servo right_wheel;

/******************************************************************
   set pins connected to PS2 controller:
     - 1e column: original
     - 2e colmun: Stef?
   replace pin numbers by the ones you use
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

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

const int pingPinLF = 8;       // pin number for the sonar sensor on the side of the bot
const int pingPinLB = 7;      // pin number for the sonar sensor on the front of the bot

const int flexpin = A0;          // assigns analog pin number A0 for the flex sensor input

const int pad_delay = 50;

void setup() {
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  gripper.attach(11);           // attachs pin number 3 to the gripper servo
  left_wheel.attach(12);        // attachs pin number 6 to the left wheel servo
  right_wheel.attach(13);       // attachs pin number 6 to the right wheel servo
  //Serial.begin(9600);
}

void loop() {
  autonomous();         // begins autonomous control mode for the robot
}

// commands for autonoumus control of the robot
void autonomous() {
  bool starter = true;
  stop_bot();
  delay(500);
  while (starter) {
    ps2x.read_gamepad(false, vibrate);
    if (ps2x.Button(PSB_R3)) {            // waits till the right joystick button is pressed down to start autonomous
      starter = false;
      Serial.println("check");
    }
  }
  gripper.write(45);
  bool done = false;
  forward();
  while (!done) {
    int d_t_w = flexer(flexpin);
    if (d_t_w > 925 || d_t_w < 500) {     // moves forward until the flex sensor on the front of the bot is bent
      done = true;
    }
  }
  gripper.write(110);  // closes girpper
  delay(300);
  turn_right();          // turns the bot to the right
  forward();             // moves bot forward
  delay(400);
  done = false;
  while (!done) {                           // moves forward until the front sonar sensor detects something wihtin 6 inches
    forward();
    if (distance_to(pingPinLF) > 2 && distance_to(pingPinLB) > 2) {
      bump_wall();
    }
    if (distance_to(pingPinLF) > 2 && distance_to(pingPinLB) < 2) {
      bump_wall();
    }
    if (distance_to(pingPinLF) < 2 && distance_to(pingPinLB) > 2) {
      bump_left();
    }
    if (distance_to(pingPinLF) < 2 && distance_to(pingPinLB) > 2) {
      bump_left();
    }
    int d_t_w = flexer(flexpin);
    if (d_t_w > 925 || d_t_w < 500) {     // moves forward until the flex sensor on the front of the bot is bent
      done = true;
    }
  }
  gripper.write(45);             // opens girpper
  delay(50);
  back_up();                     // backs up bot
  turn_around();                 // turns the bot around
  forward();                     // bot moves forward for 8 seconds
  delay(4.5e3);
  stop_bot();                    // stops the bot
  while (true) {                 // waits til start button is pressed to begin manual control
    ps2x.read_gamepad(false, vibrate);
    if (ps2x.Button(PSB_START)) {
      PS2_control();
    }
  }
}

// backs up the robot for 1 second
void back_up() {
  backward();
  delay(1500);
}

// reads the flex sensor's value
int flexer (int flexpin) {
  int flexposition;    // Input value from the analog pin.
  int servoposition;   // Output value to the servo.

  // Read the position of the flex sensor (0 to 1023):

  return flexposition = analogRead(flexpin);
}

// nudges bot towards the wall to the left
void bump_wall() {
  right_wheel.writeMicroseconds(1300);
  left_wheel.writeMicroseconds(1545);
  delay(50);
}

void bump_left() {
  left_wheel.writeMicroseconds(1700);
  right_wheel.writeMicroseconds(1440);
  delay(50);
}

// turns robot around
void turn_around() {
  right_wheel.writeMicroseconds(1700);
  left_wheel.writeMicroseconds(1700);
  delay(1100);
  stop_bot();
}

// turns robot 90 degrees right
void turn_right() {
  right_wheel.writeMicroseconds(1700);
  left_wheel.writeMicroseconds(1700);
  delay(500);
  stop_bot();
}

// uses the sonar sensor to detect how far away objects are and returns the distance in inches.
int distance_to(int pingPin) {
  long duration, inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  delay (20);
  return inches;
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

// reads values from PS controller and then designates actions for the robot to do
void PS2_control() {
  while (true) {
    ps2x.read_gamepad(false, vibrate);
    int LJx = ps2x.Analog(PSS_RX);
    Serial.println(LJx);
    if (ps2x.Button(PSB_CROSS)) {          // opens gripper
      gripper_close();
    }
    if (ps2x.Button(PSB_CIRCLE)) {         // closes gripper
      gripper_open();
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
    if (ps2x.Button(PSB_PAD_UP)) {
      forward_tap();
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {
      backward_tap();
    }
    if (ps2x.Button(PSB_PAD_LEFT)) {
      left_tap();
    }
    if (ps2x.Button(PSB_PAD_RIGHT)) {
      right_tap();
    }
    if (ps2x.Button(PSB_SQUARE)) {
      forward();
      delay(4e3);
    }
    if (ps2x.Button(PSB_R1)) {
      veer_right();
    }
    if (ps2x.Button(PSB_L1)) {
      veer_left();
    }
    if (abs((LJx - 128)) < 10 && !ps2x.Button(PSB_R2) && !ps2x.Button(PSB_L2) && !ps2x.Button(PSB_L1) && !ps2x.Button(PSB_R1)) {
      stop_bot();                          // if nothing happens on controller, the bot is stopped.
    }
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

// stops the robot
void stop_bot() {
  left_wheel.writeMicroseconds(1490);
  right_wheel.writeMicroseconds(1490);
}

// spins the robot
void  robot_spin(int val) {
  int rw, lw, difference;
  difference = map(val, 0, 250, -200, 200);
  lw = 1490 + difference;
  rw = 1490 + difference;
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
  left_wheel.writeMicroseconds(1680);
  right_wheel.writeMicroseconds(1360);
}


void forward_tap() {
  left_wheel.writeMicroseconds(1680);
  right_wheel.writeMicroseconds(1360);
  delay(pad_delay);
  stop_bot();
}

void backward_tap() {
  left_wheel.writeMicroseconds(1280);
  right_wheel.writeMicroseconds(1680);
  delay(pad_delay);
  stop_bot();
}

void left_tap() {
  left_wheel.writeMicroseconds(1300);
  right_wheel.writeMicroseconds(1300);
  delay(pad_delay);
  stop_bot();
}

void right_tap() {
  left_wheel.writeMicroseconds(1700);
  right_wheel.writeMicroseconds(1700);
  delay(pad_delay);
  stop_bot();
}

void veer_right() {
  left_wheel.writeMicroseconds(1700);
  right_wheel.writeMicroseconds(1450);
}

void veer_left() {
  left_wheel.writeMicroseconds(1550);
  right_wheel.writeMicroseconds(1300);
}
