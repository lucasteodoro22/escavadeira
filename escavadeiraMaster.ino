#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>
#include <Servo.h>
#include <Arduino.h>
#include <string.h>
#include <Wire.h>

  //Configurações RF
  RF24 radio(7, 8); // CE, CSN
  const byte address[6] = "00001";

  //Configurações para DEBUG
  int debugConcha = 0; // 0 Off / 1 ON
  int debugBraco = 0; // 0 Off / 1 ON
  int debugConj = 0; // 0 Off / 1 ON
  int debugRotMaquina = 0; // 0 Off / 1 ON
  int debugMovimento = 0; // 0 Off / 1 ON

  //Configurações de demonstração
  const int pinoDemonstracao = 13; // Pino digital botão para iniciar a demonstração

  //Configurações da Concha
  Servo servoConcha;  // Chama a biblioteca para controle do servo da Concha
  int valorPotConcha; //Potenciometro da concha
  int pinoPotConcha = A0; // Porta Potenciometro da concha
  float posicaoConcha = 100; // Posição da concha ao ligar
  float posicaoConchaAntiga = posicaoConcha;
  float acrescimoConcha = 0; // Acrescimo da concha exponencial ao valor do potenciometro
  int posicaoMaximaConcha = 157; // Valor em graus para posicao maxima da concha
  int posicaoMinimaConcha = 35; // Valor em graus para posicao maxima da concha


  //Configurações do braco
  Servo servoBraco;  // Chama a biblioteca para controle do servo da Braco
  int valorPotBraco; //Potenciometro da Braco
  int pinoPotBraco = A1; // Porta Potenciometro do braco
  float posicaoBraco = 90; // Posição da Braco ao ligar
  float posicaoBracoAntiga = posicaoBraco;
  float acrescimoBraco = 0; // Acrescimo da Braco exponencial ao valor do potenciometro
  int posicaoMaximaBraco = 180; // Valor em graus para posicao maxima do braco
  int posicaoMinimaBraco = 45; // Valor em graus para posicao maxima do braco


  //Configurações Conjunto articulado
  Servo servoConj;  // Chama a biblioteca para controle do servo da Conjunto articulado
  int valorPotConj; //Potenciometro da Conjunto articulado
  int pinoPotConj = A2; // Porta Potenciometro do Conjunto articulado
  float posicaoConj = 90; // Posição da Conjunto articulado ao ligar
  float posicaoConjAntiga = posicaoConj;
  float acrescimoConj = 0; // Acrescimo da Conjunto articulado exponencial ao valor do potenciometro
  int posicaoMaximaConj = 150; // Valor em graus para posicao maxima do Conjunto articulado
  int posicaoMinimaConj = 13; // Valor em graus para posicao maxima do Conjunto articulado


  //Configurações Rotacao da maquina
  byte posicaoServoRotMaquina = 93; //Valor base
  Servo servoRotMaquina;  // Chama a biblioteca para controle do servo da Rotacao da maquina
  int valorPotRotMaquina; //Potenciometro da Rotacao da maquina
  int pinoPotRotMaquina = A3; // Porta Potenciometro do Rotacao da maquina
  float posicaoRotMaquina = 90; // Posição da Rotacao da maquina ao ligar
  float posicaoRotMaquinaAntiga = posicaoRotMaquina;
  float acrescimoRotMaquina = 0; // Acrescimo da Rotacao da maquina exponencial ao valor do potenciometro
  int posicaoMaximaRotMaquina = 180; // Valor em graus para posicao maxima do Rotacao da maquina
  int posicaoMinimaRotMaquina = 0; // Valor em graus para posicao maxima do Rotacao da maquina


  //Configurações Esteira Movimento
  Servo servoEstDir;  // Chama a biblioteca para controle do servo da Esteira Direita
  Servo servoEstEsq;  // Chama a biblioteca para controle do servo da Esteira Esquerda
  float posicaoEstDir;
  float posicaoEstEsq;
  int valorPotEstDirEsq; //Potenciometro da Esteira Direita/Esquerda
  float valorPotFrenteRe; //Potenciometro da Esteira Frente/Re
  int pinoPotEstDirEsq = A4; // Porta Potenciometro da Esteira Direita/Esquerda
  int pinoPotFrenteRe = A5; // Porta Potenciometro da Esteira Frente/Re




