# Autonomous Obstacle Avoidance Mobile Robot

An autonomous mobile robot developed during the Summer Internship Programme at NIT Rourkela. The robot is designed to navigate an indoor environment without direct human control by detecting obstacles using an HC-SR04 ultrasonic sensor and automatically changing its direction.

The robot uses an Arduino Mega 2560 as the main controller, an L298N dual H-bridge motor driver for controlling four BO motors, and an MG90S servo motor to rotate the ultrasonic sensor for left and right environment scanning.

The robot continuously monitors the distance in front of it. When an obstacle is detected within a predefined safety distance, the robot stops, reverses slightly, scans the left and right directions, selects a suitable direction, and performs an approximately 90° turn.

A turn-history mechanism is also implemented to detect repeated left-right turning. If the robot performs multiple turns without making successful forward progress, it performs a 180° U-turn to escape the loop.

---

## Project Overview

The project consists of the following major subsystems:

1. **Arduino Mega 2560** – Main controller responsible for sensing, decision-making, servo control, and motor control.
2. **HC-SR04 Ultrasonic Sensor** – Measures the distance between the robot and obstacles.
3. **MG90S Servo Motor** – Rotates the ultrasonic sensor to scan different directions.
4. **L298N Motor Driver** – Controls the four BO motors.
5. **Four BO Motors** – Provide movement to the four-wheel robot.
6. **Buck Converter** – Provides regulated 5V power for the control electronics.
7. **Battery Pack** – Provides power to the robot.

The robot operates autonomously without a remote controller.

---

## System Architecture

```text
                         AUTONOMOUS ROBOT
                               
                    ┌─────────────────────┐
                    │      HC-SR04        │
                    │ Ultrasonic Sensor   │
                    └──────────┬──────────┘
                               │
                         Distance Data
                               │
                               ▼
                    ┌─────────────────────┐
                    │    Arduino Mega     │
                    │       2560          │
                    │                     │
                    │  Sensor Processing  │
                    │  Decision Making    │
                    │  Servo Control      │
                    │  Motor Control      │
                    └───────┬───────┬─────┘
                            │       │
                       Servo Signal Motor
                            │       │
                            ▼       ▼
                    ┌──────────┐ ┌──────────────┐
                    │  MG90S   │ │    L298N     │
                    │  Servo   │ │ Motor Driver │
                    └──────────┘ └───────┬──────┘
                                        │
                           ┌────────────┴────────────┐
                           │                         │
                           ▼                         ▼
                    Left-side BO Motors      Right-side BO Motors
                           │                         │
                           └────────────┬────────────┘
                                        │
                                        ▼
                                  Four-Wheel Robot

```
## Working Principle

The robot works as an autonomous obstacle-avoidance system.

The overall working sequence is:

1. The Arduino Mega initializes the ultrasonic sensor, servo motor, and L298N motor driver.
2. The HC-SR04 continuously measures the distance of obstacles in front of the robot.
3. If the path is clear, the robot moves forward at normal speed.
4. When an obstacle comes within 35 cm, the robot reduces its speed.
5. When an obstacle comes within the safety distance of 22 cm, the robot stops.
6. The robot moves backward slightly to create additional turning space.
7. The MG90S servo rotates the HC-SR04 toward the left.
8. The left-side distance is measured.
9. The servo rotates toward the right.
10. The right-side distance is measured.
11. The Arduino compares the left and right distances.
12. The robot selects the clearer direction.
13. The robot performs an approximately 90° turn.
14. The robot continues moving forward.
15. The program keeps track of consecutive left/right turns.
16. If the robot repeatedly changes direction without making forward progress, a loop is detected.
17. When a loop is detected, the robot performs an approximately 180° U-turn.
18. The turn history is then reset and the robot continues forward.

### Basic Flow

```text
              START
                │
                ▼
       Initialize Components
                │
                ▼
       Measure Front Distance
                │
                ▼
        Is Distance ≤ 22 cm?
           /            \
         YES             NO
          │               │
          ▼               ▼
        STOP        Is Distance ≤ 35 cm?
          │            /          \
          │          YES           NO
          │           │             │
          │           ▼             ▼
          │      Slow Forward   Normal Forward
          │
          ▼
     Reverse Slightly
          │
          ▼
      Scan LEFT
          │
          ▼
      Scan RIGHT
          │
          ▼
    Compare Distances
          │
     ┌────┴────┐
     ▼         ▼
   LEFT       RIGHT
  CLEARER     CLEARER
     │         │
     ▼         ▼
 Turn Left   Turn Right
     │         │
     └────┬────┘
          │
          ▼
    Check Turn History
          │
     ┌────┴────┐
     ▼         ▼
    Normal    Loop Detected
      │           │
      │           ▼
      │        U-Turn
      │           │
      └─────┬─────┘
            ▼
       Move Forward
```

