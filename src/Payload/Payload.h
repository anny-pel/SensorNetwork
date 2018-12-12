struct payload_t {
  //tamalho máximo do pacote 8+4+4+4+4+16+16=56bytes
  //a 256Kbps tempo de envio por pacote de 1,75ms ~ 2ms por salto
  unsigned long m_ms;//timestamp 8bytes
  unsigned int m_counter;//contador 4bytes
  unsigned int m_rtt;//tempo de resposta millisegundos 4bytes
  unsigned int m_pdr;//taxa de entrega recebidos/enviados% 4bytes
  unsigned int m_interval;//taxa de envio em millisegundos 4byte
  char m_text[16];
  char m_data[16];
  //construtuor padrão (sem parâmetros)
  payload_t():
    m_ms(0),
    m_counter(0),
    m_rtt(0),
    m_pdr(0),
    m_interval(0){};

  //construtor com parâmetros
  payload_t(unsigned long ms, unsigned int counter, unsigned int rtt, unsigned int pdr, unsigned short interval, String textStr, String dataStr):
    m_ms(ms),
    m_counter(counter),
    m_rtt(rtt),
    m_pdr(pdr),
    m_interval(interval)
  {
    textStr.toCharArray(m_text, 16);
    dataStr.toCharArray(m_data, 16);
  };
  //facilitar impressão do pacote
  void toString(String &str){
    str = "Sensor ";
    str += m_text;
    str += " valor:";
    str += m_data;
    str += " contador:";
    str += m_counter;
    str += " timestamp:";
    str += m_ms;
    str += "ms RTT:";
    str += m_rtt;
    str += "ms PDR:";
    str += m_pdr;
    str += "\% intervalo:";
    str += m_interval;
  };
  //facilitar impressão no master
  void toStringFormated(String &str){
    //sprintf();
    char buffer[20];
    sprintf(buffer,"%-5u ",m_counter);
    str += buffer;
    sprintf(buffer,"%-10lu ",m_ms);
    str += buffer;
    sprintf(buffer,"%-5u ",m_rtt);
    str += buffer;
    sprintf(buffer,"%-3u ",m_pdr);
    str += buffer;
    sprintf(buffer,"%-15s ",m_text);
    str += buffer;
    sprintf(buffer,"%-15s ",m_data);
    str += buffer;
    sprintf(buffer,"%-5u",m_interval);
    str += buffer;
  };
};
