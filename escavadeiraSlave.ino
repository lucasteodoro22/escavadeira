#include <Servo.h>

#include <Wire.h>
Servo myServo; // Cria uma instância do objeto Servo

void setup() {
  Wire.begin(8);                // join I2C bus with address #8
  Wire.onReceive(receiveEvent); // register event
  myServo.attach(9);
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  int x = Wire.read();    // receive byte as an integer
  myServo.write(x);
}
