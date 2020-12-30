#include "DS3231.h"
//#include "Wire.h"
#include <OneWire.h>  
#include <DallasTemperature.h>  
#include <SoftwareSerial.h> //블루투스 통신을 위한 선언.
//#include <AltSoftSerial.h>
#define ONE_WIRE_BUS 5 
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second


//DS3231 Clock;
//1-wire 디바이스와 통신하기 위한 준비  
OneWire oneWire(ONE_WIRE_BUS);  
    
// oneWire선언한 것을 sensors 선언시 참조함.  
DallasTemperature sensors(&oneWire);  

//스 주소를 저장할 배열 선언  
DeviceAddress insideThermometer;  

byte DoW;
byte Hour=0;
byte Minute=0;
byte Second=0;
unsigned int feedtime;
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  int cds;
  int feed_L = 0;
  int Ht=0;
  int Mt=0;
  int St=0;
  int sum1=0;
  int sum2=0;
  int sum3=0;
  int pH_version=0;
  bool feed_flag = false;
  float pH_UpLimit[3] = {8.0, 7.8, 6.5};
  float pH_DownLimit[3] = {7.0, 6.8, 6.0};

bool Century=false;
bool h12;
bool PM;
int rxPin = 8; 
int txPin = 9;
SoftwareSerial BTSerial(rxPin,txPin); //블루투스 시리얼 핀 설정.
float tempC_bluetooth; //블루투스 통신을 위해 지역변수로 선언되있는 tempC 값을 전역변수에 넣기 위한 선언.

#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate  
#define R_feed    4
#define R_sol_m  12
#define R_sol_p  13
#define R_LED   7
#define PH_temp  9
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection 

int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0; 
float pHValue,voltage;
int droptime = 0;
bool getDateflag = true;
bool GS_flag = false;
unsigned long it = 0;

void GetDateStuff() {
    // Call this if you notice something coming in on
    // the serial port. The stuff coming in should be in
    // the order YYMMDDwHHMMSS, with an 'x' at the end.
    char Char_tmp;
    char String_tmp[10];
    byte abc=0;
    
    if (BTSerial.available()) {
    if (BTSerial.read() == 'a') {
      it = 0;
      getDateflag = false;
    }
    }
    if (!getDateflag) {
      while (!GS_flag){
        if (BTSerial.available()) {
          Char_tmp = BTSerial.read();
          if (Char_tmp == 't') {
                GS_flag = true;
                getDateflag = true;
            }
        }
        String_tmp[abc] = Char_tmp;
            abc += 1;
      }
//      Serial.println(String_tmp);
      feedtime = atoi(String_tmp);
    }
        if (Serial.available()) {
    if (Serial.read() == 'a') {
      it = 0;
      getDateflag = false;
    }
    }
    if (!getDateflag) {
      while (!GS_flag){
        if (Serial.available()) {
          Char_tmp = Serial.read();
          if (Char_tmp == 't') {
                GS_flag = true;
                getDateflag = true;
            }
        }
        String_tmp[abc] = Char_tmp;
            abc += 1;
      }
//      Serial.println(String_tmp);
      feedtime = atoi(String_tmp);
    }
}

void GetpHStuff() {
    // Call this if you notice something coming in on
    // the serial port. The stuff coming in should be in
    // the order YYMMDDwHHMMSS, with an 'x' at the end.
    char Char_tmp;
    char String_tmp[10];
    byte abc=0;
    
    if (BTSerial.available()) {
    if (BTSerial.read() == 'p') {
      it = 0;
      getDateflag = false;
    }
    }
    if (!getDateflag) {
      while (!GS_flag){
        if (BTSerial.available()) {
          Char_tmp = BTSerial.read();
          if (Char_tmp == 'h') {
                GS_flag = true;
                getDateflag = true;
            }
            String_tmp[abc] = Char_tmp;
            abc += 1;
        }
      }
//      Serial.println(String_tmp);
      pH_version = String_tmp[0]-48;
    }
}

void printTemperature(DeviceAddress deviceAddress)  
{  
  float  tempC = sensors.getTempC(deviceAddress); 
  tempC_bluetooth = tempC; //전역변수에 온도값 대입.  
      
  Serial.print("Temp C: ");  
  Serial.print(tempC);  
  Serial.print(" Temp F: ");  
       
  Serial.println(DallasTemperature::toFahrenheit(tempC));   
}  
     
void printAddress(DeviceAddress deviceAddress)  
{  
  for (uint8_t i = 0; i < 8; i++)  
  {  
    if (deviceAddress[i] < 16) Serial.print("0");  
        Serial.print(deviceAddress[i], HEX);  
  }  
}  

