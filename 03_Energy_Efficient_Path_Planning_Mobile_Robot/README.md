# Energy-Efficient Path Planning for Mobile Robots using Genetic Algorithms

## Overview

This project was developed as part of the **Summer Research Internship Program at NIT Rourkela**.

The objective of the project was to explore the development of an **energy-efficient path-planning system for mobile robots**, with **Genetic Algorithms** planned as the optimization approach.

The project involved the development of a physical two-wheel differential-drive mobile robot, experimental work with sensing and mapping technologies, and the preparation of a platform for future autonomous path-planning experiments.

The completed work focused primarily on:

* Development of a two-wheel differential-drive mobile robot.
* ESP32-based motor and sensor control.
* Encoder-based distance measurement and navigation.
* IMU monitoring.
* IR-based line following.
* Web-based robot control and monitoring.
* Real-time data recording.
* Independent experiments using YDLIDAR X2, Raspberry Pi, ROS 2, and RViz2 for 2D sensing and mapping.

> **Project Status:** The complete end-to-end system involving LiDAR integration, environmental mapping, Genetic Algorithm optimization, and autonomous navigation on the physical robot was planned but could not be fully completed because of hardware and integration constraints.

---

## Project Objective

The primary objective was to develop a foundation for an intelligent mobile robot capable of navigating efficiently through an environment.

The planned workflow was:

```text
Environment Sensing
        │
        ▼
2D Mapping
        │
        ▼
Path Representation
        │
        ▼
Genetic Algorithm
        │
        ▼
Energy-Efficient Path Selection
        │
        ▼
Mobile Robot Navigation
```

The project development included building and testing the physical robot platform and independently exploring the LiDAR-based sensing and mapping stage.

---

# Completed System

## Two-Wheel Differential-Drive Mobile Robot

A two-wheel mobile robot was developed using an ESP32 as the main controller.

The robot includes encoder feedback, IMU sensing, IR-based line following, and a web-based control dashboard.

### Hardware

| Component        | Specification                        |
| ---------------- | ------------------------------------ |
| Microcontroller  | ESP32                                |
| Motors           | 2 × N25 12 V, 170 RPM Encoder Motors |
| Wheels           | 2 × 85 mm Wheels                     |
| Motor Mounting   | Motor Brackets                       |
| Motor Driver     | TB6612FNG                            |
| Battery          | 11.1 V, 2200 mAh LiPo Battery        |
| Power Regulation | DC-DC Voltage Regulator              |
| IMU              | WT901BLE                             |
| Line Sensors     | 2 × IR Sensors                       |
| Drive Mechanism  | Differential Drive                   |

---

## Robot Features

### Manual Web-Based Control

The ESP32 hosts a web-based dashboard for controlling the robot.

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

Motor speed can also be adjusted through the dashboard.

---

### Encoder-Based Motion

The N25 motors include encoder feedback.

Encoder counts are used to estimate:

* Wheel movement.
* Distance travelled by each wheel.
* Distance-based robot motion.

The dashboard allows the user to enter a required distance and command the robot to move forward or backward.

---

### Angle-Based Turning

The robot can receive a turn command in degrees.

The dashboard supports:

* Positive angle → Right turn.
* Negative angle → Left turn.
* Adjustable turning speed.

---

### Shape Navigation

Predefined motion routines were implemented for:

* Square
* Triangle
* Circle

The user can configure the required size and movement speed through the web dashboard.

---

### Line Following

Two IR sensors are used for line-following operation.

The dashboard provides:

* Live left IR sensor reading.
* Live right IR sensor reading.
* Adjustable line-following speed.
* Start and stop control.

---

### IMU Monitoring

The WT901BLE IMU provides real-time motion and orientation information.

The dashboard displays:

#### Acceleration

* Ax
* Ay
* Az

#### Angular Velocity

* Wx
* Wy
* Wz

#### Orientation

* Roll
* Pitch
* Yaw

---

### Real-Time Monitoring

The web dashboard provides live monitoring of:

