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
  
  uint16_t address_node = 0;
  uint16_t bufAddress = 0;

  int buf = 0 ;
  uint64_t timeDelay = millis();
  Serial.print("Informe o endereço em octal com digitos entre 1 a 5 (zero somente no início), máximo de 4 algarismos [ex: 12,02134]:");
  while((millis() < timeDelay + 10000) && (address_node == 0)){
    buf = 0;
    if(Serial.available()){
      timeDelay = millis();
      buf = Serial.read();
      Serial.print((char)buf);
      if((buf >= 48) && (buf < 54)){
        if((buf == 48) && (bufAddress > 0)){
          bufAddress = 0;
          printError();
        }else{
          bufAddress *= 8;
          bufAddress += buf - 48;
        }
      }else
      if((buf == 13) && (bufAddress <= 05555)){//maior endereço permitido 05555 (octal)
        address_node = bufAddress;
        bufAddress = 0;
      }else{
        bufAddress = 0;
        printError();
      }
    }
  }
  Serial.println();
  if(address_node==0){
    if((bufAddress > 0) && (bufAddress <= 05555)){
      address_node = bufAddress;
      EEPROM.put(0,address_node);//salva endereço na EEPROM
      Serial.print("\nNovo endereço salvo na EEPROM\nUtilizando endereço informado:");
    }else{
      bufAddress  = EEPROM.read(0);
      bufAddress |= EEPROM.read(1) << 8;
      if((bufAddress > 0) && (bufAddress <= 05555)){
        //se valor gravado na EEPROM for válido o utiliza para endereçanto
        address_node = bufAddress;
        Serial.print("\nUtilizando endereço gravado na EEPROM:");
      }else{
        address_node = this_node;
        Serial.print("\nUtilizando endereço gravado no código:");
      }
    }
  }else{
    EEPROM.put(0,address_node);//salva endereço na EEPROM
    Serial.print("\nNovo endereço salvo na EEPROM\nUtilizando endereço informado:");
  }
  Serial.println(address_node,OCT);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
}

void loop(){
  network.update();
  timer(2000, callback);
}
