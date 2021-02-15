# **Aquarium-Automation-System**

## **Overview**
* Team Project that developed in Korea Polytechnic Uinveristy, CP-COP competition.
* This System is consisted of Arduino and mobile application.

## **Pamphlet**
<img src="https://user-images.githubusercontent.com/48046183/103457687-af5aa400-4d44-11eb-8525-f4710b63e69f.jpg"  width="70%">

## **System Architecture**
### **H/W**

#### **MCU**
* Arduino UNO/Leonardo Board
* Arduino Sensor Shield

#### **Sensors & Actuator**
* Tempreature sensor
* PH sensor
* Photo resistor (Light sensor)
* Real-Time Clock (RTC) module
* 2ch-Relay module
* LED
* Auto-Feeder

### **Application**
#### **Aqua Control App** 
* Monitor temperature and ph value
* Activate LED and Feeder

## **Function of the System**
* **Auto-Feed System**: Feeding at setting time using Relay module X RTC module.
* **Auto-PH Control System**: When ph value that sensed by PH sensor in Real-Time is out of range, it can be controlled automatically using Solenoid valve.
* **Auto-LED ON/OFF**: Control LED with Light Sensor.

## **Development Environment**
* Arduino IDE
* App Inventor 2.0

## **Reference**
* [Aquarium Control System DIY blog](https://slowknight.tistory.com/6)
* [Hackster.io](https://www.hackster.io/)
