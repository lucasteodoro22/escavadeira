#include <Wire.h>
#include <Servo.h>

Servo myServo; // Cria uma instância do objeto Servo

void setup() {
  Wire.begin(8); // Inicializa a biblioteca Wire (I2C) com o endereço 8
  Wire.onReceive(receiveEvent); // Configura a função a ser chamada quando dados forem recebidos
  myServo.attach(9); // Anexa o servo ao pino 9
}

void loop() {
  // O loop do ATtiny88 pode ser vazio, já que a função onReceive será chamada quando os dados forem recebidos
}

void receiveEvent(int byteCount) {
  while (Wire.available()) {
    int pwmValue = Wire.read(); // Lê o valor PWM recebido
    int servoPosition = map(pwmValue, 0, 255, 0, 180); // Mapeia o valor PWM para uma posição do servo (0-180 graus)
    myServo.write(servoPosition); // Define a posição do servo
  }
}
