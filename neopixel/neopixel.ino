/*NeoPixel Group Project - ECE 160 - Engineering Practice
Ryan Greenlee, Izzy Cuasay, and Patrick Thomas
Revised: 1/25/ 2016

This program creates a tetris game using Adafruit Neopixel.
*/

// Include the Adafruit_NeoPixel library
#include <Adafruit_NeoPixel.h>

// Define constants for pin values
#define BUTTON_PIN 2
#define PIXEL_PIN 6
#define PIXEL_COUNT  100

// Define arrys for Tetrimino pieces
int yellow_block[] = {98, 99, 93, 94, 255, 255, 0, 12, 1};
int purple_T[] = {99, 98, 97, 93, 128, 0, 128, 22, 2};
int blue_L[] = {97, 92, 93, 94, 0, 0, 255, 4, 3};
int orange_L[] = { 92, 93, 94, 99, 255, 165, 0, 8, 4};
int green_Z[] = { 92, 93, 98, 99, 0, 255, 0, 16, 5};
int red_Z[] = {97, 98, 93, 94, 255, 0, 0, 24, 6};
int cyan_stick[] = {96, 97, 98, 99, 0, 255, 255, 0, 7};
int box[9];
int game_board[20][5];
int r;
int g;
int b;
int score;
bool oldbuttonState;
const int xPin = A0;
const int yPin = A1;
int xAxis, yAxis;
bool oldState;
bool tracker;
float time_played = 0;
bool winner;

// Deine pixels for the NeoPixel device
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int showType = 0;

void setup() {
  // Set pin mode for button
  pinMode(9, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize pixels
  pixels.begin();
  pixels.show();
  score = 0;

  // Map all the pixels to game_board
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 5; j++) {
      game_board[i][j] = 0;
    }
  }
  oldbuttonState = HIGH;
  Serial.begin(9600);
}

void loop() {
  button_push();
}

void button_push() {
  // Starts game when button is pressed
  int buttonState = digitalRead(9);
  if (buttonState == LOW && oldbuttonState == HIGH)
  {
    oldbuttonState = LOW;
    tetris_music();
    game_play();
  }
  if (buttonState == HIGH) {
    oldbuttonState = HIGH;
  }
}

void tetris_music()
{
  // constants for tetris song
  const int buzzerPin = 13; // piezo buzzer pin
  char notes[] = "EbCDCbaaCEDCbCDECaa";
  int beats[] = {2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1};
  int tempo = 200;
  int songLength = 19;
  int i, duration;

  for (i = 0; i < songLength; i++) // step through song arrays
  {
    duration = beats[i] * tempo; // how long note is played
    tone(buzzerPin, frequency(notes[i]), duration);
    delay(duration);
  }
}

void game_play() {
  // Run the game.
  winner = true;
  bool exit_ = true;
  while (score < 30 && exit_) {
    random_drop();
    for (int j = 0; j < 5; j++) {
      if (game_board[19][j] != 0) {
        you_lose();
        exit_ = false;
        break;
      }
    }
  }
  if (winner) {
    you_win();
  }
}

void you_win() {
  // Function to run when player wins.
  win_music();
  Serial.println("Congrats you won!!!!");
  Serial.print("Time Played: ");
  Serial.print(time_played);
  Serial.print(" | Score: ");
  Serial.println(score);
  theaterChaseRainbow(10);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 5; j++) {
      game_board[i][j] = 0;
    }
  }
  color_board();
  time_played = 0;
  score = 0;
}