## Features

* Fully autonomous obstacle avoidance
* Four-wheel mobile robot
* Arduino Mega 2560 based control
* HC-SR04 ultrasonic obstacle detection
* MG90S servo-based sensor scanning
* Left and right environment scanning
* Automatic direction selection
* Approximately 90° left/right turning
* 180° U-turn capability
* PWM-based motor speed control
* Slow-down zone before obstacles
* Turn-history based loop detection
* Automatic recovery from repeated left-right turns
* Battery-powered operation
* Serial Monitor debugging
* Reactive navigation without a remote controller

---

## Hardware Components

| Component                        |    Quantity | Purpose                            |
| -------------------------------- | ----------: | ---------------------------------- |
| Arduino Mega 2560                |           1 | Main controller                    |
| L298N Dual H-Bridge Motor Driver |           1 | Controls the DC motors             |
| BO Motors                        |           4 | Robot movement                     |
| BO Wheels                        |           4 | Mechanical movement                |
| HC-SR04 Ultrasonic Sensor        |           1 | Obstacle distance measurement      |
| MG90S Servo Motor                |           1 | Rotates the ultrasonic sensor      |
| 3.7V Li-ion Batteries            |           2 | Main power source                  |
| Buck Converter                   |           1 | Provides regulated 5V              |
| Breadboard                       |           1 | Power distribution and connections |
| Jumper Wires                     | As required | Electrical connections             |
| Robot Chassis                    |           1 | Mechanical frame                   |
| Screws/Nuts/Spacers              | As required | Mechanical assembly                |

---

## Robot Dimensions

The robot chassis is approximately:

```text
Length ≈ 20 cm
Width  ≈ 15 cm
```

The final dimensions are slightly larger after installing the wheels, motors, sensor, and other components.

The compact chassis is suitable for testing in small indoor environments.

---

## System Overview

The robot follows a reactive obstacle avoidance approach.

The HC-SR04 continuously measures the distance in front of the robot. During normal movement, the ultrasonic sensor remains approximately centered. When an obstacle is detected within the predefined safety distance, the robot stops, reverses slightly, rotates the servo-mounted ultrasonic sensor to scan the left and right directions, compares the measured distances, and selects the clearer direction.

The robot also maintains a simple turn history. If it repeatedly turns left and right without successfully moving forward, the system assumes that it may be trapped in a navigation loop and performs a 180° U-turn to recover.

```text
                    ┌─────────────────┐
                    │    HC-SR04      │
                    │ Ultrasonic Sensor│
                    └────────┬────────┘
                             │
                             │ Distance
                             ▼
                    ┌─────────────────┐
                    │  Arduino Mega    │
                    │      2560        │
                    │                  │
                    │ Sensor Reading   │
                    │ Decision Making  │
                    │ Servo Control    │
                    │ Motor Control    │
                    │ Loop Detection   │
                    └───────┬─────────┘
                            │
                 ┌──────────┴──────────┐
                 │                     │
                 ▼                     ▼
          ┌─────────────┐       ┌─────────────┐
          │    MG90S    │       │    L298N    │
          │    Servo    │       │ Motor Driver│
          └─────────────┘       └──────┬──────┘
                                       │
                              ┌────────┴────────┐
                              │                 │
                              ▼                 ▼
                       Left BO Motors    Right BO Motors
                              │                 │
                              └────────┬────────┘
                                       │
                                       ▼
                              Four-Wheel Robot
```

---

# Controller

## Arduino Mega 2560

The Arduino Mega 2560 acts as the main controller of the robot.

It performs the following tasks:

* Reads the HC-SR04 ultrasonic sensor
* Calculates obstacle distance
* Controls the MG90S servo
* Scans the left and right directions
* Controls the L298N motor driver
* Controls motor speed using PWM
* Detects obstacles
* Selects a movement direction
* Performs 90° turns
* Performs 180° U-turns
* Tracks recent turning behavior
* Detects possible navigation loops
* Sends debugging information through the Serial Monitor

---

# Motor Driver

## L298N Dual H-Bridge Motor Driver

The Arduino Mega cannot directly supply the current required by the four BO motors. Therefore, an L298N motor driver is used between the Arduino and the motors.

