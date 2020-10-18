// This script initialises an Arduino framework and uses the Ardiuno LoRa driver. 
// The Heltec library didn't work for us.

//libraries to receive LoRa packets and send data over wifi
#include <Arduino.h>
// parse json 
#include <ArduinoJson.h>
//wifi
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>
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




void setup() {
  Serial.begin(9600); // Make sure that the baudrate of your setup aligns with this rate!
  LoRa.setPins(18, 14, 26); // Initializing LoRa device, these settings should fit a Heltec ESP32 LoRa v2 board
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) { // LoRa is set to 868 MHz as this project was run in Europe, please note that this setting is non-trivial, as the board too, have  a limited range.
    Serial.println("Starting LoRa failed!");
    while (1);
  }


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




  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    
    // read packet
    while (LoRa.available()) {
      // print the string (DEV)
      Serial.print((char)LoRa.read());  
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

  }
  // else
  // {
  //   Serial.println("empty ");
  // }
    
 
  //   // define a string object to print the json to?
  //   String json_data;
  //   //  create and parse json object to pass to HTTP POST request later
  //   StaticJsonBuffer<69> jsonBuffer;
  //   char json_payload[] = "{'sensor_value': '1111', 'created_at': '2020-12-01 00:00:00' }";
  //   JsonObject& root = jsonBuffer.parseObject(json_payload);

  //   if(!root.success()) {
  //     Serial.println("parseObject() failed");
  //   } else {
  //     Serial.println("JSON OK");
  //   }

  //   //put the json in the string object
  //   root.printTo(json_data);
  //   Serial.print(json_data);


  //   // check wifi connection
  //   if (WiFi.status() == WL_CONNECTED){
  //   Serial.println("Wifi connected");
  //   } else {

  //     // set up wifi
  //     WiFi.begin(ssid, password);
  //     // try connecting to wifi and display status about that
  //     Serial.println("Connecting");
  //     while(WiFi.status() != WL_CONNECTED) { 
  //       delay(500);
  //       Serial.print(".");
  //     }
  //     // print the IP adress of the device (?)
  //     Serial.println(WiFi.localIP());
  //   }
  
  // // initialize http library and point to server
  //   // startup http client
  // HTTPClient http;
  // http.begin(serverName);
  // // send the JSON and print response code
  // int httpResponseCode = http.POST(json_data);
  // Serial.println(httpResponseCode);
  //delay(10000);
}