# Project Documentation

This folder contains supporting technical documentation for the project:

> **Energy-Efficient Path Planning for Mobile Robots using Genetic Algorithms**

The documentation covers the development of the two-wheel mobile robot, its hardware and working features, the independent YDLIDAR X2 and ROS 2 experiments, and the overall development status of the project.

---

## Documentation Scope

The project was developed in multiple stages involving:

* Two-wheel differential-drive robot development.
* ESP32-based robot control.
* Encoder-based motion monitoring.
* IMU sensing.
* IR-based line following.
* Web-based robot control and monitoring.
* Data recording and reporting.
* Independent YDLIDAR X2 experiments.
* ROS 2 and RViz2-based LiDAR visualization and mapping.
* Planned Genetic Algorithm-based energy-efficient path optimization.

---

## Recommended Documentation Files

This folder may contain the following documents:

### Hardware Components

```text
Hardware_Components.md
```

Contains the hardware used in the project, including:

* ESP32.
* N25 encoder motors.
* TB6612FNG motor driver.
* 85 mm wheels.
* WT901BLE IMU.
* IR sensors.
* 11.1 V 2200 mAh LiPo battery.
* Voltage regulator.
* Raspberry Pi.
* YDLIDAR X2.

---

### Robot Connections

```text
Robot_Connections.md
```

Documents the electrical and communication connections between:

* ESP32.
* TB6612FNG motor driver.
* Motor encoders.
* WT901BLE IMU.
* IR sensors.
* Battery and voltage regulator.

This documentation should be based on the actual working robot implementation.

---

### System Architecture

```text
System_Architecture.md
```

Describes the architecture of the project, including the two main experimental systems:

1. ESP32-based mobile robot.
2. Independent YDLIDAR X2 and ROS 2 sensing and mapping setup.

These systems are documented separately because the LiDAR was not successfully integrated with the final two-wheel robot.

---

### Project Status

```text
Project_Status.md
```

Documents the development progress and clearly separates:

* Successfully completed work.
* Independently tested work.
* Planned work that was not fully completed.

This provides an accurate technical record of the project.

---

## Completed Mobile Robot Work

The completed two-wheel robot platform includes:

* ESP32-based control.
* Differential-drive motion.
* Two N25 encoder motors.
* Encoder monitoring.
* Distance-based movement.
* Angle-based turning.
* Square, triangle, and circle navigation.
* IR-based line following.
* WT901BLE IMU monitoring.
* Web-based manual control.
* Real-time sensor monitoring.
* Data recording.
* CSV export.
* PDF report generation.

---

## Independent LiDAR and ROS 2 Work

The project also included independent experiments using:

* YDLIDAR X2.
* Raspberry Pi.
* Ubuntu 24.04.
* ROS 2.
* YDLIDAR ROS 2 driver.
* RViz2.
* Laser scan visualization.
* 2D mapping experiments.

The LiDAR sensing and mapping experiments were conducted separately from the final ESP32 mobile robot.

---

## Important Project Status Note

The original project aimed to explore an integrated system involving:

```text
LiDAR Sensing
      ↓
Environment Mapping
      ↓
Path Planning
      ↓
Genetic Algorithm Optimization
      ↓
Energy-Efficient Path Selection
      ↓
Autonomous Mobile Robot Navigation
```

The complete end-to-end system was not fully completed during the project period.

In particular:

* The YDLIDAR X2 could not be successfully integrated onto the final two-wheel mobile robot because of hardware and integration constraints.
* The complete Genetic Algorithm-based energy-efficient path-planning system was not deployed on the physical robot.

Therefore, the documentation distinguishes clearly between the **completed robot implementation**, **independent LiDAR experiments**, and **planned future integration**.

---

## Related Project Folders

### ESP32 Robot Implementation

```text
../ESP32_Robot/
```

Contains the working ESP32 source code and robot-specific documentation.

### LiDAR and ROS 2 Experiments

```text
../LiDAR_ROS2/
```

Contains documentation and media from the YDLIDAR X2, ROS 2, and RViz2 experiments.

### Project Media

```text
../Media/
```

Contains:

* Robot photographs.
* Robot working videos.
* Dashboard screenshots.

---

## Purpose of This Documentation

The purpose of this folder is to provide a clear technical record of the project and make the development process easier to understand and reproduce.

The documentation is intended to clearly distinguish between:

* **What was successfully implemented and tested.**
* **What was experimentally explored independently.**
* **What was planned for future integration and optimization.**
