#include <Payload.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7,8);
RF24Network network(radio);
const uint16_t this_node = 00;

void setup(void)
{
  Serial.begin(57600);
  Serial.println("master node");
 
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
}

void loop(void){
  network.update();
  
  while ( network.available() ){
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.m_counter);
    Serial.print(" from node ");
    Serial.print(payload.m_text);
    Serial.print(", data: ");
    Serial.print(payload.m_data);
    Serial.print(" at ");
    Serial.println(payload.m_ms);
  }
}

