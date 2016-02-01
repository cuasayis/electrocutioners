/*
  Patrick Thomas, Izzy Cuasay, Ryan Greenlee - Robunny Rumble Project
  Date Created: January 29, 2016
  Due: February 2, 2016
  
  This program is a transmitter for the robot that transmit input
  from two joysticks to a robot.
*/

// Include the proper libraries
#include <SPI.h>
#include <nRF24L01.H>
#include <RF24.h>

// Set up the wireless transceiver
#define CE_PIN   9
#define CSN_PIN 10

// Define the transmit pipe
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Create a radio object
RF24 radio(CE_PIN, CSN_PIN);

// Set up pins for joystick
const int xPin1 = A0;
const int yPin1 = A1;
const int swPin1 = 5;
const int xPin2 = A2;
const int yPin2 = A3;
const int swPin2 = 6;
int inputValues[6];

void setup() {
  // Set pin modes.
  pinMode(swPin1, INPUT);
  pinMode(swPin2, INPUT);
  digitalWrite(swPin1, HIGH);
  digitalWrite(swPin2, HIGH);

  // Start the radio and open a pope to write data
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setChannel(19);

  Serial.begin(9600);

}

void loop() {
  // Read the joystick and write that data to the radio pipe
  // Store input values to array.
  inputValues[0] = digitalRead(swPin1);
  inputValues[1] = digitalRead(swPin2);
  inputValues[2] = analogRead(xPin2);
  inputValues[3] = analogRead(yPin2);
  inputValues[4] = analogRead(xPin1);
  inputValues[5] = analogRead(yPin1);

  // Write value to radio
  radio.write(inputValues, sizeof(inputValues));

  Serial.println(inputValues[0]);
  Serial.println(inputValues[1]);
}