```text
Arduino Mega
     │
     │ Control Signals
     ▼
   L298N
     │
     ├──────────────► Left BO Motors
     │
     └──────────────► Right BO Motors
```

The L298N provides:

* Motor direction control
* Motor speed control
* Separate control for the left and right motor groups

The ENA and ENB pins are connected to Arduino PWM pins for speed control.

---

# Motor Connections

The current software configuration uses the following pins:

| L298N Pin | Arduino Mega Pin | Function              |
| --------- | ---------------- | --------------------- |
| ENA       | D3               | Left motor PWM        |
| IN1       | D8               | Left motor direction  |
| IN2       | D9               | Left motor direction  |
| IN3       | D10              | Right motor direction |
| IN4       | D11              | Right motor direction |
| ENB       | D5               | Right motor PWM       |

## Left Motor Direction

### Forward

```text
IN1 = HIGH
IN2 = LOW
```

### Backward

```text
IN1 = LOW
IN2 = HIGH
```

## Right Motor Direction

### Forward

```text
IN3 = LOW
IN4 = HIGH
```

### Backward

```text
IN3 = HIGH
IN4 = LOW
```

The opposite logic on the right side is due to the physical orientation of the motors.

---

# HC-SR04 Ultrasonic Sensor

The HC-SR04 is used as the primary obstacle detection sensor.

## Connections

| HC-SR04 Pin | Arduino Mega |
| ----------- | ------------ |
| VCC         | 5V           |
| GND         | GND          |
| TRIG        | D6           |
| ECHO        | D7           |

## Working Principle

The Arduino sends a short trigger pulse to the HC-SR04.

```text
Arduino
   │
   │ TRIG pulse
   ▼
HC-SR04
   │
   │ Ultrasonic wave
   ▼
Obstacle
   │
   │ Reflected wave
   ▼
HC-SR04
   │
   │ ECHO pulse
   ▼
Arduino
```

The Arduino measures the duration of the echo signal and calculates the approximate distance.

The calculation used is:

```text
Distance = Time × 0.034 / 2
```

The division by 2 is required because the ultrasonic wave travels from the sensor to the obstacle and then back to the sensor.

## No Echo Condition

If no echo is received within the timeout period, the program assumes that there is no nearby obstacle and returns:

```text
400 cm
```

---

# MG90S Servo Motor

The MG90S servo rotates the HC-SR04 sensor so that the robot can scan different directions using a single ultrasonic sensor.

## Servo Connection

| MG90S Wire/Signal | Arduino Mega |
| ----------------- | ------------ |
| Signal            | D4           |
| VCC               | 5V           |
| GND               | GND          |

## Servo Positions

The current program uses approximately:

```text
LEFT   = 25°
CENTER = 90°
RIGHT  = 155°
```

The scanning arrangement is:

```text
             LEFT
              25°
               \
                \
                 ● 90°
                /
               /
           RIGHT
             155°
```

The sensor is normally kept at the center position while moving forward.

When an obstacle is detected, the servo moves to the left and right positions to measure the available space.

---

# Pin Configuration

## Complete Pin Configuration

```text
HC-SR04
----------------
TRIG → D6
ECHO → D7

MG90S
----------------
Signal → D4

L298N
----------------
ENA → D3
ENB → D5

IN1 → D8
IN2 → D9
IN3 → D10
IN4 → D11
```

## Complete Pin Table

| Device  | Signal | Arduino Mega |
| ------- | ------ | ------------ |
| HC-SR04 | TRIG   | D6           |
| HC-SR04 | ECHO   | D7           |
| MG90S   | Signal | D4           |
| L298N   | ENA    | D3           |
| L298N   | IN1    | D8           |
| L298N   | IN2    | D9           |
| L298N   | IN3    | D10          |
| L298N   | IN4    | D11          |
| L298N   | ENB    | D5           |

---

# Power Supply

The robot uses two 3.7V Li-ion batteries as the main power source.

The power system is divided between motor power and regulated electronics power.

```text
                2 × 3.7V Batteries
                       │
                 Battery Pack
                       │
             ┌─────────┴─────────┐
             │                   │
             ▼                   ▼
          L298N             Buck Converter
       Motor Supply              │
             │                   │
             ▼                   ▼
          BO Motors              5V
                                 │
                    ┌────────────┼────────────┐
                    ▼            ▼            ▼
                  Mega          MG90S       HC-SR04
```

## Buck Converter

The buck converter is adjusted to approximately:

```text
5.0V
```

A multimeter should be used to verify the output voltage before connecting the electronics.

## Common Ground

All components must share a common ground:

```text
Battery GND
    │
    ├── L298N GND
    ├── Buck Converter GND
    ├── Arduino Mega GND
    ├── MG90S GND
    └── HC-SR04 GND
```

A common ground is necessary for reliable communication between the controller, sensors, servo, and motor driver.

---

# Motor Speed Control

PWM is used to control the speed of the motors.

The current software settings are:

```text
FORWARD_SPEED = 130
SLOW_SPEED    = 90
TURN_SPEED    = 200
```

These values are passed to the L298N enable pins using `analogWrite()`.

## Normal Speed

When the path is clear:

```text
PWM = 130
```

## Slow Speed

When approaching an obstacle:

```text
PWM = 90
```

## Turning Speed

During a turn:

```text
PWM = 200
```

The actual robot speed depends on battery voltage, motor characteristics, wheel size, floor surface, and robot weight.

---

# Obstacle Detection Threshold

The main safety distance is:

```text
SAFE_DISTANCE = 22 cm
```

When:

```text
Front Distance ≤ 22 cm
```

the robot considers the path blocked and starts the obstacle avoidance procedure.

---

# Slow-Down Zone

The robot begins slowing down before reaching the main safety distance.

## Clear Path

```text
Distance > 35 cm
```

The robot moves at:

```text
PWM = 130
```

## Approaching Obstacle

```text
22 cm < Distance ≤ 35 cm
```

The robot moves at:

```text
PWM = 90
```

## Obstacle Detected

```text
Distance ≤ 22 cm
```

The robot stops and begins obstacle avoidance.

The behavior can therefore be summarized as:

```text
Distance > 35 cm
        │
        ▼
Normal Speed
PWM = 130

35 cm → 22 cm
        │
        ▼
Slow Speed
PWM = 90

≤ 22 cm
        │
        ▼
STOP
        │
        ▼
Obstacle Avoidance
```

---

# Obstacle Avoidance Algorithm

When an obstacle is detected within 22 cm, the robot performs the following sequence:

```text
Obstacle detected
       │
       ▼
     STOP
       │
       ▼
Reverse slightly
       │
       ▼
Scan LEFT
       │
       ▼
Scan RIGHT
       │
       ▼
Compare distances
       │
       ▼
Select direction
       │
       ▼
Perform approximately 90° turn
       │
       ▼
Move forward
```

The robot therefore does not simply turn in a fixed direction. It first checks the available space on both sides.

---

# Reverse Operation

Before scanning and turning, the robot reverses slightly to create additional space.

The normal reverse time is:

```text
REVERSE_TIME = 250 ms
```

For loop recovery, a longer reverse operation is used:

```text
400 ms
```

The exact distance moved during the reverse depends on motor speed, battery voltage, and floor conditions.

---

# Left and Right Scanning

When an obstacle is detected, the robot scans both sides.

## Step 1 — Scan Left

The servo moves to:

```text
25°
```

The robot measures the distance.

## Step 2 — Return to Center

The servo returns to:

```text
90°
```

## Step 3 — Scan Right

The servo moves to:

```text
155°
```

The robot measures the distance.

## Step 4 — Return to Center

The servo returns to:

```text
90°
```

The measured values are then compared.

---

# Direction Selection

The robot compares the left and right distances.

## Left is Significantly Clearer

If:

```text
leftDistance > rightDistance + 5
```

the robot turns left.

## Right is Significantly Clearer

If:

```text
rightDistance > leftDistance + 5
```

the robot turns right.

## Both Directions Are Almost Equal

If the two distances are nearly equal, the robot uses its previous turn direction to alternate the next turn.

This prevents the robot from repeatedly choosing the same direction when both sides appear similar.

---

# 90° Left Turn

For a left turn:

```text
Left motors  → Backward
Right motors → Forward
```

The robot performs a pivot-style turn.

The current timing is:

```text
TURN_90_TIME = 750 ms
```

The actual angle is dependent on the robot and environment.

---

# 90° Right Turn

For a right turn:

```text
Left motors  → Forward
Right motors → Backward
```

The robot performs a pivot-style turn.

The current timing is:

```text
TURN_90_TIME = 750 ms
```

This value should be experimentally calibrated for the actual robot.

---

# Turn Calibration

The value:

```text
750 ms
```

does not mathematically guarantee exactly 90°.

The actual turning angle depends on:

