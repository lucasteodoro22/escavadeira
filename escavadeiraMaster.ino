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
  int debugConcha = 1; // 0 Off / 1 ON
  int debugBraco = 1; // 0 Off / 1 ON
  int debugConj = 1; // 0 Off / 1 ON
  int debugRotMaquina = 1; // 0 Off / 1 ON
  int debugMovimento = 1; // 0 Off / 1 ON

  //Configurações de demonstração
  const int pinoDemonstracao = 13; // Pino digital botão para iniciar a demonstração

  //Configurações da Concha
  Servo servoConcha;  // Chama a biblioteca para controle do servo da Concha
  int valorPotConcha; //Potenciometro da concha
  int pinoPotConcha = A0; // Porta Potenciometro da concha
  float posicaoConcha = 100; // Posição da concha ao ligar
  float posicaoConchaAntiga = posicaoConcha;
  float acrescimoConcha = 0; // Acrescimo da concha exponencial ao valor do potenciometro
  int posicaoMaximaConcha = 141; // Valor em graus para posicao maxima da concha
  int posicaoMinimaConcha = 45; // Valor em graus para posicao maxima da concha


  //Configurações do braco
  Servo servoBraco;  // Chama a biblioteca para controle do servo da Braco
  int valorPotBraco; //Potenciometro da Braco
  int pinoPotBraco = A1; // Porta Potenciometro do braco
  float posicaoBraco = 90; // Posição da Braco ao ligar
  float posicaoBracoAntiga = posicaoBraco;
  float acrescimoBraco = 0; // Acrescimo da Braco exponencial ao valor do potenciometro
  int posicaoMaximaBraco = 138; // Valor em graus para posicao maxima do braco
  int posicaoMinimaBraco = 31; // Valor em graus para posicao maxima do braco


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
  servoConcha.attach(9);  //Pino PWM que manda sinal para o servo da Concha
  servoBraco.attach(10);  //Pino PWM que manda sinal para o servo do Braco
  servoConj.attach(6);  //Pino PWM que manda sinal para o servo do Conjunto articulado
  //servoRotMaquina.attach(11);  //Pino PWM que manda sinal para o servo da Rotação da maquina || Vai ser usado um escravo Attiny88 pois esse pino 11 é usado pelo RF24
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
    Serial.println(dadosRecebidosRF);

    char delimiter = '-';
    String dadosFormatadosRF[10];
    formataDadosRF(dadosRecebidosRF, delimiter, dadosFormatadosRF, 10);
    if(dadosFormatadosRF[1] == "demonstracao"){ //Verifica se foi iniciado o modo demonstração
      demonstracao();
    }
    if(dadosFormatadosRF[1] == "3"){
      //Serial.println(dadosFormatadosRF[0]);
      int valor = dadosFormatadosRF[0].toInt();
      controleConcha(valor);
    }
    if(dadosFormatadosRF[1] == "1"){
      //Serial.println(dadosFormatadosRF[0]);
      int valor = dadosFormatadosRF[0].toInt();
      controleBraco(valor);
    }
    if(dadosFormatadosRF[1] == "2"){
      //Serial.println(dadosFormatadosRF[0]);
      int valor = dadosFormatadosRF[0].toInt();
      controleConj(valor);
    }
    if(dadosFormatadosRF[1] == "4"){
      //Serial.println(dadosFormatadosRF[0]);
      int valor = dadosFormatadosRF[0].toInt();
      controleRotMaquina(valor);
    }
    if(dadosFormatadosRF[1] == "5"){
      //Serial.println(dadosFormatadosRF[0]);
      int valor = dadosFormatadosRF[0].toInt();
      controleMovimento(valor);
    }
  }
   delay(10);
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
      acrescimoConcha = 3.5;
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
      if(debugConcha == 1){
        Serial.print("Posição da Concha: ");
        Serial.print(posicaoConcha);
        Serial.println(" Graus");
      }
    }
  }

  if(valorPotConcha < 500){
    if(valorPotConcha < 25){
      acrescimoConcha = 3.5;
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
      acrescimoBraco = 3.5;
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
      if(debugBraco == 1){
        Serial.print("Posição da Braço: ");
        Serial.print(posicaoBraco);
        Serial.println(" Graus");
      }
    }
  }

  if(valorPotBraco < 500){
    if(valorPotBraco < 25){
      acrescimoBraco = 3.5;
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
   Wire.beginTransmission(8); // Endereço do ATtiny88 escravo por I2C
   delay(10);
  //------ Inicio Controle da Conj ---------------
  //valorPotConj = analogRead(pinoPotConj);
  if(valorPotConj > 520){
    if(valorPotConj > 1020){
      acrescimoConj = 3;
    }else 
    if(valorPotConj > 900){
       acrescimoConj = 1.5;
    }else if(valorPotConj > 700){
      acrescimoConj = 1;
    }else if(valorPotConj > 600){
      acrescimoConj = 0.5;
    }else{
      acrescimoConj = 0.1;
    }

    posicaoConj = posicaoConj + acrescimoConj;
    if(posicaoConj > posicaoMaximaConj){
      posicaoConj = posicaoMaximaConj;
    }
    if(posicaoConjAntiga != posicaoConj){
      //servoConj.write(posicaoConj); //Aplica a posição da Conj em graus
      int posicaoViaI2C = int(posicaoConj); // Converte o float para inteiro
      Wire.write(posicaoViaI2C); //Aplica a posição da RotMaquina em graus
      if(debugConj == 1){
        Serial.print("Posicao do Conjuto Articulado: ");
        Serial.print(posicaoConj);
        Serial.println(" Graus");
      }
    }
  }

  if(valorPotConj < 500){
    if(valorPotConj < 10){
      acrescimoConj = 3;
    }else if(valorPotConj < 100){
      acrescimoConj = 1.5;
    }else if(valorPotConj < 300){
      acrescimoConj = 1;
    }else if(valorPotConj < 400){
      acrescimoConj = 0.5;
    }else{
      acrescimoConj = 0.1;
    }

    posicaoConj = posicaoConj - acrescimoConj;
    if(posicaoConj < posicaoMinimaConj){
      posicaoConj = posicaoMinimaConj;
    }
    if(posicaoConjAntiga != posicaoConj){
      //servoConj.write(posicaoConj); //Aplica a posição da Conj em graus
      int posicaoViaI2C = int(posicaoConj); // Converte o float para inteiro
      Wire.write(posicaoViaI2C); //Aplica a posição da RotMaquina em graus
      if(debugConj == 1){
        Serial.print("Posição do Conjuto Articulado: ");
        Serial.print(posicaoConj);
        Serial.println(" Graus");
      }
    }
  }
  
  Wire.endTransmission();
  delay(10);
  return;
  //------ Fim Controle da Conj ---------------
}

void controleRotMaquina(int valorPotRotMaquina){
  //------ Inicio Controle da RotMaquina ---------------
  //valorPotRotMaquina = analogRead(pinoPotRotMaquina);
  if(valorPotRotMaquina > 550){
    if(valorPotRotMaquina > 1000){
      acrescimoRotMaquina = 3.5;
    }else 
    if(valorPotRotMaquina > 900){
       acrescimoRotMaquina = 1.5;
    }else if(valorPotRotMaquina > 700){
      acrescimoRotMaquina = 1;
    }else if(valorPotRotMaquina > 600){
      acrescimoRotMaquina = 0.5;
    }else{
      acrescimoRotMaquina = 0.1;
    }

    posicaoRotMaquina = posicaoRotMaquina + acrescimoRotMaquina;
    if(posicaoRotMaquina > posicaoMaximaRotMaquina){
      posicaoRotMaquina = posicaoMaximaRotMaquina;
    }
    if(posicaoRotMaquinaAntiga != posicaoRotMaquina){
      servoRotMaquina.write(posicaoRotMaquina); //Aplica a posição da RotMaquina em graus
      if(debugRotMaquina == 1){
        Serial.print("Valor Potenciometro Rotacao Maquina: ");
        Serial.print(valorPotRotMaquina);
        Serial.println("");
      }
    }
  }

  if(valorPotRotMaquina < 480){
    if(valorPotRotMaquina < 25){
      acrescimoRotMaquina = 3.5;
    }else if(valorPotRotMaquina < 100){
      acrescimoRotMaquina = 1.5;
    }else if(valorPotRotMaquina < 300){
      acrescimoRotMaquina = 1;
    }else if(valorPotRotMaquina < 400){
      acrescimoRotMaquina = 0.5;
    }else{
      acrescimoRotMaquina = 0.1;
    }

    posicaoRotMaquina = posicaoRotMaquina - acrescimoRotMaquina;
    if(posicaoRotMaquina < posicaoMinimaRotMaquina){
      posicaoRotMaquina = posicaoMinimaRotMaquina;
    }
    if(posicaoRotMaquinaAntiga != posicaoRotMaquina){
      servoRotMaquina.write(posicaoRotMaquina); //Aplica a posição da RotMaquina em graus
      if(debugRotMaquina == 1){
        Serial.print("Valor Potenciometro Rotacao Maquina: ");
        Serial.print(valorPotRotMaquina);
        Serial.println("");
      }
    }
  }
  return;
  //------ Fim Controle da RotMaquina ---------------
}

void controleMovimento(int valorPotFrenteRe){
  //------ Inicio Controle Movimento ---------------
    
    //valorPotFrenteRe = analogRead(pinoPotFrenteRe); 
    //Serial.println(valorPotFrenteRe);
    if(valorPotFrenteRe > 490 && valorPotFrenteRe < 520){ // Confirma se esta parado para virar parado
      //Controla virar parado
      valorPotEstDirEsq = analogRead(pinoPotEstDirEsq);
      if(valorPotEstDirEsq > 550){
        int valorAceleracao = map(valorPotEstDirEsq, 0, 1023, 0, 180);
        Serial.println(valorAceleracao);
        servoEstDir.write(valorAceleracao); //Acelera com valor em graus
        servoEstEsq.write(valorAceleracao); //Acelera com valor em graus
        //delay(10); 
      }else if(valorPotEstDirEsq < 450){
        int valorAceleracao = map(valorPotEstDirEsq, 0, 1023, 0, 180);
        servoEstDir.write(valorAceleracao); //Acelera com valor em graus
        servoEstEsq.write(valorAceleracao); //Acelera com valor em graus
        //delay(10); 
      }else{
        servoEstDir.write(93); // Para o movimento
        servoEstEsq.write(93); // Para o movimento
      }
      if(debugMovimento == 1){
        //Serial.println(valorPotEstDirEsq);  
      }
    }
    else{ //Controla Frente e ré e virar andando
      if(valorPotFrenteRe > 600){
        int valorAceleracao = map(valorPotFrenteRe, 0, 1023, 0, 180);
        int diferenca = valorAceleracao - 90;
        int valorAceleracaoDir = 90 - diferenca;
        servoEstDir.write(valorAceleracaoDir); //Acelera com valor em graus
        servoEstEsq.write(valorAceleracao); //Acelera com valor em graus
        //delay(10); 
      }else if(valorPotFrenteRe < 400){
        int valorAceleracao = map(valorPotFrenteRe, 0, 1023, 0, 180);
        int diferenca = 90 - valorAceleracao;
        int valorAceleracaoDir = 90 + diferenca;
        servoEstDir.write(valorAceleracaoDir); //Acelera com valor em graus
        servoEstEsq.write(valorAceleracao); //Acelera com valor em graus
        //delay(10); 
      }else{
        servoEstDir.write(93); // Para o movimento
        servoEstEsq.write(93); // Para o movimento
      }
      if(debugMovimento == 1){
        Serial.println(valorPotFrenteRe);  
      }
    }
    //Controla virar parado
    return;
  //------ Fim Controle Movimento ---------------
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
