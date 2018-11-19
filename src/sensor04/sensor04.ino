#include <Sender.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <EEPROM.h>

const uint16_t this_node   = 04;//sensor 4

MPU6050 accelgyro;
unsigned int interval = 2000;//taxa de envio em segundos 1byte
unsigned int sent = 0;//paotes enviados
unsigned int received = 0;//pacotes recebidos
unsigned int rtt = 0;//tempo de resposta millisegundos 4bytes
unsigned int pdr = 0;//taxa de entrega recebidos/enviados% 4bytes

void callback(){
  String data = "(";
  data += accelgyro.getAccelerationX();
  data += ",";
  data += accelgyro.getAccelerationY();
  data += ")";
  if (sendMessage(received+1, rtt, pdr, interval,"acceleration",data)){
    sent++;
    Serial.println("ok");
  }else{
    Serial.println("error");
  }
}

void setup(void){
  Serial.begin(115200);
  Serial.println("Nó 03 - sensor de movimento - giroscópio");
  Wire.begin();
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "Giroscópio connection successful" : "Giroscópio connection failed");
  
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
  beginNetwork(90,address_node);
}

void loop(){
  updateNetwork();
  timer(interval, callback);
  RF24NetworkHeader header;
  payload_t payload;
  if(receiveMessage(header,payload)){
    rtt = payload.m_ms - millis();
    received++;
    pdr = ((float)received/(float)sent)*100;
    if(payload.m_interval > 1000){
      interval = payload.m_interval;
    }
  }
}
