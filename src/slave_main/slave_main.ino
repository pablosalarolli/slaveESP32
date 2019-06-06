// Código do ESCRAVO
/*Autores:
  Leonardo Gonçalves
  Lucas Mantuan Ayres
  Pablo France Salarolli
*/
#include "gainProtocol.h"

byte addr = 0, opcode = 0;
int dado = 0, flag = 0;

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  pinMode(2, OUTPUT);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  digitalWrite(2, 0);
}

void loop() {
  while (!Serial2.available()) {}
  flag = recebeMensagem(&addr, &opcode, &dado);
  Serial.print("\nflag ");
  Serial.println(flag, DEC);
  Serial.print("addr ");
  Serial.println(addr, HEX);
  Serial.print("opcode ");
  Serial.println(opcode, HEX);
  Serial.print("dado ");
  Serial.println(dado, HEX);
}
