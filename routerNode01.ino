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

struct data1{
  int tandanode;
  float temperature;
  float humidity;
  uint8_t jam;
  uint8_t menit;
  uint8_t detik;
};

struct data receivedData;
struct data sentData;

sentData.jam = now.hour();
sentData.menit = now.minute();
sentData.detik = now.second();


const unsigned long interval = 12000;
const unsigned long node01ReadStart = 4000;
const unsigned long node01ReadEnd = 6000;
const unsigned long node11Start = 0;
const unsigned long node11End = 4000;
const unsigned long node12Start = 6000;
const unsigned long node12End = 10000;


void setup()
{
  Serial.begin(9600);
  dht.begin();
  rf24.begin();
  rf24.setDataRate(RF24_2MBPS);
  rf24.openReadingPipe(1, 0xF0F0F0F0E1LL);
  rf24.openWritingPipe(0xF0F0F0F0E2LL)
  rf24.setPALevel(RF24_PA_MIN);
  rtc.begin();
}

void loop()
{
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

 
  if (currentMillis % interval >= node01ReadStart && currentMillis % interval <= node01ReadEnd)
  {
    rf24.stopListening();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(humidity) || isnan(temperature))
    {
      Serial.println("Failed to read DHT11 sensor!");
      return;
    }

    Serial.println("Reading Data from Node 01:");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Jam: ");
    Serial.println(now.hour());
    Serial.print("Menit: ");
    Serial.println(now.minute());
    Serial.print("Detik: ");
    Serial.println(now.second());
    Serial.println();
  }

if (currentMillis % interval >= node11Start && currentMillis % interval <= node11End){
  rf24.startListening();

  if (rf24.available()){
    rf24.read(&receivedData, sizeof(receivedData));

    if (receivedData.tandanode == 11){
      Serial.print("Received Data from Node: ");
      Serial.println(receivedData.tandanode);
      Serial.print("Temperature: ");
      Serial.print(receivedData.temperature);
      Serial.println(" °C");
      Serial.print("Humidity: ");
      Serial.print(receivedData.humidity);
      Serial.println(" %");
      Serial.print("Jam: ");
      Serial.println();

       if(currentMillis % interval >= 2000 && currentMillis % interval <= 4000){
       
        sentData.tandanode = receivedData.tandanode;
        sentData.temperature = receivedData.temperature;
        sentData.humidity = receivedData.humidity;
        sentData.jam = now.hour();
        sentData.menit = now.minute();
        sentData.detik = now.second();

        rf24.stopListening();
        rf24.write(&sentData, sizeof(sentData));
       }
    }
  }
}

if (currentMillis % interval >= node12Start && currentMillis % interval <= node12End){
  rf24.startListening();

  if (rf24.available()){
    rf24.read(&receivedData, sizeof(receivedData));
    if (receivedData.tandanode == 12){
      Serial.print("Received Data from Node: ");
      Serial.println(receivedData.tandanode);
      Serial.print("Temperature: ");
      Serial.print(receivedData.temperature);
      Serial.println(" °C");
      Serial.print("Humidity: ");
      Serial.print(receivedData.humidity);
      Serial.println(" %");
      Serial.print("Jam: ");
      Serial.println(receivedData.jam);
      Serial.print("Menit: ");
      Serial.println(receivedData.menit);
      Serial.print("Detik: ");
      Serial.println(receivedData.detik);
      Serial.println();

      if(currentMillis % interval >= 2000 && currentMillis % interval <= 4000){
        
        sentData.tandanode = receivedData.tandanode;
        sentData.temperature = receivedData.temperature;
        sentData.humidity = receivedData.humidity;
        sentData.jam = now.hour();
        sentData.menit = now.minute();
        sentData.detik = now.second();

        rf24.stopListening();
        rf24.write(&sentData, sizeof(sentData));
       }
    }
  }
}
  delay(1000);
}
