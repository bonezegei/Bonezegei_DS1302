/*
  This Library is written for DS1307 RTC
  Author: Bonezegei (Jofel Batutay)
  Date: Feb 28 2024
*/

#include "Bonezegei_DS1302.h"

Bonezegei_DS1302::Bonezegei_DS1302() {}
Bonezegei_DS1302::Bonezegei_DS1302(uint8_t addr) {
  _addr = addr;
}

char Bonezegei_DS1302::begin() {

}
uint8_t Bonezegei_DS1302::convert(uint8_t data) {
  return 0;
}
uint8_t Bonezegei_DS1302::convertBCD(int data) {
  return 0;
}
char Bonezegei_DS1302::getTime() {
  return 0;
}

uint8_t Bonezegei_DS1302::getSeconds() {
  return _seconds;
}

uint8_t Bonezegei_DS1302::getMinute() {
  return _minutes;
}

uint8_t Bonezegei_DS1302::getHour() {
  return _hour;
}

uint8_t Bonezegei_DS1302::getDay() {
  return _day;
}

uint8_t Bonezegei_DS1302::getDate() {
  return _date;
}

uint8_t Bonezegei_DS1302::getMonth() {
  return _month;
}

uint8_t Bonezegei_DS1302::getYear() {
  return _year;
}

uint8_t Bonezegei_DS1302::getAMPM() {
  return _ampm;
}

uint8_t Bonezegei_DS1302::getFormat() {
  return _hour12;
}

void Bonezegei_DS1302::setFormat(uint8_t fmt) {
}

void Bonezegei_DS1302::setAMPM(uint8_t ampm) {
}

void Bonezegei_DS1302::setTime(const char *t) {
}
void Bonezegei_DS1302::setDate(const char *d) {

}
void Bonezegei_DS1302::setDay(uint8_t d) {
}
