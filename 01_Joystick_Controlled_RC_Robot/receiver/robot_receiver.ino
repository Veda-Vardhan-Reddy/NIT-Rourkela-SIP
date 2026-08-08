#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define ESPNOW_CHANNEL 1

#define ENA 16
#define IN1 17
#define IN2 18
#define ENB 1
#define IN3 2
#define IN4 4

int motorSpeed = 230;

typedef struct struct_message {
  char command;
} struct_message;

struct_message incomingData;

void setSpeed(int leftSpeed, int rightSpeed) {
  ledcWrite(ENA, leftSpeed);
  ledcWrite(ENB, rightSpeed);
}

void stopMotors() {
  setSpeed(0, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  setSpeed(motorSpeed, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBackward() {
  setSpeed(motorSpeed, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  setSpeed(motorSpeed, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  setSpeed(motorSpeed, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void onDataRecv(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int len) {
  if (len != sizeof(struct_message)) return;

  memcpy(&incomingData, data, sizeof(incomingData));

  Serial.print("Received command: ");
  Serial.println(incomingData.command);

  switch (incomingData.command) {
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    default:
      stopMotors();
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttach(ENA, 5000, 8);
  ledcAttach(ENB, 5000, 8);

  stopMotors();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
  Serial.println("Receiver ready");
}

void loop() {
  delay(10);
}
