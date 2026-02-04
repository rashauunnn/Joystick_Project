#include <esp_now.h>
#include <WiFi.h>

// Motor A pins (Left Motor)
#define ENA 23
#define IN1 5
#define IN2 18

// Motor B pins (Right Motor)
#define ENB 22
#define IN3 19
#define IN4 21

// Data structure matching transmitter
struct PacketData {
  byte xAxisValue;
  byte yAxisValue;
};
PacketData receivedData;

// Motor setup
void setupMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// Control motor logic
void driveMotors(byte xVal, byte yVal) {
  int yMapped = map(yVal, 0, 254, -255, 255);
  int xMapped = map(xVal, 0, 254, -255, 255);

  int leftMotor = yMapped + xMapped;
  int rightMotor = yMapped - xMapped;

  leftMotor = constrain(leftMotor, -255, 255);
  rightMotor = constrain(rightMotor, -255, 255);

  // Left motor
  if (leftMotor >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, leftMotor);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -leftMotor);
  }

  // Right motor
  if (rightMotor >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, rightMotor);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -rightMotor);
  }
}

// Updated callback signature for ESP32 core v3.x+
void OnDataRecv(const esp_now_recv_info_t *recvInfo, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  driveMotors(receivedData.xAxisValue, receivedData.yAxisValue);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);

  setupMotors();
  Serial.println("Receiver ready.");
}

void loop() {
  // All logic handled in callback
}