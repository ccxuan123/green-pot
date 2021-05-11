# GreenPot - Mini green house 🌱

Mini project for out Real Time System course. 

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


