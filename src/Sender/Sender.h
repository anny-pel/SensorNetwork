#include <RF24Network.h>

const uint16_t master_node = 00;

unsigned long last_sent;
unsigned long packets_sent;

boolean sendMessage(String text, String data, RF24Network network){
  Serial.println("Sending...");
  payload_t payload = {millis(), packets_sent++, text, data};
  RF24NetworkHeader header(master_node);
  return network.write(header,&payload,sizeof(payload));
}

void timer(const unsigned long interval, void (*callback_func)(void)){
  unsigned long now = millis();
  if (now - last_sent >= interval){
    callback_func();
    last_sent = now;
  }  
}