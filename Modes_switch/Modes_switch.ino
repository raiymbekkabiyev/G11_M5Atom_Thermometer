#include "M5Atom.h"
#include <FastLED.h>

//vars 
uint8_t FSM = 0;
//colors
int GRB_COLOR_WHITE = 0xffffff;
int GRB_COLOR_BLACK = 0x000000;
int GRB_COLOR_RED = 0x00ff00;
int GRB_COLOR_ORANGE = 0xa5ff00;
int GRB_COLOR_YELLOW = 0xffff00;
int GRB_COLOR_GREEN = 0xff0000;
int GRB_COLOR_BLUE = 0x0000ff;
int GRB_COLOR_PURPLE = 0x008080;

void setup() {
  Serial.begin(115200);
  M5.begin(true, false, true);

}

void loop() {

  switch(FSM){
    case 0: 
      //Show Active temperature + Units
    case 1: 
      //Show average of last 24 hours of temperature + Units
    case 2: 
      //Show color scale of temperature range + current temperature as color
    case 3: 
      //Show graph of temperature across a predefined range
    case 4: 
      //Change units
  }


   if (M5.Btn.wasPressed()) {
      FSM++;
      if (FSM > 4)
      {
        FSM = 0;
      }
    }

    M5.update();

}
