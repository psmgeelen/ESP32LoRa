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
//wifi
#include <WiFi.h>
//HTTP Client
#include <HTTPClient.h>

//# WiFi Setup
// set up wifi credentials 
const char* ssid     = "harseny";
const char* password = "1234abcd";

// set up server adress to send data to 
// the server is a PostgREST service
// it accepts the IP, a port and the name of the table we write to
const char* serverName = "http://217.160.172.124:3000/humidity_log";



#define ONE_WIRE_BUS 3

// OneWire / DT are used by the temp sensor, so we just initialize the libs.
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

char json_string[200] ;

  // initial temp value (gets overwritten in the loop)
int tempValue = 10;




StaticJsonDocument<200> doc;

void setup() {
  doc["deviceid"] = 1;
  doc["battery"] = analogRead(A4); 
  // last value given wat 1713.

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


  // initial wifi setup. This is repeated within the loop to catch disconnects
  WiFi.begin(ssid, password);
  // try connecting to wifi and display status about that
  Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  // print the IP adress of the device (?)
  Serial.println(WiFi.localIP());
}



void loop() {
  // Temp Section
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.println("DONE"); 
  /********************************************************************/
  tempValue = sensors.getTempCByIndex(0);
  
  
  Serial.print(tempValue);
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
  
  // initialize http library and point to server
  // startup http client
  HTTPClient http;
  http.begin(serverName);
  // send the JSON and print response code
  int httpResponseCode = http.POST(json_string);
  Serial.println(httpResponseCode);

  delay(1000);                       // wait for a second
}