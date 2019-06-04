// Código do ESCRAVO
/*Autores: 
Leonardo Gonçalves
Lucas Mantuan Ayres
Pablo France Salarolli
*/

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  pinMode(2,OUTPUT);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
}

void loop() { //Choose Serial1 or Serial2 as required
  while (Serial2.available()) {
    digitalWrite(2,1);
    Serial2.print(char(Serial2.read()));
  }
  digitalWrite(2,0);
}

void enviaMensagem(byte addr, byte opcode, int dado){
  byte cabecalho, dadoA, dadoB, checksum;
  cabecalho = montaCabecalho(addr, opcode);  
  divideDado(dado, &dadoA, &dadoB);
  checksum = geraChecksum(cabecalho + dadoA + dadoB);
  Serial2.write(cabecalho);
  Serial2.write(dadoA);
  Serial2.write(dadoB);
  Serial2.write(checksum);
}

byte montaCabecalho(byte addr, byte opcode){
  return (byte)(((addr & 0x0F) << 4) | (opcode & 0x0F));
}

void divideDado(int dado, byte* dadoA, byte* dadoB){
  *dadoA = (byte)((dado & 0xFF00) >> 8);
  *dadoB = (byte)(dado & 0x00FF);
}

byte geraChecksum(int soma){
  int wrappsoma = 0;
  byte checksum = 0;
  wrappsoma = soma >> 8;
  wrappsoma = wrappsoma+(soma & 0xFF);
  checksum = ~wrappsoma;
  return checksum;
}
