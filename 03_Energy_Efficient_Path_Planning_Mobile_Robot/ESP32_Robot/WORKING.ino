#include <WiFi.h>
#include <WebServer.h>
#include <math.h>
#include "webpage.h"

#define PWMA 25
#define AIN1 26
#define AIN2 27
#define PWMB 13
#define BIN1 14
#define BIN2 12
#define STBY 33

#define ENCA_A 16
#define ENCA_B 17
#define ENCB_A 15
#define ENCB_B 4

#define IR_LEFT  34
#define IR_RIGHT 35

#define IMU_RX 22
#define IMU_TX 23
#define GRAVITY 9.80665

HardwareSerial IMU(1);

float imuAx = 0, imuAy = 0, imuAz = 0;
float imuAx_ms2 = 0, imuAy_ms2 = 0, imuAz_ms2 = 0;
float imuWx = 0, imuWy = 0, imuWz = 0;
float imuRoll = 0, imuPitch = 0, imuYaw = 0;

const int ENC_DIR_A = 1;
const int ENC_DIR_B = -1;

const char* AP_SSID = "ESP32-Robot";
const char* AP_PASS = "robot123";
WebServer server(80);

volatile long encoderCountA = 0;
volatile long encoderCountB = 0;

const int PWM_FREQ = 20000;
const int PWM_RES = 8;
const int CH_A = 0;
const int CH_B = 1;

float wheelDiameterM = 0.085;
float pulsesPerWheelRev = 360.0;
float wheelBaseM = 0.16;
int defaultSpeed = 150;

enum MotionMode {
  IDLE,
  MOVE_DIST,
  TURN_ANGLE,
  SHAPE_SQUARE,
  SHAPE_TRIANGLE,
  SHAPE_CIRCLE,
  MANUAL,
  LINE_FOLLOW
};

volatile MotionMode motionMode = IDLE;

long startCountA = 0;
long startCountB = 0;
long targetCounts = 0;
int cmdSpeedA = 0;
int cmdSpeedB = 0;
int shapeStep = 0;
int shapeRepeats = 0;
float shapeSideM = 0.5;
float shapeRadiusM = 0.25;
unsigned long statusTimer = 0;

// line follower state
int lastLineState = 0;              // -1 left, 1 right, 0 unknown
unsigned long lineLostTime = 0;
const unsigned long lineRecoverMs = 220;

void sendCmd(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
  uint8_t cmd[5] = {b0, b1, b2, b3, b4};
  IMU.write(cmd, 5);
  IMU.flush();
}

void calibrateAccelerometer() {
  Serial.println("Calibrating accelerometer... keep the sensor still and level.");
  sendCmd(0xFF, 0xAA, 0x69, 0x88, 0xB5);
  delay(100);
  sendCmd(0xFF, 0xAA, 0x01, 0x01, 0x00);
  delay(5000);
  sendCmd(0xFF, 0xAA, 0x01, 0x00, 0x00);
  delay(500);
  while (IMU.available()) IMU.read();
  Serial.println("Calibration done. Starting data read...");
}

void readIMU() {
  static uint8_t buf[20];
  static uint8_t idx = 0;

  while (IMU.available()) {
    uint8_t b = IMU.read();

    if (idx == 0 && b != 0x55) continue;
    buf[idx++] = b;

    if (idx == 2 && buf[1] != 0x61 && buf[1] != 0x71) {
      idx = 0;
      continue;
    }

    if (idx == 20) {
      idx = 0;

      if (buf[1] == 0x61) {
        imuAx = (int16_t)(buf[3] << 8 | buf[2]) / 32768.0 * 16;
        imuAy = (int16_t)(buf[5] << 8 | buf[4]) / 32768.0 * 16;
        imuAz = (int16_t)(buf[7] << 8 | buf[6]) / 32768.0 * 16;

        imuAx_ms2 = imuAx * GRAVITY;
        imuAy_ms2 = imuAy * GRAVITY;
        imuAz_ms2 = imuAz * GRAVITY;

        imuWx = (int16_t)(buf[9] << 8 | buf[8]) / 32768.0 * 2000;
        imuWy = (int16_t)(buf[11] << 8 | buf[10]) / 32768.0 * 2000;
        imuWz = (int16_t)(buf[13] << 8 | buf[12]) / 32768.0 * 2000;

        imuRoll = (int16_t)(buf[15] << 8 | buf[14]) / 32768.0 * 180;
        imuPitch = (int16_t)(buf[17] << 8 | buf[16]) / 32768.0 * 180;
        imuYaw = (int16_t)(buf[19] << 8 | buf[18]) / 32768.0 * 180;
      }
    }
  }
}

