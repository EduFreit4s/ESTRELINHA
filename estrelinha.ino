// Estrelinha
// Data: 16/07/2019 
// Versão: final

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

#define PIN_BOTAO 5                   // BOTÃO

/****************************************************************************************************/

                                /// CONFIGURAÇÕES

#define VELOCIDADE_TESTE 20                   // VELOCIDADE AUTO TESTE
#define TOLERANCIA 25                         // SENSIBILIDADE DO SISTEMA 
#define VELOCIDADE_SERVO 25                   // VELOCIDADE DOS ATUADORES
#define VELOCIDADE_MONITOR 60000              // TAXA DE ATUALIZAÇÃO DO MONITOR /// 60.000 == 1 MINUTO (NÃO MEXER!) 

#define ESTRELINHA 6                          // POSIÇÃO ALVORADA
#define GRAU_MINUTO 240000                    // 240.000 1°/4 MINUTOS

/*******************************************************************************************************************/

                                 /// ATUADORES


Servo horizontal;                      // OBJETO SERVO HORIZONTAL 
#define h_limite_min 50                // LIMITES DO EIXO "HORIZONTAL" 
#define h_limite_max 130

Servo vertical;                        // OBJETO SERVO VERTICAL
#define v_limite_min 30                // LIMITES DO EIXO "VERTICAL"
#define v_limite_max 142                                                                                                            ////!\\\\ //150                                              

/*********************************************************************************************************************/

                                 /// VARIÁVEIS

// SERVOS

int servo_h =  86;                     // SERVO HORIZONTAL                                                                  
int servo_v =  23;                     // SERVO VERTICAL                                                                           

// CONTADORES

unsigned long contagem_total = 0;     // CONTA QUANTAS ROTAÇÕES O MOTOR FEZ DURANTE ANÁLISE
unsigned long ultima_contagem = 0;    // NECESSÁRIO PARA OBTER RPM
int rpm = 0;                          // ROTAÇÕES POR MINUTO

// TEMPO

unsigned long tempo;                            // CONTA O TEMPO EM MILISEGUNDOS APÓS LIGAR ARDUINO
unsigned long tempo_anterior_cronologico = 0;
unsigned long tempo_anterior_servo = 0;         // UTILIZADO NOS ATUADORES
unsigned long tempo_anterior_monitor = 0;       // UTILIZADO NO MONITOR
unsigned long intervalo = 60000;                // UM MINUTO, NÃO MEXER!

int minutos = 0;                                // USADO NO MONITOR SERIAL PARA DEFINIR O TEMPO EM QUE AMOSTRA FOI COLETADA


// BOTÃO

boolean estado_anterior;
boolean botao;

/**********************************************************************************************************/

void setup() {

  Serial.begin(9600);                  // INICIA COMUNICAÇÃO SERIAL

  pinMode(PIN_BOTAO, INPUT);
  estado_anterior = digitalRead(PIN_BOTAO);


  horizontal.attach(PIN_SERVO_H);      // ATRACA OBJETO SERVO HORIZONTAL AO PINO 10
  vertical.attach(PIN_SERVO_V);        // ATRACA OBJETO SERVO VERTICAL AO PINO 11

  //horizontal.write(servo_h);                                                                                                          ////!\\\\                                                                                           

  delay(500);
  
/***************************************************************************************/

        /// AUTO TESTE MECÃNICO

/*                                                                                                                               
  -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\                                                                                                                               ////!\\\\
  for(servo_v = 90; servo_v >= 30; servo_v--){
    vertical.write(servo_v);
    delay(VELOCIDADE_TESTE);
  }
  for(servo_v = 30; servo_v <= 150; servo_v++){
    vertical.write(servo_v);
    delay(VELOCIDADE_TESTE);
  }
  for(servo_v = 150; servo_v >= 90; servo_v--){
    vertical.write(servo_v);
    delay(VELOCIDADE_TESTE);
  }

 delay(1000);

  for(servo_h = 90; servo_h >= 45; servo_h--){
    horizontal.write(servo_h);
    delay(VELOCIDADE_TESTE);
  }
  for(servo_h = 45; servo_h <= 135; servo_h++){
    horizontal.write(servo_h);
    delay(VELOCIDADE_TESTE);
  }
  for(servo_h = 135; servo_h >= 90; servo_h--){
    horizontal.write(servo_h);
    delay(VELOCIDADE_TESTE);
  }
  -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\
  */

        /// FIM DO TESTE

  vertical.write(servo_v);
  horizontal.write(servo_h);    
  delay(1000);                         // TEMPO NECESSÁRIO PARA O SISTEMA ATIGINR A POSIÇÃO PADRÃO
}


