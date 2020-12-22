#pragma once 
#include<Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define MIDI_SERVICE_UUID "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define MIDI_CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"
#define DEV_NAME "M5StickC"

/**
 * BLE over MIDIの簡易実装
 */
class BLE_MIDI: BLEServerCallbacks {
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristic;
  BLEAdvertising *pAdvertising;

  bool connected = false;

  void onConnect(BLEServer* pServer);
  void onDisconnect(BLEServer* pServer);
  void generateHeader(uint8_t *header);
  void send(uint8_t *data);

public:
  void init();
  void start();
  void noteOn(uint8_t ch, uint8_t note, uint8_t vel);
  void noteOff(uint8_t ch, uint8_t note);
  void control(uint8_t ch, uint8_t cc, uint8_t value);
  bool isConnected();
};