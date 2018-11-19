#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <Payload.h>

RF24 radio(7,8);
RF24Network network(radio);

const uint16_t master_node = 00;

unsigned long last_sent;

//rotina para envio de mensagem
bool sendMessage(unsigned int counter, unsigned int rtt, unsigned int pdr, unsigned int interval, String text, String data){
  Serial.println("Sending...");
  payload_t payload(millis(), counter, rtt, pdr, interval, text, data);
  RF24NetworkHeader header(master_node);
  return network.write(header,&payload,sizeof(payload));
};

//rotina para revebimento de mensagem
bool receiveMessage(RF24NetworkHeader &header,payload_t &payload){
  if(network.available()){
    network.read(header,&payload,sizeof(payload));
    Serial.println("Received");
    return true;
  }
  return false;
}

//rotina para definir rotina de callback quando o intervalo termina
void timer(const unsigned long interval, void (*callback_func)(void)){
  unsigned long now = millis();
  if (now - last_sent >= interval){
    callback_func();
    last_sent = now;
  }
};

//atualiza a rede
void updateNetwork(void){
  network.update();
};

//inicia a rede
void beginNetwork(uint16_t channel, uint16_t address){
  SPI.begin();
  radio.begin();
  network.begin(channel, address);
};