void loop(){

  tempo = millis();                    // CONTA O TEMPO DESDE O PRIMEIRO LOOP

/*
 -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\
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

  -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\
*/

/*******************************************************************************/

/// ATIVO FOTOSSENSIVEL 

/* -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\
                                                                                                                                         
if(tempo - tempo_anterior_servo >= VELOCIDADE_SERVO){
  
  tempo_anterior_servo = tempo;

  if( delta_v < (TOLERANCIA * (-1)) || delta_v > TOLERANCIA){        // FUNÇÃO CONTROLA O ATUADOR VERTICAL

    if(m_superior > m_inferior){
      servo_v--;
    }else if(m_superior < m_inferior){
      servo_v++;
    }
    
    vertical.write(servo_v);                                    // POSICIONA SERVO VERTICAL
  
    }

    if(servo_v <= v_limite_min) servo_v = v_limite_min;         // LIMITE MÍNIMO DO SERVO VERTICAL
    if(servo_v >= v_limite_max) servo_v = v_limite_max;         // LIMITE MÁXIMO DO SERVO VERTICAL


  

  if( delta_h < (TOLERANCIA * (-1)) || delta_h > TOLERANCIA){       // FUNÇÃO CONTROLA O ATUADOR HORIZONTAL

    if (m_esquerda > m_direita){
      servo_h++;    
    }else if (m_esquerda < m_direita){
      servo_h--;
    }
    
    horizontal.write(servo_h);                                 // POSICIONA SERVO HORIZONTAL
  
    }

    if(servo_h <= h_limite_min) servo_h = h_limite_min;      // LIMITE MÍNIMO DO SERVO HORIZONTAL
    if(servo_h >= h_limite_max) servo_h = h_limite_max;      // LIMITE MÁXIMO DO SERVO HORIZONTAL
  
} -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\

*/ 
    
/****************************************************************************************************/

///////////// ATIVO CRONOLOGICO



if(tempo - tempo_anterior_cronologico >= GRAU_MINUTO){

  tempo_anterior_cronologico = tempo;

  servo_v++;

}

if(servo_v >= v_limite_max) servo_v = v_limite_max;

vertical.write(servo_v);

/**************************************************************************/

  botao = digitalRead(PIN_BOTAO);                  // LEITURA DO BOTÃO

  if((botao == HIGH)&&(estado_anterior == LOW)){   // FUNÇÃO QUE EVITA MULTIPLAS LEITURAS EM UM ÚNICO CLIQUE DURANTE VÁRIOS CICLOS DO LOOP 
  contagem_total++;
  }
  estado_anterior = botao;

/**************************************************************************/
  
  if (tempo - tempo_anterior_monitor >= VELOCIDADE_MONITOR) {      // ATUALIZA O CONSOLE DE ACORDO COM A TAXA DE AUTALIZAÇÃO /// PADRÃO: 60.000
   
    tempo_anterior_monitor = tempo;                       

    rpm = contagem_total - ultima_contagem;       // FUNÇÃO RPM    /// ATENÇÃO! RPM = ROTAÇÕES POR TAXA DE ATUALIZAÇÃO DO MONITOR SERIAL
    ultima_contagem = contagem_total;

    Serial.print("MINUTO: "); Serial.print(minutos); Serial.print("   RPM: "); Serial.print(rpm); Serial.print("    TOTAL: "); Serial.print(contagem_total);
    Serial.print("\n");

    minutos++;                                    // MOSTRA MINUTO QUE AMOSTRA FOI COLETADA
  }
  
/***************************************************************************/
  
/*  if( (se+sd+ie+id) / 4 <= ESTRELINHA){           // GATILHO POR DO SOL -----------------------------------------------------------------------------------  ////!\\\\
    horizontal.write(90); 
    vertical.write(90);
  }
                    -----------------------------------------------------------------------------------------------------------------------------------------  ////!\\\\
 */
}
