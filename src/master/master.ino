#include <Payload.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7,8);
RF24Network network(radio);
const uint16_t this_node = 00;
uint8_t  nivel1 = 0;
uint8_t  nivel2 = 0;
uint8_t  nivel3 = 0;
uint16_t nivel4 = 0;
uint16_t nivel5 = 0;

uint8_t getNivel(uint16_t address){
  uint8_t count = 0;
  while(address > 0){
    count++;
    address = address >> 3;
  }
  return count;
}

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
    String msg;
    payload.toString(msg);
    Serial.print("Received from node:");
    Serial.print(header.from_node,OCT);
    Serial.print(" ");
    Serial.println(msg);
    //taxas de 250Kbps, 1Mbps ou 2Mbps, pacotes de 56bytes demoram aproximandamente 2ms por salto
    //cada salto acrescenta um delay de 5ms, quanto mais alto o nível maior a prioridade
    switch(getNivel(header.from_node)){
      case 1:
        nivel1++;
        payload.m_interval = (nivel1*5)+2*(nivel2*5)+3*(nivel3*5)+4*(nivel4*5)+5*(nivel5*5) + 1000;
        break;
      case 2:
        nivel2++;
        payload.m_interval = (nivel2*5)+3*(nivel3*5)+4*(nivel4*5)+5*(nivel5*5) + 1000;
        break;
      case 3:
        nivel3++;
        payload.m_interval = (nivel3*5)+4*(nivel4*5)+5*(nivel5*5) + 1000;
        break;
      case 4:
        nivel4++;
        payload.m_interval = (nivel4*5)+5*(nivel5*5) + 1000;
        break;
      case 5:
        nivel5++;
        payload.m_interval = (nivel5*5) + 1000;
        break;
      default:
        Serial.print("Nivel:");
        Serial.println(getNivel(header.from_node));
    };
    header.to_node = header.from_node;
    network.write(header,&payload,sizeof(payload));
  }
}