void win_music()
{
  const int buzzerPin = 13;
  char win_notes[] = "abCDFAGFECEDEbbCDECaa";
  int win_beats[] = {1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2};
  int win_tempo = 200;
  int win_songLength = 21;
  int k, win_duration;

  for (k = 0; k < win_songLength; k++) // step through song arrays
  {
    win_duration = win_beats[k] * win_tempo; // how long note is played
    tone(buzzerPin, frequency(win_notes[k]), win_duration);
    delay(win_duration);
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (int i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void you_lose() {
  // Function to run when player loses
  lose_music();
  Serial.println("Sorry, You Lost. Try Again");
  Serial.print("Time Played: ");
  Serial.print(time_played);
  Serial.print(" | Score: ");
  Serial.println(score);
  delay(4000);
  winner = false;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 5; j++) {
      game_board[i][j] = 0;
    }
  }
  color_board();
  time_played = 0;
  score = 0;
}

void lose_music() {
  // Music to play when player loses.
  const int buzzerPin = 13;
  char lose_notes[] = "cegCgec";
  int lose_beats[] = {2,2,2,2,2,2,2};
  int lose_tempo = 300;
  int lose_songLength = 7;
  int l, lose_duration;

  for (l = 0; l < lose_songLength; l++) // step through song arrays
  {
    lose_duration = lose_beats[l] * lose_tempo; // how long note is played
    tone(buzzerPin, frequency(lose_notes[l]), lose_duration);
    delay(lose_duration);
  }
}


void color_board() {
  // Colors the NeoPixel board
  for (int i = 0; i < 20; i++) {
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

void random_drop() {
  // Places random piece on the board
  int rn = random(1, 8);
  switch (rn) {
    case 1:
      for (int i = 0; i < 9; i++) {
        box[i] = yellow_block[i];
      }
      break;
    case 2:
      for (int i = 0; i < 9; i++) {
        box[i] = purple_T[i];
      }
      break;
    case 3:
      for (int i = 0; i < 9; i++) {
        box[i] = blue_L[i];
      }
      break;
    case 4:
      for (int i = 0; i < 9; i++) {
        box[i] = orange_L[i];
      }
      break;
    case 5:
      for (int i = 0; i < 9; i++) {
        box[i] = green_Z[i];
      }
      break;
    case 6:
      for (int i = 0; i < 9; i++) {
        box[i] = red_Z[i];
      }
      break;
    case 7:
      for (int i = 0; i < 9; i++) {
        box[i] = cyan_stick[i];
      }
      break;
  }
  drop_shape();
}

void drop_shape() {
  // Move piece down the board
  tracker = true;
  while (tracker) {
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(box[4], box[5], box[6]));
    }
    pixels.show();
    drop_delay();
    if (!tracker) {
      break;
    }
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(0, 0, 0));
    }
    pixels.show();
    drop_pixels();
  }
  add_to_gameboard();
  row_check();
}

void drop_delay() {
  // Delays dropping of the piece and reads joystick to move the
  // piece based on input.
  for (int i = 0; i <= 10; i++) {
    delay(50);
    time_played += 0.05;
    xAxis = analogRead(xPin);
    yAxis = analogRead(yPin);
    int buttonState = digitalRead(2);
    if (buttonState == HIGH) {
      oldbuttonState = HIGH;
    }
    if (xAxis < 650 && yAxis < 650 && yAxis > 350) {
      oldState = LOW;
    }
    if (xAxis > 650 && oldState == LOW) {
      rotate_shape();
      oldState = HIGH;
    }
    if (yAxis > 650 && oldState == LOW) {
      move_sideways(1);
      oldState = HIGH;
    }
    if (yAxis < 350 && oldState == LOW) {
      move_sideways(-1);
      oldState = HIGH;
    }
    if (buttonState == LOW && oldbuttonState == HIGH)
    {
      drop_to_bottom();
      oldbuttonState = LOW;
      tracker = false;
      break;
    }
    test_stacker();
    for (int i = 0; i < 4; i++) {
      if (box[i] / 5 == 0) {
        tracker = false;
        break;
      }
    }
    if (!tracker) {
      break;
    }
  }
}

void add_to_gameboard() {
  int r;
  int c;
  for (int i = 0; i < 4; i++) {
    r = box[i] / 5;
    c = box[i] % 5;
    game_board[r][c] = box[8];
  }
  color_board();
}

void test_stacker() {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 5; j++) {
      if (game_board[i][j] != 0) {
        for (int k = 0; k < 4; k++) {
          if ((box[k] / 5 - i) == 1 && box[k] % 5 == j) {
            tracker = false;
          }
        }
      }
    }
  }
}

void row_check() {
  // Check if row can be cleared and increases the score
  for (int r = 19; r > -1; r--) {
    if (game_board[r][0] != 0 && game_board[r][1] != 0 && game_board[r][2] != 0 && game_board[r][3] != 0 && game_board[r][4] != 0) {
      clear_row(r);
      score++;
    }
  }
  color_board();
}

void clear_row(int row) {
  // Clears a row if row get filled with pieces.
  for (int r = row; r < 20; r++) {
    for (int c = 0; c < 5; c++) {
      if (r != 19) {
        game_board[r][c] = game_board[r + 1][c];
      }
      else {
        game_board[r][c] = 0;
        clear_row_music();
      }
    }
  }
}

void clear_row_music() {
  // Music to be played when row is cleared.
  const int buzzerPin = 13;
  char clear_notes[] = "CDEFGABC";
  int clear_beats[] = {1,1,1,1,1,1,3};
  int clear_tempo = 10;
  int clear_songLength = 7;
  int j, clear_duration;

  // add function for when row is cleared

  for (j = 0; j < clear_songLength; j++) // step through song arrays
  {
    clear_duration = clear_beats[j] * clear_tempo; // how long note is played
    tone(buzzerPin, frequency(clear_notes[j]), clear_duration);
    delay(clear_duration);
  }
}

