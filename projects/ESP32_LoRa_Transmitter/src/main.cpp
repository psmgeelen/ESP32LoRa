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
// set GPIO Pin for temp sensor. REQUIRES BIDIRECTIONAL GPIO PIN


// set up server adress to send data to 
// the server is a PostgREST service
// it accepts the IP, a port and the name of the table we write to
const char* serverName = "217.160.172.124:3000/humidity_log";



#define ONE_WIRE_BUS 3

// OneWire / DT are used by the temp sensor, so we just initialize the libs.
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

char json_string[200] ;

  // initial temp value (gets overwritten in the loop)
int tempValue = 10;



// define a static json document with size 200, we are going to be adding our outputs to it.
// expects the form '{ "deviceid" : "1", "battery":200, "temperature":205, "humidity":210 }'
StaticJsonDocument<200> doc;

void setup() {
  doc["deviceid"] = 1;


  sensors.begin(); 
  Serial.print("Number of sensors = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Parasite mode = ");
  Serial.println(sensors.isParasitePowerMode());
  Serial.begin(9600); // Make sure that the baudrate of your setup aligns with this rate!

  // Initializing LoRa device, these settings should fit a Heltec ESP32 LoRa v2 board
  LoRa.setPins(18, 14, 26); 
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) { // LoRa is set to 868 MHz as this project was run in Europe, please note that this setting is non-trivial, as the board too, have  a limited range.
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Dallas Temperature IC Control Library Demo"); 


}

void loop() {
  // battery section. simply gets the DAC value (TODO: moving average of the values)
  doc["battery"] = analogRead(A4); 
  // last value given wat 1713.



  // Temp Section
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.println("DONE"); 
  /********************************************************************/
  tempValue = sensors.getTempCByIndex(0);
  
  

  // add temp value to JSON object
  doc["temperature"] = tempValue;


  // Read the humidity sensor. This one is just reading the DAC GPIO.
  // read sensor value 
  int dacoutput = analogRead(25);
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