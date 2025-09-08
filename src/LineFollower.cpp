#include "LineFollower.hh"

#define LIMIAR_BRANCO 750

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
    for (uint16_t i = 0; i < 150; i++) {
        _qtrSensor.calibrate();
    }


    digitalWrite(LED_BUILTIN, LOW);
    
}

void LineFollower::pidControl() {
    
    uint16_t position = _qtrSensor.readLineWhite(_sensorArray);

    int error = position - 3500;

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

void LineFollower::stop() {

    _leftMotor.stop();
    _rightMotor.stop();

}
void LineFollower::debug() {
    Serial.print("Sensor readings: ");

    uint16_t p = _qtrSensor.readLineWhite(_sensorArray);
    
    for (int i = 0; i < SENSOR_NUM; i++) {
        Serial.print(p);
        if (i < SENSOR_NUM - 1) {
            Serial.print(", ");
        }
    }
    Serial.println();
}

void LineFollower::followLine() {
    
    // --- 1. O PID e a leitura dos sensores principais sempre rodam ---
    pidControl();

    // --- 2. Lógica de controle dos sensores laterais ---

    // Se sensores laterais estão ATIVOS
   /* if (_sensoresLateralDireito && _sensoresLateralEsquerdo) {
        
        // Verifica se é uma INTERSECÇÃO (todos sensores frontais no branco)
        bool intersecao = true;
        for (int i = 0; i < 8; i++) {
            if (sensorValues[i] >= LIMIAR_BRANCO) {
                intersecao = false;
                break;
            }
        }

        if (intersecao) {
            Serial.println("INTERSECÇÃO detectada! Desativando sensores laterais.");
            _sensoresLateralDireito = false;
            _sensoresLateralEsquerdo = false;
        } 
        else {
            // Lógica normal de contagem (apenas quando sensores estão ativos)
            if (digitalRead(2) == LOW && digitalRead(4) == HIGH) {
                if (millis() - ultimateTime > 500) {
                    Serial.print("Marca lateral detectada! Contagem: ");
                    Serial.println(count);
                    count++;
                    ultimateTime = millis();
                }
            }
        }
    }
    // Se sensores laterais estão DESATIVADOS (após intersecção)
    else {
        // Só reativa quando AMBOS sensores laterais detectarem branco
        if (digitalRead(2) == HIGH && digitalRead(4) == HIGH) {
            Serial.println("SAÍDA DA INTERSECÇÃO: Reativando sensores laterais.");
            _sensoresLateralDireito = true;
            _sensoresLateralEsquerdo = true;
        }
    }
*/

   if (digitalRead(2) == LOW){
        stop();
        if(digitalRead(4)==HIGH && ((millis()-ultimoTempo) >= 30000)){
            count++;
            ultimoTempo = millis();    
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(LED_BUILTIN, LOW);
        } 
        return;
    }
    // --- 3. Condição de parada final ---
    
    
    
    
    
    if (count > 1) {
        Serial.print("LINHA DE CHEGADA DETECTADA! PARANDO...\n");
        digitalWrite(LED_BUILTIN, HIGH);
        stop();
        _leftMotor.setSpeed(50);
        _rightMotor.setSpeed(50);
        delay(700);
        digitalWrite(LED_BUILTIN, LOW);
        stop();
        while(1); // Para totalmente
    }
                                                                                           
}