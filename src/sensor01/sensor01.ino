#include <Payload.h>
#include <Sender.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

const uint16_t this_node   = 01;
const uint8_t  sensor_port = 4;

RF24 radio(7,8);
RF24Network network(radio);

void callback(){
  int data = digitalRead(sensor_port);
  if (sendMessage("inclinacao",String(data), network))
    Serial.println("ok");
  else
    Serial.println("error");
}

void setup(void){
  Serial.begin(57600);
  Serial.println("No 01 - sensor de inclinacao - chave de mercúrio");
  
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
}

void loop(){
  network.update();
  timer(2000, callback);
}