void move_sideways(int p) {
  // Move the pieces sideways.
  bool possible = HIGH;
  for (int i = 0; i < 4; i++) {
    if (box[i] % 5 == 0 && p == -1) {
      possible = LOW;
      break;
    }
    if (box[i] % 5 == 4 && p == 1) {
      possible = LOW;
      break;
    }
  }
  if (possible == HIGH) {
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(0, 0, 0));
    }
    pixels.show();
    for (int i = 0; i < 4; i++) {
      box[i] = box[i] + p;
    }
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(box[4], box[5], box[6]));
    }
    pixels.show();
  }
}

void drop_pixels() {
  for (int i = 0; i < 4; i++) {
    box[i] = box[i] - 5;
  }
}

void drop_to_bottom() {
  // Drops the piece to the bottom.
  int row_to_bottom ;
  row_to_bottom = box[0] / 5;
  for (int i = 1; i < 4; i++) {
    if ((box[i] / 5) < row_to_bottom) {
      row_to_bottom = (box[i] / 5);
    }
  }
  for (int r = 0; r < 20; r++) {
    for (int c = 0; c < 5; c++) {
      if (game_board[r][c] != 0) {
        for (int i = 0; i < 4; i++) {
          if (box[i] % 5 == c && row_to_bottom > ((box[i] / 5) - r - 1)) {
            row_to_bottom = (box[i] / 5) - r - 1;
          }
        }
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    pixels.setPixelColor(box[i], pixels.Color(0, 0, 0));
  }
  pixels.show();
  for (int i = 0; i < row_to_bottom ; i++) {
    drop_pixels();
  }
  for (int i = 0; i < 4; i++) {
    pixels.setPixelColor(box[i], pixels.Color(box[4], box[5], box[6]));
  }
  pixels.show();
}

void rotate_shape() {
  // Rotates the piece.
  int c1;
  int c2;
  int c3;
  int c4;
  bool allow_turn = true;
  int copy_box[4];
  for (int i = 0; i < 4; i++) {
    copy_box[i] = box[i];
  }
  switch (box[7]) {
    case 0:
      c1 = 7;
      c2 = 1;
      c3 = -5;
      c4 = -11;
      break;
    case 1:
      c1 = -9;
      c2 = -5;
      c3 = -1;
      c4 = 3;
      break;
    case 2:
      c1 = -7;
      c2 = -1;
      c3 = 5;
      c4 = 11;
      break;
    case 3:
      c1 = 9;
      c2 = 5;
      c3 = 1;
      c4 = -3;
      break;
    case 4:
      c1 = 2;
      c2 = 6;
      c3 = 0;
      c4 = -6;
      break;
    case 5:
      c1 = -10;
      c2 = -4;
      c3 = 0;
      c4 = 4;
      break;
    case 6:
      c1 = -2;
      c2 = -6;
      c3 = 0;
      c4 = 6;
      break;
    case 7:
      c1 = 10;
      c2 = 4;
      c3 = 0;
      c4 = -4;
      break;
    case 8:
      c1 = 6;
      c2 = 0;
      c3 = -6;
      c4 = -10;
      break;
    case 9:
      c1 = -4;
      c2 = 0;
      c3 = 4;
      c4 = -2;
      break;
    case 10:
      c1 = -6;
      c2 = 0;
      c3 = 6;
      c4 = 10;
      break;
    case 11:
      c1 = 4;
      c2 = 0;
      c3 = -4;
      c4 = 2;
      break;
    case 12:
      c1 = 0;
      c2 = 0;
      c3 = 0;
      c4 = 0;
      break;
    case 13:
      c1 = 0;
      c2 = 0;
      c3 = 0;
      c4 = 0;
      break;
    case 14:
      c1 = 0;
      c2 = 0;
      c3 = 0;
      c4 = 0;
      break;
    case 15:
      c1 = 0;
      c2 = 0;
      c3 = 0;
      c4 = 0;
      break;
    case 16:
      c1 = 6;
      c2 = 0;
      c3 = -4;
      c4 = -10;
      break;
    case 17:
      c1 = -4;
      c2 = 0;
      c3 = -6;
      c4 = -2;
      break;
    case 18:
      c1 = -6;
      c2 = 0;
      c3 = 4;
      c4 = 10;
      break;
    case 19:
      c1 = 4;
      c2 = 0;
      c3 = 6;
      c4 = 2;
      break;
    case 20:
      c1 = 6;
      c2 = 0;
      c3 = -6;
      c4 = -4;
      break;
    case 21:
      c1 = -4;
      c2 = 0;
      c3 = 4;
      c4 = -6;
      break;
    case 22:
      c1 = -6;
      c2 = 0;
      c3 = 6;
      c4 = 4;
      break;
    case 23:
      c1 = 4;
      c2 = 0;
      c3 = -4;
      c4 = 6;
      break;
    case 24:
      c1 = 2;
      c2 = -4;
      c3 = 0;
      c4 = -6;
      break;
    case 25:
      c1 = -10;
      c2 = -6;
      c3 = 0;
      c4 = 4;
      break;
    case 26:
      c1 = -2;
      c2 = 4;
      c3 = 0;
      c4 = 6;
      break;
    case 27:
      c1 = 10;
      c2 = 6;
      c3 = 0;
      c4 = -4;
      break;
  }
  copy_box[0] = box[0] + c1;
  copy_box[1] = box[1] + c2;
  copy_box[2] = box[2] + c3;
  copy_box[3] = box[3] + c4;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 5; j++) {
      for (int k = 0; k < 4; k++) {
        if (game_board[i][j] != 0 && copy_box[k] / 5 == i && copy_box[k] % 5 == j) {
          allow_turn = false;
          break;
        }
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    if (copy_box[i] > 99) {
      allow_turn = false;
      break;
    }
  }
  int connected_count_1 = 0;
  int connected_count_2 = 0;

  for (int i = 0; i < 4; i++) {
    if ((box[i] / 5 == box[i + 1] / 5 && box[i + 1] % 5 - box[i] % 5 == 1)) {
      connected_count_1++;
    }
    if ((box[i] / 5 == box[i + 1] / 5 && box[i] % 5 - box[i + 1] % 5 == 1)) {
      connected_count_1++;
    }
    if ((box[i] % 5 == box[i + 1] % 5 && box[i] / 5 - box[i + 1] / 5 == 1)) {
      connected_count_1++;
    }
    if ((box[i] % 5 == box[i + 1] % 5 && box[i + 1] / 5 - box[i] / 5 == 1)) {
      connected_count_1++;
    }
  }
  for (int i = 0; i < 3; i++) {
    if ((copy_box[i] / 5 == copy_box[i + 1] / 5 && copy_box[i + 1] % 5 - copy_box[i] % 5 == 1)) {
      connected_count_2++;
    }
    if ((copy_box[i] / 5 == copy_box[i + 1] / 5 && copy_box[i] % 5 - copy_box[i + 1] % 5 == 1)) {
      connected_count_2++;
    }
    if ((copy_box[i] % 5 == copy_box[i + 1] % 5 && copy_box[i] / 5 - copy_box[i + 1] / 5 == 1)) {
      connected_count_2++;
    }
    if ((copy_box[i] % 5 == copy_box[i + 1] % 5 && copy_box[i + 1] / 5 - copy_box[i] / 5 == 1)) {
      connected_count_2++;
    }
  }
  for (int i = 1; i < 4; i++) {
    if ((box[i] / 5 == box[i - 1] / 5 && box[i - 1] % 5 - box[i] % 5 == 1)) {
      connected_count_1++;
    }
    if ((box[i] / 5 == box[i - 1] / 5 && box[i] % 5 - box[i - 1] % 5 == 1)) {
      connected_count_1++;
    }
    if ((box[i] % 5 == box[i - 1] % 5 && box[i] / 5 - box[i - 1] / 5 == 1)) {
      connected_count_1++;
    }
    if ((box[i] % 5 == box[i - 1] % 5 && box[i - 1] / 5 - box[i] / 5 == 1)) {
      connected_count_1++;
    }
  }
  for (int i = 1; i < 4; i++) {
    if ((copy_box[i] / 5 == copy_box[i - 1] / 5 && copy_box[i - 1] % 5 - copy_box[i] % 5 == 1)) {
      connected_count_2++;
    }
    if ((copy_box[i] / 5 == copy_box[i - 1] / 5 && copy_box[i] % 5 - copy_box[i - 1] % 5 == 1)) {
      connected_count_2++;
    }
    if ((copy_box[i] % 5 == copy_box[i - 1] % 5 && copy_box[i] / 5 - copy_box[i - 1] / 5 == 1)) {
      connected_count_2++;
    }
    if ((copy_box[i] % 5 == copy_box[i - 1] % 5 && copy_box[i - 1] / 5 - copy_box[i] / 5 == 1)) {
      connected_count_2++;
    }
  }

  if (connected_count_1 != connected_count_2) {
    allow_turn = false;
  }

  if (allow_turn) {
    if (box[7] % 4 == 3) {
      box[7] = box[7] - 3;
    }
    else {
      box[7]++;
    }
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(box[i], pixels.Color(0, 0, 0));
    }
    pixels.show();
    for (int i = 0; i < 4; i++) {
      pixels.setPixelColor(copy_box[i], pixels.Color(box[4], box[5], box[6]));
    }
    pixels.show();
    for (int i = 0; i < 4; i++) {
      box[i] = copy_box[i];

    }
  }
}

int frequency(char note)
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 13;  // number of notes we're storing

  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A'};
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698};

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return (frequencies[i]);    // Yes! Return the frequency
    }
  }
  return (0); // We looked through everything and didn't find it,
  // but we still need to return a value, so return 0.
}
