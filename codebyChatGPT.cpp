#include <Arduino.h>
#include <FlexCAN_T4.h>

// Pinos de conexão
#define pwm_Pin A1     // Pino para PWM do fan
#define vfg_Pin A0     // Pino de leitura do sensor de tachômetro do fan

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;

// Variáveis de controle
int pulseCount = 0;     // Contador de pulsos do tachômetro
int rpm = 0;            // Velocidade do fan em RPM
long lastMillis = 0;    // Para medir o tempo de cálculo do RPM
int dutyCycle = 100;    // Percentagem inicial de funcionamento
int countmsg = 0;
bool endiass = 0;       // 0 para little endian, 1 para big endian

// Configuração inicial
void setup() {
    Serial.begin(9600);

    // Configuração dos pinos
    pinMode(pwm_Pin, OUTPUT);
    pinMode(vfg_Pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(vfg_Pin), tachInterrupt, FALLING); // Corrige o nome da função

    // Inicialização do CAN bus a 500kps
    canBus.begin();
    canBus.setBaudRate(500000);

    setFanSpeed(dutyCycle);
}

// Função de interrupção para contar pulsos
void tachInterrupt() {
    pulseCount++;
}

// Função para ajustar a velocidade do fan
void setFanSpeed(int dutyCycle) {
    int pwmValue = map(dutyCycle, 0, 100, 0, 255); // Mapear dutyCycle de 0 a 100% para 0 a 255
    analogWrite(pwm_Pin, pwmValue);
}

// Função de cálculo do RPM
void calculateRPM() {
    long currentMillis = millis();
    if (currentMillis - lastMillis >= 1000) { // Atualiza a cada segundo
        rpm = (pulseCount * 30); // Cada rotação completa gera 2 pulsos, portanto multiplica pulseCount por 30
        pulseCount = 0;
        lastMillis = currentMillis;
    }
}

// Função para enviar o valor do RPM em percentagem
void sendRPMPercentage() {
    int rpmPercentage = map(rpm, 0, 255, 0, 100); // Converte RPM da ventoinha para percentagem
    CAN_message_t fanrpm_msg;
    fanrpm_msg.id = 0x666;  // ID de mensagem CAN
    fanrpm_msg.len = 1;
    fanrpm_msg.buf[0] = rpmPercentage; // Define o valor de RPM em percentagem no primeiro byte
    canBus.write(fanrpm_msg); // Envia a mensagem pelo CAN
}

// Função para alterar o estado de endianess
void endiassstate() {
    if (countmsg == 5) {
        endiass = !endiass; // Alterna endianess
        countmsg = 0;
    }
    countmsg++;
}

// Função para receber e definir o duty cycle
void receiveDutyCycle() {
    CAN_message_t fandutycycle_msg;
    if (canBus.read(fandutycycle_msg)) {
        countmsg++;
        int receivedDutyCycle = fandutycycle_msg.buf[0];
        setFanSpeed(receivedDutyCycle);
    }
}

// Loop principal
void loop() {
    receiveDutyCycle();    // Recebe duty cycle pelo CAN
    calculateRPM();        // Calcula o RPM
    sendRPMPercentage();   // Envia o RPM em percentagem pelo CAN
}
