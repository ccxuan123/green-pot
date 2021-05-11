# GreenPot - Mini green house 🌱
This is a mini project for my University course `Real-Time System` as the requirement is to develop a real-time system project with Arduino Uno based on [Arduino_FreeRTOS_Library](https://github.com/feilipu/Arduino_FreeRTOS_Library).

## Motivation
I started gardening at my apartment because of the lockdown in my city (COVID-19). The problem arises when my neighbour cats will come to shit on my pot as it is fresh & clean soil. So I try to develop something to scare the cat to prevent them disturb my plants. And why not automate the watering process and add an extra light source for plant at night. 

P.S. Literally scare the shit out of those cats twice. Still have to clean up the mess though.

## Hardware

### Parts List
* 1 × Arduino Uno (micro-controller)
* 1 × Ultrasonic sensor (to detect cat)
* 1 × Soil moisture sensor
* 1 × LED light
* 1 × LDR module
* 1 × Buzzer
* 1 x Spray bottle
* 1 × Servo motor

### Modifying Srpay Bottle
<img src="https://raw.githubusercontent.com/ccxuan123/green-pot/main/.img/water_pump.jpeg" alt="Modified Water Sprayer">

### Schematics
add on later

## Software
You need [Arduino IDE](https://www.arduino.cc/en/software) to write and upload code to Arduino Uno.

## Using Platform I/O
This branch contains project files from [Platform I/O](https://platformio.org/). It is an extension for IDE. CX use it in Visual Studio Code. The Platform I/O project files is located in [PIO](PIO) folder. The main code is inside `src` folder.  

## Yay! Full code is in Beta 🎉
The project folder is in [green-pot-full-assemble](PIO/green-pot-full-assemble).
The main code is in [here](PIO/green-pot-full-assemble/src/main.cpp)

### Library used
* FreeRTOS by Richard Barry ([GitHub repo](https://github.com/feilipu/Arduino_FreeRTOS_Library))
* Servo by Micheal Margolis ([GitHub repo](https://github.com/arduino-libraries/Servo))

## Component function test

- [x] Ultrasonic Sensor [Test code](PIO/ultrasonic-delay-trigger/src/main.cpp)
- [x] LDR [Test code](PIO/test-LDR/src/main.cpp)
- [x] USB LED 
- [x] Servo motor (water spray)
- [x] Soil Humidity sensor [Test code](PIO/test-soil-moisture/src/main.cpp)

## Covert `main.cpp` to `.ino`
Just remove the `<Arduino.h>` header in `main.cpp`