void setup() {
  Serial.begin(9600);
  pinMode(pinoDemonstracao, INPUT); // Define o pino do botão para demonstração
  servoConcha.attach(10);  //Pino PWM que manda sinal para o servo da Concha
  servoBraco.attach(9);  //Pino PWM que manda sinal para o servo do Braco
  servoConj.attach(6);  //Pino PWM que manda sinal para o servo do Conjunto articulado
  //servoRotMaquina.attach(11);  //Pino PWM que manda sinal para o servo da Rotação da maquina || Vai ser usado um arduino escravo pois esse pino 11 é usado pelo RF24
  servoEstDir.attach(5);  //Pino PWM que manda sinal para o servo da Esteira Direita
  servoEstEsq.attach(3);  //Pino PWM que manda sinal para o servo da Esteira Esquerda

  radio.begin();
  radio.openReadingPipe(0, address);// Define a leitura de dados do RF
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();//Fica escutando se tem comandos vindo pelo RF

  
  Wire.begin(); // Inicializa a biblioteca Wire (I2C)

}

void loop() {
  
  if (radio.available()) {
    char dadosRecebidosRF[15] = "";
    radio.read(&dadosRecebidosRF, sizeof(dadosRecebidosRF));
    //Serial.println(dadosRecebidosRF);

    char delimiter = '-';
    String dadosFormatadosRF[10];
    formataDadosRF(dadosRecebidosRF, delimiter, dadosFormatadosRF, 10);
    if(dadosFormatadosRF[1] == "demonstracao"){ //Verifica se foi iniciado o modo demonstração
      enviaI2CPararMovimentoMaquina();
      demonstracao();
    }else if(dadosFormatadosRF[1] == "1"){ // Concha
      int valor = dadosFormatadosRF[0].toInt();
      controleConcha(valor);
    }else if(dadosFormatadosRF[1] == "3"){ // Braco
      int valor = dadosFormatadosRF[0].toInt();
      controleBraco(valor);
    }else if(dadosFormatadosRF[1] == "2"){ // Conjunto
      int valor = dadosFormatadosRF[0].toInt();
      controleConj(valor);
    }else if(dadosFormatadosRF[1] == "4"){ // Rotacao
      int valor = dadosFormatadosRF[0].toInt();
      controleRotMaquina(valor);
    }else if(dadosFormatadosRF[1] == "5"){ // Movimento
      String valor = dadosFormatadosRF[0];
      int posicaoDirEsq, posicaoFrenteRe;
      int posicaoDelimitador = valor.indexOf('=');
      if (posicaoDelimitador != -1) {
        // Divide a string com base no caractere '='
        String primeiroParte = valor.substring(0, posicaoDelimitador);
        String segundoParte = valor.substring(posicaoDelimitador + 1);
        
        // Converte as partes para inteiros
        posicaoDirEsq = primeiroParte.toInt();
        posicaoFrenteRe = segundoParte.toInt();
        
        controleMovimento(posicaoFrenteRe,posicaoDirEsq);
      }
    }
  }
}

void demonstracao(){
  delay(2000);
  //Abre tudo
  int servoConjPosicao = servoConj.read();
  for (int i = 1; i <= 180; i++) {
    servoConjPosicao++;
    if(servoConjPosicao > posicaoMaximaConj){
      break;
    }
    servoConj.write(servoConjPosicao); //Aplica a posição da concha em graus
    delay(15);
  }

  int servoBracoPosicao = servoBraco.read();
  for (int i = 1; i <= 180; i++) {
    servoBracoPosicao++;
    if(servoBracoPosicao > posicaoMaximaBraco){
      break;
    }
    servoBraco.write(servoBracoPosicao); //Aplica a posição da concha em graus
    delay(15);
  }

  int servoConchaPosicao = servoConcha.read();
  for (int i = 1; i <= 180; i++) {
    servoConchaPosicao++;
    if(servoConchaPosicao > posicaoMaximaConcha){
      break;
    }
    servoConcha.write(servoConchaPosicao); //Aplica a posição da concha em graus
    delay(15);
  }

  //Fecha tudo

  for (int i = 1; i <= 180; i++) {
    servoConchaPosicao--;
    if(servoConchaPosicao < posicaoMinimaConcha){
      break;
    }
    servoConcha.write(servoConchaPosicao); //Aplica a posição da concha em graus
    delay(15);
  }

  for (int i = 1; i <= 180; i++) {
    servoBracoPosicao--;
    if(servoBracoPosicao < posicaoMinimaBraco){
      break;
    }
    servoBraco.write(servoBracoPosicao); //Aplica a posição da concha em graus
    delay(15);
  }

  for (int i = 1; i <= 180; i++) {
    servoConjPosicao--;
    if(servoConjPosicao < 80){
      break;
    }
    servoConj.write(servoConjPosicao); //Aplica a posição da concha em graus
    delay(15);
  }
  Serial.println("Demonstracao Finalizada");
  delay(5000);
  return;
}

