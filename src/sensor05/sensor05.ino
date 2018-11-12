#include <Payload.h>
#include <Sender.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

const uint16_t this_node   = 112;
const uint8_t  sensor_port = A0;
const uint8_t  red_led_port = 9;

RF24 radio(7,8);
RF24Network network(radio);

void callback(){
  int data = digitalRead(sensor_port);
  if (sendMessage("touch", String(data), network)){
    Serial.println("ok");
    digitalWrite(red_led_port, LOW); 
  }else{
    Serial.println("error");
    digitalWrite(red_led_port, HIGH);
  }
}

void setup(void){
  Serial.begin(57600);
  Serial.println("No 012 - sensor touch capacitivo");
  
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);

  pinMode(sensor_port, INPUT);
  pinMode(red_led_port, OUTPUT);
}

void loop(){
  network.update();
  timer(1000, callback);
}
