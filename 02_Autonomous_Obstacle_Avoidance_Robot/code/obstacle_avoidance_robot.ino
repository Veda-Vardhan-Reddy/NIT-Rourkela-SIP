#include <Servo.h>

Servo myservo;

//================================================
// PINS
//================================================

// Ultrasonic
#define TRIG_PIN 6
#define ECHO_PIN 7

// L298N
#define ENA 3
#define ENB 5

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// MG90S
#define SERVO_PIN 4

//================================================
// SETTINGS
//================================================

const int SAFE_DISTANCE = 22;

const int FORWARD_SPEED = 130;
const int SLOW_SPEED = 90;
const int TURN_SPEED = 200;

// Servo positions
const int CENTER = 90;
const int LEFT = 25;
const int RIGHT = 155;

// Turn timings
// Tune these according to your robot
const int TURN_90_TIME = 750;
const int TURN_180_TIME = 1500;

// Reverse distance/time
const int REVERSE_TIME = 250;

//================================================
// LOOP DETECTION
//================================================

// Number of consecutive turns without
// successful forward movement
int turnCount = 0;

// Last turn:
// 'L' = left
// 'R' = right
// 'N' = none
char lastTurn = 'N';

//================================================
// ULTRASONIC
//================================================

long duration;

int getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // No echo = assume path is clear
  if (duration == 0)
  {
    return 400;
  }

  int distance = duration * 0.034 / 2;

  return distance;
}

//================================================
// STOP
//================================================

void stopRobot()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//================================================
// FORWARD
//================================================

void moveForward()
{
  analogWrite(ENA, FORWARD_SPEED);
  analogWrite(ENB, FORWARD_SPEED);

  // LEFT MOTORS FORWARD
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // RIGHT MOTORS FORWARD
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//================================================
// SLOW FORWARD
//================================================

void moveSlow()
{
  analogWrite(ENA, SLOW_SPEED);
  analogWrite(ENB, SLOW_SPEED);

  // LEFT MOTORS FORWARD
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // RIGHT MOTORS FORWARD
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//================================================
// BACKWARD
//================================================

void moveBackward()
{
  analogWrite(ENA, FORWARD_SPEED);
  analogWrite(ENB, FORWARD_SPEED);

  // LEFT MOTORS BACKWARD
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // RIGHT MOTORS BACKWARD
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//================================================
// TURN LEFT 90°
//================================================

void turnLeft90()
{
  Serial.println("TURNING LEFT 90");

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);

  // LEFT BACKWARD
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // RIGHT FORWARD
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(TURN_90_TIME);

  stopRobot();

  delay(100);
}

//================================================
// TURN RIGHT 90°
//================================================

void turnRight90()
{
  Serial.println("TURNING RIGHT 90");

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);

  // LEFT FORWARD
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // RIGHT BACKWARD
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(TURN_90_TIME);

  stopRobot();

  delay(100);
}

//================================================
// TURN 180°
//================================================

void turn180()
{
  Serial.println("************************");
  Serial.println("U-TURN");
  Serial.println("************************");

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);

  // LEFT FORWARD
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // RIGHT BACKWARD
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(TURN_180_TIME);

  stopRobot();

  delay(150);
}

//================================================
// SERVO CENTER
//================================================

void lookCenter()
{
  myservo.write(CENTER);

  delay(100);
}

//================================================
// LOOK LEFT
//================================================

int lookLeft()
{
  myservo.write(LEFT);

  delay(220);

  int distance = getDistance();

  Serial.print("LEFT : ");
  Serial.println(distance);

  return distance;
}

//================================================
// LOOK RIGHT
//================================================

int lookRight()
{
  myservo.write(RIGHT);

  delay(220);

  int distance = getDistance();

  Serial.print("RIGHT : ");
  Serial.println(distance);

  return distance;
}

//================================================
// RECORD LEFT TURN
//================================================

void recordLeftTurn()
{
  if (lastTurn == 'R')
  {
    turnCount++;
  }
  else
  {
    turnCount = 1;
  }

  lastTurn = 'L';

  Serial.print("Turn count = ");
  Serial.println(turnCount);
}

//================================================
// RECORD RIGHT TURN
//================================================

void recordRightTurn()
{
  if (lastTurn == 'L')
  {
    turnCount++;
  }
  else
  {
    turnCount = 1;
  }

  lastTurn = 'R';

  Serial.print("Turn count = ");
  Serial.println(turnCount);
}

//================================================
// RESET LOOP COUNTER
//================================================

void resetTurnMemory()
{
  turnCount = 0;
  lastTurn = 'N';

  Serial.println("FORWARD MOVEMENT SUCCESSFUL");
  Serial.println("Turn counter RESET");
}

//================================================
// OBSTACLE AVOIDANCE
//================================================

