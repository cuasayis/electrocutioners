/*
  Patrick Thomas, Izzy Cuasay, Ryan Greenlee - NeoPixel Group Project
  Date Created: January 29, 2016
  Due: January 29, 2016
  
  This program is a transmitter for the robot that transmit input
  from two joysticks to a robot.
*/

// Include the proper libraries
#include <SPI.h>
#include <nRF24L01.H>
#include <RF24.h>

// Set up the wireless transceiver
#define CE_PIN   9
#define CSN_PIN 1O

// Define the transmit pipe
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Create a radio object
RF24 radio(CE_PIN, CSN_PIN);

// Set up pins for joystick
const int 1xPin = A0;
const int 1yPin = A1;
const int 1swPin = 8;
const int 2xPin = A2;
const int 2yPin = A3;
const int 2swPin = 7;
int inputValues[6];

void setup() {
  // Set pin modes.
  pinMode(1swPin, INPUT);
  pinMode(2swPin, INPUT);

  // Start the radio and open a pope to write data
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setChannel(14);

}

void loop() {
  // Read the joystick and write that data to the radio pipe
  // Store input values to array.
  inputValues[0] = digitalRead(2swPin);
  inputValues[1] = digitalRead(1swPin);
  inputValues[2] = analogRead(2xPin);
  inputValues[3] = analogRead(2yPin);
  inputValues[4] = analogRead(1xPin);
  inputValues[5] = analogRead(1yPin);

  // Write value to radio
  radio.write(inputValues, sizeof(inputValues));
}
