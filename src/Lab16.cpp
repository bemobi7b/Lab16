/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Rich/CTD2023/Lab16/src/Lab16.ino"
#include "oled-wing-adafruit.h"

void setup();
void loop();
void resetDisplay();
#line 3 "c:/Users/Rich/CTD2023/Lab16/src/Lab16.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);

OledWingAdafruit display;
BleAdvertisingData dataStuff;

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);

const size_t UART_TX_BUF_SIZE = 20;
uint8_t txBuf[10];

String writeOut;
int writer;
bool disconnected = false;
bool reconnected = false;

void setup()
{
  Serial.begin(9600);
  display.setup();
  resetDisplay();
  display.display();
  BLE.on();
  RGB.control(true);
  txCharacteristic.setValue(txBuf, 10);
  dataStuff.appendServiceUUID(serviceUuid);
  BLE.advertise(&dataStuff);
  BLE.setDeviceName("Bennett's Argon");
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);
  txCharacteristic.setValue(txBuf, 10);
}
void loop()
{
  display.loop();
  if (disconnected){
    BLE.disconnect();
    disconnected = false;
    RGB.color(255,255,0);
    Serial.println("OFFER");
    resetDisplay();
    display.println("Disconnected...");
    display.display();
    delay(3000);
    reconnected = false;
  }
  if (!BLE.connected())
  {
    RGB.color(255, 255, 0);
  }
  else if (BLE.connected())
  {
    if (reconnected == false){
      resetDisplay();
      display.display();
      reconnected = true;
    }
    RGB.color(0, 0, 255);
    uint8_t txBuf[UART_TX_BUF_SIZE];
    String message = "This is magic!\n";
    message.toCharArray((char *)txBuf, message.length() + 1);
    txCharacteristic.setValue(txBuf, message.length() + 1);
    delay(1000);
  }
}
void resetDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}
void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
for (size_t i = 0; i < len; i++) {   
  writeOut = String(data[i]);
  writer = writeOut.toInt();
  Serial.write(data[i]);
  Serial.write(writeOut);
  if (writer == 49){
    resetDisplay();
    display.println("Entered 1");
    display.display();
  }
  else if(writer == 48){
    disconnected = true;
  }
}
}