* Motor speed
* Battery voltage
* Wheel diameter
* Wheel grip
* Floor surface
* Robot weight
* Motor-to-motor differences

Therefore, the turn time should be experimentally calibrated.

---

# Loop Detection

A simple left-right navigation strategy can cause problems in a square room.

For example:

```text
Obstacle
   ↓
Turn LEFT
   ↓
Obstacle
   ↓
Turn RIGHT
   ↓
Obstacle
   ↓
Turn LEFT
   ↓
Obstacle
   ↓
Turn RIGHT
   ↓
...
```

The robot may continuously alternate directions without making useful forward progress.

To address this problem, the program stores the recent turn direction.

Two variables are used:

```text
int turnCount;
char lastTurn;
```

The `lastTurn` variable stores:

```text
'L' → Last turn was LEFT
'R' → Last turn was RIGHT
'N' → No previous turn
```

---

# Turn Counter

The robot increases the turn count when the direction alternates.

For example:

```text
LEFT
RIGHT
LEFT
```

results in repeated turn counting.

When the robot successfully moves forward on a clear path, the turn history is reset.

```text
FORWARD MOVEMENT SUCCESSFUL
        ↓
Turn counter RESET
```

---

# U-Turn Recovery

If the robot detects too many consecutive turns without successful forward movement, it assumes that it may be trapped in a loop.

The threshold is:

```text
turnCount >= 3
```

The recovery sequence is:

```text
Repeated turning detected
          │
          ▼
      LOOP DETECTED
          │
          ▼
   Reverse approximately
       400 ms
          │
          ▼
       STOP
          │
          ▼
      180° U-TURN
          │
          ▼
   Reset turn memory
          │
          ▼
     Move forward
```

---

# 180° U-Turn

The U-turn is performed by rotating the two sides of the robot in opposite directions.

```text
Left motors  → Forward
Right motors → Backward
```

The current timing is:

```text
TURN_180_TIME = 1500 ms
```

Like the 90° turn, this value is time-based and must be calibrated experimentally.

---

# Main Algorithm

The complete navigation logic is:

```text
START
  │
  ▼
Read front distance
  │
  ├─────────────── Distance > 35 cm ──────────────┐
  │                                                │
  │                                                ▼
  │                                          Move Forward
  │                                          PWM = 130
  │                                                │
  │                                                ▼
  │                                       Reset turn memory
  │
  ├──────────── 22–35 cm ──────────────────────────┐
  │                                                │
  │                                                ▼
  │                                           Move Slowly
  │                                            PWM = 90
  │
  └────────────── Distance ≤ 22 cm ────────────────┐
                                                   │
                                                   ▼
                                                 STOP
                                                   │
                                                   ▼
                                           Check loop counter
                                                   │
                              ┌────────────────────┴────────────────────┐
                              │                                         │
                       turnCount < 3                              turnCount ≥ 3
                              │                                         │
                              ▼                                         ▼
                      Reverse slightly                             LOOP DETECTED
                              │                                         │
                              ▼                                         ▼
                        Scan LEFT                                  Reverse
                              │                                         │
                              ▼                                         ▼
                       Scan RIGHT                                  180° U-Turn
                              │                                         │
                              ▼                                         ▼
                      Compare distances                          Reset memory
                              │                                         │
                    ┌─────────┴─────────┐                               │
                    │                   │                               │
               Left clearer        Right clearer                        │
                    │                   │                               │
                    ▼                   ▼                               │
               Turn LEFT           Turn RIGHT                           │
                    │                   │                               │
                    └─────────┬─────────┘                               │
                              │                                         │
                              └─────────────────────────────────────────┘
                                                │
                                                ▼
                                          Move Forward
```

---

# Software

The project was programmed using:

* **Programming Language:** C/C++
* **Platform:** Arduino Mega 2560
* **IDE:** Arduino IDE
* **Servo Library:** `Servo.h`
* **Sensor Interface:** Direct `digitalWrite()` and `pulseIn()`
* **Motor Control:** PWM using `analogWrite()`

---

# Software Library

The main external library used is:

```cpp
#include <Servo.h>
```

The HC-SR04 is controlled directly without requiring an external ultrasonic sensor library.

---

# Code Structure

The main program is divided into several functions:

```text
getDistance()
       │
       ├── Measures ultrasonic distance
       │
       ▼
stopRobot()
       │
       ├── Stops all motors
       │
       ▼
moveForward()
       │
       ├── Moves robot forward
       │
       ▼
moveSlow()
       │
       ├── Moves robot slowly
       │
       ▼
moveBackward()
       │
       ├── Moves robot backward
       │
       ▼
turnLeft90()
       │
       ├── Performs left turn
       │
       ▼
turnRight90()
       │
       ├── Performs right turn
       │
       ▼
turn180()
       │
       ├── Performs U-turn
       │
       ▼
lookLeft()
       │
       ├── Scans left
       │
       ▼
lookRight()
       │
       ├── Scans right
       │
       ▼
avoidObstacle()
       │
       └── Main obstacle avoidance logic
```

---

# Important Program Parameters

The main adjustable parameters are:

```cpp
const int SAFE_DISTANCE = 22;

const int FORWARD_SPEED = 130;
const int SLOW_SPEED = 90;
const int TURN_SPEED = 200;

const int CENTER = 90;
const int LEFT = 25;
const int RIGHT = 155;

const int TURN_90_TIME = 750;
const int TURN_180_TIME = 1500;

const int REVERSE_TIME = 250;
```

These values can be adjusted according to the physical behavior of the robot.

---

# Serial Monitor

The program uses the Arduino Serial Monitor for debugging.

The communication speed is:

```text
9600 baud
```

Example output:

```text
========================
OBSTACLE AVOIDANCE BOT
========================

FRONT : 45 cm
FRONT : 38 cm
FRONT : 31 cm
FRONT : 24 cm
FRONT : 21 cm

========================
OBSTACLE DETECTED
========================

LEFT : 48
RIGHT : 27

LEFT IS CLEARER
TURNING LEFT 90
```

If repeated turning is detected:

```text
LOOP DETECTED!
MORE THAN 2 TURNS WITHOUT FORWARD MOVEMENT
U-TURN
```

Serial output is useful for understanding the robot's decision-making process during testing.

---

# Complete Connection Table

## Arduino Mega to L298N

| Arduino Mega | L298N |
| ------------ | ----- |
| D3           | ENA   |
| D5           | ENB   |
| D8           | IN1   |
| D9           | IN2   |
| D10          | IN3   |
| D11          | IN4   |
| GND          | GND   |

## Arduino Mega to HC-SR04

| Arduino Mega | HC-SR04 |
| ------------ | ------- |
| D6           | TRIG    |
| D7           | ECHO    |
| 5V           | VCC     |
| GND          | GND     |

## Arduino Mega to MG90S

| Arduino Mega | MG90S  |
| ------------ | ------ |
| D4           | Signal |
| 5V           | VCC    |
| GND          | GND    |

---

# Complete Hardware Architecture

```text
                         ┌────────────────┐
                         │    HC-SR04     │
                         │  Ultrasonic    │
                         └───────┬────────┘
                                 │
                              D6/D7
                                 │
                                 ▼
                    ┌────────────────────────┐
                    │     Arduino Mega       │
                    │         2560           │
                    │                        │
                    │  Distance Measurement  │
                    │  Decision Making       │
                    │  Servo Control         │
                    │  Motor Control         │
                    │  Loop Detection        │
                    └───────┬─────────┬──────┘
                            │         │
                           D4    D3/D5/D8-D11
                            │         │
                            ▼         ▼
                       ┌────────┐ ┌────────────┐
                       │ MG90S  │ │   L298N    │
                       │ Servo  │ │   Driver   │
                       └────────┘ └──────┬─────┘
                                         │
                              ┌──────────┴──────────┐
                              │                     │
                              ▼                     ▼
                       Left BO Motors       Right BO Motors
                              │                     │
                              └──────────┬──────────┘
                                         │
                                         ▼
                                  Four-Wheel Robot
```

---

# Power Architecture

```text
                    2 × 3.7V Li-ion Cells
                              │
                              ▼
                        Battery Pack
                              │
                   ┌──────────┴──────────┐
                   │                     │
                   ▼                     ▼
                L298N               Buck Converter
            Motor Supply                  │
                   │                      │
                   ▼                      ▼
                Motors                    5V
                                          │
                             ┌────────────┼────────────┐
                             │            │            │
                             ▼            ▼            ▼
                           Mega         MG90S        HC-SR04
```

---

# How to Run

## Step 1 — Hardware Assembly

Connect the components according to the connection tables provided in this README.

Ensure that:

* Motor connections are secure.
* Ultrasonic TRIG and ECHO connections are correct.
* Servo signal is connected to D4.
* L298N control pins are connected correctly.
* All grounds are connected together.
* Buck converter output is verified using a multimeter.

