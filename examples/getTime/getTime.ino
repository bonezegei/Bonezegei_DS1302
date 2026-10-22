/*
  Get Time And Date
  Author: Bonezegei (Jofel Batutay)
  Date : October 22, 2026
*/

#include <Bonezegei_DS1302.h>

#define CE_PIN  2
#define CLK_PIN 3
#define IO_PIN  4

Bonezegei_DS1302 rtc(CE_PIN, CLK_PIN, IO_PIN);

void setup() {
  Serial.begin(115200);
  rtc.begin();
}

void loop() {

  if (rtc.getTime()) {
    // Serial.printf only does not work on atmega
    char time_data[32];
    sprintf(time_data, "Time %02d:%02d:%02d ", rtc.getHour(), rtc.getMinute(), rtc.getSeconds());
    Serial.print(time_data);

    if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format

      if (rtc.getAMPM()) {  // return 0 = AM  1 = PM
        Serial.print("PM  ");
      } else {
        Serial.print("AM  ");
      }
    }

    // Serial.printf only does not work on atmega
    char date_data[32];
    sprintf(date_data, "Date %02d-%02d-%d \n", rtc.getMonth(), rtc.getDate(), rtc.getYear());
    Serial.print(date_data);
  }

  delay(1000);
}