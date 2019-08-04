
// Estrelinha
// Data: 04/08/2019 
// Versão: final

#include <Servo.h>  // BIBLIOTECA NECESSÁRIA

Servo horizontal;                      
Servo vertical;   

                                /// CONFIGURAÇÕES

                             
#define MEDIADOR 3.42

#define H_LIMITE_MIN 40                       // 45°                          
#define H_LIMITE_MAX 95                      // 135°

#define V_LIMITE_MIN 40                       // 33°  / 8:40 am     |  30°
#define V_LIMITE_MAX 130                      // 153° / 16:40 pm    |  150°
                                

#define VELOCIDADE_TESTE 20                   // VELOCIDADE AUTO TESTE
#define TOLERANCIA 35                         // SENSIBILIDADE DO SISTEMA 
#define VELOCIDADE_SERVO 25                   // VELOCIDADE DOS ATUADORES
#define VELOCIDADE_MONITOR 1000              // TAXA DE ATUALIZAÇÃO DO MONITOR /// 60.000 == 1 MINUTO (NÃO MEXER!) 
#define VELOCIDADE_DEMO 25

#define ESTRELINHA 6                          // POSIÇÃO ALVORADA
#define GRAU_MINUTO 2000                    // 240.000 1°/4 MINUTOS
#define V_NORMAL 85 
#define H_NORMAL 65

                                  /// PINAGEM

// SENSORES

#define VOLT A4

#define LDR_1 A0                      // SUPERIOR DIREITO
#define LDR_2 A1                      // SUPERIOR ESQUERDO
#define LDR_3 A2                      // INFERIOR ESQUERDO
#define LDR_4 A3                      // INFERIOR DIREITO

// SERVOS

#define PIN_SERVO_V 11                // SERVO VERTICAL
#define PIN_SERVO_H 10                // SERVO HORIZONTAL 

// BOTÃO

#define PIN_BOTAO 2                   // BOTÃO
#define PIN_SISTEMA 6


                                 /// VARIÁVEIS

// SERVOS

byte servo_h =  65;                     // SERVO HORIZONTAL      //-4                                                            
byte servo_v =  83;                     // SERVO VERTICAL        //-7                                                                  

// CONTADORES

unsigned long contagem_total = 0;     // CONTA QUANTAS ROTAÇÕES O MOTOR FEZ DURANTE ANÁLISE
unsigned long ultima_contagem = 0;    // NECESSÁRIO PARA OBTER RPM
int rpm = 0;                          // ROTAÇÕES POR MINUTO

// TEMPO

unsigned long tempo;                            // CONTA O TEMPO EM MILISEGUNDOS APÓS LIGAR ARDUINO
unsigned long tempo_anterior_cronologico = 0;
unsigned long tempo_anterior_servo = 0;         // UTILIZADO NOS ATUADORES
unsigned long tempo_anterior_monitor = 0;       // UTILIZADO NO MONITOR
unsigned long tempo_anterior_demo_1 = 0;                // UM MINUTO, NÃO MEXER!
unsigned long tempo_anterior_demo_2 = 0;

int minutos = 0;                                // USADO NO MONITOR SERIAL PARA DEFINIR O TEMPO EM QUE AMOSTRA FOI COLETADA


// BOTÃO

bool  estado_anterior;
bool  botao;
bool  modo_anterior;
bool  modo;

byte sistema = 1;

bool estado = false;
bool ciclo = false;

/**********************************************************************************************************/

void setup() {
  
  /// COMUNICAÇÃO SERIAL
  Serial.begin(9600);                  

  /// BOTÃO
  pinMode(PIN_BOTAO, INPUT);
  estado_anterior = digitalRead(PIN_BOTAO);
  
  pinMode(PIN_SISTEMA, INPUT_PULLUP);
  modo_anterior = digitalRead(PIN_SISTEMA);


  /// SERVO ANCORADO
  horizontal.attach(PIN_SERVO_H);      
  vertical.attach(PIN_SERVO_V);        

  horizontal.write(H_NORMAL);
  delay(500);

  /// AUTO TESTE MECÃNICO
  
  for(servo_v = V_NORMAL; servo_v >= V_LIMITE_MIN; servo_v--){
    vertical.write(servo_v);
    delay(VELOCIDADE_TESTE);
  }
  
  for(servo_v = V_LIMITE_MIN; servo_v <= V_LIMITE_MAX; servo_v++){
    vertical.write(servo_v);
    delay(VELOCIDADE_TESTE);
  }
  
  for(servo_v = V_LIMITE_MAX; servo_v >= V_NORMAL; servo_v--){
     vertical.write(servo_v);
     delay(VELOCIDADE_TESTE);
  }
  
  delay(1000);
  
  for(servo_h = H_NORMAL; servo_h >= H_LIMITE_MIN; servo_h--){
    horizontal.write(servo_h);
    delay(VELOCIDADE_TESTE);
  }
  
  for(servo_h = H_LIMITE_MIN; servo_h <= H_LIMITE_MAX; servo_h++){
    horizontal.write(servo_h);
    delay(VELOCIDADE_TESTE);
  }
  
  for(servo_h = H_LIMITE_MAX; servo_h >= H_NORMAL; servo_h--){
    horizontal.write(servo_h);
    delay(VELOCIDADE_TESTE);
  }

  delay(500);
  
  /// FIM DO TESTE
  
  vertical.write(V_NORMAL);
  horizontal.write(H_NORMAL);                              
}

