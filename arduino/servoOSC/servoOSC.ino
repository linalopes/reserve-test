// Uncomment this if you need debugging
// #define ARDUINOOSC_DEBUGLOG_ENABLE

#include <ArduinoOSCWiFi.h>
#include "ESP8266_ISR_Servo.h"  // Include the new ISR Servo library

// WiFi details
const char* ssid = "Bandwidth Together";
const char* pwd = "mandanudes";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

// OSC details
const char* host = "192.168.1.200";
const int recv_port = 54321;
const int bind_port = 54345;
const int send_port = 55555;
const int publish_port = 54445;

// Servo configuration
const int servoPin = 14;  // GPIO pin connected to the servo motor
int servoIndex;           // Variable to hold the servo index for ISR control
int servoPos = 0;         // Initial position of the servo

// Function to control servo based on OSC message
void onOscServoControl(const OscMessage& m) {
    float pos = m.arg<float>(0);  // Get the float value from the OSC message
    int servoPos = (int)pos;      // Convert it to an integer
    servoPos = constrain(servoPos, 0, 180);  // Constrain the value between 0 and 180 degrees
    
    ISR_Servo.setPosition(servoIndex, servoPos);  // Move the servo using ISR library
    Serial.print("Servo moved to position: ");
    Serial.println(servoPos);  // Print the position to Serial for debugging
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    // Initialize the servo motor using the ISR Servo library
    servoIndex = ISR_Servo.setupServo(servoPin, 500, 2400);  // Set up servo with min/max pulse
    if (servoIndex != -1) {
        Serial.println("Servo attached successfully!");
        ISR_Servo.setPosition(servoIndex, servoPos);  // Set the initial servo position
    } else {
        Serial.println("Error attaching servo!");
    }

    // Connect to WiFi
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());

    // Subscribe to OSC messages for controlling the servo
    OscWiFi.subscribe(recv_port, "/servo", onOscServoControl);
    OscWiFi.subscribe(recv_port, "/right", onOscServoControl);
    OscWiFi.subscribe(recv_port, "/left", onOscServoControl);
}

void loop() {
    // Handle OSC messages
    OscWiFi.update();
}
