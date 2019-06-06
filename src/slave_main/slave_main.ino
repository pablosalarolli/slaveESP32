// Código do ESCRAVO
/*Autores:
  Leonardo Gonçalves
  Lucas Mantuan Ayres
  Pablo France Salarolli
*/
#include "gainProtocol.h"

/*================================ CONSTANTES ===============================*/

#define MEU_ADDR 0x01
#define MASTER_ADDR 0x0F

#define PIN_420 12
#define PIN_LM35 14

#define LED_ON 23
#define LED_BARRAMENTO 22
#define LED_SENSOR 19
#define LED_SPVP 18

#define RX_PIN 16
#define TX_PIN 17
#define SERIAL_CTRL 2

/*================================ TYPE DEFS ================================*/

enum slaveStates {
  AGUARDANDO,             // Estado ligado, aguardando chegada de um frame
  RECEBE_MSG,             // Estado responsável por tratar a mensagem recebida
  ATUALIZA_SP,            // Estado que atualiza o valor do set-point no escravo
  VERIFICA_SP,            // Estado que verifica o valor do set-point no escravo
  ATUALIA_SENSOR,         // Estado que atualiza qual sensor usado no escravo
  VERIFICA_SENSOR,        // Estado que verifica qual sensor usado no escravo
  LER_SENSOR,             // Estado que efetua a medição do sensor
  RESPONDE,               // Estado que responde ao mestre
  ATUALIZA_BUFFER_ERRO,   // Estado que atualiza o buffer de erros com a flag
  VERIFICA_BUFFER_ERRO    // Estado que verifica o buffer de erros com a flag
};

/*========================== VARIÁVEIS DO PROTOCOLO =========================*/

//Variáveis relacionadas ao protocolo
byte addr = 0, opcode = 0;
int dado = 0, flag = 0;

/*=========================== VARIÁVEIS DO ESCRAVO ==========================*/

//Variáveis internas do escravo
int set_point = 40;
int sensor = 0; // 0 = 4-20 mA, 1 = LM35
int buffer_erro[10];
enum slaveStates estado = AGUARDANDO;

/*============================== SETUP E LOOP ===============================*/

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
}

void loop() {
  switch (estado) {
    case AGUARDANDO:
      if (Serial2.available())
        estado = RECEBE_MSG;
      break;

    case RECEBE_MSG:
      flag = recebeMensagem(&addr, &opcode, &dado);
      if (flag)
        estado = ATUALIZA_BUFFER_ERRO;
      else
        estado = trataMSG();
      break;

    case ATUALIZA_SP:
      set_point = dado;
      estado = AGUARDANDO;
      break;

    case ATUALIA_SENSOR:
      sensor = dado;
      estado = AGUARDANDO;
      break;

    case VERIFICA_SP:
      dado = set_point;
      estado = RESPONDE;
      break;

    case VERIFICA_SENSOR:
      dado = sensor;
      estado = RESPONDE;
      break;

    case LER_SENSOR:
      // ler o sensor dado = lerSensor(sensor);
      estado = RESPONDE;
      break;

    case RESPONDE:
      enviaMensagem(MASTER_ADDR, opcode, dado);
      estado = AGUARDANDO;
      break;
    //
    //    case ATUALIZA_BUFFER_ERRO:
    //      estado = ;
    //      break;
    //
    //    case VERIFICA_BUFFER_ERRO:
    //      estado = ;
    //      break;

    default:
      flag = 4;
      estado = ATUALIZA_BUFFER_ERRO;
      break;
  }


  //  Serial.print("flag ");
  //  Serial.println(flag, DEC);
  //  Serial.print("addr ");
  //  Serial.println(addr, HEX);
  //  Serial.print("opcode ");
  //  Serial.println(opcode, HEX);
  //  Serial.print("dado ");
  //  Serial.println(dado, HEX);
}

/*=========================== FUNCOES AUXILIARES ============================*/

enum slaveStates trataMSG() {
  if (addr != MEU_ADDR)
    return AGUARDANDO;
  switch (opcode) {
    case 0b0000:
      estado = ATUALIZA_SP;
      break;
    case 0b0001:
      estado = VERIFICA_SP;
      break;
    case 0b0010:
      estado = ATUALIA_SENSOR;
      break;
    case 0b0011:
      estado = VERIFICA_SENSOR;
      break;
    case 0b0100:
      estado = LER_SENSOR;
      break;
    case 0b0101:
      estado = VERIFICA_BUFFER_ERRO;
      break;
    default:
      flag = 3;
      estado = ATUALIZA_BUFFER_ERRO;
      break;
  }
}

int lerSensor(int sensor) {
  if (sensor)
    return analogRead(PIN_420);
  else
    return analogRead(PIN_LM35);
}
