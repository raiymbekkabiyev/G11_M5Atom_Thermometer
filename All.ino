//Libraries
#include "M5Atom.h"
//...for text scroll
#include <FastLED.h>
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


//Variables
int white (0xffffff), red (0x00ff00), orange (0xa5ff00), yellow (0xffff00), green (0xff0000), blue (0x0000ff), purple (0x008080), black (0x000000);  //Helps reference colors codes by name
int palette[] = {black, white, red};  //Select color palette by putting in order (credit: Mike Klepper)
int paletteB[] = {black, purple, red};
//...for menu
bool boolUp = false, stayingRight = false, stayingLeft = false;
float rightTiltTimeCheck, leftTiltTimeCheck;
float tiltThreshold (.5);  //We tested to find this value which defines the angles at which the device must be tilted at to be considered as facing certain directions
float aX, aY, aZ;  //Acceleration data
int tiltState (0);  //0=error, 1=down, 2=up, 3=facing you, 4=facing away from you, 5=right, 6=left
int unitsFlag (0);  //Units of temperature displayed; 0=C, 1=F
int menuMode (0);  //0=off, 1=show temp and units, 2=show 24hr avg and units, 3=show color scale and current temp color, 4=show temp graph over predefined range, 5=change units
bool isActive(false), modeIsActive(true);
#define CHECKINTERVAL 500  //How long you have to tilt the device to change modes (milliseconds)
//...for recording temp
#define INTERVAL 5000; //How often to record temperature (milliseconds)
unsigned long targetTime (0);
float every5s [60], every1m [60], every1h [60], every1d [60], mAvg, hAvg, dAvg(999.9), temp(999.99), tempF(999.99);
int sCtr (0), mCtr (0), hCtr (0);
//...for determining temp range (in C)
float veryCold (-20.0);  // tempC < -20 is purple
float cold (0);  // -20 < tempC < -0 is blue
float chill (20);  // 0 < tempC < 20 is green
float warmMax (40);  // 20 < tempC < 40 is yellow;  tempC > 40 is extremly hot = red color
//...for graphing
int temporaryColor (white);
//...for test scroll
#define PIN 27
int scrollSpeed (125);
float targetTimeScroll (0);  //##testing
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);  //Constructor
const uint16_t colors[] = { matrix.Color(255, 255, 255), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };  //First color is what displays



//Following is a series of arrays of pixel data which can be used to code an image on the display (inspired by Mike Klepper's code)
int zero[25] = { 0, 0, 1, 0, 0,
                 0, 1, 0, 1, 0,
                 0, 1, 0, 1, 0,
                 0, 1, 0, 1, 0,
                 0, 0, 1, 0, 0,
               };

int one[25] = { 0, 0, 1, 0, 0,
                0, 1, 1, 0, 0,
                0, 0, 1, 0, 0,
                0, 0, 1, 0, 0,
                0, 1, 1, 1, 0,
              };

int two[25] = { 0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 0, 0, 0,
                0, 1, 1, 1, 0,
              };

int three[25] = { 0, 1, 1, 1, 0,
                  0, 0, 0, 1, 0,
                  0, 1, 1, 1, 0,
                  0, 0, 0, 1, 0,
                  0, 1, 1, 1, 0,
                };

int four[25] = { 0, 1, 0, 1, 0,
                 0, 1, 0, 1, 0,
                 0, 1, 1, 1, 0,
                 0, 0, 0, 1, 0,
                 0, 0, 0, 1, 0,
               };

int five[25] = { 0, 1, 1, 1, 0,
                 0, 1, 0, 0, 0,
                 0, 1, 1, 1, 0,
                 0, 0, 0, 1, 0,
                 0, 1, 1, 1, 0,
               };

int bigC[25] = { 0, 0, 1, 1, 0,
                 0, 1, 0, 0, 0,
                 0, 1, 0, 0, 0,
                 0, 1, 0, 0, 0,
                 0, 0, 1, 1, 0,
               };

int bigF[25] = { 0, 1, 1, 1, 0,
                 0, 1, 0, 0, 0,
                 0, 1, 1, 1, 0,
                 0, 1, 0, 0, 0,
                 0, 1, 0, 0, 0,
               };