float metersPerCount() {
  return (PI * wheelDiameterM) / pulsesPerWheelRev;
}

long metersToCounts(float meters) {
  return lround(fabs(meters) / metersPerCount());
}

long degreesToTurnCounts(float degrees) {
  float arc = PI * wheelBaseM * (fabs(degrees) / 360.0);
  return metersToCounts(arc);
}

void IRAM_ATTR readEncoderA() {
  if (digitalRead(ENCA_B) > 0) encoderCountA += ENC_DIR_A;
  else encoderCountA -= ENC_DIR_A;
}

void IRAM_ATTR readEncoderB() {
  if (digitalRead(ENCB_B) > 0) encoderCountB += ENC_DIR_B;
  else encoderCountB -= ENC_DIR_B;
}

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  ledcWrite(PWMA, 0);
  ledcWrite(PWMB, 0);
  cmdSpeedA = 0;
  cmdSpeedB = 0;
}

void setMotorA(int speedVal) {
  speedVal = constrain(speedVal, -255, 255);
  if (speedVal > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, speedVal);
  } else if (speedVal < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(PWMA, -speedVal);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, 0);
  }
  cmdSpeedA = speedVal;
}

void setMotorB(int speedVal) {
  speedVal = constrain(speedVal, -255, 255);
  if (speedVal > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, speedVal);
  } else if (speedVal < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWMB, -speedVal);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, 0);
  }
  cmdSpeedB = speedVal;
}

void setBoth(int a, int b) {
  setMotorA(a);
  setMotorB(b);
}

void beginMoveDistance(float meters, int speedVal) {
  startCountA = encoderCountA;
  startCountB = encoderCountB;
  targetCounts = metersToCounts(meters);
  motionMode = MOVE_DIST;
  if (meters >= 0) setBoth(speedVal, speedVal);
  else setBoth(-speedVal, -speedVal);
}

void beginTurnDegrees(float degrees, int speedVal) {
  startCountA = encoderCountA;
  startCountB = encoderCountB;
  targetCounts = degreesToTurnCounts(degrees);
  motionMode = TURN_ANGLE;
  if (degrees >= 0) setBoth(speedVal, -speedVal);
  else setBoth(-speedVal, speedVal);
}

void beginSquare(float sideMeters, int speedVal) {
  shapeSideM = sideMeters;
  shapeStep = 0;
  shapeRepeats = 0;
  defaultSpeed = speedVal;
  startCountA = encoderCountA;
  startCountB = encoderCountB;
  targetCounts = metersToCounts(shapeSideM);
  motionMode = SHAPE_SQUARE;
  setBoth(speedVal, speedVal);
}

void beginTriangle(float sideMeters, int speedVal) {
  shapeSideM = sideMeters;
  shapeStep = 0;
  shapeRepeats = 0;
  defaultSpeed = speedVal;
  startCountA = encoderCountA;
  startCountB = encoderCountB;
  targetCounts = metersToCounts(shapeSideM);
  motionMode = SHAPE_TRIANGLE;
  setBoth(speedVal, speedVal);
}

void beginCircle(float radiusMeters, int speedVal) {
  shapeRadiusM = radiusMeters;
  startCountA = encoderCountA;
  startCountB = encoderCountB;
  float outerDist = 2.0 * PI * radiusMeters;
  targetCounts = metersToCounts(outerDist);
  motionMode = SHAPE_CIRCLE;
  int inner = max(60, speedVal / 2);
  setBoth(speedVal, inner);
}

