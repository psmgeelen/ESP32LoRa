// This script initialises an Arduino framework and uses the Ardiuno LoRa driver. 
// The Heltec library didn't work for us.
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
// TempSensor is a digital "1Wire" protocol sensor. 
#include <OneWire.h> 
#include <DallasTemperature.h>

// arduinoJSON is used to create a JSON object from our readings
#include <ArduinoJson.h>

// create json object. We write sensor values to this object, then deserialize and send it over LORA;
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.



// Configure temperature sensor


// set GPIO Pin for temp sensor. REQUIRES BIDIRECTIONAL GPIO PIN
#define ONE_WIRE_BUS 17 

// OneWire / DT are used by the temp sensor, so we just initialize the libs.
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

char json_string[200] ;

  // initial temp value (gets overwritten in the loop)
int tempValue = 10;




StaticJsonDocument<200> doc;

void setup() {

 
  
  

  doc["deviceid"] = 1;
  // doc["temperature"] = 1000;
  // doc["humidity"] = 1000;
  // doc["battery"] = 25124;
  // doc["time"] = 1351824120;


  sensors.begin(); 
  Serial.print("Number of sensors = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Parasite mode = ");
  Serial.println(sensors.isParasitePowerMode());
  Serial.begin(9600); // Make sure that the baudrate of your setup aligns with this rate!
  LoRa.setPins(18, 14, 26); // Initializing LoRa device, these settings should fit a Heltec ESP32 LoRa v2 board
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) { // LoRa is set to 868 MHz as this project was run in Europe, please note that this setting is non-trivial, as the board too, have  a limited range.
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Dallas Temperature IC Control Library Demo"); 
}



void loop() {
  // Temp Section
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.println("DONE"); 
  /********************************************************************/
  Serial.print("Temperature is: "); 

  tempValue = sensors.getTempCByIndex(0);
  
  
  Serial.print(tempValue);
  // add temp value to JSON object
  doc["temperature"] = tempValue;


  // Read the humidity sensor. This one is just reading the DAC GPIO.
  // read sensor value 
  int dacoutput = analogRead(32);
  doc["humidity"] = dacoutput;



  // ## LoRa Section
  
  // Check what we are sending
  serializeJson(doc, json_string);
  Serial.print(json_string);
  // Open LORA packet
  LoRa.beginPacket();
  
  // print the JSON string into the package and send
  LoRa.print(json_string);

  LoRa.endPacket();
  

  delay(1000);                       // wait for a second
}