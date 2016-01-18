/*NeoPixel Group Project - ECE 160 - Engineering Practice
Ryan Greenlee, Izzy Cuasay, and Patrick Tomas
Revised: 1/18/ 2016

This program creates a tetris game using Adafruit Neopixel.
*/

// Include the Adafruit_NeoPixel library
#include <Adafruit_NeoPixel.h>

// Define constants for pin values
const int BUTTON_PIN = 2;
const int PIXEL_PIN = 6;
const int PIXEL_COUNT = 50;

// Define arrays for Tetrimino pieces.
int yellow_block[] = {48, 49, 43, 44, 255, 255, 0};
int purple_T[] = {47, 48, 49, 43, 128, 0, 128};
int blue_L[] = {47, 42, 43, 44, 0, 0, 255};
int orange_L[] = {49, 42, 43, 44, 255, 165, 0};
int green_Z[] = {48, 49, 42, 43, 0, 255, 0};
int red_Z[] = {47, 48, 43, 44, 255, 0, 0};
int cyan_stick[] = {46, 47, 48, 49, 0, 255, 255};
int box[7];
int game_board[10][5];
int r;
int g;
int b;
bool oldbuttonState;

// Define pixels for the NeoPixel device
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int showType = 0;

void setup() {
  // Set pin mode for button.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize pixels
  pixels.begin();
  pixels.show();

  // Map all the pixels to game_board
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      int rn = random(1,8);
      game_board[i][j] = rn;
    }
  }
  
  //color_board();
  oldbuttonState = HIGH;
}

void loop() {
  //drop_shape();
  //color_board();
  //random_drop();
  button_push();
}

void color_board() {
  // Color the board with random colors.
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      if (game_board[i][j] == 0) {
        r = 0;
        g = 0;
        b = 0;
      }
      else if (game_board[i][j] == 1) {      // yellow
        r = 255;
        g = 255;
        b = 0;
      }
      else if (game_board[i][j] == 2) {      // purple
        r = 128;
        g = 0;
        b = 128;
      }
      else if (game_board[i][j] == 3) {      // blue
        r = 0;
        g = 0;
        b = 255;
      }
      else if (game_board[i][j] == 4) {      // orange
        r = 255;
        g = 165;
        b = 0;
      }
      else if (game_board[i][j] == 5) {      // green
        r = 0;
        g = 255;
        b = 0;
      }
      else if (game_board[i][j] == 6) {      // red
        r = 255;
        g = 0;
        b = 0;
      }
      else if (game_board[i][j] == 7) {     // cyan
        r = 0;
        g = 255;
        b = 255;
      }
      int pos = 5 * i + j;
      pixels.setPixelColor(pos, pixels.Color(r, g, b));
    }
  }
  pixels.show();
}

void random_drop(){
  // Selects a random shape to drop on the NeoPixel board.
  int rn = random(1,8);
  switch (rn){
    case 1:
    for (int i = 0; i < 7; i++) {
      box[i] = yellow_block[i];
    }
    break;
    case 2:
    for (int i = 0; i < 7; i++) {
      box[i] = purple_T[i];
    }
    break;
    case 3:
    for (int i = 0; i < 7; i++) {
      box[i] = blue_L[i];
    }
    break;
    case 4:
    for (int i = 0; i < 7; i++) {
      box[i] = orange_L[i];
    }
    break;
    case 5:
    for (int i = 0; i < 7; i++) {
      box[i] = green_Z[i];
    }
    break;
    case 6:
    for (int i = 0; i < 7; i++) {
      box[i] = red_Z[i];
    }
    break;
    case 7:
    for (int i = 0; i < 7; i++) {
      box[i] = cyan_stick[i];
    }
    break;
  }

  // Calls function to display shape on the board.
  drop_shape();
}

void button_push(){
  // Drops a shape when button is pressed.
  int buttonState = digitalRead(2);
  if (buttonState == LOW && oldbuttonState == HIGH)
  {
    random_drop();
    oldbuttonState = LOW;
  }
  if (buttonState == HIGH){
    oldbuttonState = HIGH;
  }
}

void drop_pixels() {
  // Moves piexel down the board.
  for (int i = 0; i < 4; i++) {
    box[i] = box[i] - 5;
  }
}

void drop_shape() {
  // Moves shape down the board.
  int c;
  //for (int i = 0; i < 7; i++) {
  //  box[i] = purple_T[i];
  //}
  for (c = 0; c < 10; c++) {
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
  }
}
