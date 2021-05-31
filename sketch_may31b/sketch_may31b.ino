//trying to display active temperature on display
#include "M5Atom.h"
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//constant vars
#define DISPLAY_PIN 27

//vars
bool IMU6886Flag = false;
uint8_t FSM = 0;
float tempC = 0;
CRGB ListOfColors[] = {0x000000, 0x00ff00, 0xffffff}; //black, red, white
float tempInC = 0; //Temperature in Celcius
float tempInF = 0; //Temperature in Farengate
//Using Accelerometer for understanding orientation of the devise
float accX = 0;
float accY = 0;
float accZ = 0;

//To make it clear that the screen is activated, I want to draw a circle
int circleWithX[25] =
{
  0, 1, 1, 1, 0,
  1, 0, 2, 0, 1,
  1, 2, 2, 2, 1,
  1, 0, 2, 0, 1,
  0, 1, 1, 1, 0
};

//Functions
//this function will be used to display circle on the screen
void drawMatrix(int mat[], int color1)
{
  for (int i = 0; i < 25; i++)
  {
    M5.dis.drawpix(i, color1);
  }
}

//Inspired by Mike Klepper's article I will use Accelerometer to see if screen is facing upwards
/*
   Screen Up:
   |accX| < LOW_TOL -15, |accY| < LOW_TOL, accZ ~ -980
*/
//This is approach Mike used for orientation detection. Credits to Mike!
float LOW_TOL = 100;
float HIGH_TOL = 900;

float scaledAccX = 0;
float scaledAccY = 0;
float scaledAccZ = 0;



//SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  M5.begin(true, false, true);
  IMU6886Flag = M5.IMU.Init() == 0;
  //Catching Error with IMU initialization
  if (!IMU6886Flag)
  {
    Serial.println("ERROR with IMU");
  }
}


//VOID LOOP
void loop() {
  if (IMU6886Flag) {
    //Temperature is being measured constanly on the background
    M5.IMU.getTempData(&tempInC);
    Serial.printf(" Temp : %.2f C \r\n", tempInC);
    float tempInF = tempC * 9 / 5 + 32;
    Serial.printf(" Temp : %.2f F \r\n", tempInF);

    //Taking input from Accelerometer 
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    Serial.printf("Accel: %.2f, %.2f, %.2f mg\r\n", accX * 1000, accY * 1000, accZ * 1000);
    scaledAccX = accX * 1000;
    scaledAccY = accY * 1000;
    scaledAccZ = accZ * 1000;
    
    //Here we need to Activate the screen and conditions are facing it upwards and pressing a btn
    if (abs(scaledAccX) < LOW_TOL && abs(scaledAccY) < LOW_TOL && abs(scaledAccZ) > HIGH_TOL && scaledAccZ < 0)
    {
      drawMatrix(circleWithX, 0x00ff00);
    }
    else {
      break; 
    }













  }
}
