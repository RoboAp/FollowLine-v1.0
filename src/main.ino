#include <QTRSensors.h>
#include <Arduino.h>
#include <LineFollower.hh>

int count = 0; // Aqui a variável é criada e inicializada 
float ultimoTempo = 0;

const uint8_t sensorPins[] = {A0 ,A1, A2, A3, A4, A5, A6, A7};
const uint8_t emitterPin = 12;

Motor rightMotor(5, 6), leftMotor(9, 10);
LineFollower lineFollower(rightMotor, leftMotor);

int buttonState = 0;

void setup() {
  pinMode(11, INPUT_PULLUP); // Botão de start
  lineFollower.init(sensorPins, emitterPin);
  lineFollower.calibrationRoutine();
}

void loop() {
  if (digitalRead(11) == LOW) {
    buttonState = 1;
    count = 0; // Reseta contador ao iniciar
    delay(500); // Debounce
    ultimoTempo = millis();
    while (buttonState == 1) {
      lineFollower.followLine();
    }
  }
}