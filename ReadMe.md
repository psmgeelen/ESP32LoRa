# ReadMe for ESP32LoRa project
*developed by Arseny Starostin and Pieter Geelen*

## Scope
of this project is to do an end-to-end project with a "real-world" application. The use-case is measuring the moisture of the soil a vegetable garden. The architecture should be based on as much "off-shelve" code and hardware and should some what generic, hopefully supporting other projects accordingly. 

## Materials
We use some Heltec ESP32 LoRa v2 modules, some moisture sensors and a Rasperry Pi 4 as our IoT Master (collecting and visualising the data). The Raspberry Pi 4 is outfitter with a WaveShare LoRa HAT. 

## IDE 
This project is developed in Visual Studio Code, a free IDE that supports many extensions. We use the plaformio.org extension for programming on our ESP32 modules as it supports a full suite of firmware and libaries. To put simply, platformio takes care of many of "human-errors" that are common with these projects. Please note that this project was developed on Ubuntu 20.04

## Setup
So what do you need to do when cloning this project? Here is a list below:
* Install VS code
* Install platformio.org extension
* Configure serial port
    * Make sure you understand what baudrate is and adjust accordingly with a tool like minicom 
    * Add your user to the dailout group in order to be able to connect to the serial adapter without super-user rights. The command should be: ```sudo adduser $USER dialout``` assuming that you are currently logged in with the user that you want to use for developing the project. 

## Folder Strucure
The repository has the main information in the root directory. Additional documentation can be found in the ```doc``` folder, the individual scripts can be found in the ```projects``` folder. Each project file will have its own ```ReadMe``` file, explaining why certain code was scripted the way it is and what experiences we had. 

go to projects:
* ESP32 LoRa Transmitter [link]
* ESP32 LoRa Receiver [link]
* Raspberry Pi 4 Receiver
* Raspberry Pi "server" script