void controleConcha(int valorPotConcha){
  //------ Inicio Controle da Concha ---------------
  //valorPotConcha = analogRead(pinoPotConcha);
  if(valorPotConcha > 510){
    if(valorPotConcha > 1000){
      acrescimoConcha = 1;
    }else 
    if(valorPotConcha > 900){
       acrescimoConcha = 0.7;
    }else if(valorPotConcha > 700){
      acrescimoConcha = 0.5;
    }else if(valorPotConcha > 600){
      acrescimoConcha = 0.3;
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
      if(debugConcha == 1){
        Serial.print("Posição da Concha: ");
        Serial.print(posicaoConcha);
        Serial.println(" Graus");
      }
    }
  }

  if(valorPotConcha < 500){
    if(valorPotConcha < 25){
      acrescimoConcha = 1;
    }else if(valorPotConcha < 100){
      acrescimoConcha = 0.7;
    }else if(valorPotConcha < 300){
      acrescimoConcha = 0.5;
    }else if(valorPotConcha < 400){
      acrescimoConcha = 0.3;
    }else{
      acrescimoConcha = 0.1;
    }

    posicaoConcha = posicaoConcha - acrescimoConcha;
    if(posicaoConcha < posicaoMinimaConcha){
      posicaoConcha = posicaoMinimaConcha;
    }
    if(posicaoConchaAntiga != posicaoConcha){
      servoConcha.write(posicaoConcha); //Aplica a posição da concha em graus
      if(debugConcha == 1){
        Serial.print("Posição da Concha: ");
        Serial.print(posicaoConcha);
        Serial.println(" Graus");
      }
    }
  }
  return; 
  //------ FIM Controle da Concha ---------------
}

void controleBraco(int valorPotBraco){
  //------ Inicio Controle da Braco ---------------
  //valorPotBraco = analogRead(pinoPotBraco);
  if(valorPotBraco > 510){
    if(valorPotBraco > 1000){
      acrescimoBraco = 1;
    }else 
    if(valorPotBraco > 900){
       acrescimoBraco = 0.7;
    }else if(valorPotBraco > 700){
      acrescimoBraco = 0.5;
    }else if(valorPotBraco > 600){
      acrescimoBraco = 0.3;
    }else{
      acrescimoBraco = 0.1;
    }

    posicaoBraco = posicaoBraco + acrescimoBraco;
    if(posicaoBraco > 180){
      posicaoBraco = 180;
    }
    if(posicaoBracoAntiga != posicaoBraco){
      servoBraco.write(posicaoBraco); //Aplica a posição da Braco em graus
      if(debugBraco == 1){
        Serial.print("Posição da Braço: ");
        Serial.print(posicaoBraco);
        Serial.println(" Graus");
      }
    }
  }

  if(valorPotBraco < 500){
    if(valorPotBraco < 25){
      acrescimoBraco = 1;
    }else if(valorPotBraco < 100){
      acrescimoBraco = 0.7;
    }else if(valorPotBraco < 300){
      acrescimoBraco = 0.5;
    }else if(valorPotBraco < 400){
      acrescimoBraco = 0.3;
    }else{
      acrescimoBraco = 0.1;
    }

    posicaoBraco = posicaoBraco - acrescimoBraco;
    if(posicaoBraco < posicaoMinimaBraco){
      posicaoBraco = posicaoMinimaBraco;
    }
    if(posicaoBracoAntiga != posicaoBraco){
      servoBraco.write(posicaoBraco); //Aplica a posição da Braco em graus
      if(debugBraco == 1){
        Serial.print("Posição da Braço: ");
        Serial.print(posicaoBraco);
        Serial.println(" Graus");
      }
    }
  }
  return;
  //------ Fim Controle da Braco ---------------
}