//Functions

//Function to shift values of a 60-element array over one and replace the first value (for recording temp)
void shiftFloatArray(float myArray[], float firstValue) {
  for (int ctr = 59; ctr > 0; ctr--)
    myArray[ctr] = myArray[ctr - 1];
  myArray[0] = firstValue;
}

//Function to to find average of certin number of elements within a 60-element (for recording temp)
float average(float array[60], int averageOf) {
  float tempSum (0);
  for (int ctr = 0; ctr < averageOf; ctr++)
    tempSum += array[ctr];
  return (tempSum / (float)averageOf);
}

//Function that takes a 25 element matrix of image data and prints it on the display (for menu numbers) (credit: Mike Klepper)
void drawArray(int shapeMatrix[], int colors[]) {
  for (int i = 0; i < 25; i++)
    M5.dis.drawpix(i, colors[shapeMatrix[i]]);
}

//Function that returns a color relative to the temperature (for color of current temp and graph)
int tempToColor(float tempC) {
  if (tempC < veryCold)
    return (purple);
  else if (tempC < cold)
    return (blue);
  else if (tempC < chill)
    return (green);
  else if (tempC < warmMax)
    return (yellow);
  else
    return (red);
}

//Fill top bar with color range
void tempRangeBar() {
  M5.dis.drawpix(0, purple);
  M5.dis.drawpix(1, blue);
  M5.dis.drawpix(2, green);
  M5.dis.drawpix(3, yellow);
  M5.dis.drawpix(4, red);
}


void setup() {
  Serial.begin(115200);
  M5.begin(true, false, true);
  M5.IMU.Init();  //Starts up the hardware in the device which detects acceleration, gyration, and temperature
  M5.dis.clear();

  for (int ctr = 0; ctr < 60; ctr++)
    every5s[ctr] = every1m[ctr] = every1h[ctr] = every1d[ctr] = 999.99;  //Initialize temp storage

  //For scroll text
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(colors[0]);
  matrix.setFont(&TomThumb);
}


int x  = matrix.height();  //For scroll text



