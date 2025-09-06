#ifndef MOTOR_HH
#define MOTOR_HH

#include <Arduino.h>

/**
 * @class Motor
 * @brief Classe responsável pela implementação de um motor e seu comportamento, utilizando dois pinos PWM conectados a uma ponte H qualquer.
 * 
 */
class Motor {
public:

    /**
     * @brief Construtor para um objeto motor. Recebe os pinos responsáveis pelo seu controle PWM.
     * 
     * Faz com que após a instanciação dos motores, eles inicializem parados.
     * 
     * @param dirPin1 
     * @param dirPin2 
     */
    Motor(int dirPin1, int dirPin2) :
        _dirPin1 {dirPin1}, _dirPin2 {dirPin2}, _speed {0}

    {
        stop();
    }

    /**
     * @brief Inicializa os pinos conectados à ponte H como OUTPUT. Chamada dentro da função init() do objeto Line Follower, dentro de setup().
     * 
     */
    void init() {
        pinMode(_dirPin1, OUTPUT);
        pinMode(_dirPin2, OUTPUT);
    }

    /**
     * @brief Define a velocidade do motor (0 - 255).
     * 
     * @param speed 
     */
    void setSpeed(int speed);

    /**
     * @brief Retorna a velocidade do motor
     * 
     * @return int 
     */
    int getSpeed();

    /**
     * @brief Define a direção do motor. 
     * 
     * true faz com que o motor vá para frente.
     * false faz com que o motor vá para trás.
     * 
     * @param forward 
     */
    void setDirection(bool forward);

    /**
     * @brief Para o motor.
     * 
     */
    void stop();

private:

    // Pino de direção 1.
    int _dirPin1;  

    // Pino de direção 2.
    int _dirPin2;   

    // Atributo de velocidade.
    int _speed;     

    // Direção atual do motor (true = frente, false = trás).
    bool _direction;  

    // Função responsável por aplicar os estados definidos ao motor. Chamada em toda função relacionada à alteração de comportamento do motor.
    void applyState();
};

#endif // MOTOR_HH