## Step 2 — Upload the Program

The Arduino program is available at:

```text
code/obstacle_avoidance_robot.ino
```

Open the file in Arduino IDE.

## Step 3 — Select Board

Select:

```text
Arduino Mega or Mega 2560
```

and choose the appropriate COM/USB port.

## Step 4 — Upload

Upload the program to the Arduino Mega 2560.

## Step 5 — Open Serial Monitor

Open the Serial Monitor and select:

```text
9600 baud
```

## Step 6 — Power the Robot

Power the robot and place it in an open indoor test area.

## Step 7 — Test

The robot should:

1. Move forward.
2. Detect approaching obstacles.
3. Slow down near obstacles.
4. Stop when an obstacle is within 22 cm.
5. Reverse slightly.
6. Scan left.
7. Scan right.
8. Select the clearer direction.
9. Turn approximately 90°.
10. Continue moving forward.
11. Detect repeated turning.
12. Perform a U-turn when necessary.

---

# Testing and Calibration

Several parameters may require physical calibration.

## Ultrasonic Sensor

Verify that the measured distances approximately match actual distances.

## Servo Angles

Adjust:

```text
LEFT = 25°
CENTER = 90°
RIGHT = 155°
```

if the sensor does not point in the desired directions.

## Forward Speed

Adjust:

```text
FORWARD_SPEED = 130
```

according to the desired movement speed.

## Slow Speed

Adjust:

```text
SLOW_SPEED = 90
```

if the robot approaches obstacles too quickly or slowly.

## Turn Speed

Adjust:

```text
TURN_SPEED = 200
```

according to the robot's turning behavior.

## 90° Turn

Tune:

```text
TURN_90_TIME = 750 ms
```

until the robot approximately turns 90°.

## U-Turn

Tune:

```text
TURN_180_TIME = 1500 ms
```

until the robot approximately turns 180°.

---

# Project Limitations

## Single Ultrasonic Sensor

Only one HC-SR04 is used.

Therefore, the robot cannot continuously measure front, left, and right distances simultaneously.

The servo must rotate the sensor to scan different directions.

## Servo Scanning Delay

The sensor requires time to move between positions.

The sequence:

```text
CENTER → LEFT → CENTER → RIGHT → CENTER
```

introduces a delay in obstacle avoidance.

## Time-Based Turning

The robot does not directly measure its rotation angle.

The program assumes that:

```text
750 ms ≈ 90°
```

and:

```text
1500 ms ≈ 180°
```

These values are experimental and environment-dependent.

## No Wheel Encoders

The current robot does not use wheel encoder feedback for precise distance or rotation measurement.

## No IMU

The WT901BLE IMU is not used in this project.

Therefore, the robot does not use orientation feedback to determine its exact heading.

## Reactive Navigation

The robot uses local sensor information to make immediate movement decisions.

It does not build a map of the environment or perform global path planning.

---

# What This Project Demonstrates

This project provided hands-on experience with several robotics concepts.

## Embedded Systems

The Arduino Mega integrates the sensor, servo, and motor control system.

## Sensor Interfacing

The HC-SR04 provides distance measurements using ultrasonic waves.

## Actuator Control

The MG90S servo and BO motors are controlled electronically.

## Motor Driver Integration

The L298N provides the required motor-driving interface.

## PWM Motor Control

PWM is used to control motor speed.

## Autonomous Navigation

The robot makes movement decisions without direct human control.

## Obstacle Avoidance

The robot detects obstacles and changes its direction.

## Reactive Path Planning

The robot chooses its next movement using current sensor measurements.

## State-Based Behavior

The robot maintains turn history to identify repeated turning patterns.

## Loop Recovery

A U-turn mechanism allows the robot to recover from repeated left-right navigation patterns.

---

# Future Improvements

Possible improvements include:

* Adding multiple ultrasonic sensors
* Adding LiDAR for 360° environment sensing
* Adding wheel encoders
* Adding an IMU for orientation feedback
* Replacing time-based turning with encoder/IMU-based turning
* Implementing PID motor control
* Adding battery-level monitoring
* Improving obstacle detection accuracy
* Implementing mapping
* Implementing SLAM
* Adding global path planning
* Implementing A* or other path-planning algorithms
* Adding autonomous waypoint navigation
* Using ROS 2 for advanced robot control
* Adding a web or mobile monitoring interface

---

# Project Structure

