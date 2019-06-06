﻿#include "gainProtocol.h"

/* enviaMensagem: envia uma mensagem completa do nosso protocolo
Entradas: endereço de destino (addr), código da operação (opcode) e o dado (dado)
Saídas: ---
*/
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

/* montaCabecalho: monta o cabeçalho (1 byte) a partir dos nibbles addr e opcode
Entradas: addr e opcode (1 nibble cada)
Saídas: retorna o cabeçalho completo com 1 byte
*/
byte montaCabecalho(byte addr, byte opcode){
  return byte(((addr & 0x0F) << 4) | (opcode & 0x0F));
}

/* divideDado: Divide a palavra do dado em dois bytes para transmissão
Entradas: dado e ponteiros para saídas
Saídas: 
- valores dos bytes mais e menos significativos dadoA e dadoB, respectivamente, retornados através de ponteiros recebidos nos parêmetros de entrada
*/
void divideDado(int dado, byte *dadoA, byte *dadoB){
  *dadoA = byte((dado & 0xFF00) >> 8);
  *dadoB = byte(dado & 0x00FF);
}

/* geraChecksum: Retorna um byte de checksum a partir de uma dada soma de entrada
Entradas: soma (16 bits) dos bytes que deseja-se transmitir ou que foram recebidos
Saídas: retorna o byte checksum

*/
byte geraChecksum(int soma){
  int wrappsoma = 0;
  byte checksum = 0;
  wrappsoma = soma >> 8;
  wrappsoma = wrappsoma+(soma & 0xFF);
  checksum = ~wrappsoma;
  return checksum;
}

/* recebeMensagem: Trata da mensaem recebida 
Entradas: ---
Saídas: 
- addr, opcode e dado, todos retornados através de ponteiros recebidos nos parêmetros de entrada
- retorno de uma flag de erro:
  - flag = 0 -> Sem erros
  - flag = 1 -> Erro no número de bytes recebidos
  - flag = 2 -> Erro de checksum

*/
int recebeMensagem(byte *addr, byte *opcode, int *dado){
  byte buff[4], cabecalho, dadoA, dadoB, checksum;
  int i = 0;
  int flag = 1;
  while(Serial2.available()>0){
    if(i<4){
      buff[i] = Serial2.read();
      if(i==3){
        flag = 0;
      }
    }
    else{
      Serial2.read();
      flag = 1;
    }
    i++;
  }
  if(flag){
	Serial2flush();
    return flag;
  }
  cabecalho = buff[0];
  dadoA = buff[1];
  dadoB = buff[2];
  checksum = buff[3];
  if(geraChecksum(cabecalho + dadoA + dadoB + checksum)){
    flag = 2;
    return flag;
  }
  divideCabecalho(cabecalho, addr, opcode); //Já são ponteiros, por isso a não necessidade de endereçar (&addr,&opcode)
  *dado = montaDado(dadoA, dadoB);
  return flag;
}

/* divideCabecalho: Divide o byte do cabeçalho nas informações de endereço e código de operação
Entradas: cabeçalho e ponteiros para saídas
Saídas: 
- addr, opcode, todos retornados exclusivamente através de ponteiros recebidos nos parêmetros de entrada
*/
void divideCabecalho(byte cabecalho, byte *addr, byte *opcode){
  *addr = byte((cabecalho & 0xF0) >> 4);
  *opcode = byte(cabecalho & 0x0F);
}

/* montaDado: monta o dado (2 bytes) a partir dos bytes mais (A) e menos (B) significativos
Entradas: bytes mais (dadoA) e menos (dadoB) significativos
Saídas: retorna o dado completo com 16 bits
*/
int montaDado(byte dadoA, byte dadoB){
  return int(((dadoA & 0x00FF) << 8) | (dadoB & 0x00FF));
}

/* Serial2flush: limpa buffer de entrada da porta serial
Entradas: ---
Saídas: ---
*/
void Serial2flush(void){
	while(Serial2.available() > 0) {
		char t = Serial2.read();
	}
}
