#include "M5Atom.h"
#define INTERVAL 5000; //Interval to record temperature (milliseconds)

void setup() {
  M5.begin(true, false, true);
  M5.IMU.Init();
}

unsigned long targetTime (0);
float every5s [60], every1m [60], every1h [60], every1d [60];
int sCtr (0), mCtr (0), hCtr (0), dCtr (0);
float temp (0), tempSum (0);

void loop() {
  if ( millis() > targetTime) {
    M5.IMU.getTempData(&temp);
    every5s[sCtr] = temp;
    sCtr++;
    if (sCtr == 12 || sCtr == 24 || sCtr == 36 || sCtr == 48 || sCtr == 60) {//##use remainder modulus %
      tempSum = 0;
      for (int ctr = 1; ctr < 13; ctr++) {
        tempSum += every5s[sCtr - ctr];
      }
      every1m[mCtr] = tempSum / 12.0;
      mCtr++;
      if (mCtr > 59) {
        tempSum = 0;
        for (int ctr = 0; ctr < 60; ctr++) {
          tempSum += every1m[mCtr - ctr];
        }
        every1h[hCtr] = tempSum / 60.0;
        hCtr++;
        if (hCtr > 59) {
          for (int ctr = 0; ctr < 60; ctr++) {
            tempSum += every1h[hCtr - ctr];
          }
          if (dCtr < 23) {
            every1d[dCtr] = tempSum / 24.0;
            dCtr++;
          }
          else {
            for (int ctr = 0; ctr < 59; ctr++) {
              every1d[ctr + 1] = every1d[ctr];
            }
            every1d[0] = tempSum / 24.0;
          }
        }
      }
      if (sCtr > 59)
        sCtr = 0;
    }
    targetTime += INTERVAL;
    //
    Serial.print("\nData of every 5 secs for last 5 mins:  ");
    for (int ctr = 0; ctr < 60; ctr++) {
      Serial.printf("%.2f,  ", every5s [ctr]);
    }
    Serial.print("\nAverage temp data of every 1 min for last 60 mins:  ");
    for (int ctr = 0; ctr < 60; ctr++) {
      Serial.printf("%.2f,  ", every1m [ctr]);
    }
    Serial.print("\nAverage temp data of every 1 hr for last 60 hrs:  ");
    for (int ctr = 0; ctr < 60; ctr++) {
      Serial.printf("%.2f,  ", every1h [ctr]);
    }
    Serial.print("\nAverage temp data of every 1 day for last 60 days:  ");
    for (int ctr = 0; ctr < 60; ctr++) {
      Serial.printf("%.2f,  ", every1d [ctr]);
    }
    Serial.println();
    //
  }
  M5.update();
}