* Robot operating mode.
* Encoder A count.
* Encoder B count.
* Distance travelled by each wheel.
* Left IR sensor.
* Right IR sensor.
* Acceleration.
* Angular velocity.
* Roll.
* Pitch.
* Yaw.

---

## Data Recording and Reporting

The robot dashboard includes a data recording system.

The user can select the sampling period and record:

```text
Time
Robot Mode
Encoder A
Encoder B
IR Left
IR Right
Distance A
Distance B
Acceleration X
Acceleration Y
Acceleration Z
Angular Velocity X
Angular Velocity Y
Angular Velocity Z
Roll
Pitch
Yaw
```

### Export Options

Recorded data can be exported as:

* CSV file.
* PDF report.

---

# ESP32 Robot Architecture

```text
                    ┌─────────────────────┐
                    │     Web Browser     │
                    │  Robot Dashboard    │
                    └──────────┬──────────┘
                               │
                              Wi-Fi
                               │
                               ▼
                    ┌─────────────────────┐
                    │        ESP32        │
                    │   Main Controller   │
                    └──────────┬──────────┘
                               │
          ┌────────────────────┼────────────────────┐
          │                    │                    │
          ▼                    ▼                    ▼
   ┌─────────────┐      ┌─────────────┐      ┌─────────────┐
   │  TB6612FNG  │      │  WT901BLE   │      │ IR Sensors  │
   │ Motor Driver│      │     IMU     │      │ Left / Right│
   └──────┬──────┘      └─────────────┘      └─────────────┘
          │
     ┌────┴────┐
     │         │
     ▼         ▼
 N25 Motor A  N25 Motor B
 + Encoder    + Encoder
```

---

# Independent YDLIDAR X2 and ROS 2 Experiments

As part of the planned sensing and mapping stage, the **YDLIDAR X2** was tested independently.

The experimental setup used:

* YDLIDAR X2.
* Raspberry Pi.
* Ubuntu 24.04.
* ROS 2.
* `ydlidar_ros2_driver`.
* RViz2.

The LiDAR experiments were conducted separately from the final ESP32 two-wheel robot.

---

## LiDAR and Mapping Pipeline

The experimental pipeline was:

```text
                ┌──────────────────┐
                │    YDLIDAR X2    │
                │   2D Laser Scan  │
                └────────┬─────────┘
                         │
                    USB / Serial
                         │
                         ▼
                ┌──────────────────┐
                │  Raspberry Pi    │
                │   Ubuntu 24.04   │
                └────────┬─────────┘
                         │
                         ▼
                ┌──────────────────┐
                │      ROS 2       │
                │ ydlidar_ros2_    │
                │      driver      │
                └────────┬─────────┘
                         │
                         ▼
                       /scan
                         │
                         ▼
                ┌──────────────────┐
                │ SLAM / Mapping   │
                └────────┬─────────┘
                         │
                         ▼
                       /map
                         │
                         ▼
                ┌──────────────────┐
                │      RViz2       │
                │ Visualization    │
                └──────────────────┘
```

The experiments demonstrated LiDAR scan visualization and 2D mapping within the ROS 2 environment.

---

## ROS 2 Topics Used During Experiments

The experimental ROS 2 setup involved topics such as:

| Topic          | Purpose                           |
| -------------- | --------------------------------- |
| `/scan`        | Laser scan data                   |
| `/map`         | Generated occupancy grid map      |
| `/map_updates` | Incremental map updates           |
| `/tf`          | Coordinate transformations        |
| `/tf_static`   | Static coordinate transformations |

The LiDAR frame was represented using `laser_frame`, while mapping and visualization used the ROS coordinate transformation system.

---

# Important Development Status

The project was developed in multiple stages.

## Successfully Completed

* Two-wheel differential-drive robot platform.
* ESP32-based control system.
* TB6612FNG motor control.
* Encoder monitoring.
* Distance-based movement.
* Angle-based turning.
* Shape navigation.
* IR-based line following.
* WT901BLE IMU monitoring.
* Web-based robot dashboard.
* Real-time sensor monitoring.
* Data recording.
* CSV export.
* PDF report generation.
* Independent YDLIDAR X2 testing.
* ROS 2 LiDAR driver setup.
* RViz2 visualization.
* 2D mapping experiments.

