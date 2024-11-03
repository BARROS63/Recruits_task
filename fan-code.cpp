#include <Arduino.h>
#include <FlexCAN_T4.h>
// Pinos de conexão
#define pwm_Pin A1     // Pino para PWM do fan
#define vfg_Pin A0    // Pino de leitura do sensor de tachômetro do fan

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;

// Variáveis de controlo
int pulseCount = 0;  // Contador de pulsos do tachômetro
int rpm = 0;         // Velocidade do fan em RPM
long lastMillis = 0; // Para medir o tempo de cálculo do RPM
int dutyCycle = 100; //Percentagem inicial do funcionamento da ventoinha, considerou-se 100 porque quando se liga o carro para andar é conveniente que as ventoinhas funcionem ao máximo desde o inicio para fazer com que o carro demore o máximo temoo a aquecer
int countmsg = 0;

bool endiass = 0;// 0 little endian (direita para a esquerda) e 1 big endian (esquerda para a direita)
// Configuração inicial
void setup() {
    Serial.begin(9600);

    //Configuração dos pinos
    pinMode(pwm_Pin, OUTPUT);
    pinMode(vfg_Pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(vfg_Pin), vfgInterrupt, FALLING);//converte o pin para um pin de interrupção que só se ativará quando o valor do pin trocar de high para low

    //Inicialização do CAN bus a 500kps
    can1.begin();
    can1.setBaudRate(500000);

    setFanSpeed(dutyCycle);
}

// Função de interrupção para contar pulsos
void tachInterrupt() {
    pulseCount++;
}

// Função para ajustar a velocidade do fan
void setFanSpeed(int dutyCycle) {
    int pwmValue = map(dutyCycle, 0, 100, 0, 255); //valor pretendido recebido por can para a ventoinha funcionar em percentagem
    analogWrite(pwmPin, pwmValue); // Controla PWM entre 0 e 100% e os valores 0 a 255 são o intervalo para o sinal analogico PWM porque correspondem a um intervalo de 8 bits
}

// Função de cálculo do RPM
void calculateRPM() {
    long currentMillis = millis();
    if (currentMillis - lastMillis >= 1000) { // Atualiza a cada segundo
        rpm = (pulseCount * 30); // Cada rotação completa gera 2 pulsos, (pulseCount / 2) * 60 = pulseCount * 30
        pulseCount = 0;
        lastMillis = currentMillis;
    }
}

void sendRPMPercentage(){
    int rpmPercentage = map(rpm, 0, 255, 0, 100); //converte rpm ventoinha para percentagem
    CAN_message_t fanrpm_msg;//struct da biblioteca FlexCAN_T4
    fanrpm_msg.id = 0x666;//id da bms para o teency C3 (considero que deveria ser para c1 mas não encontrei esse id)
    fanrpm_msg.len = 1;//tamanho da mensagem limitado a byte porque na bms há 8 bytes a ser utilizador
    fanrpm_msg.buf[7] = rpmPercentage;//atualiza o valor para aquele buffer
    can1.write(fanrpm);
}
void endiassstate(){ // nem sei ao certo se isto ´é necessário porque como só tenho um byte a ser enviado não dá para o escrever de outra forma
    if (countmsg == 5){
        endiass != endiass;
        countmsg = 0; 
    }
    msgcount++; 
}
void receiveDutyCycle(){
    CAN_message_t fandutycycle_msg; //struct da biblioteca FlexCAN_T4
    if (can1.read(fandutycycle_msg)){
        countmsg++;
        int receiveDutyCycle = fandutycyle_msg.buf[0];
        setFanSpeed(receiveDutyCycle);
    }
}
// Loop principal
void loop() {

    receiveDutyCycle();// valor recebido através de can para meter a fan a rodar
    calculateRPM();   // Calcula o rpm
    sendRPMPercentage(); // enviar os rpm em percentagem
}