void beginLineFollow(int speedVal) {
  defaultSpeed = constrain(speedVal, 0, 255);
  lastLineState = 0;
  lineLostTime = 0;
  motionMode = LINE_FOLLOW;
}

void stopLineFollow() {
  motionMode = IDLE;
  stopMotors();
}

void lineFollowStep() {
  int leftSensor = digitalRead(IR_LEFT);
  int rightSensor = digitalRead(IR_RIGHT);

  // Assumed sensor logic:
  // LOW  = black line detected
  // HIGH = white surface
  // If your sensor logic is opposite, swap LOW and HIGH in the conditions below.

  if (leftSensor == LOW && rightSensor == LOW) {
    lineLostTime = 0;
    setBoth(defaultSpeed, defaultSpeed);
  }
  else if (leftSensor == LOW && rightSensor == HIGH) {
    lineLostTime = 0;
    lastLineState = -1;
    // left sensor on line -> turn left strongly
    setBoth(-defaultSpeed / 3, defaultSpeed);
  }
  else if (leftSensor == HIGH && rightSensor == LOW) {
    lineLostTime = 0;
    lastLineState = 1;
    // right sensor on line -> turn right strongly
    setBoth(defaultSpeed, -defaultSpeed / 3);
  }
  else {
    if (lineLostTime == 0) lineLostTime = millis();

    if (millis() - lineLostTime < lineRecoverMs) {
      if (lastLineState < 0) {
        setBoth(-defaultSpeed / 3, defaultSpeed);
      } else if (lastLineState > 0) {
        setBoth(defaultSpeed, -defaultSpeed / 3);
      } else {
        stopMotors();
      }
    } else {
      stopMotors();
    }
  }
}

long avgMovedCounts() {
  long a = labs(encoderCountA - startCountA);
  long b = labs(encoderCountB - startCountB);
  return (a + b) / 2;
}

String modeName() {
  switch (motionMode) {
    case IDLE: return "idle";
    case MOVE_DIST: return "move";
    case TURN_ANGLE: return "turn";
    case SHAPE_SQUARE: return "square";
    case SHAPE_TRIANGLE: return "triangle";
    case SHAPE_CIRCLE: return "circle";
    case MANUAL: return "manual";
    case LINE_FOLLOW: return "line_follow";
  }
  return "unknown";
}

void updateMotion() {
  if (motionMode == IDLE || motionMode == MANUAL || motionMode == LINE_FOLLOW) return;

  if (motionMode == MOVE_DIST || motionMode == TURN_ANGLE || motionMode == SHAPE_CIRCLE) {
    if (avgMovedCounts() >= targetCounts) {
      stopMotors();
      motionMode = IDLE;
    }
    return;
  }

  if (motionMode == SHAPE_SQUARE) {
    if (avgMovedCounts() >= targetCounts) {
      stopMotors();
      delay(120);
      if (shapeStep == 0) {
        shapeStep = 1;
        startCountA = encoderCountA;
        startCountB = encoderCountB;
        targetCounts = degreesToTurnCounts(90);
        setBoth(defaultSpeed, -defaultSpeed);
      } else {
        shapeStep = 0;
        shapeRepeats++;
        if (shapeRepeats >= 4) {
          stopMotors();
          motionMode = IDLE;
        } else {
          startCountA = encoderCountA;
          startCountB = encoderCountB;
          targetCounts = metersToCounts(shapeSideM);
          setBoth(defaultSpeed, defaultSpeed);
        }
      }
    }
    return;
  }

  if (motionMode == SHAPE_TRIANGLE) {
    if (avgMovedCounts() >= targetCounts) {
      stopMotors();
      delay(120);
      if (shapeStep == 0) {
        shapeStep = 1;
        startCountA = encoderCountA;
        startCountB = encoderCountB;
        targetCounts = degreesToTurnCounts(120);
        setBoth(defaultSpeed, -defaultSpeed);
      } else {
        shapeStep = 0;
        shapeRepeats++;
        if (shapeRepeats >= 3) {
          stopMotors();
          motionMode = IDLE;
        } else {
          startCountA = encoderCountA;
          startCountB = encoderCountB;
          targetCounts = metersToCounts(shapeSideM);
          setBoth(defaultSpeed, defaultSpeed);
        }
      }
    }
  }
}

