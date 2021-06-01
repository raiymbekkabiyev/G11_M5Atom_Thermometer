#include "M5Atom.h"

int white (0xffffff), red (0x00ff00), orange (0xa5ff00), yellow (0xffff00), green (0xff0000), blue (0x0000ff), purple (0x008080), black (0x000000);  //Helps reference colors codes by name
int palette[] = {black, white, red};  //Select color palette by putting in order (credit: Mike Klepper)
int paletteB[] = {black, purple, red};

//Function that takes a 25 element matrix of image data and prints it on the display (credit: Mike Klepper)
void drawArray(int shapeMatrix[], int colors[]) {
  M5.dis.clear();
  for (int i = 0; i < 25; i++) {
    if (colors[shapeMatrix[i]] != black)
      M5.dis.drawpix(i, colors[shapeMatrix[i]]);
  }
}

//Following is a series of arrays of pixel data which can be used to code an image on the display (credit: Mike Klepper)
int upArrow[25] = { 0, 0, 1, 0, 0,
                    0, 1, 1, 1, 0,
                    1, 0, 1, 0, 1,
                    0, 0, 1, 0, 0,
                    0, 0, 1, 0, 0
                  };

int downArrow[25] = { 0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0,
                      1, 0, 1, 0, 1,
                      0, 1, 1, 1, 0,
                      0, 0, 1, 0, 0
                    };

int leftArrow[25] = { 0, 0, 1, 0, 0,
                      0, 1, 0, 0, 0,
                      1, 1, 1, 1, 1,
                      0, 1, 0, 0, 0,
                      0, 0, 1, 0, 0
                    };

int rightArrow[25] = { 0, 0, 1, 0, 0,
                       0, 0, 0, 1, 0,
                       1, 1, 1, 1, 1,
                       0, 0, 0, 1, 0,
                       0, 0, 1, 0, 0
                     };

int circle[25] = { 0, 1, 1, 1, 0,
                   1, 0, 0, 0, 1,
                   1, 0, 0, 0, 1,
                   1, 0, 0, 0, 1,
                   0, 1, 1, 1, 0
                 };

int X[25] = { 2, 0, 0, 0, 2,
              0, 2, 0, 2, 0,
              0, 0, 2, 0, 0,
              0, 2, 0, 2, 0,
              2, 0, 0, 0, 2
            };

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


float tiltThreshold (.5);  //We tested to find this value which defines the angles at which the device must be tilted at to be considered as facing certain directions
float aX, aY, aZ;
int tiltState (0);  //0=error, 1=down, 2=up, 3=facing you, 4=facing away from you, 5=right, 6=left
int units (0);  //units of temperature displayed; 0=C, 1=F
int menuMode (0);  //0=off, 1=show temp and units, 2=show 24hr avg and units, 3=show color scale and current temp color, 4=show temp graph over predefined range, 5=change units
bool isActive(false), modeIsActive(false), was5, wasWas5, wasWasWas5, was6, wasWas6, wasWasWas6;

void setup() {
  Serial.begin(115200);
  M5.begin(true, false, true);
  M5.IMU.Init();  //Starts up the hardware in the device which detects acceleration, gyration, and temperature
  M5.dis.clear();
}

void loop() {
  M5.IMU.getAccelData(&aX, &aY, &aZ);  //Gets acceleration data from the device

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
  Serial.println(menuMode);


  if (tiltState == 2 && M5.Btn.wasPressed()) {
    isActive = true;
    menuMode = 1;
  }

  if (tiltState == 1) {
    isActive = false;
    M5.dis.clear();
    menuMode = 0;
  }

  if (isActive == true) {
    if (M5.Btn.wasPressed())
      modeIsActive = true;

    if (tiltState == 0 && wasWasWas5)  {
      menuMode++;
      if (menuMode == 6)
        menuMode = 1;
    }

    if (tiltState == 0 && wasWasWas6)  {
      menuMode--;
      if (menuMode == 0)
        menuMode = 5;
    }

    if (tiltState == 5) {
      if (was5 == true) {
        if (wasWas5 == true)
          wasWasWas5 = true;
        wasWas5 = true;
      }
      was5 = true;
    }
    else
      was5 = wasWas5 = wasWasWas5 = false;

    if (tiltState == 6) {
      if (was6 == true) {
        if (wasWas6 == true)
          wasWasWas6 = true;
        wasWas6 = true;
      }
      was6 = true;
    }
    else
      was6 = wasWas6 = wasWasWas6 = false;


    switch (menuMode) {
      case 1:
        if (!modeIsActive)
          drawArray(one, palette);
        else
          drawArray(one, paletteB);
        break;
      case 2:
        if (!modeIsActive)
          drawArray(two, palette);
        else
          drawArray(two, paletteB);
        break;
      case 3:
        if (!modeIsActive)
          drawArray(three, palette);
        else
          drawArray(three, paletteB);
        break;
      case 4:
        if (!modeIsActive)
          drawArray(four, palette);
        else
          drawArray(four, paletteB);
        break;
      case 5:
        if (!modeIsActive)
          drawArray(five, palette);
        else
          drawArray(five, paletteB);
        break;
      default:
        M5.dis.fillpix(0xffffff);
        break;
    }

  }


  M5.update();
}
