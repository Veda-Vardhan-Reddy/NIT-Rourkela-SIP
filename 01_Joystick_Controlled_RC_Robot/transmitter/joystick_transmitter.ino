#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 32

#define ESPNOW_CHANNEL 1

uint8_t receiverMac[] = {0x30, 0xED, 0xA0, 0xBB, 0x43, 0x28};

typedef struct struct_message {
  char command;
} struct_message;

struct_message outgoingData;
esp_now_peer_info_t peerInfo = {};

void onDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

char getCommandFromJoystick(int xValue, int yValue) {
  if (yValue > 3000) return 'F';
  if (yValue < 1000) return 'B';
  if (xValue < 1000) return 'L';
  if (xValue > 3000) return 'R';
  return 'S';
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(JOY_SW, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = ESPNOW_CHANNEL;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP-NOW Transmitter Ready");
}

void loop() {
  int xValue = analogRead(JOY_X);
  int yValue = analogRead(JOY_Y);
  int swValue = digitalRead(JOY_SW);

  outgoingData.command = getCommandFromJoystick(xValue, yValue);

  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&outgoingData, sizeof(outgoingData));

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("  Y: ");
  Serial.print(yValue);
  Serial.print("  SW: ");
  Serial.print(swValue);
  Serial.print("  Sent: ");
  Serial.print(outgoingData.command);
  Serial.print("  Result: ");
  Serial.println(result == ESP_OK ? "OK" : "ERROR");

  delay(100);
}
