#include <Payload.h>
#include <Sender.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

const uint16_t this_node   = 012;//sensor 3

RF24 radio(7,8);
RF24Network network(radio);
MPU6050 accelgyro;

void callback(){
  int16_t x,y,z;
  accelgyro.getRotation(&x, &y, &z);
  String data = "(";
  data += x;
  data += ",";
  data += y;
  data += ")";
  if (sendMessage("rotation",data, network))
    Serial.println("ok");
  else
    Serial.println("error");
}

void setup(void){
  Serial.begin(115200);
  Serial.println("Nó 03 - sensor de movimento - giroscópio");
  Wire.begin();
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "Giroscópio connection successful" : "Giroscópio connection failed");
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
}

void loop(){
  network.update();
  timer(2000, callback);
}
