// Estrelinha
// Autor: Eduardo Freitas
// Data: 14/07/2019 
// Versão: 0.42 

#include <Servo.h>                    // BIBLIOTECA NECESSÁRIA

/****************************************************************************************************/

                                  /// PINAGEM

// SENSORES

#define LDR_1 A0                      // SUPERIOR DIREITO
#define LDR_2 A1                      // SUPERIOR ESQUERDO
#define LDR_3 A2                      // INFERIOR ESQUERDO
#define LDR_4 A3                      // INFERIOR DIREITO

// SERVOS

#define PIN_SERVO_V 11                // SERVO VERTICAL
#define PIN_SERVO_H 10                // SERVO HORIZONTAL 

// BOTÃO

#define PIN_BOTAO 9                   // BOTÃO

/****************************************************************************************************/

                                /// CONFIGURAÇÕES


#define SENSE 50                       // SENSIBILIDADE DO SISTEMA
#define ATRASO 30                      // VELOCIDADE DO SISTEMA
#define ATRASO_TESTE 15                // VELOCIDADE DOS SERVO DURANTE TESTE
#define ESTRELINHA 6                   // SENSIBILIDADE ESCURIDÃO
/*******************************************************************************************************************/

                                 /// ATUADORES


Servo horizontal;                      // OBJETO SERVO HORIZONTAL 
#define h_limite_min 45                // LIMITES DO EIXO "HORIZONTAL" 
#define h_limite_max 135

Servo vertical;                        // OBJETO SERVO VERTICAL
#define v_limite_min 30                // LIMITES DO EIXO "VERTICAL"
#define v_limite_max 140

/*********************************************************************************************************************/

                                 /// VARIÁVEIS

// SERVOS

int servo_h =  90;                     // SERVO HORIZONTAL
int servo_v =  90;                     // SERVO VERTICAL

// CONTADORES

unsigned long contagem_total = 0;     // CONTA QUANTAS ROTAÇÕES O MOTOR FEZ DURANTE ANÁLISE
unsigned long ultima_contagem = 0;    // NECESSÁRIO PARA OBTER RPM
int rpm = 0;                          // ROTAÇÕES POR MINUTO

// TEMPO

unsigned long tempo;                 // CONTA O TEMPO EM MILISEGUNDOS APÓS LIGAR ARDUINO
unsigned long intervalo = 60000;     // [ 60000 ] UM MINUTO, NÃO MEXER!
unsigned long tempo_anterior = 0;
int minutos = 0;                     // USADO NO MONITOR SERIAL PARA DEFINIR O TEMPO EM QUE AMOSTRA FOI COLETADA

// BOTÃO

boolean estado_anterior;
boolean botao;

/**********************************************************************************************************/

void setup() {

  Serial.begin(9600);                  // INICIA COMUNICAÇÃO SERIAL

  pinMode(pin_botao, INPUT);
  estado_anterior = digitalRead(PIN_BOTAO);


  horizontal.attach(PIN_SERVO_H);      // ATRACA OBJETO SERVO HORIZONTAL AO PINO 10
  vertical.attach(PIN_SERVO_V);        // ATRACA OBJETO SERVO VERTICAL AO PINO 11

  horizontal.write(servo_h);

  delay(500);
  
/***************************************************************************************/

        /// AUTO TESTE MECÃNICO

  for(servo_v = 90; servo_v >= 30; servo_v--){
    vertical.write(servo_v);
    delay(ATRASO_TESTE);
  }
  for(servo_v = 30; servo_v <= 150; servo_v++){
    vertical.write(servo_v);
    delay(ATRASO_TESTE);
  }
  for(servo_v = 150; servo_v >= 90; servo_v--){
    vertical.write(servo_v);
    delay(ATRASO_TESTE);
  }

 delay(1000);

  for(servo_h = 90; servo_h >= 45; servo_h--){
    horizontal.write(servo_h);
    delay(ATRASO_TESTE);
  }
  for(servo_h = 45; servo_h <= 135; servo_h++){
    horizontal.write(servo_h);
    delay(ATRASO_TESTE);
  }
  for(servo_h = 135; servo_h >= 90; servo_h--){
    horizontal.write(servo_h);
    delay(ATRASO_TESTE);
  }

        /// FIM DO TESTE
      
  delay(1000);                         // TEMPO NECESSÁRIO PARA O SISTEMA ATIGINR A POSIÇÃO PADRÃO
}

