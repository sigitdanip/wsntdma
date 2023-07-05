#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
DHT dht(2, DHT11);
RF24 rf24(7, 8);

struct data
{
  int tandanode;
  float temperature;
  float humidity;
  int jam;
  int menit;
  int detik;
};

struct data1
{
  String tandanode;
  String temperature;
  String humidity;
  String jam;
  String menit;
  String detik;
};

struct data2
{
  String tandanode;
  String temperature;
  String humidity;
  String jam;
  String menit;
  String detik;
};

struct data receivedData;
struct data2 sentData;
struct data1 sentDataR;

const unsigned long interval = 15000;
bool sendDataNode1 = false;
bool sendDataNode11 = false;
bool sendDataNode12 = false;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  rf24.begin();
  rf24.setDataRate(RF24_2MBPS);
  rf24.openReadingPipe(1, 0xF0F0F0F0E1LL);
  rf24.openWritingPipe(0xF0F0F0F0E2LL);
  rf24.setPALevel(RF24_PA_MIN);
  rtc.begin();
}

void loop()
{

  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

  if(currentMillis - 0 >= interval){
  if (currentMillis % interval >= 6000 && currentMillis % interval <= 9000)
  {
    if (!sendDataNode1)
    {
      sendDataNode1 = true;

      sentDataR.tandanode = String(1);
      sentDataR.temperature = String(dht.readTemperature());
      sentDataR.humidity = String(dht.readHumidity());
      sentDataR.jam = String(now.hour());
      sentDataR.menit = String(now.minute());
      sentDataR.detik = String(now.second());

      if (isnan(sentDataR.humidity) || isnan(sentDataR.temperature))
      {
        Serial.println("Failed to read DHT11 sensor!");
        return;
      }

      rf24.stopListening();
      rf24.write(&sentDataR, sizeof(sentDataR));
      Serial.print("Sent data node 1 to Gateway at: ");
      Serial.print(sentDataR.jam);
      Serial.print(":");
      Serial.print(sentDataR.menit);
      Serial.print(":");
      Serial.print(sentDataR.detik);
      Serial.println();
    }
  }
  else
  {
    sendDataNode1 = false;
  }

  if (currentMillis % interval >= 0 && currentMillis % interval <= 3000)
  {
      rf24.startListening();
      if (rf24.available())
      {
        rf24.read(&receivedData, sizeof(receivedData));

        if (receivedData.tandanode == 11)
        {
          Serial.print("Received Data from Node: ");
          Serial.print(receivedData.tandanode);
          Serial.print(", Temperature: ");
          Serial.print(receivedData.temperature);
          Serial.print(" °C, ");
          Serial.print("Humidity: ");
          Serial.print(receivedData.humidity);
          Serial.print(" %, ");
          Serial.print(now.hour());
          Serial.print(":");
          Serial.print(now.minute());
          Serial.print(":");
          Serial.print(now.second());
          Serial.println();
        }
      }
  }
      if (currentMillis % interval >= 3000 && currentMillis % interval <= 6000){
          if(!sendDataNode11)
          {
            sendDataNode11 = true;

          sentData.tandanode = String(receivedData.tandanode);
          sentData.temperature = String(receivedData.temperature);
          sentData.humidity = String(receivedData.humidity);
          sentData.jam = String(now.hour());
          sentData.menit = String(now.minute());
          sentData.detik = String(now.second());

          rf24.stopListening();
          rf24.write(&sentData, sizeof(sentData));
          Serial.print("Sent data node 11 to Gateway at ");
          Serial.print(sentData.jam);
          Serial.print(":");
          Serial.print(sentData.menit);
          Serial.print(":");
          Serial.print(sentData.detik);
          Serial.println();
        }
      }
      else
      {
         sendDataNode11 = false;
      }
       
  if (currentMillis % interval >= 9000 && currentMillis % interval <= 12000)
  {
      rf24.startListening();

      if (rf24.available())
      {
        rf24.read(&receivedData, sizeof(receivedData));
        if (receivedData.tandanode == 12)
        {
          Serial.print("Received Data from Node: ");
          Serial.print(receivedData.tandanode);
          Serial.print(", Temperature: ");
          Serial.print(receivedData.temperature);
          Serial.print(" °C, ");
          Serial.print("Humidity: ");
          Serial.print(receivedData.humidity);
          Serial.print(" %, ");
          Serial.print(now.hour());
          Serial.print(":");
          Serial.print(now.minute());
          Serial.print(":");
          Serial.print(now.second());
          Serial.println();
        }
      }
  }
    if (currentMillis % interval >= 12000 && currentMillis % interval <= 15000)
  {
          sentData.tandanode = String(receivedData.tandanode);
          sentData.temperature = String(receivedData.temperature);
          sentData.humidity = String(receivedData.humidity);
          sentData.jam = String(now.hour());
          sentData.menit = String(now.minute());
          sentData.detik = String(now.second());

          rf24.stopListening();
          rf24.write(&sentData, sizeof(sentData));
          Serial.print("Sent data node 12 to Gateway at: ");
          Serial.print(sentData.jam);
          Serial.print(":");
          Serial.print(sentData.menit);
          Serial.print(":");
          Serial.print(sentData.detik);
          Serial.println();
      }
  }
}
