// Pinos de conexão
const int pwmPin = 15;     // Pino para PWM do fan
const int vfgPin = 14;    // Pino de leitura do sensor de tachômetro do fan

// Variáveis de controle
int pulseCount = 0;  // Contador de pulsos do tachômetro
int rpm = 0;                  // Velocidade do fan em RPM
long lastMillis = 0; // Para medir o tempo de cálculo do RPM

// Função de interrupção para contar pulsos
void tachInterrupt() {
    pulseCount++;
}

// Configuração inicial
void setup() {
    Serial.begin(9600);
    pinMode(pwmPin, OUTPUT);
    pinMode(vfgPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(vfgPin), vfgInterrupt, FALLING);
}

// Função para ajustar a velocidade do fan
void setFanSpeed(int dutyCycle) {
    analogWrite(pwmPin, map(dutyCycle, 0, 100, 0, 255)); // Controla PWM entre 0 e 100%
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

// Loop principal
void loop() {
    setFanSpeed(50);  // Ajusta o PWM para 50% (você pode mudar conforme necessário)
    calculateRPM();   // Calcula o RPM

    // Imprime a velocidade do fan
    Serial.print(rpm);

    delay(500);  // Intervalo de leitura para o serial monitor
}