void handleStatus() {
  String json = "{";
  json += "\"mode\":\"" + modeName() + "\",";
  json += "\"countA\":" + String(encoderCountA) + ",";
  json += "\"countB\":" + String(encoderCountB) + ",";
  json += "\"irLeft\":" + String(digitalRead(IR_LEFT)) + ",";
  json += "\"irRight\":" + String(digitalRead(IR_RIGHT)) + ",";
  json += "\"metersA\":" + String(encoderCountA * metersPerCount(), 4) + ",";
  json += "\"metersB\":" + String(encoderCountB * metersPerCount(), 4) + ",";
  json += "\"speedA\":" + String(cmdSpeedA) + ",";
  json += "\"speedB\":" + String(cmdSpeedB) + ",";
  json += "\"imuAxMs2\":" + String(imuAx_ms2, 3) + ",";
  json += "\"imuAyMs2\":" + String(imuAy_ms2, 3) + ",";
  json += "\"imuAzMs2\":" + String(imuAz_ms2, 3) + ",";
  json += "\"imuWx\":" + String(imuWx, 3) + ",";
  json += "\"imuWy\":" + String(imuWy, 3) + ",";
  json += "\"imuWz\":" + String(imuWz, 3) + ",";
  json += "\"imuRoll\":" + String(imuRoll, 3) + ",";
  json += "\"imuPitch\":" + String(imuPitch, 3) + ",";
  json += "\"imuYaw\":" + String(imuYaw, 3);
  json += "}";
  server.send(200, "application/json", json);
}

void handleStop() {
  motionMode = IDLE;
  stopMotors();
  server.send(200, "text/plain", "stopped");
}

void handleManual() {
  if (!server.hasArg("cmd")) {
    server.send(400, "text/plain", "missing cmd");
    return;
  }

  String cmd = server.arg("cmd");
  int speedVal = server.hasArg("speed") ? server.arg("speed").toInt() : defaultSpeed;
  speedVal = constrain(speedVal, 0, 255);
  motionMode = MANUAL;

  if (cmd == "forward") setBoth(speedVal, speedVal);
  else if (cmd == "backward") setBoth(-speedVal, -speedVal);
  else if (cmd == "left") setBoth(-speedVal, speedVal);
  else if (cmd == "right") setBoth(speedVal, -speedVal);
  else if (cmd == "forwardLeft") setBoth(speedVal / 2, speedVal);
  else if (cmd == "forwardRight") setBoth(speedVal, speedVal / 2);
  else if (cmd == "backwardLeft") setBoth(-(speedVal / 2), -speedVal);
  else if (cmd == "backwardRight") setBoth(-speedVal, -(speedVal / 2));
  else if (cmd == "stop") {
    motionMode = IDLE;
    stopMotors();
  } else {
    server.send(400, "text/plain", "bad cmd");
    return;
  }

  server.send(200, "text/plain", "manual ok");
}

void handleMove() {
  if (!server.hasArg("meters")) {
    server.send(400, "text/plain", "missing meters");
    return;
  }
  float meters = server.arg("meters").toFloat();
  int speedVal = server.hasArg("speed") ? server.arg("speed").toInt() : defaultSpeed;
  beginMoveDistance(meters, speedVal);
  server.send(200, "text/plain", "move started");
}

void handleTurn() {
  if (!server.hasArg("degrees")) {
    server.send(400, "text/plain", "missing degrees");
    return;
  }
  float degrees = server.arg("degrees").toFloat();
  int speedVal = server.hasArg("speed") ? server.arg("speed").toInt() : defaultSpeed;
  beginTurnDegrees(degrees, speedVal);
  server.send(200, "text/plain", "turn started");
}