void loop(){

  // CONTA TEMPO
  tempo = millis();
  
  int tensao = analogRead(VOLT); 
  float volt = tensao*5.000/1023.000;
    
  // LEITURAS
  int sd = analogRead(LDR_1);         
  int se = analogRead(LDR_2);          
  int ie = analogRead(LDR_3);          
  int id = analogRead(LDR_4);
  int media = (sd+se+ie+id)/4;          

  /// BOTÃO
  botao = digitalRead(PIN_BOTAO);                                   // LEITURA DO BOTÃO
  if((botao == HIGH)&&(estado_anterior == LOW)){
    if(volt >= MEDIADOR) contagem_total++;   // FUNÇÃO QUE EVITA MULTIPLAS LEITURAS EM UM ÚNICO CLIQUE DURANTE VÁRIOS CICLOS DO LOOP 
  }
  estado_anterior = botao;

  /// SELEÇÃO DO SISTEMA 
  modo = digitalRead(PIN_SISTEMA);                          // LEITURA DO BOTÃO
  if((modo == HIGH)&&(modo_anterior == LOW)) sistema++;   // FUNÇÃO QUE EVITA MULTIPLAS LEITURAS EM UM ÚNICO CLIQUE DURANTE VÁRIOS CICLOS DO LOOP 
  modo_anterior = modo;
  if(sistema >= 5) sistema = 1;

  /// SERIAL MONITOR
  if(tempo - tempo_anterior_monitor >= VELOCIDADE_MONITOR){         // ATUALIZA O CONSOLE DE ACORDO COM A TAXA DE AUTALIZAÇÃO DESEJADA
    tempo_anterior_monitor = tempo;                       
    rpm = contagem_total - ultima_contagem;       // FUNÇÃO RPM     /!\ ATENÇÃO! RPM = ROTAÇÕES POR TAXA DE ATUALIZAÇÃO DO MONITOR SERIAL
    ultima_contagem = contagem_total;
    Serial.print("MODO: "); Serial.print(sistema);Serial.print("    MINUTO: "); Serial.print(minutos); Serial.print("   RPM: "); Serial.print(rpm); Serial.print("    TOTAL: "); Serial.print(contagem_total); Serial.print("     LUX: "); Serial.print(media); Serial.print("     TENSÃO: "); Serial.print(volt);
    Serial.print("\n");
    minutos++;                                                      // MOSTRA MINUTO QUE AMOSTRA FOI COLETADA
  }

  /// SISTEMAS 
   
    /// ATIVO FOTOSSENSIVEL
    if(sistema == 1){

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
        if(servo_v <= V_LIMITE_MIN) servo_v = V_LIMITE_MIN;        
        if(servo_v >= V_LIMITE_MAX) servo_v = V_LIMITE_MAX;        
  
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
        if(servo_h <= H_LIMITE_MIN) servo_h = H_LIMITE_MIN;      
        if(servo_h >= H_LIMITE_MAX) servo_h = H_LIMITE_MAX;     
      
      } 

    } 
    
    /// ATIVO CRONOLOGICO
    if(sistema == 2){

      horizontal.write(H_NORMAL);
      if(estado == false){ 
        vertical.write(V_LIMITE_MIN);
        estado = true;
      }
          
      if(tempo - tempo_anterior_cronologico >= GRAU_MINUTO){
        tempo_anterior_cronologico = tempo;
        servo_v++;
      }
      
      // LIMITE DO SERVO VERTICAL
      if(servo_v >= V_LIMITE_MAX) servo_v = V_LIMITE_MAX;
      vertical.write(servo_v);
  
    }

    if(sistema == 3){
      vertical.write(V_NORMAL); 
      horizontal.write(H_NORMAL); 
    }

    /// DEMONSTRAÇÃO
    if(sistema == 4){
      
      if(ciclo == false){
        if(tempo - tempo_anterior_demo_1 >= VELOCIDADE_DEMO){ 
          tempo_anterior_demo_1 = tempo;
          if(servo_v > V_LIMITE_MIN){
            servo_v--;
          }
        }
      }
       
      if(servo_v == V_LIMITE_MIN) ciclo = true;
 
      if(ciclo == true){
        if(tempo - tempo_anterior_demo_2 >= VELOCIDADE_DEMO){ 
          tempo_anterior_demo_2 = tempo;
          if(servo_v < V_LIMITE_MAX){
            servo_v++;
          }
        }
      }

      if(servo_v == V_LIMITE_MAX) ciclo = false;
      
      vertical.write(servo_v);
   
    }
      
}
