#define CE_PIN 11
#define CSN_PIN 10
//#define CSN_PIN 3 // uncomment for ATtiny85 3 pins solution

#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

unsigned long payload = 0;

void setup() {
radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
    radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address); // Write to device address '2Node'
}



void loop(){
   const char text[] = "concha-62";
  radio.write(&text, sizeof(text));
  
  delay(1000); // Aguarda um segundo antes de enviar mais dados
}
