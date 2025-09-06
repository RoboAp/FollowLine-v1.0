#include "LineFollower.hh"


void LineFollower::init(const uint8_t sensorPins[], const int emmiterPin) {
    
    Serial.begin(9600);
    _rightMotor.init();
    _leftMotor.init();
    _qtrSensor.setTypeAnalog();
    _qtrSensor.setSensorPins(sensorPins, SENSOR_NUM);
    _qtrSensor.setEmitterPin(emmiterPin);

}

void LineFollower::calibrationRoutine() {

    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);

    // 0,1 ms por sensor * 4 amostras por leitura de sensor * 6 sensores
    // * 10 leituras por chamada de calibrate() = ~24 ms por chamada de calibrate().
    // Executada 400 vezes, leva cerca de 10 segundos.
    for (uint16_t i = 0; i < 400; i++) {
        _qtrSensor.calibrate();
    }


    digitalWrite(LED_BUILTIN, LOW);
    
}

void LineFollower::pidControl() {
    
    uint16_t position = _qtrSensor.readLineWhite(_sensorArray);

    int error = position - 2500;

    _P = error;
    _I += error;
    _D = error - _lastError;
    _lastError = error;

    int pidOutput = static_cast<int>(_kP * _P + _kD * _D + _kI * _I);

    int motorSpeed1 = _baseSpeed - pidOutput;
    int motorSpeed2 = _baseSpeed + pidOutput;

    motorSpeed1 = constrain(motorSpeed1, 0, _maxSpeed);
    motorSpeed2 = constrain(motorSpeed2, 0, _maxSpeed);

    _leftMotor.setSpeed(motorSpeed1);
    _rightMotor.setSpeed(motorSpeed2);
    
}

void LineFollower::followLine() {

    pidControl();

}