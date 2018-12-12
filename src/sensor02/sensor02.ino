#include <Sender.h>
#include <EEPROM.h>

const uint16_t this_node  = 02;
unsigned int interval     = 2000;//intervalo de envio das mensagens em millisegundos
unsigned int milliseconds = 0;//contador em millisegundos, para gerar interrupção quando for igual ao intervalo
unsigned int sent         = 0;//paotes enviados
unsigned int received     = 0;//pacotes recebidos
unsigned int rtt          = 0;//tempo de resposta millisegundos 4bytes
unsigned int pdr          = 0;//taxa de entrega recebidos/enviados% 4bytes
bool send                 = true;
bool receive              = false;

void callback(){
  //digitalRead(4);
  String data;
  if((~PIND & (1 << PD4)) >> PD4){
    data = "detected";
  }else{
    data = "not detected";
  }
  if (sendMessage(received+1, rtt, pdr, interval,"obstacle",data)){
    sent++;
    Serial.println("ok");
    send = false;
    receive = true;    
  }else{
    Serial.println("error");
  }
}

//rotina para impressão do menu de configuração do endereço
void printError(){
  Serial.println("\nEntrada inválida!!");
  Serial.print("Informe o endereço em octal com digitos entre 1 a 5 (zero somente no início), máximo de 4 algarismos [ex: 12,02134]:");
}

void setup(void){
  DDRD  &= ~(1 << PD4);PORTD &= ~(1 << PD4);//pinMode(4,INPUT);
  Serial.begin(115200);
  Serial.println("No 02 - sensor de obstáculo - infravermelho");

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
  //Timer operando em modo CTC e Com saídas OC1A e OC1B desconectadas
  //Compare Output Mode COM1A = 00 e WGM = 12 = 0xC
  //TIMER COUNTER CONTROL REGISTER A >>>  COM2A|COM2A|COM2B|COM2B|     |     |WGM21|WGM20
  //TIMER COUNTER CONTROL REGISTER B >>>  FOC2A|FOC2B|     |     |WGM22| CS22| CS21| CS20
  //TIMER INTERRUPTION MASK          >>>       |     |     |     |     |OCIEB|OCIEA| TOIE
  TCCR2A  = 0x02;//Modo CTC          >>>    0  |  0  |  0  |  0  |  0  |  0  |  1  |  0
  TCCR2B  = 0x05;// Prescaler 128    >>>    0  |  0  |  0  |  0  |  0  |  1  |  0  |  1
  //Definindo valor limite para comparação
  OCR2B  = 0x7D;//contagem -->> (125*128*62,5)/1000000000 = 1ms
  TCNT2  = 0x00;//iniciando contador
  TIMSK2 = 0x04;//Habilita interrupção do Comparador da Saída B, foi usado o TIMER2 B pois a porta do TIMER2 A é utilizada pelo SPI  
}

void loop(){
  updateNetwork();
  //timer(interval, callback);
  if(send){
    callback();
  }
  if(receive){
    RF24NetworkHeader header;
    payload_t payload;
    //se receber a resposta: recalcula RTT e PDR, e atualiza o intervalo para o próximo envio
    if(receiveMessage(header,payload)){
      receive = false;
      rtt =  millis() - payload.m_ms;
      received++;
      pdr = ((float)received/(float)sent)*100;
      if(payload.m_interval > 1000){
        interval = payload.m_interval;
      }
    }
  }
}
//Rotina de interrupção para contagem dos millisegundos
ISR(TIMER2_COMPB_vect){
  TCNT2 = 0x00;// Reinicializa o registrador do Timer2
  milliseconds++;
  if(milliseconds >= interval){
    milliseconds = 0;
    send = true;
  }
}
