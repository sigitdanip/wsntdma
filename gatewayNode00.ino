#include <Arduino.h>
#include <RF24.h>

struct SensorData {
  String temperature;
  String humidity;
  String detik;
  String menit;
  String jam;
  String tandanode;
};

RF24 rf24(D2, D1);

void setup() {
  Serial.begin(9600);
  rf24.begin();
  rf24.setDataRate(RF24_2MBPS);
  rf24.openReadingPipe(1, 0xF0F0F0F0E1LL);
  rf24.setPALevel(RF24_PA_MIN);
  rf24.startListening();
}

void loop() {
  if (rf24.available()) {
    
    SensorData data;
    rf24.read(&data, sizeof(data));

        if (isnan(humidity) || isnan(temperature))
    {
      Serial.println("Failed to read DHT11 sensor!");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(data.humidity);
    Serial.print(" %, Time: ");
    Serial.print(data.jam);
    Serial.print(":");
    Serial.print(data.menit);
    Serial.print(":");
    Serial.print(data.detik);
    Serial.print(", Tandanode: ");
    Serial.println(data.tandanode);
  }
}
