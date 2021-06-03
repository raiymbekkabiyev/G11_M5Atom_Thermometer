#include "M5Atom.h"
#include <FastLED.h>
//Libraries for text scroll
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>

#include <iostream>
#include <sstream>
using namespace std;

#ifndef PSTR
#define PSTR
#endif

#define PIN 27
#define INTERVAL 5000

//vars
bool IMU6886Flag = false;
int scrollSpeed = 150;
float tempC = 0.0; //Temperature in Celcius
float tempF = 0.0;
float targetTime (0);


//constructor
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = { matrix.Color(255, 255, 255), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };  //First color is what displays

void setup() {
  Serial.begin(115200);
  M5.begin(true, false, true);
  IMU6886Flag = M5.IMU.Init() == 0;
  if (!IMU6886Flag)
  {
    Serial.println("ERROR with IMU");
  }
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(colors[0]);
  matrix.setFont(&TomThumb);
}

int x  = matrix.height();

void loop() {
  if (IMU6886Flag) {
    //reading temperature
    if ( millis() > targetTime) {  //If it's time to record the temperature
      M5.IMU.getTempData(&tempC);  //Get the current temperature from the device and store it in the variable temp
      targetTime += INTERVAL;  //Set the new target time to one interval away from the last target time
    }

    matrix.fillScreen(0);
    matrix.setCursor(x, matrix.height());
    matrix.printf("%.2f C \r\n", tempC);
    if (--x < -60) {
      x = matrix.width();
    }
    matrix.show();
    delay(scrollSpeed);
  }
}