void setup() {
    // Start the serial port
    Serial.begin(9600);
    sensors.begin();
 BTSerial.begin(9600); // 블루투스 통신 시작.
    pinMode(R_feed,OUTPUT);
    pinMode(R_sol_m,OUTPUT);
    pinMode(R_sol_p,OUTPUT);
    pinMode(R_LED,OUTPUT);
    Serial.println("pH meter experiment!"); //Test the serial monitor 
 
  Serial.print(sensors.getDeviceCount(), DEC);   
 
  if (sensors.isParasitePowerMode()) Serial.println("ON");  
  else Serial.println("OFF");  
      
  //버스에서 첫번째 장치의 주소를 가져온다.  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");   

  printAddress(insideThermometer);  
    
  //데이터시트에서 확인결과 9~12비트까지 설정 가능  
  sensors.setResolution(insideThermometer, 10);  
  
  Serial.print(sensors.getResolution(insideThermometer), DEC);

        // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}
 
void loop() {
    cds = analogRead(A1);
    Serial.print("cds Sensor : ");
    Serial.println(cds);
  
    BTSerial.print(tempC_bluetooth, 1);
    BTSerial.print("|");
    BTSerial.println(pHValue);

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(0, pixels.Color(150,150,150)); // Moderately bright green color.
    pixels.setPixelColor(1, pixels.Color(150,150,150));
    pixels.setPixelColor(2, pixels.Color(150,150,150));
    pixels.setPixelColor(3, pixels.Color(150,150,150));
    pixels.setPixelColor(4, pixels.Color(150,150,150));
    pixels.setPixelColor(5, pixels.Color(150,150,150));
    pixels.setPixelColor(6, pixels.Color(150,150,150));
    pixels.setPixelColor(7, pixels.Color(150,150,150));
    pixels.setPixelColor(8, pixels.Color(150,150,150));
    pixels.setPixelColor(9, pixels.Color(150,150,150));
    pixels.show(); // This sends the updated pixel color to the hardware.
    if(cds>500)
    {
      Serial.println("LED ON");
      digitalWrite(R_LED, HIGH);
    } else digitalWrite(R_LED, LOW);
    pHArray[pHArrayIndex++]=analogRead(SensorPin);
   if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
   voltage = avergearray(pHArray, ArrayLenth)*5.0/1024; 
   pHValue = 3.5*voltage+Offset;
   samplingTime=millis();

   Serial.print("Voltage:");
   Serial.print(voltage,2); 
   Serial.print("    pH value: ");
   Serial.println(pHValue,2);
   printTime=millis();
   Serial.print("pH version : ");
   Serial.println(pH_version);
   Serial.print("pH range is : ");
   Serial.print(pH_UpLimit[pH_version]);
   Serial.print("~");
   Serial.println(pH_DownLimit[pH_version]);
  
  delay(900);

  droptime++;

  if(pHValue >= pH_UpLimit[pH_version]){
    if(droptime%5 < 1){  //용액 투여 후 pH 변화를 확인하는 여유 시간 5초를 두고 pH용액을 투여. 
                          //여유시간이 없을 경우 변화를 감지하는 시간 부족으로 필요 이상의 용액을 투여할 수 있으므로
    Serial.println("PH- for 1sec");                      
    digitalWrite(R_sol_m, HIGH);
    }
    else digitalWrite(R_sol_m, LOW);
  }

  if(pHValue < pH_DownLimit[pH_version]){
    if(droptime%5 < 1){
      Serial.println("PH+ for 1sec");
      digitalWrite(R_sol_p, HIGH);
    }
    else digitalWrite(R_sol_p, LOW);
  }
  if(droptime>=10) droptime = 0;

  GetDateStuff();
  GetpHStuff();

      it++;
      if (GS_flag) {
        GS_flag = false;
      }
        if(feedtime){
        if(it >= feedtime){
          Serial.println("feeding fish");
          digitalWrite(R_feed, HIGH);
          it = 0;
          feed_flag = true;
        }
        }
        if(feed_flag == true)
        {
          feed_L++;
        }
        if(feed_L == 5){
          Serial.println("end");
          digitalWrite(R_feed, 0);
          feed_L = 0;
          feed_flag = false;
        }
        Serial.print(feed_L);
        Serial.print(":");
        Serial.print(it);
        Serial.print(":");
        Serial.println(feedtime);
//        Serial.println(digitalRead(R_feed));
  
  Serial.print("Requesting temperatures...");  
//  sensors.requestTemperatures();   //연결되어 있는 전체 센서의 온도 값을 읽어옴  
  sensors.requestTemperaturesByIndex(0); //첫번째 센서의 온도값 읽어옴  
  Serial.println("DONE");  
     
  //센서에서 읽어온 온도를 출력  
  printTemperature(insideThermometer); 
}

double avergearray(int* arr, int number)
{
  int i;
  int max,min;
  double avg; 
  long amount=0;
  if(number<=0)
  {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5)
  {
    //less than 5, calculated directly statistics
    for(i=0;i<number;i++)
    {
      amount+=arr[i];
    }
    avg = amount/number; 
    return avg;
  }
  else
  {
    if(arr[0]<arr[1])
    {
      min = arr[0];
      max=arr[1];
    }
    else
    {
      min=arr[1];
      max=arr[0];
    }
    for(i=2;i<number;i++)
    {
      if(arr[i]<min)
      {
        amount+=min;        //arr<min
        min=arr[i]; 
      }
      else
      {
        if(arr[i]>max)
        {
           amount+=max;    //arr>max
           max=arr[i]; 
        }
        else
        {
           amount+=arr[i]; //min<=arr<=max
        }
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;  
}
