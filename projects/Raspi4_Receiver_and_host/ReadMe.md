# Raspberry Pi 4 "server"

In this section we discuss the option of using a LoRa-HAT, in our case the WaveShare SX1262 HAT. We use a UART connection through the GPIO header to connect the HAT and configure and control the LoRa adapter. Below we walk through the setup steps and the different "controls" you can do for debugging and other issues. As always, this is not a perfect list, but we hope to support your IoT endavour the best we can. The overall steps include:

* Making a MicroSD-card that is usable
* Update, upgrade and configure the Raspberry Pi for UART
* Install Docker and spin up containers

## Get an OS on your Raspberry Pi
We tried using Ubuntu 64-bit and were not able to configuring everything the way we need. For this reason we recommend using Raspbian OS, that can be downloaded from the Raspberry Pi foundation website [here](https://www.raspberrypi.org/downloads/). In order to "install" the OS, you simply write the image to the MicroSD-card. In Ubuntu you would simply use the "Disk" utility, but tools like [Etcher](https://www.balena.io/etcher/) shouldnt work any different. After flashing the MicroSD-card it is important to add an empty file to the `boot` drive called "ssh". Keep in mind that this file has no extension or content. You can do this with any text editor, simply storing an empty file called "ssh" and making sure that no extesion is added when saving, like e.g. *.txt. 

## Configuring the OS
The Micro-SD is ready for use and can now be inserted into the Raspberry Pi. In our setup we connect through SSH. In Linux the command is `ssh pi@LocalIP` in which `pi` is the username and `LocalIP` is the IP-adres that you find when looking up the device in your connected devices in your router. It will prompt for a password, which is by default "raspberry". It makes sense to change that later, especially if you want to expose the ports of this device to the outside network. Next steps would be to update and upgrade the OS with `sudo apt update && sudo apt upgrade -y`. After you have succesfully updated your system you want to run `sudo raspi-config`, which opens a CLI tool for configuring your Raspberry Pi. In order to setup you UART properly you need to got to Use `Interfacing Options` and select `configure serial`. Answer `<NO>` to login shell and `<Yes>` to configure serial hardware enable. 
### Checking UART
Your UART connection should be setup. When in doubt make sure that you do a loop-test, in which you send and receive messages locally on your Raspberry Pi. The script is provided in this directory and make sure you run it with `sudo`. Before running, you obviously must connect the 8th and 10th pin, connecting the output (transmit) and input (receive) of the UART connection, referred to as `UART0_TXD` and `UART0_RXD` respectively. An image below displays the exact location 
![Pinout Raspberry Pi](../../doc/Raspberry-Pi-GPIO-Header-with-Photo.png)

## Docker and her containers
First things first, Docker is awesome! Docker uses containers to deploy applications "in the field". The advantage (amongs many other) of using Docker, is that Docker deploys the applications by deploying images (like recipies) in containers. Containers are stand-alone applications that contains programs and configurations, all in a single box. Docker does this in a very efficient way costing very little computional overhead. To add Docker is very well supported by all relevant software companies and by the Open-Source community. If you really want to understand all the ins and outs on Docker, I strongly recommend taking the Docker Mastery course that can be found on [Udemy](https://www.udemy.com/course/docker-mastery/). 
![Docker-Logo](../../doc/Moby-logo.png)

In our case we want to use Docker to:
1. Setup our own virtual-network for our applications
2. Spin up a Postgres Database
3. Spin up Grafana a Dashboarding tool. 

Keep in mind that namespace within in the network is identical to name of the container. So if I would be connecting my DB-IDE tool to the database, I would use in this case `localhost:5432`, but when I want to connect Grafana to the database (which are in the same virtual network), I would use `db:5432` because the container is named `db`. 

Another interesting note is that containers do not have to be persistent, which means that if you shut down your Raspberry Pi, you would lose the data in your database and dashboarding utility. We have added volumes to make the database and dashboard persistant. If you restart your Raspberry Pi, you only have to start the exisitng containers with e.g. `docker start db`, and you should be golden. 