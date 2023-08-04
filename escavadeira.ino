#include <Servo.h>

  //Configurações da Concha
  Servo servoConcha;  // Chama a biblioteca para controle do servo da Concha
  int valorPotConcha; //Potenciometro da concha
  int pinoPotConcha = A0; // Porta Potenciometro da concha
  float posicaoConcha = 90; // Posição da concha ao ligar
  float posicaoConchaAntiga = posicaoConcha;
  float acrescimoConcha = 0; // Acrescimo da concha exponencial ao valor do potenciometro
  int posicaoMaximaConcha = 138; // Valor em graus para posicao maxima da concha
  int posicaoMinimaConcha = 30; // Valor em graus para posicao maxima da concha


  //Configurações do braco
  Servo servoBraco;  // Chama a biblioteca para controle do servo da Braco
  int valorPotBraco; //Potenciometro da Braco
  int pinoPotBraco = A1; // Porta Potenciometro do braco
  float posicaoBraco = 90; // Posição da Braco ao ligar
  float posicaoBracoAntiga = posicaoBraco;
  float acrescimoBraco = 0; // Acrescimo da Braco exponencial ao valor do potenciometro
  int posicaoMaximaBraco = 138; // Valor em graus para posicao maxima do braco
  int posicaoMinimaBraco = 30; // Valor em graus para posicao maxima do braco


  //Configurações Conjunto articulado
  Servo servoConj;  // Chama a biblioteca para controle do servo da Conjunto articulado
  int valorPotConj; //Potenciometro da Conjunto articulado
  int pinoPotConj = A2; // Porta Potenciometro do Conjunto articulado
  float posicaoConj = 90; // Posição da Conjunto articulado ao ligar
  float posicaoConjAntiga = posicaoConj;
  float acrescimoConj = 0; // Acrescimo da Conjunto articulado exponencial ao valor do potenciometro
  int posicaoMaximaConj = 138; // Valor em graus para posicao maxima do Conjunto articulado
  int posicaoMinimaConj = 30; // Valor em graus para posicao maxima do Conjunto articulado


  //Configurações Rotacao da maquina
  Servo servoRotMaquina;  // Chama a biblioteca para controle do servo da Rotacao da maquina
  int valorPotRotMaquina; //Potenciometro da Rotacao da maquina
  int pinoPotRotMaquina = A3; // Porta Potenciometro do Rotacao da maquina
  float posicaoRotMaquina = 90; // Posição da Rotacao da maquina ao ligar
  float posicaoRotMaquinaAntiga = posicaoRotMaquina;
  float acrescimoRotMaquina = 0; // Acrescimo da Rotacao da maquina exponencial ao valor do potenciometro
  int posicaoMaximaRotMaquina = 138; // Valor em graus para posicao maxima do Rotacao da maquina
  int posicaoMinimaRotMaquina = 30; // Valor em graus para posicao maxima do Rotacao da maquina


  //Configurações Esteira Direita/Esquerda
  Servo servoEstDir;  // Chama a biblioteca para controle do servo da Esteira Direita
  Servo servoEstEsq;  // Chama a biblioteca para controle do servo da Esteira Esquerda
  int valorPotEstDirEsq; //Potenciometro da Esteira Direita/Esquerda
  int pinoPotEstDirEsq = A4; // Porta Potenciometro da Esteira Direita/Esquerda

  //Configurações Esteira Frente/Re
  int valorPotFrenteRe; //Potenciometro da Esteira Frente/Re
  int pinoPotFrenteRe = A5; // Porta Potenciometro da Esteira Frente/Re




void setup() {
  Serial.begin(9600);
  servoConcha.attach(11);  //Pino PWM que manda sinal para o servo da Concha
  servoBraco.attach(10);  //Pino PWM que manda sinal para o servo do Braco
  servoConj.attach(9);  //Pino PWM que manda sinal para o servo do Conjunto articulado
  servoRotMaquina.attach(6);  //Pino PWM que manda sinal para o servo da Rotação da maquina
  servoEstDir.attach(5);  //Pino PWM que manda sinal para o servo da Esteira Direita
  servoEstEsq.attach(3);  //Pino PWM que manda sinal para o servo da Esteira Esquerda
}

