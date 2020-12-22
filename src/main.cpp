#include <Arduino.h>
#include <M5StickC.h>
#include "ble-midi.h"

BLE_MIDI midi = BLE_MIDI();

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);

  midi.init();
  midi.start();
}

void loop() {
  delay(20);
  M5.update();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print(midi.isConnected() ? "connected     " : "advertising...");

  if(!midi.isConnected())return;

  if(M5.BtnA.wasPressed())
    midi.noteOn(0, 0x3C, 127);
  else if(M5.BtnA.wasReleased())
    midi.noteOff(0, 0x3C);
}