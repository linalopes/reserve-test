#include "ESP8266_ISR_Servo.h"

#define SERVO_PIN 12  // Pin D6 on ESP8266 (GPIO 12)

int servoIndex;
int angle = 0;  // Variable to store the servo angle

void setup() {
    Serial.begin(115200);

    // Initialize the servo on the defined pin
    servoIndex = ISR_Servo.setupServo(SERVO_PIN, 500, 2400);  // Adjust pulse widths for your servo

    if (servoIndex != -1) {
        Serial.println("Servo attached successfully!");
    }
    else {
        Serial.println("Error attaching servo!");
    }
}

void loop() {
    // Move the servo to 0 degrees
    ISR_Servo.setPosition(servoIndex, 0);
    delay(1000);  // Wait for 1 second

    // Move the servo to 180 degrees
    ISR_Servo.setPosition(servoIndex, 180);
    delay(1000);  // Wait for 1 second
}