void loop() {

  if ( millis() > targetTime) {  //If it's time to record the temperature
    M5.IMU.getTempData(&temp);  //Get the current temperature from the device and store it in the variable temp
    tempF = temp * 9.0 / 5.0 + 32;

    shiftFloatArray(every5s, temp);
    sCtr++;

    if (sCtr == 12)  {  //If it has been a minute
      mAvg = average(every5s, 12);
      shiftFloatArray(every1m, mAvg);
      mCtr++;

      if (mCtr == 60) {  //If it has been an hour
        hAvg = average(every1m, 60);
        shiftFloatArray(every1h, hAvg);
        hCtr++;

        if (hCtr == 24) {  //If it has been a day
          dAvg = average(every1h, 24);
          shiftFloatArray(every1d, dAvg);
          hCtr = 0;
        }

        mCtr = 0;
      }

      sCtr = 0;
    }

    targetTime += INTERVAL;  //Set the new target time to one interval away from the last target time
  } //End of what happens if it's time to record the temperature


  M5.IMU.getAccelData(&aX, &aY, &aZ);  //Gets acceleration data from the device

  //Determines which way the device is facing
  if (abs(aX) < tiltThreshold && abs(aY) < tiltThreshold && aZ > tiltThreshold)
    tiltState = 1;
  else if (abs(aX) < tiltThreshold && abs(aY) < tiltThreshold && aZ < -tiltThreshold)
    tiltState = 2;
  else if (abs(aX) < tiltThreshold && aY > tiltThreshold && abs(aZ) < tiltThreshold)
    tiltState = 3;
  else if (abs(aX) < tiltThreshold && aY < -tiltThreshold && abs(aZ) < tiltThreshold)
    tiltState = 4;
  else if (aX > tiltThreshold && abs(aY) < tiltThreshold && abs(aZ) < tiltThreshold)
    tiltState = 5;
  else if (aX < -tiltThreshold && abs(aY) < tiltThreshold && abs(aZ) < tiltThreshold)
    tiltState = 6;
  else
    tiltState = 0;


  if (tiltState == 2 && M5.Btn.wasPressed()) {  //Turns screen on if faced up and button is pressed
    isActive = true;
    if (menuMode == 0)
      menuMode = 1;
  }

  if (tiltState == 1) {  //Turns screen off if faced down
    isActive = false;
    M5.dis.clear();
    menuMode = 0;
    modeIsActive = true;
  }

  if (isActive == true) {  //Dictates operation if on
    if (M5.Btn.wasPressed())  //
      modeIsActive = !modeIsActive;


    if (boolUp == true) {
      if (tiltState == 5) {
        rightTiltTimeCheck = millis() + CHECKINTERVAL;
        stayingRight = true;
      }
      else if (tiltState == 6) {
        leftTiltTimeCheck = millis() + CHECKINTERVAL;
        stayingLeft = true;
      }
    }

    if (tiltState == 2 || tiltState == 0)
      boolUp = true;
    else
      boolUp = false;

    if (millis() < rightTiltTimeCheck && tiltState != 5)
      stayingRight = false;

    if (millis() > rightTiltTimeCheck && stayingRight == true) {
      if (!modeIsActive) {
        menuMode++;
        if (menuMode == 6)
          menuMode = 1;
      }
      else if (menuMode == 5) {
        if (unitsFlag == 0) {
          unitsFlag = 1;
        }
        else if (unitsFlag == 1) {
          unitsFlag = 0;
        }
      }
      stayingRight = false;
    }

    if (millis() < leftTiltTimeCheck && tiltState != 6)
      stayingLeft = false;

    if (millis() > leftTiltTimeCheck && stayingLeft == true) {
      if (!modeIsActive) {
        menuMode--;
        if (menuMode == 0)
          menuMode = 5;
      }
      else if (menuMode == 5) {
        if (unitsFlag == 0) {
          unitsFlag = 1;
        }
        else if (unitsFlag == 1) {
          unitsFlag = 0;
        }
      }
      stayingLeft = false;
    }


    switch (menuMode) {  //Dictates what happens in each mode if active or not

      case 1:
        if (!modeIsActive) {
          drawArray(one, palette);
        }
        else {  //Show Active temperature + Units
          drawArray(one, paletteB);

          if ( millis() > targetTimeScroll) {
            matrix.fillScreen(0);
            matrix.setCursor(x, matrix.height());
            if (unitsFlag == 0)
              matrix.printf("%.2f C \r\n", temp);
            else
              matrix.printf("%.2f F \r\n", tempF);
            if (--x < -60) {
              x = matrix.width();
            }
            //                      matrix.show();  //##Debug
            targetTimeScroll += scrollSpeed;  //Set the new target time to one interval away from the last target time
          }
        }
        break;

      case 2:
        if (!modeIsActive)
          drawArray(two, palette);
        else  //Show average of last 24 hours of temperature + Units
          drawArray(two, paletteB);  //##will put in after case 1 is debugged
        break;

      case 3:
        if (!modeIsActive)
          drawArray(three, palette);
        else {  //Show color scale of temperature range + current temperature as color
          M5.dis.fillpix(tempToColor(temp));
          tempRangeBar();
        }
        break;

      case 4:
        if (!modeIsActive)
          drawArray(four, palette);
        else {  //Show graph of temperature across a predefined range (25 hours)
          M5.dis.clear();
          for (int i = 0; i < 25; i++)
          {
            if (every1h[i] < 900) {
              temporaryColor = tempToColor(every1h[i]);
              M5.dis.drawpix(i, temporaryColor);
            }
          }
        }
        break;

      case 5:
        if (!modeIsActive)
          drawArray(five, palette);
        else {  //Change units
          if (unitsFlag == 0) {
            drawArray(bigC, paletteB);
          }
          else if (unitsFlag == 1) {
            drawArray(bigF, paletteB);
          }
        }
        break;

      default:  //If in error state screen goes red
        M5.dis.fillpix(0x00ff00);

    }  //End of menu switch

  }  //End of isActive


  M5.update();
}  //End of loop
