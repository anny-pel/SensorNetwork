# Aplicação Master Desktop

Desenvolvida em [pico-Céu](https://github.com/ceu-lang/pico-ceu), a aplicação Master Desktop exibe a topologia atual da árvore de sensores gerada pela biblioteca nRF24Network assim como os conteúdos dos pacotes enviados (RTT, medição do sensor etc.).

O arquivo ```sensorNetworkStatic.ceu``` utiliza somente a linguagem Céu e exibe uma árvore estática. Tentou-se desenvolver uma abordagem dinâmica utilizando somente a linguagem Céu (arquivo ```sensorNetwork.ceu```), mas enfrentamos problemas no uso de recursões). A solução escolhida foi utilizar um misto entre as linguagens C e Céu.

O arquivo ```arvore.c``` apresenta os primeiros passos para essa solução, que inclui a criação e inserção dos nós na árvore dinamicamente.

![arvore.c](img/arvore_c.png)


Já o ```sensorNetworkC.ceu``` utiliza a funções em C para criar e desenhar a árvore.
![arvore.c](img/arvore_ceu.png)
