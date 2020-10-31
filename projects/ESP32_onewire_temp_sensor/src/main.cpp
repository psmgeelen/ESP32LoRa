// This script initialises an Arduino framework and uses the Ardiuno LoRa driver. 
// The Heltec library didn't work for us.
#include <Arduino.h>
//#include <SPI.h>
//#include <LoRa.h>
// TempSensor is a digital "1Wire" protocol sensor. 
#include <OneWire.h> 
#include <DallasTemperature.h>
// set GPIO Pin for temp sensor
#define ONE_WIRE_BUS 17 // USE BIDIRECTIONAL GPIO PIN HERE
//#define DS18B20MODEL 0x28

int counter = 0;
//int tempValue = 10;

// Configure temp sensor
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);



void setup() {
  sensors.begin(); 
  Serial.begin(9600); // Make sure that the baudrate of your setup aligns with this rate!
  
  while (!Serial);

  Serial.println("Dallas Temperature IC Control Library Demo"); 
}

void loop() {
  Serial.print("Number of sensors = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Parasite mode = ");
  Serial.println(sensors.isParasitePowerMode());
  // Temp Section
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  delay(1000);
  Serial.println("DONE"); 
  /********************************************************************/
  Serial.print("Temperature is: "); 
  Serial.print(sensors.getTempCByIndex(0));


  delay(5000);                       // wait for a second
}