void handleShape() {
  if (!server.hasArg("name")) {
    server.send(400, "text/plain", "missing name");
    return;
  }

  String name = server.arg("name");
  int speedVal = server.hasArg("speed") ? server.arg("speed").toInt() : defaultSpeed;

  if (name == "square") {
    float side = server.hasArg("size") ? server.arg("size").toFloat() : 0.5;
    beginSquare(side, speedVal);
  } else if (name == "triangle") {
    float side = server.hasArg("size") ? server.arg("size").toFloat() : 0.5;
    beginTriangle(side, speedVal);
  } else if (name == "circle") {
    float radius = server.hasArg("size") ? server.arg("size").toFloat() : 0.25;
    beginCircle(radius, speedVal);
  } else {
    server.send(400, "text/plain", "shape must be square triangle or circle");
    return;
  }

  server.send(200, "text/plain", "shape started");
}

void handleLineFollowStart() {
  int speedVal = server.hasArg("speed") ? server.arg("speed").toInt() : 110;
  beginLineFollow(speedVal);
  server.send(200, "text/plain", "line follow started");
}

void handleLineFollowStop() {
  stopLineFollow();
  server.send(200, "text/plain", "line follow stopped");
}

void handleConfig() {
  if (server.hasArg("wheelDiameterM")) wheelDiameterM = server.arg("wheelDiameterM").toFloat();
  if (server.hasArg("pulsesPerWheelRev")) pulsesPerWheelRev = server.arg("pulsesPerWheelRev").toFloat();
  if (server.hasArg("wheelBaseM")) wheelBaseM = server.arg("wheelBaseM").toFloat();
  if (server.hasArg("defaultSpeed")) defaultSpeed = server.arg("defaultSpeed").toInt();
  server.send(200, "text/plain", "config saved in RAM");
}

void handleResetEncoders() {
  encoderCountA = 0;
  encoderCountB = 0;
  server.send(200, "text/plain", "encoders reset");
}

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void setup() {
  Serial.begin(115200);
  IMU.begin(115200, SERIAL_8N1, IMU_RX, IMU_TX);
  delay(500);
  calibrateAccelerometer();

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  ledcAttachChannel(PWMA, PWM_FREQ, PWM_RES, CH_A);
  ledcAttachChannel(PWMB, PWM_FREQ, PWM_RES, CH_B);

  pinMode(ENCA_A, INPUT_PULLUP);
  pinMode(ENCA_B, INPUT_PULLUP);
  pinMode(ENCB_A, INPUT_PULLUP);
  pinMode(ENCB_B, INPUT_PULLUP);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCA_A), readEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB_A), readEncoderB, RISING);

  stopMotors();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("Dashboard IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/manual", handleManual);
  server.on("/move", handleMove);
  server.on("/turn", handleTurn);
  server.on("/shape", handleShape);
  server.on("/lineFollowStart", handleLineFollowStart);
  server.on("/lineFollowStop", handleLineFollowStop);
  server.on("/stop", handleStop);
  server.on("/config", handleConfig);
  server.on("/resetEncoders", handleResetEncoders);
  server.begin();
}

void loop() {
  readIMU();
  server.handleClient();

  if (motionMode == LINE_FOLLOW) {
    lineFollowStep();
  } else {
    updateMotion();
  }

  if (millis() - statusTimer > 1000) {
    statusTimer = millis();
    Serial.print("Mode: ");
    Serial.print(modeName());
    Serial.print(" A: ");
    Serial.print(encoderCountA);
    Serial.print(" B: ");
    Serial.print(encoderCountB);
    Serial.print(" IR_L: ");
    Serial.print(digitalRead(IR_LEFT));
    Serial.print(" IR_R: ");
    Serial.print(digitalRead(IR_RIGHT));
    Serial.print(" Ax: ");
    Serial.print(imuAx_ms2, 3);
    Serial.print(" Ay: ");
    Serial.print(imuAy_ms2, 3);
    Serial.print(" Az: ");
    Serial.println(imuAz_ms2, 3);
  }
}