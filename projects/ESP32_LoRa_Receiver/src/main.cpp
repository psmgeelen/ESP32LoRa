// This script initialises an Arduino framework and uses the Ardiuno LoRa driver. 
// The Heltec library didn't work for us.

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600); // Make sure that the baudrate of your setup aligns with this rate!
  LoRa.setPins(18, 14, 26); // Initializing LoRa device, these settings should fit a Heltec ESP32 LoRa v2 board
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) { // LoRa is set to 868 MHz as this project was run in Europe, please note that this setting is non-trivial, as the board too, have  a limited range.
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}