---

## Planned but Not Fully Completed

The following stages were part of the intended complete system but were not successfully integrated during the project period:

* Physical integration of the YDLIDAR X2 onto the final two-wheel robot.
* End-to-end LiDAR-based autonomous navigation on the robot.
* Complete integration of environmental mapping with the robot control system.
* Full implementation and deployment of the Genetic Algorithm for energy-efficient path selection on the physical robot.

These limitations were mainly due to hardware and system integration constraints during the project.

---

# Planned Genetic Algorithm Workflow

The intended future optimization pipeline is shown below:

```text
Generated / Known Environment Map
              │
              ▼
      Define Start and Goal
              │
              ▼
      Generate Candidate Paths
              │
              ▼
        Genetic Algorithm
              │
      ┌───────┼────────┐
      ▼       ▼        ▼
   Selection Crossover Mutation
      │       │        │
      └───────┼────────┘
              ▼
      Evaluate Fitness
              │
              ▼
    Select Efficient Path
              │
              ▼
     Robot Motion Commands
```

A future fitness function could consider multiple factors, including:

* Path length.
* Number of turns.
* Motion smoothness.
* Estimated energy consumption.
* Obstacle avoidance.

---

# Repository Structure

```text
03_Energy_Efficient_Path_Planning_Mobile_Robot/
│
├── README.md
│
├── ESP32_Robot/
│   ├── WORKING.ino
│   ├── webpage.h
│   └── README.md
│
├── LiDAR_ROS2/
│   ├── README.md
│   │
│   └── Media/
│       ├── LiDAR setup media
│       ├── ROS 2 / RViz screenshots
│       ├── Mapping data
│       └── README.md
│
├── Documentation/
│   └── Project documentation
│
└── Media/
    ├── Robot_Photos/
    ├── Robot_Videos/
    └── Dashboard_Screenshots/
```

---

# Media

The repository contains project media demonstrating:

## Mobile Robot

* Physical robot photographs.
* Robot working demonstrations.
* Web dashboard screenshots.

📁 `Media/Robot_Photos/`

📁 `Media/Robot_Videos/`

📁 `Media/Dashboard_Screenshots/`

## LiDAR and ROS 2

* YDLIDAR X2 experimental setup.
* ROS 2 and RViz2 visualization.
* Mapping experiments.
* Generated map data.

📁 `LiDAR_ROS2/Media/`

---

# Source Code

The ESP32 robot implementation is available in:

📁 [`ESP32_Robot`](./ESP32_Robot)

Main files:

* `WORKING.ino` — Main ESP32 robot control program.
* `webpage.h` — Web dashboard interface.

---

# Future Work

The following improvements can extend this project into the originally intended complete system:

1. Integrate YDLIDAR X2 directly with the mobile robot platform.
2. Connect the Raspberry Pi ROS 2 sensing system with the ESP32 motor-control system.
3. Perform real-time environment mapping while the robot moves.
4. Implement Genetic Algorithm-based path optimization.
5. Develop an energy-consumption fitness function.
6. Compare optimized and non-optimized paths.
7. Measure travel distance, turning effort, execution time, and estimated energy consumption.
8. Implement closed-loop autonomous navigation using the generated path.

---

# Key Learning Outcomes

This project provided hands-on experience with:

* Differential-drive mobile robots.
* ESP32 development.
* Motor drivers and encoder motors.
* Encoder-based motion estimation.
* IMU sensing.
* IR sensor-based line following.
* Embedded web servers.
* Real-time web dashboards.
* Robot data logging.
* Raspberry Pi.
* Ubuntu.
* ROS 2.
* LiDAR sensing.
* RViz2 visualization.
* 2D mapping concepts.
* Mobile robot navigation and path-planning concepts.

---

## Project Context

This project was carried out as part of the author's **Summer Research Internship at NIT Rourkela** in the area of mobile robotics and autonomous systems.

The work represents the development and experimental stages completed during the internship and provides a foundation for future integration of LiDAR-based mapping and Genetic Algorithm-based energy-efficient path planning.
