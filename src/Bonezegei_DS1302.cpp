/*
  This Library is written for DS1307 RTC
  Author: Bonezegei (Jofel Batutay)
  Date: Feb 28 2024
  Updated: October 22, 2026
*/

#include "Bonezegei_DS1302.h"

Bonezegei_DS1302::Bonezegei_DS1302() {
  _cePin = 0;
  _clkPin = 0;
  _ioPin = 0;
  _hour12 = 24;
  _ampm = 0;
}

Bonezegei_DS1302::Bonezegei_DS1302(uint8_t cePin, uint8_t clkPin, uint8_t ioPin) {
  _cePin = cePin;
  _clkPin = clkPin;
  _ioPin = ioPin;
  _hour12 = 24;
  _ampm = 0;
}

char Bonezegei_DS1302::begin() {
  pinMode(_cePin, OUTPUT);
  pinMode(_clkPin, OUTPUT);
  digitalWrite(_cePin, LOW);
  digitalWrite(_clkPin, LOW);

  // Disable write protection
  writeRegister(0x8E, 0x00);

  // Clear clock halt bit (CH) if set
  uint8_t sec = readRegister(0x81);
  if (sec & 0x80) {
    writeRegister(0x80, sec & 0x7F);
  }

  return 1;
}

char Bonezegei_DS1302::begin(uint8_t cePin, uint8_t clkPin, uint8_t ioPin) {
  _cePin = cePin;
  _clkPin = clkPin;
  _ioPin = ioPin;
  return begin();
}

uint8_t Bonezegei_DS1302::convert(uint8_t data) {
  return ((data >> 4) * 10) + (data & 0x0F);
}

uint8_t Bonezegei_DS1302::convertBCD(int data) {
  return (uint8_t)(((data / 10) << 4) | (data % 10));
}

char Bonezegei_DS1302::getTime() {
  digitalWrite(_cePin, HIGH);
  delayMicroseconds(1);
  writeByte(0xBF); // Clock Burst Read command
  for (uint8_t i = 0; i < 8; i++) {
    _data[i] = readByte();
  }
  digitalWrite(_cePin, LOW);
  delayMicroseconds(1);

  _seconds = convert(_data[0] & 0x7F);
  _minutes = convert(_data[1] & 0x7F);

  uint8_t rawHour = _data[2];
  if (rawHour & 0x80) { // 12-hour format
    _hour12 = 12;
    _ampm = (rawHour & 0x20) ? 1 : 0;
    _hour = convert(rawHour & 0x1F);
  } else { // 24-hour format
    _hour12 = 24;
    _hour = convert(rawHour & 0x3F);
    _ampm = (_hour >= 12) ? 1 : 0;
  }

  _date = convert(_data[3] & 0x3F);
  _month = convert(_data[4] & 0x1F);
  _day = convert(_data[5] & 0x07);
  _year = convert(_data[6]);

  return 1;
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
  if (fmt == 12 || fmt == 24) {
    _hour12 = fmt;
    uint8_t hVal = 0;
    if (_hour12 == 12) {
      hVal = 0x80 | (_ampm ? 0x20 : 0x00) | convertBCD(_hour % 12 == 0 ? 12 : _hour % 12);
    } else {
      hVal = convertBCD(_hour % 24) & 0x3F;
    }
    writeRegister(0x84, hVal);
  }
}

void Bonezegei_DS1302::setAMPM(uint8_t ampm) {
  _ampm = ampm ? 1 : 0;
  if (_hour12 == 12) {
    uint8_t hVal = 0x80 | (_ampm ? 0x20 : 0x00) | convertBCD(_hour % 12 == 0 ? 12 : _hour % 12);
    writeRegister(0x84, hVal);
  }
}

void Bonezegei_DS1302::setTime(const char *t) {
  int h = 0, m = 0, s = 0;
  if (sscanf(t, "%d:%d:%d", &h, &m, &s) >= 2) {
    _hour = (uint8_t)h;
    _minutes = (uint8_t)m;
    _seconds = (uint8_t)s;

    writeRegister(0x80, convertBCD(_seconds) & 0x7F);
    writeRegister(0x82, convertBCD(_minutes));

    uint8_t hVal = 0;
    if (_hour12 == 12) {
      hVal = 0x80 | (_ampm ? 0x20 : 0x00) | convertBCD(_hour % 12 == 0 ? 12 : _hour % 12);
    } else {
      hVal = convertBCD(_hour % 24) & 0x3F;
    }
    writeRegister(0x84, hVal);
  }
}

void Bonezegei_DS1302::setDate(const char *d) {
  int m = 0, dt = 0, y = 0;
  int parsed = sscanf(d, "%d:%d:%d", &m, &dt, &y);
  if (parsed < 3) {
    parsed = sscanf(d, "%d/%d/%d", &m, &dt, &y);
  }
  if (parsed < 3) {
    parsed = sscanf(d, "%d-%d-%d", &m, &dt, &y);
  }

  if (parsed >= 3) {
    if (y >= 2000) y -= 2000;
    _month = (uint8_t)m;
    _date = (uint8_t)dt;
    _year = (uint8_t)y;

    writeRegister(0x86, convertBCD(_date));
    writeRegister(0x88, convertBCD(_month));
    writeRegister(0x8C, convertBCD(_year));
  }
}

void Bonezegei_DS1302::setDay(uint8_t d) {
  _day = d;
  writeRegister(0x8A, convertBCD(_day) & 0x07);
}

void Bonezegei_DS1302::writeByte(uint8_t value) {
  pinMode(_ioPin, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_ioPin, (value >> i) & 0x01);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);
  }
}

uint8_t Bonezegei_DS1302::readByte() {
  uint8_t value = 0;
  pinMode(_ioPin, INPUT);
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bit = digitalRead(_ioPin);
    value |= (bit << i);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);
  }
  return value;
}

void Bonezegei_DS1302::writeRegister(uint8_t reg, uint8_t value) {
  digitalWrite(_cePin, HIGH);
  delayMicroseconds(1);
  writeByte(0x8E); // Write Protect register address
  writeByte(0x00); // Disable write protect
  digitalWrite(_cePin, LOW);
  delayMicroseconds(1);

  digitalWrite(_cePin, HIGH);
  delayMicroseconds(1);
  writeByte(reg);
  writeByte(value);
  digitalWrite(_cePin, LOW);
  delayMicroseconds(1);
}

uint8_t Bonezegei_DS1302::readRegister(uint8_t reg) {
  digitalWrite(_cePin, HIGH);
  delayMicroseconds(1);
  writeByte(reg);
  uint8_t val = readByte();
  digitalWrite(_cePin, LOW);
  delayMicroseconds(1);
  return val;
}