```text
02_Autonomous_Obstacle_Avoidance_Robot/
│
├── README.md
│
├── code/
│   └── obstacle_avoidance_robot.ino
│
└── media/
    ├── obstacle_avoidance_robot.jpg
    └── obstacle_avoidance_demo.mp4
```

---

# Media

## Robot

Add the robot image here:

```text
media/obstacle_avoidance_robot.jpg
```

Example Markdown:

```markdown
![Autonomous Obstacle Avoidance Robot](media/obstacle_avoidance_robot.jpg)
```

## Demonstration Video

Add the demonstration video to the `media/` folder or upload it to a suitable video platform and place the link here.

```text
Obstacle Avoidance Robot Demonstration
```

---

# Project Outcome

A functional autonomous four-wheel obstacle avoidance robot was successfully developed.

The robot is capable of:

* Moving autonomously
* Detecting obstacles using ultrasonic sensing
* Slowing down when approaching obstacles
* Stopping before obstacles
* Scanning left and right using a servo-mounted ultrasonic sensor
* Selecting a suitable direction
* Performing approximately 90° turns
* Detecting repeated turning patterns
* Performing a 180° U-turn when a navigation loop is detected

The project provided practical experience in autonomous robotics, embedded programming, sensor interfacing, motor control, servo control, power management, and reactive navigation.

---

# Internship

This project was developed as part of the **Summer Internship Programme (SIP) at the National Institute of Technology Rourkela (NIT Rourkela)**.

The project provided hands-on experience in designing, assembling, programming, testing, and debugging a small autonomous mobile robot.

---

# Conclusion

The Autonomous Obstacle Avoidance Robot demonstrates how an embedded controller, ultrasonic sensing, servo-based environmental scanning, motor control, and simple navigation logic can be combined to create a completely autonomous mobile robot.

The system uses a reactive navigation strategy rather than predefined paths. It continuously observes its surroundings, slows down when approaching obstacles, scans alternative directions when blocked, and dynamically selects its next movement.

The addition of turn-history-based loop detection improves the robot's behavior in environments where simple left-right obstacle avoidance could otherwise result in repeated navigation cycles. When such a pattern is detected, the robot performs a 180° U-turn and attempts to continue navigation from a different direction.

Overall, the project demonstrates the complete development cycle of a small autonomous robot, including hardware integration, electrical connections, embedded programming, sensor interfacing, actuator control, power management, debugging, calibration, and autonomous navigation.

## Dynamic Environment Demonstration

Two identical autonomous obstacle-avoidance robots were developed and deployed together in a custom-built **circular dynamic environment** for testing.

The environment was designed with obstacles of different shapes and positions to evaluate how the robots respond to changing surroundings.

### Environment Setup

The test environment consists of:

- A circular operating area
- Multiple obstacles with different shapes
- Obstacles placed at different positions
- Two autonomous obstacle-avoidance robots
- Dynamic movement and changing obstacle conditions

### Testing

Both robots were placed inside the circular environment and operated autonomously.

Each robot:

1. Moves forward without manual control.
2. Continuously detects obstacles using the HC-SR04 ultrasonic sensor.
3. Slows down when approaching an obstacle.
4. Stops when an obstacle reaches the safety distance.
5. Scans the surrounding area using the servo-mounted ultrasonic sensor.
6. Compares the available space on the left and right.
7. Selects a suitable direction.
8. Performs an approximately 90° turn.
9. Continues navigating through the environment.
10. Uses the turn-history mechanism to reduce repeated left-right turning.
11. Performs a 180° U-turn when a loop condition is detected.

### Dynamic Environment Test

The purpose of this demonstration was to evaluate the robots under a more challenging environment rather than testing them only with a single static obstacle.

The different obstacle shapes and positions required the robots to repeatedly detect, evaluate, and react to their surroundings.

The deployment of two robots also demonstrated that the obstacle-avoidance algorithm could operate independently on multiple robots in the same environment.

### Demonstration Video

The complete demonstration of the two autonomous robots operating in the dynamic environment is available below:

[Watch the Dynamic Environment Demonstration](media/Dynamic_Env.mp4)

### Test Environment

```text
              Circular Dynamic Environment

                 ┌─────────────────┐
             ┌───┘                 └───┐
           /                             \
          /       ▲          █            \
         /                                     \
        |          🤖 Robot 1                  |
        |                                      |
        |              ●                       |
        |                         ◆            |
        |                                      |
        |     █          🤖 Robot 2            |
         \                                    /
          \          ▲                       /
           \                               /
             └───────────────────────────┘

        ▲  █  ●  ◆
        Different obstacle shapes
```