void loop() {
  //------ Inicio Controle da Concha ---------------
  valorPotConcha = analogRead(pinoPotConcha);
  if(valorPotConcha > 510){
    if(valorPotConcha > 1000){
      acrescimoConcha = 5;
    }else 
    if(valorPotConcha > 900){
       acrescimoConcha = 1.5;
    }else if(valorPotConcha > 700){
      acrescimoConcha = 1;
    }else if(valorPotConcha > 600){
      acrescimoConcha = 0.5;

    }else{
      acrescimoConcha = 0.1;
    }
    posicaoConcha = posicaoConcha + acrescimoConcha;
    if(posicaoConcha > posicaoMaximaConcha){
      posicaoConcha = posicaoMaximaConcha;
    }
    if(posicaoConcha < posicaoMinimaConcha){
      posicaoConcha = posicaoMinimaConcha;
    }
    if(posicaoConchaAntiga != posicaoConcha){
      servoConcha.write(posicaoConcha); //Aplica a posição da concha em graus
      delay(10); 
      Serial.print("Posição da Concha: ");
      Serial.print(posicaoConcha);
      Serial.println(" Graus");
    }
  }

  if(valorPotConcha < 500){
    if(valorPotConcha < 25){
      acrescimoConcha = 5;
    }else if(valorPotConcha < 100){
      acrescimoConcha = 1.5;
    }else if(valorPotConcha < 300){
      acrescimoConcha = 1;
    }else if(valorPotConcha < 400){
      acrescimoConcha = 0.5;
    }else{
      acrescimoConcha = 0.1;
    }
    posicaoConcha = posicaoConcha - acrescimoConcha;
    if(posicaoConcha < 0){
      posicaoConcha = 0;
    }
    if(posicaoConchaAntiga != posicaoConcha){
      servoConcha.write(posicaoConcha); //Aplica a posição da concha em graus
      delay(10); 
      Serial.print("Posição da Concha: ");
      Serial.print(posicaoConcha);
      Serial.println(" Graus");
    }
  }
  
  //------ FIM Controle da Concha ---------------

  //------ Inicio Controle da Braco ---------------
  valorPotBraco = analogRead(pinoPotBraco);
  if(valorPotBraco > 510){
    if(valorPotBraco > 1000){
      acrescimoBraco = 5;
    }else 
    if(valorPotBraco > 900){
       acrescimoBraco = 1.5;
    }else if(valorPotBraco > 700){
      acrescimoBraco = 1;
    }else if(valorPotBraco > 600){
      acrescimoBraco = 0.5;
    }else{
      acrescimoBraco = 0.1;
    }
    posicaoBraco = posicaoBraco + acrescimoBraco;
    if(posicaoBraco > 180){
      posicaoBraco = 180;
    }
    if(posicaoBracoAntiga != posicaoBraco){
      servoBraco.write(posicaoBraco); //Aplica a posição da Braco em graus
      delay(10); 
      Serial.print("Posição da Braço: ");
      Serial.print(posicaoBraco);
      Serial.println(" Graus");
    }
  }

  if(valorPotBraco < 500){
    if(valorPotBraco < 25){
      acrescimoBraco = 5;
    }else if(valorPotBraco < 100){
      acrescimoBraco = 1.5;
    }else if(valorPotBraco < 300){
      acrescimoBraco = 1;
    }else if(valorPotBraco < 400){
      acrescimoBraco = 0.5;
    }else{
      acrescimoBraco = 0.1;
    }
    posicaoBraco = posicaoBraco - acrescimoBraco;
    if(posicaoBraco < 0){
      posicaoBraco = 0;
    }
    if(posicaoBracoAntiga != posicaoBraco){
      servoBraco.write(posicaoBraco); //Aplica a posição da Braco em graus
      delay(10); 
      Serial.print("Posição da Braço: ");
      Serial.print(posicaoBraco);
      Serial.println(" Graus");
    }
  }
  //------ Fim Controle da Braco ---------------


  //------ Fim Controle Movimento ---------------
  servoEstDir.write(valorPotEstDirEsq); //Aplica a posição da Braco em graus
  //------ Fim Controle Movimento ---------------
  
}
