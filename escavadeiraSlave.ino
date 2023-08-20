#include <Servo.h>
#include <Wire.h>
#include <string.h>

Servo myServo; // Cria uma instância do objeto Servo

void setup() {
  Serial.begin(9600);
  Wire.begin(0x08);                // join I2C bus with address #8
  Wire.onReceive(receiveEvent); // register event
  myServo.attach(9);
}

void loop() {
  delay(500);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
  }
  int x = Wire.read();    // receive byte as an integer
  myServo.write(x);
}
