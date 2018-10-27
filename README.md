# Rede de sensores

O projeto tem como objetivo implementar uma rede de sensores doméstica, utilizando comunicação via rádio através do transceiver nRF24l01+. Um nó da rede (master) será responsável por apresentar todos os dados recebidos dos sensores e configurar o intervalo de transmissão das mensagens enviadas pelos sensores. A exibição dos dados e a configuração desse parâmetro será realizada via serial, utilizando um monitor serial Desktop.

<img src="imgs/nRF24l01Plus.jpg" width="170"/> 

Para criação da rede serão utilizadas as bibliotecas [RF24Network](https://tmrh20.github.io/RF24Network/) e [RF24Mesh](http://tmrh20.github.io/RF24Mesh/). O projeto pretende, ainda, explicitar as diferenças entre as bibliotecas e os cenários de uso mais adequados para cada uma.

<img src="imgs/diagrama.gif"/>


## Equipamentos
- 6 x nRF24l01+
- 5 sensores a serem definidos ao longo do projeto

## Integrantes
- [Anny Caroline](https://github.com/AnnyCaroline/)
- [Wellington](https://github.com/wellington34226)