void controleConj(int valorPotConj){
  //------ Inicio Controle da Conj ---------------
  //valorPotConj = analogRead(pinoPotConj);
  if(valorPotConj > 520){
    if(valorPotConj > 1000){
      acrescimoConj = 1;
    }else if(valorPotConj > 900){
       acrescimoConj = 0.7;
    }else if(valorPotConj > 700){
      acrescimoConj = 0.5;
    }else if(valorPotConj > 600){
      acrescimoConj = 0.3;
    }else{
      acrescimoConj = 0.1;
    }

    posicaoConj = posicaoConj - acrescimoConj;
    if(posicaoConj < posicaoMinimaConj){
      posicaoConj = posicaoMinimaConj;
    }
    if(posicaoConjAntiga != posicaoConj){
      servoConj.write(posicaoConj); //Aplica a posição da Conj em graus
      if(debugConj == 1){
        Serial.print("Posicao do Conjuto Articulado Maior: ");
        Serial.print(posicaoConj);
        Serial.println(" Graus");
      }
    }
  }

  if(valorPotConj < 500){
    if(valorPotConj < 25){
      acrescimoConj = 1;
    }else if(valorPotConj < 100){
      acrescimoConj = 0.7;
    }else if(valorPotConj < 300){
      acrescimoConj = 0.5;
    }else if(valorPotConj < 400){
      acrescimoConj = 0.3;
    }else{
      acrescimoConj = 0.1;
    }

    posicaoConj = posicaoConj + acrescimoConj;
    if(posicaoConj > posicaoMaximaConj){
      posicaoConj = posicaoMaximaConj;
    }
    if(posicaoConjAntiga != posicaoConj){
      servoConj.write(posicaoConj); //Aplica a posição da Conj em graus
      if(debugConj == 1){
        Serial.print("Posição do Conjuto Articulado Menor: ");
        Serial.print(posicaoConj);
        Serial.println(" Graus");
      }
    }
  }
  return;
  //------ Fim Controle da Conj ---------------
}

void controleRotMaquina(int valorPotRotMaquina){
   //------ Inicio Controle da RotMaquina ---------------
   
      
  //valorPotRotMaquina = analogRead(pinoPotRotMaquina);
  
   int velocidadeRotacaoMaquina = map(valorPotRotMaquina, 1023, 0, 89, 96);
   enviaI2CMovimentoRotMaquina(velocidadeRotacaoMaquina);// Envia a velocidade e forma de posição para o servo
   if(debugRotMaquina == 1){
        Serial.print("Velocidade Rotacao Maquina: ");
        Serial.print(velocidadeRotacaoMaquina);
        Serial.println(" Graus");
    }
   delay(30); //Pausa
   enviaI2CPararMovimentoMaquina(); //Para a rotacao apos o delay
  return;
  //------ Fim Controle da RotMaquina ---------------
}

void controleMovimento(int valorPotFrenteRe, int valorPotEstDirEsq){
   // Mapeia os valores lidos para o intervalo de controle dos servos (0-180)
    int velocidadeEsquerda = map(valorPotEstDirEsq, 0, 1023, 0, 180);
    int velocidadeDireita = map(valorPotEstDirEsq, 0, 1023, 0, 180);

    int direcao = map(valorPotFrenteRe, 0, 1023, 90, -90);

    int diferencaVelocidade = abs(direcao); // Calcula a diferença de velocidade entre os motores para virar

    // Se a direção for para a direita
    if (direcao > 0) {
      velocidadeDireita -= diferencaVelocidade;
    }
    // Se a direção for para a esquerda
    else if (direcao < 0) {
      velocidadeEsquerda -= diferencaVelocidade;
    }

    // Controla a velocidade dos servos de acordo com a posição do joystick
    servoEstEsq.write(180 - velocidadeEsquerda);
    servoEstDir.write(velocidadeDireita); // Inverte a velocidade

  delay(10);
  pararMovimentoEsteira();
}

void formataDadosRF(const String &inputString, char delimiter, String *outputArray, int maxParts) {
    int partIndex = 0;
    int startIndex = 0;
    int endIndex;

    while (partIndex < maxParts - 1 && (endIndex = inputString.indexOf(delimiter, startIndex)) >= 0) {
        outputArray[partIndex++] = inputString.substring(startIndex, endIndex);
        startIndex = endIndex + 1;
    }

    outputArray[partIndex] = inputString.substring(startIndex);
}

void enviaI2CMovimentoRotMaquina(byte velocidade){
  posicaoServoRotMaquina = velocidade;
  Wire.beginTransmission(0x08); // Endereço do Arduino escravo por I2C
  Wire.write(posicaoServoRotMaquina); //Aplica a posição da RotMaquina em graus
  Wire.endTransmission();
}

void enviaI2CPararMovimentoMaquina(){
    enviaI2CMovimentoRotMaquina(92);//Parar de Rotacionar maquina
}

void pararMovimentoEsteira(){
  servoEstDir.write(90); //Para Movimentacao maquina
  servoEstEsq.write(90); //Para Movimentacao maquina
}
