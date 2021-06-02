#include "M5Atom.h"
#define INTERVAL 5000; //Interval to record temperature (milliseconds)

unsigned long targetTime (0);
float every5s [60], every1m [60], every1h [60], every1d [60], mAvg, hAvg, dAvg(999.9), temp(999.99);
int sCtr (0), mCtr (0), hCtr (0);


void shiftFloatArray(float myArray[], float firstValue) {
  for (int ctr = 59; ctr > 0; ctr--)
  myArray[ctr] = myArray[ctr - 1];
  myArray[0] = firstValue;
}

float average(float array[60], int averageOf) {
  float tempSum (0);
  for (int ctr = 0; ctr < averageOf; ctr++)
    tempSum += array[ctr];
  return (tempSum / (float)averageOf);
}

void setup() {
  M5.begin(true, false, true);
  M5.IMU.Init();

  for (int ctr = 0; ctr < 60; ctr++)
    every5s[ctr] = every1m[ctr] = every1h[ctr] = every1d[ctr] = 999.99;
}


void loop() {
  if ( millis() > targetTime) {  //If it's time to record the temperature
    M5.IMU.getTempData(&temp);  //Get the current temperature from the device and store it in the variable temp

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

    //Used to check data storage on serial monitor
    Serial.print("\nData of every 5 secs for last 5 mins:  ");
    for (int ctr = 0; ctr < 60; ctr++)
      Serial.printf("%.2f,  ", every5s [ctr]);
    Serial.print("\nAverage temp data of every 1 min for last 60 mins:  ");
    for (int ctr = 0; ctr < 60; ctr++)
      Serial.printf("%.2f,  ", every1m [ctr]);
    Serial.print("\nAverage temp data of every 1 hr for last 60 hrs:  ");
    for (int ctr = 0; ctr < 60; ctr++)
      Serial.printf("%.2f,  ", every1h [ctr]);
    Serial.print("\nAverage temp data of every 1 day for last 60 days:  ");
    for (int ctr = 0; ctr < 60; ctr++)
      Serial.printf("%.2f,  ", every1d [ctr]);
    Serial.println();

  } //End of what happens if it's time to record the temperature
  M5.update();
}
