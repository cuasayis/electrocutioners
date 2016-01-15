// Inlude the Adafruit_NeoPixel library.
#include <Adafruit_NeoPixel.h>

// Define constants for pin values.
const int BUTTON_PIN = 2;
const int PIXEL_PIN = 6;
const int PIXEL_COUNT  50;

// Define arrays for Tetrimino pieces.
int yellow_block[] = {48, 49, 43, 44, 255, 255, 0};
int purble_T[] = {47, 48, 49, 43, 128, 0, 128};
int blue_L[] = {47, 42, 43, 44, 0, 0, 255};
int orange_L[] = {49, 42, 43, 44, 255, 165, 0};
int green_Z[] = {48, 49, 42, 43, 0, 255, 0};
int red_Z[] = {47, 48, 43, 44, 255, 0, 0};
int cyan_stick[] = {41, 42, 43, 44, 0, 255, 255};
int box[7];

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pixels.begin();
  pixels.show();
  for (int i = 0; i < 7; i++) {
    box[i] = green_Z[i];
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int c = 9;
  while (c > 0) {
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(box[4], box[5], box[6]));
    }
    pixels.show();
    delay(500);
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(0, 0, 0));
    }
    pixels.show();
    drop_pixels();
    c = c - 1;
  }
}

void drop_pixels() {
  for (int i = 0; i < 4; i++) {
    box[i] = box[i] - 5;
  }
}
