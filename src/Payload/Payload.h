struct payload_t {
  unsigned long m_ms;
  unsigned long m_counter;
  char m_text[16];
  char m_data[16];

  payload_t(){}

  payload_t(unsigned long ms, unsigned long counter, String textStr, String dataStr){
    m_ms = ms;
    m_counter = counter;
    textStr.toCharArray(m_text, 16);
    dataStr.toCharArray(m_data, 16);
  }
};
