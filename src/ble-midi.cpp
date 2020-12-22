#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "ble-midi.h"

/**
 * デバイスの初期化し
 * BLE MIDI に必要なサービスとキャラクタリスティックを登録する
*/
void BLE_MIDI::init() {
  BLEDevice::init(DEV_NAME);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(this);
  pService = pServer->createService(MIDI_SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    BLEUUID(MIDI_CHARACTERISTIC_UUID),
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_WRITE_NR
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEAdvertisementData data = BLEAdvertisementData();
  data.setCompleteServices(BLEUUID(MIDI_SERVICE_UUID));
  data.setName(DEV_NAME);
  pAdvertising = pServer->getAdvertising();
  pAdvertising->setAdvertisementData(data);
}

void BLE_MIDI::start() {
  pAdvertising->start();
}

void BLE_MIDI::onConnect(BLEServer *pServer) {
  connected = true;
}

void BLE_MIDI::onDisconnect(BLEServer *pServer) {
  connected = false;
}

bool BLE_MIDI::isConnected() {
  return connected;
}

/**
 * タイムスタンプからBLE MIDI のヘッダを生成する
*/
void BLE_MIDI::generateHeader(uint8_t *header) {
  unsigned long t = millis();
  header[0] = (1 << 7) | ((t >> 7) & ((1 << 6) - 1));
  header[1] = (1 << 7) | (t & ((1 << 7) - 1));
}

void BLE_MIDI::send(uint8_t *data) {
  pCharacteristic->setValue(data, 5);
  pCharacteristic->notify();
}

void BLE_MIDI::noteOn(uint8_t ch, uint8_t note, uint8_t vel) {
  uint8_t header[2];
  generateHeader(header);
  uint8_t data[5] = {header[0], header[1], 0x90 | ch, note, vel};
  send(data);
}

void BLE_MIDI::noteOff(uint8_t ch, uint8_t note) {
  uint8_t header[2];
  generateHeader(header);
  uint8_t data[5] = {header[0], header[1], 0x80 | ch, note, 0};
  send(data);
}

void BLE_MIDI::control(uint8_t ch, uint8_t cc, uint8_t value) {
  uint8_t header[2];
  generateHeader(header);
  uint8_t data[5] = {header[0], header[1], 0xB0 | ch, cc, value};
  send(data);
}