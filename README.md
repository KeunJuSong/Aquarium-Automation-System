# **Aquarium-Automation-System**

## **OverView**
* Team Project that developed in Korea Polytechnic Uinveristy, CP-COP competition.
* This System is consisted of Arduino and mobile application.

## **System Architecture**
### **H/W**

#### **MCU**
* Arduino UNO/Leonardo Board
#### **Sensors & Actuator**
* Tempreature sensor
* PH sensor
* RTC(Real-Time Clock) module
* Relay module
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
* Aquarium Control System DIY
  https://slowknight.tistory.com/6
