# ESP32 Two-Wheel Mobile Robot

This folder contains the working implementation of the two-wheel differential-drive mobile robot developed as part of the **Energy-Efficient Path Planning for Mobile Robots using Genetic Algorithms** project.

The robot uses an ESP32 as the main controller and combines encoder-based motion, IMU sensing, IR-based line following, and a web-based control dashboard.

---

## Hardware

| Component          | Specification                        |
| ------------------ | ------------------------------------ |
| Microcontroller    | ESP32                                |
| Motors             | 2 × N25 12 V, 170 RPM Encoder Motors |
| Wheels             | 2 × 85 mm                            |
| Motor Driver       | TB6612FNG                            |
| Battery            | 11.1 V, 2200 mAh LiPo Battery        |
| Voltage Regulation | DC-DC Voltage Regulator              |
| IMU                | WT901BLE                             |
| Line Sensors       | 2 × IR Sensors                       |
| Drive Type         | Differential Drive                   |

---

## Working Principle

The robot uses two independently controlled wheels to achieve differential-drive motion.

The ESP32 controls the motor driver and receives feedback from the wheel encoders. Encoder counts are used to estimate wheel movement and distance travelled.

The WT901BLE IMU provides acceleration, angular velocity, and orientation measurements.

Two IR sensors are used for line-following operation.

The ESP32 also hosts a web-based dashboard that allows the robot to be controlled and monitored in real time.

---

## Main Features

### 1. Manual Web Control

The robot can be controlled through an ESP32-hosted web dashboard.

Available controls include:

* Forward
* Backward
* Left
* Right
* Forward-left
* Forward-right
* Backward-left
* Backward-right
* Stop

The dashboard also allows the manual motor speed to be adjusted.

---

### 2. Encoder-Based Distance Movement

The robot can be commanded to travel a specified distance in meters.

The dashboard provides:

* Distance input
* Motor speed selection
* Encoder count monitoring
* Distance estimation for both wheels

The robot uses the configured wheel diameter and encoder pulses per wheel revolution to estimate wheel travel.

Default configuration in the dashboard:

```text
Wheel diameter:          0.085 m
Pulses per revolution:   360
Wheel base:              0.16 m
Default speed:            150
```

---

### 3. Angle-Based Turning

The robot can perform a specified turn using a degree-based command.

The dashboard accepts:

* Turning angle
* Turning speed

Positive angles represent right turns, while negative angles represent left turns.

---

### 4. Shape Navigation

The robot includes predefined shape-navigation functions.

Available shapes:

* Square
* Triangle
* Circle

The user can specify the shape size and robot speed from the dashboard.

---

### 5. Line Following

Two IR sensors are used for line-following operation.

The dashboard provides:

* Line-following speed control
* Start line following
* Stop line following
* Live left and right IR sensor readings

The sensor states are displayed on the dashboard in real time.

---

### 6. IMU Monitoring

The WT901BLE IMU provides real-time sensor information.

The dashboard displays:

#### Acceleration

```text
Ax
Ay
Az
```

in m/s².

#### Angular Velocity

```text
Wx
Wy
Wz
```

in °/s.

#### Orientation

```text
Roll
Pitch
Yaw
```

in degrees.

---

### 7. Real-Time Robot Dashboard

The ESP32 hosts a web-based dashboard for robot operation and monitoring.

The dashboard provides:

* Robot operating mode
* Encoder counts
* Wheel distances
* IR sensor states
* IMU measurements
* Manual control
* Distance movement
* Turning
* Shape navigation
* Line following
* Robot configuration
* Data recording

---

## Data Recording

The dashboard includes a real-time recorder for collecting robot data.

The sampling period can be configured by the user.

Recorded parameters include:

```text
Time
Mode
Encoder A
Encoder B
IR Left
IR Right
Meters A
Meters B
Ax
Ay
Az
Wx
Wy
Wz
Roll
Pitch
Yaw
```

---

## CSV Data Export

Recorded data can be exported from the dashboard as a CSV file.

The generated file contains the recorded robot, encoder, IR sensor, and IMU measurements.

Example filename:

```text
robot_imu_log.csv
```

---

## PDF Report Generation

The dashboard also provides an option to generate a PDF report from the recorded data and dashboard information.

Example filename:

```text
robot_imu_report.pdf
```

---

## Configuration

The robot parameters can be configured through the web dashboard.

### Wheel Diameter

```text
0.085 m
```

This corresponds to the 85 mm wheels used on the robot.

### Encoder Resolution

```text
360 pulses / wheel revolution
```

### Wheel Base

```text
0.16 m
```

### Motor Speed

The motor speed can be configured within the ESP32 motor-control range:

```text
0 – 255
```

---

## Source Files

| File          | Description                                   |
| ------------- | --------------------------------------------- |
| `WORKING.ino` | Main ESP32 robot control program              |
| `webpage.h`   | ESP32-hosted web dashboard and user interface |

---

## System Overview

```text
                  ┌─────────────────────┐
                  │        ESP32        │
                  │   Main Controller   │
                  └──────────┬──────────┘
                             │
              ┌──────────────┼──────────────┐
              │              │              │
              ▼              ▼              ▼
       ┌────────────┐ ┌────────────┐ ┌────────────┐
       │  TB6612FNG │ │ WT901BLE   │ │ IR Sensors │
       │ Motor      │ │    IMU     │ │  L / R     │
       │ Driver     │ └────────────┘ └────────────┘
       └─────┬──────┘
             │
       ┌─────┴─────┐
       ▼           ▼
   N25 Motor A  N25 Motor B
   + Encoder    + Encoder
```

---

## Role in the Main Project

This robot forms the **physical mobile-robot platform** for the larger project:

> **Energy-Efficient Path Planning for Mobile Robots using Genetic Algorithms**

The platform was developed to provide motion control, sensing, encoder feedback, and experimental data collection required for future autonomous path-planning experiments.

The YDLIDAR X2 and ROS 2 mapping experiments are documented separately in the `LiDAR_ROS2` folder.

---

## Project Status

### Completed

* Two-wheel differential-drive robot
* ESP32-based control
* Encoder-based distance measurement
* IMU integration
* IR-based line following
* Manual web control
* Distance-based movement
* Angle-based turning
* Shape navigation
* Real-time sensor monitoring
* Data recording
* CSV export
* PDF report generation

### Not Fully Integrated

The YDLIDAR X2 was tested separately using a Raspberry Pi, Ubuntu, ROS 2, and RViz2. Due to hardware and integration issues, the LiDAR was not successfully integrated onto the final two-wheel robot.

The complete end-to-end LiDAR mapping → Genetic Algorithm optimization → physical robot navigation pipeline was therefore not completed.

---

## Related Work

The independent YDLIDAR X2 and ROS 2 mapping experiments are available here:

```text
../LiDAR_ROS2/
```

These experiments were conducted as part of the planned sensing and mapping stage of the project.