void avoidObstacle()
{
  Serial.println();
  Serial.println("========================");
  Serial.println("OBSTACLE DETECTED");
  Serial.println("========================");

  // STOP
  stopRobot();

  delay(100);

  // ---------------------------------------------
  // FIRST: CHECK IF WE ARE ALREADY IN A LOOP
  // ---------------------------------------------

  if (turnCount >= 3)
  {
    Serial.println("LOOP DETECTED!");
    Serial.println("MORE THAN 2 TURNS WITHOUT FORWARD MOVEMENT");

    // Move backward slightly
    moveBackward();
    delay(400);

    stopRobot();
    delay(100);

    // U-TURN
    turn180();

    // Reset memory
    turnCount = 0;
    lastTurn = 'N';

    lookCenter();

    return;
  }

  // ---------------------------------------------
  // REVERSE SLIGHTLY
  // ---------------------------------------------

  moveBackward();

  delay(REVERSE_TIME);

  stopRobot();

  delay(100);

  // ---------------------------------------------
  // SCAN LEFT
  // ---------------------------------------------

  int leftDistance = lookLeft();

  // ---------------------------------------------
  // CENTER
  // ---------------------------------------------

  lookCenter();

  // ---------------------------------------------
  // SCAN RIGHT
  // ---------------------------------------------

  int rightDistance = lookRight();

  // ---------------------------------------------
  // CENTER
  // ---------------------------------------------

  lookCenter();

  // ---------------------------------------------
  // PRINT RESULTS
  // ---------------------------------------------

  Serial.println("------------------------");

  Serial.print("LEFT   : ");
  Serial.println(leftDistance);

  Serial.print("RIGHT  : ");
  Serial.println(rightDistance);

  Serial.print("TURNS  : ");
  Serial.println(turnCount);

  Serial.println("------------------------");

  //================================================
  // BOTH SIDES BLOCKED
  //================================================

  if (leftDistance <= SAFE_DISTANCE &&
      rightDistance <= SAFE_DISTANCE)
  {
    Serial.println("BOTH SIDES BLOCKED");

    turnCount++;

    // If this is the third consecutive turn
    // without forward progress → U-TURN
    if (turnCount >= 3)
    {
      Serial.println("TOO MANY TURNS");
      Serial.println("PERFORMING U-TURN");

      moveBackward();
      delay(400);

      stopRobot();

      delay(100);

      turn180();

      turnCount = 0;
      lastTurn = 'N';

      return;
    }

    // Otherwise choose opposite direction
    // from the previous turn

    if (lastTurn == 'L')
    {
      Serial.println("PREVIOUS = LEFT");
      Serial.println("TURNING RIGHT");

      turnRight90();

      lastTurn = 'R';
    }
    else
    {
      Serial.println("PREVIOUS = RIGHT/NONE");
      Serial.println("TURNING LEFT");

      turnLeft90();

      lastTurn = 'L';
    }

    return;
  }

  //================================================
  // LEFT IS CLEARER
  //================================================

  if (leftDistance > rightDistance + 5)
  {
    Serial.println("LEFT IS CLEARER");

    recordLeftTurn();

    // Check loop BEFORE turning
    if (turnCount >= 3)
    {
      Serial.println("LOOP DETECTED");
      Serial.println("U-TURN");

      moveBackward();
      delay(400);

      turn180();

      turnCount = 0;
      lastTurn = 'N';

      return;
    }

    turnLeft90();

    return;
  }

  //================================================
  // RIGHT IS CLEARER
  //================================================

  if (rightDistance > leftDistance + 5)
  {
    Serial.println("RIGHT IS CLEARER");

    recordRightTurn();

    // Check loop BEFORE turning
    if (turnCount >= 3)
    {
      Serial.println("LOOP DETECTED");
      Serial.println("U-TURN");

      moveBackward();
      delay(400);

      turn180();

      turnCount = 0;
      lastTurn = 'N';

      return;
    }

    turnRight90();

    return;
  }

  //================================================
  // BOTH DIRECTIONS ALMOST SAME
  //================================================

  Serial.println("LEFT AND RIGHT ALMOST SAME");

  // Alternate based on previous direction
  if (lastTurn == 'L')
  {
    recordRightTurn();

    if (turnCount >= 3)
    {
      Serial.println("LOOP DETECTED");

      moveBackward();
      delay(400);

      turn180();

      turnCount = 0;
      lastTurn = 'N';

      return;
    }

    turnRight90();
  }
  else
  {
    recordLeftTurn();

    if (turnCount >= 3)
    {
      Serial.println("LOOP DETECTED");

      moveBackward();
      delay(400);

      turn180();

      turnCount = 0;
      lastTurn = 'N';

      return;
    }

    turnLeft90();
  }
}

//================================================
// SETUP
//================================================

void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  myservo.attach(SERVO_PIN);

  myservo.write(CENTER);

  stopRobot();

  delay(1000);

  Serial.println();
  Serial.println("========================");
  Serial.println("OBSTACLE AVOIDANCE BOT");
  Serial.println("========================");
}

//================================================
// MAIN LOOP
//================================================

void loop()
{
  int frontDistance = getDistance();

  Serial.print("FRONT : ");
  Serial.print(frontDistance);
  Serial.println(" cm");

  //================================================
  // OBSTACLE AT 22 CM OR LESS
  //================================================

  if (frontDistance <= SAFE_DISTANCE)
  {
    stopRobot();

    avoidObstacle();
  }

  //================================================
  // APPROACHING OBSTACLE
  //================================================

  else if (frontDistance <= 35)
  {
    moveSlow();
  }

  //================================================
  // CLEAR PATH
  //================================================

  else
  {
    moveForward();

    // IMPORTANT:
    // The robot has a clear forward path.
    // Therefore, it has successfully moved forward.
    // Reset the left-right loop counter.

    resetTurnMemory();
  }

  delay(30);
}
