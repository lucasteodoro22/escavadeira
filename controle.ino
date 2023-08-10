#define CE_PIN 11 
#define CSN_PIN 10
#include "RF24.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

//Inicia a biblioteca RF24
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Define o ID do slave

int analogConcha = 5; // Define o pino para controle da Concha
int analogBraco = 3; // Define o pino para controle da Braco
int analogConj = 4; // Define o pino para controle da Conj
int analogRotMaquina = 2; // Define o pino para controle da RotMaquina
int analogMovimento = 1; // Define o pino para controle da Movimento
int analogDirEsq = 0; // Define o pino para controle da DirEsq


void setup() {
  radio.begin(); // Inicia o RF
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.setPALevel(RF24_PA_MIN); //Potencia do Sinal
  radio.openWritingPipe(address); // Ativa modo gravação
  ADC_Init(); // Inicia a função para os pinos analogicos
}



void loop(){
  uint16_t valorConcha = ADC_Read(analogConcha); 
  if(valorConcha > 520 || valorConcha < 500){
    char text[10];
    itoa(valorConcha, text, 10);
    strcat(text, "-1");
    radio.write(&text, sizeof(text));
  }
  
  uint16_t valorBraco = ADC_Read(analogBraco); 
  if(valorBraco > 520 || valorBraco < 500){
    char text[10];
    itoa(valorBraco, text, 10);
    strcat(text, "-2");
    radio.write(&text, sizeof(text));
  }
  uint16_t valorConj = ADC_Read(analogConj); 
  if(valorConj > 520 || valorConj < 500){
    char text[10];
    itoa(valorConj, text, 10);
    strcat(text, "-3");
    radio.write(&text, sizeof(text));
  }
  uint16_t valorRotMaquina = ADC_Read(analogRotMaquina); 
  if(valorRotMaquina > 520 || valorRotMaquina < 500){
    char text[10];
    itoa(valorRotMaquina, text, 10);
    strcat(text, "-4");
    radio.write(&text, sizeof(text));
  }
  uint16_t valorMovimento = ADC_Read(analogMovimento); 
  if(valorMovimento > 520 || valorMovimento < 500){
    char text[10];
    itoa(valorMovimento, text, 10);
    strcat(text, "-5");
    radio.write(&text, sizeof(text));
  }  
  uint16_t valorDirEsq = ADC_Read(analogDirEsq); 
  if(valorDirEsq > 520 || valorDirEsq < 500){
    char text[10];
    itoa(valorDirEsq, text, 10);
    strcat(text, "-6");
    radio.write(&text, sizeof(text));
  }  
     
  
  delay(10); // Aguarda um segundo antes de enviar mais dados
}


void ADC_Init() {
    // Configura a tensão de referência para AVCC (5V)
    ADMUX = (1 << REFS0);
    
    // Habilita o ADC e define o prescaler para 64
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t ADC_Read(uint8_t channel) {
    // Seleciona o canal de entrada do ADC
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Inicia e espera a conversão ser concluída
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    
    // Retorna o valor da conversão
    return ADC;
}

