#ifndef LINEFOLLOWER_HH
#define LINEFOLLOWER_HH

#include <Motor.hh>
#include <QTRSensors.h>

#define SENSOR_NUM 8    
extern int count;  // Apenas DECLARA a variável, não a cria
/**
 * @class LineFollower
 * @brief Implementa funcionalidades de um robô seguidor de linhas que utiliza sensores QTR, da Pololu, e um algoritmo de controle PID.
 * 
 * Essa classe implementa rotinas de inicialização, calibração e o próprio algoritmo de controle.
 * 
 */
class LineFollower {
    public: 
    
    /**
     * @brief Construtor para um objeto seguidor de linhas, recebe ambos motores, que devem ser instanciados separadamente.
     * 
     * @param rightMotor 
     * @param leftMotor 
     */
    LineFollower(Motor &rightMotor, Motor &leftMotor) : _rightMotor {rightMotor}, _leftMotor {leftMotor} 
    {
        
    };
    
    /**
     * @brief Inicializa o seguidor de linha dentro do escopo da função setup(), no arquivo main.ino.
     * 
     * Essa função inicializa a transmissão serial de dados, bem como define o tipo do sensor e os pinos a serem utilizados.
     * 
     * @param sensorPins Array de pinos conectados ao sensor QTR.
     * @param emmiterPin Pino responsável pelo controle do emissor do sensor.
     */
    void init(const uint8_t sensorPins[], const int emmiterPin);
    
    /**
     * @brief Implementa uma rotina de calibração, seguindo o padrão fornecido pela documentação pela biblioteca QTRSensors.
     * 
     */
    void calibrationRoutine();
    
    /**
     * @brief Implementa um algoritmo de controle PID para os 6 sensores utilizados. Setpoint com uma leitura de 2500 para manter a linha no meio do sensor.
     * 
     * O algoritmo usa a leitura do sensor frontal e compensa na velocidade dos motores para centralizar a linha da melhor forma possível.
     * Para melhor funcionalidade, é recomendável alterar as constantes que são definidas abaixo de acordo com o tipo de robô.
     */
    void pidControl();

    /**
     * @brief Chama a rotina do controle PID, criada para inserção de possíveis novas rotinas mais sofisticadas para cenários diversos (como leitura lateral).
     * 
     */
    void followLine();

    /**
     * @brief Imprime a posição da linha lida pelos sensores para finalidade de debug.
     * 
     */

    void stop();
    void debug();
   
private:
    long ultimateTime = 0;  
    bool _sensoresLateralDireito; // Estado inicial: Sensores laterais ativos.
    bool _sensoresLateralEsquerdo; // Estado inicial: Sensores laterais ativos.
    // Array que armazena as leituras de cada sensor.
    uint16_t _sensorArray[SENSOR_NUM];

    // Referência aos motores que controlam o robô (pass-by-reference para evitar nova instância dos motores).
    Motor &_rightMotor, &_leftMotor;

    // Objeto do sensor QTR, utilizado para fazer a leitura da linha.
    QTRSensors _qtrSensor;

    // Constantes utilizadas pelo algoritmo PID, devem ser ajustadas de acordo com o robô/ambiente.
    float _kP{0.35f}, _kI{0.000f}, _kD{3.0f};    
    
    //  float _kP{0.1f}, _kI{0.001f}, _kD{10.0f};     

    // Erro adquirido pelo controle PID, inicializado em 0.
    int _lastError{0};                          

    // Velocidade base dos motores.
    int _baseSpeed{60};

    // Velocidade máxima dos motores.
    int _maxSpeed{80};               

    // Valores dos componentes Proporcional, Integral e Derivativo. Precisam ser armazenados para a retroalimentação do algoritmo.          
    int _P, _I, _D;

};

#endif //LINEFOLLOWER_HH