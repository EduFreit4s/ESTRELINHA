// Estrelinha
// Data: 16/07/2019 
// Versão: final

#include <Servo.h>  // BIBLIOTECA NECESSÁRIA

Servo horizontal;                      
Servo vertical;   

                                /// CONFIGURAÇÕES

#define SISTEMA 0                             // 0 = FIXO / 1 = FOTOSSENSIVEL / 2 = CRONOLOGICO

#define h_limite_min 40                       // 45°                          
#define h_limite_max 95                      // 135°

#define v_limite_min 40                       // 33°  / 8:40 am     |  30°
#define v_limite_max 130                      // 153° / 16:40 pm    |  150°
                                

#define VELOCIDADE_TESTE 20                   // VELOCIDADE AUTO TESTE
#define TOLERANCIA 35                         // SENSIBILIDADE DO SISTEMA 
#define VELOCIDADE_SERVO 25                   // VELOCIDADE DOS ATUADORES
#define VELOCIDADE_MONITOR 60000              // TAXA DE ATUALIZAÇÃO DO MONITOR /// 60.000 == 1 MINUTO (NÃO MEXER!) 

#define ESTRELINHA 6                          // POSIÇÃO ALVORADA
#define GRAU_MINUTO 2000                    // 240.000 1°/4 MINUTOS


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


                                 /// VARIÁVEIS

// SERVOS

int servo_h =  65;                     // SERVO HORIZONTAL      //-4                                                            
int servo_v =  83;                     // SERVO VERTICAL        //-7                                                                  

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
  
  /// COMUNICAÇÃO SERIAL
  Serial.begin(9600);                  

  /// BOTÃO
  pinMode(PIN_BOTAO, INPUT);
  estado_anterior = digitalRead(PIN_BOTAO);

  /// SERVO ANCORADO
  horizontal.attach(PIN_SERVO_H);      
  vertical.attach(PIN_SERVO_V);        

  if(SISTEMA == 2 || SISTEMA == 1) horizontal.write(servo_h);                                                                                                 
  delay(500);

  /// AUTO TESTE MECÃNICO
  if(SISTEMA == 1){
    for(servo_v = 90; servo_v >= v_limite_min; servo_v--){
      vertical.write(servo_v);
      delay(VELOCIDADE_TESTE);
    }
    for(servo_v = v_limite_min; servo_v <= v_limite_max; servo_v++){
      vertical.write(servo_v);
      delay(VELOCIDADE_TESTE);
    }
    for(servo_v = v_limite_max; servo_v >= 90; servo_v--){
      vertical.write(servo_v);
      delay(VELOCIDADE_TESTE);
    }
     delay(1000);
    for(servo_h = servo_h; servo_h >= 20; servo_h--){
      horizontal.write(servo_h);
      delay(VELOCIDADE_TESTE);
    }
    for(servo_h = 20; servo_h <= 110; servo_h++){
      horizontal.write(servo_h);
      delay(VELOCIDADE_TESTE);
    }
    for(servo_h = 110; servo_h >= 65; servo_h--){
      horizontal.write(servo_h);
      delay(VELOCIDADE_TESTE);
    }
    delay(500);
  }
  /// FIM DO TESTE
  
  vertical.write(servo_v);
  horizontal.write(servo_h);                              

  if(SISTEMA == 2) servo_v = v_limite_min;
}

void loop(){

  // CONTA TEMPO
  tempo = millis();                    


  /// ATIVO FOTOSSENSIVEL
  if(SISTEMA == 1){

    // LEITURAS
    int sd = analogRead(LDR_1);         
    int se = analogRead(LDR_2);          
    int ie = analogRead(LDR_3);          
    int id = analogRead(LDR_4);          

    // MEDIAS
    int m_superior = (sd + se) / 2;      
    int m_inferior = (id + ie) / 2;      
    int m_direita  = (sd + id) / 2;      
    int m_esquerda = (se + ie) / 2;      

    // DIFERENÇAS
    int delta_v  = m_superior - m_inferior;             
    int delta_h  = m_esquerda - m_direita;

    /// POSIÇÃO PADRÃO NA AUSENCIA DE LUZ
    if((se+sd+ie+id) / 4 <= ESTRELINHA){           
      horizontal.write(90); 
      vertical.write(90);
    }

  /**********************************************************************************************************/
    // VELOCIDADE DO SISTEMA FOTOSSENSIVEL                                                                                                                               
    if(tempo - tempo_anterior_servo >= VELOCIDADE_SERVO){
      tempo_anterior_servo = tempo;
      
      // ATUADOR VERTICAL
      if( delta_v < (TOLERANCIA * (-1)) || delta_v > TOLERANCIA){        
        if(m_superior > m_inferior){
          servo_v--;
        }else if(m_superior < m_inferior){
          servo_v++;
        }
        vertical.write(servo_v);                                    
      }
      // LIMITES DOS SERVO VERTICAL
      if(servo_v <= v_limite_min) servo_v = v_limite_min;        
      if(servo_v >= v_limite_max) servo_v = v_limite_max;        

     //////////////////////////////////////////////////////////////////////////////////////////////////
  
      // ATUADOR HORIZONTAL
      if( delta_h < (TOLERANCIA * (-1)) || delta_h > TOLERANCIA){       
        if (m_esquerda > m_direita){
          servo_h++;    
        }else if (m_esquerda < m_direita){
          servo_h--;
        }
        horizontal.write(servo_h);                                
      }
      // LIMITES DOS SERVO HORIZONTAL
      if(servo_h <= h_limite_min) servo_h = h_limite_min;      
      if(servo_h >= h_limite_max) servo_h = h_limite_max;     
    } 
  }

/***************************************************************************************************/
   
  /// ATIVO CRONOLOGICO
  if(SISTEMA == 2){
    if(tempo - tempo_anterior_cronologico >= GRAU_MINUTO){
      tempo_anterior_cronologico = tempo;
      servo_v++;
    }
    // LIMITE DO SERVO VERTICAL
    if(servo_v >= v_limite_max) servo_v = v_limite_max;
    vertical.write(servo_v);
  }


  /// BOTÃO
  botao = digitalRead(PIN_BOTAO);                                   // LEITURA DO BOTÃO
  if((botao == HIGH)&&(estado_anterior == LOW)) contagem_total++;   // FUNÇÃO QUE EVITA MULTIPLAS LEITURAS EM UM ÚNICO CLIQUE DURANTE VÁRIOS CICLOS DO LOOP 
  estado_anterior = botao;


  /// SERIAL MONITOR
  if(tempo - tempo_anterior_monitor >= VELOCIDADE_MONITOR){         // ATUALIZA O CONSOLE DE ACORDO COM A TAXA DE AUTALIZAÇÃO DESEJADA
    tempo_anterior_monitor = tempo;                       
    rpm = contagem_total - ultima_contagem;       // FUNÇÃO RPM     /!\ ATENÇÃO! RPM = ROTAÇÕES POR TAXA DE ATUALIZAÇÃO DO MONITOR SERIAL
    ultima_contagem = contagem_total;
    Serial.print("MINUTO: "); Serial.print(minutos); Serial.print("   RPM: "); Serial.print(rpm); Serial.print("    TOTAL: "); Serial.print(contagem_total);
    Serial.print("\n");
    minutos++;                                                      // MOSTRA MINUTO QUE AMOSTRA FOI COLETADA
  }
  
}
