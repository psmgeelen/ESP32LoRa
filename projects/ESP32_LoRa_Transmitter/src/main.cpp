// This script initialises an Arduino framework and uses the Ardiuno LoRa driver. 
// The Heltec library didn't work for us.
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>


int counter = 0;

void setup() {
  Serial.begin(9600); // Make sure that the baudrate of your setup aligns with this rate!
  LoRa.setPins(18, 14, 26); // Initializing LoRa device, these settings should fit a Heltec ESP32 LoRa v2 board
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) { // LoRa is set to 868 MHz as this project was run in Europe, please note that this setting is non-trivial, as the board too, have  a limited range.
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // send packet
  LoRa.beginPacket();
/*
* LoRa.setTxPower(txPower,RFOUT_pin);
* txPower -- 0 ~ 20
* RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
*   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
*   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}