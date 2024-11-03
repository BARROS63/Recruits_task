#include <Arduino.h>
#include <FlexCAN_T4.h>

void testFanControl() {
    // Test variables
    int testDutyCycle = 50; // Example: 50% duty cycle to set fan speed
    int expectedRPMPercentage = 50; // Expected RPM as 50% when duty cycle is 50%
    int messageCount = 0; // Counter for messages to test endianess

    // Send a CAN message with the desired duty cycle percentage
    CAN_message_t testMsg;
    testMsg.id = 0x123; // Example ID for testing
    testMsg.len = 1;
    testMsg.buf[0] = testDutyCycle; // 50% duty cycle test value
    canBus.write(testMsg);

    // Simulate receiving and processing the duty cycle
    receiveDutyCycle();

    // Check if PWM value is correctly mapped to a 50% duty cycle
    int pwmValue = analogRead(pwm_Pin); // Read the PWM output for validation
    int expectedPWM = map(testDutyCycle, 0, 100, 0, 255);
    if (pwmValue == expectedPWM) {
        Serial.println("Duty Cycle Test: PASS");
    } else {
        Serial.println("Duty Cycle Test: FAIL");
    }

    // Simulate RPM calculation
    pulseCount = expectedRPMPercentage / 30; // Manually set pulse count for 50% RPM
    calculateRPM();
    if (rpm == expectedRPMPercentage) {
        Serial.println("RPM Calculation Test: PASS");
    } else {
        Serial.println("RPM Calculation Test: FAIL");
    }

    // Test CAN message sending for RPM percentage
    CAN_message_t rpmMsg;
    if (canBus.read(rpmMsg) && rpmMsg.buf[0] == expectedRPMPercentage) {
        Serial.println("RPM CAN Message Test: PASS");
    } else {
        Serial.println("RPM CAN Message Test: FAIL");
    }

    // Check endianess toggle after 5 messages
    for (int i = 0; i < 5; i++) {
        messageCount++;
        endiassstate();
    }
    if (endiass == true) { // Endianess should toggle to '1' after 5 messages
        Serial.println("Endianess Toggle Test: PASS");
    } else {
        Serial.println("Endianess Toggle Test: FAIL");
    }
}

void setup() {
    Serial.begin(9600);
    canBus.begin();
    canBus.setBaudRate(500000);
    testFanControl(); // Run the test function on setup
}

void loop() {
    // Main loop left empty as tests are only run once in setup
}
