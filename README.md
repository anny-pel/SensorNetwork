# Rede de sensores

O projeto tem como objetivo implementar uma rede de sensores doméstica, utilizando comunicação via rádio através do transciver RF24L01+. Um nó da rede (master) será responsável por apresentar todos os dados recebidos dos sensores e configurar o intervalo de transmissão das mensagens enviadas pelos sensores. A exibição dos dados e a configuração desse parâmetro será realizada via serial, utilizando um monitor serial Desktop.

![RF24L01+](imgs/RF24L01Plus.jpg)
![diagrama](imgs/diagrama.jpg)

Para criação da rede serão utilizadas as bibliotecas [RF24Network](https://tmrh20.github.io/RF24Network/) e [RF24Mesh](http://tmrh20.github.io/RF24Mesh/). O projeto pretende, ainda, explicitar as diferenças entre as bibliotecas e os cenários de uso mais adequados para cada uma.

## Equipamentos

- 4 x RF24L01+
