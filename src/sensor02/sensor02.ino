#include <Payload.h>
#include <Sender.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

const uint16_t this_node   = 02;

RF24 radio(7,8);
RF24Network network(radio);

void callback(){
  //digitalRead(4);
  bool sent;
  if((~PIND & (1 << PD4)) >> PD4){
    sent = sendMessage("obstacle","detected", network);
  }else{
    sent = sendMessage("obstacle","not detected", network);
  }
  if (sent)
    Serial.println("ok");
  else
    Serial.println("error");
}

void setup(void){
  DDRD  &= ~(1 << PD4);PORTD &= ~(1 << PD4);//pinMode(4,INPUT);
  Serial.begin(115200);
  Serial.println("No 02 - sensor de obstáculo - infravermelho");

  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
}

void loop(){
  network.update();
  timer(2000, callback);
}
