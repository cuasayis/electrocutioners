test from Ryan
#include <Adafruit_NeoPixel.h>
#define BUTTON_PIN 2
#define PIXEL_PIN 6
#define PIXEL_COUNT  50
int yellow_block[4] = {48,49,43,44,255,255,0};
int purble_T[4] = {47,48,49,43,128,0,128};
int blue_L[4] = {47,42,43,44,0,0,255};
int orange_L[4] = {49,42,43,44,255,165,0};
int green_Z[4] = {48,49,42,43,0,255,0};
int red_Z[4] = {47,48,43,44,255,0,0};
int cyan_stick = {41,42,43,44,0,255,255};
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pixels.begin();
  pixels.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  // create_shape(red_Z);
}

void create_shape(shape[7]){
  int c = 0;
  while (c > 0){
    for (int i = 0; i<4; i++){
     pixels.setPixelColor(shape[i], pixels.Color(shape[4],shape[5],shape[6)); 
    }
    pixels.show();
    delay(500);
    for (int i = 0; i<4; i++){
     pixels.setPixelColor(shape[i], pixels.Color(0,0,0)); 
    }
    pixels.show();
    drop_pixels();
    c++;
  }
}

void drop_pixels(){
  for (int i = 0; i<4; i++){
    box[i]=box[i]-5;
  }
}
