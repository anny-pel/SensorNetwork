# Rede de sensores

O projeto tem como objetivo implementar uma rede de sensores doméstica, utilizando comunicação via rádio através do transceiver nRF24l01+. Um nó da rede (master) será responsável por apresentar todos os dados recebidos dos sensores e configurar o intervalo de transmissão das mensagens enviadas pelos sensores. A exibição dos dados será realizada via serial, utilizando uma aplicação Desktop.

<img src="imgs/nRF24l01Plus.jpg" width="170"/> 

Para criação da rede será utilizadas a biblioteca [RF24Network](https://tmrh20.github.io/RF24Network/).

<img src="imgs/diagrama.gif"/>

## Integrantes
- [Anny Caroline](https://github.com/AnnyCaroline/)
- [Wellington](https://github.com/wellington34226)

## Equipamentos
- 6 x nRF24l01+
- 5 sensores
  - sensor de inclinação com chave de mercúrio;
  - sensor touch capacitivo;
  - sensor de obstáculo infravermelho;
  - 2x sensor de movimento / giroscópio.

## Endereçamento
- **Via serial:** quando um nó é ligado, ele aguarda 10 segundos o recebimento de algum endereço via serial. O endereço informado é gravado na EEPROM;
- **Via EEPROM:** caso nenhum endereço seja inserido, tenta-se obter o último endereço gravado na EEPROM;
- **Via código:** caso não se consiga obter um endereço válido da EEPROM, o endereço inserido via código é utilizado.

## Aplicação desktop
Desenvolvida em [pico-Céu](https://github.com/ceu-lang/pico-ceu), a aplicação desktop apresenta, em tempo real, a árvore de sensores e o conteúdo dos últimos pacotes enviados.