void loop(){

  tempo = millis();                    // CONTA O TEMPO DESDE O PRIMEIRO LOOP

  int sd = analogRead(LDR_1);          // REALIZA LEITURA ANALOGICA DO LDR 1 E ATRIBUE À "SD" (SUPERIOR DIREITO)
  int se = analogRead(LDR_2);          // REALIZA LEITURA ANALOGICA DO LDR 2 E ATRIBUE À "SE" (SUPERIOR ESQUERDO)
  int ie = analogRead(LDR_3);          // REALIZA LEITURA ANALOGICA DO LDR 3 E ATRIBUE À "IE" (INFERIOR ESQUERDO)
  int id = analogRead(LDR_4);          // REALIZA LEITURA ANALOGICA DO LDR 4 E ATRIBUE À "ID" (INFERIOR DIREITO)

  int m_superior = (sd + se) / 2;      // MÉDIA DOS QUADRANTES ACIMA DA ABSCISSA
  int m_inferior = (id + ie) / 2;      // MÉDIA DOS QUADRANTES ABAIXO DA ABSCISSA
  int m_direita  = (sd + id) / 2;      // MÉDIA DOS QUADRANTES ADIREITA DA ORDENADA
  int m_esquerda = (se + ie) / 2;      // MÉDIA DOS QUADRANTES AESQUERDA DA ORDENADA

  int delta_v  = m_superior - m_inferior;             // DIFERENÇA UTILIZADA PARA CONTROLAR OS ATUADORES
  int delta_h  = m_esquerda - m_direita;


/*******************************************************************************/

  if(-1 * SENSE > delta_v || delta_v > SENSE){        // FUNÇÃO CONTROLA O ATUADOR VERTICAL

    if(m_superior > m_inferior){
      servo_v--;
    }else if(m_superior < m_inferior){
      servo_v++;
    }
    
  vertical.write(servo_v);                                    // POSICIONA SERVO VERTICAL
  }

  if (servo_v <= v_limite_min) servo_v = v_limite_min;        // LIMITE MÍNIMO DO SERVO VERTICAL
  if(servo_v >= v_limite_max) servo_v = v_limite_max;         // LIMITE MÁXIMO DO SERVO VERTICAL

          /*****************/

  if(-1 * SENSE > delta_h || delta_h > SENSE){        // FUNÇÃO CONTROLA O ATUADOR HORIZONTAL

    if (m_esquerda > m_direita){
      servo_h++;    
    }else if (m_esquerda < m_direita){
      servo_h--;
    }
    
  horizontal.write(servo_h);                                 // POSICIONA SERVO HORIZONTAL
  }

  if (servo_h <= h_limite_min) servo_h = h_limite_min;      // LIMITE MÍNIMO DO SERVO HORIZONTAL
  if (servo_h >= h_limite_max) servo_h = h_limite_max;      // LIMITE MÁXIMO DO SERVO HORIZONTAL
  
/**************************************************************************/

  botao = digitalRead(PIN_BOTAO);                  // LEITURA DO BOTÃO

  if((botao == HIGH)&&(estado_anterior == LOW)){   // FUNÇÃO QUE EVITA MULTIPLAS LEITURAS EM UM ÚNICO CLIQUE DURANTE VÁRIOS CICLOS DO LOOP 
  contagem_total++;
  }
  estado_anterior = botao;

/**************************************************************************/
  
  if (tempo - tempo_anterior >= intervalo) {      // ATUALIZA O CONSOLE A CADA MINUTO COM RPM (ROTAÇÕES POR MINUTO) E RT (ROTAÇÕES TOTAIS)
   
    tempo_anterior = tempo;                       // GATILHO DA FUNÇÃO

    rpm = contagem_total - ultima_contagem;       // FUNÇÃO RPM
    ultima_contagem = contagem_total;

    Serial.print("MINUTO: "); Serial.print(minutos); Serial.print("   RPM: "); Serial.print(rpm); Serial.print("    TOTAL: "); Serial.print(contagem_total);
    Serial.print("\n");

    minutos++;                                    // MOSTRA MINUTO QUE AMOSTRA FOI COLETADA
  }
  
/***************************************************************************/
  
  if( (se+sd+ie+id) / 4 <= ESTRELINHA){           // GATILHO POR DO SOL 
    horizontal.write(90); 
    vertical.write(90);
  }
  
  delay(ATRASO);  
}
