// Código do ESCRAVO
/*Autores: 
Leonardo Gonçalves
Lucas Mantuan Ayres
Pablo France Salarolli
*/
#include "gainProtocol.h"

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
