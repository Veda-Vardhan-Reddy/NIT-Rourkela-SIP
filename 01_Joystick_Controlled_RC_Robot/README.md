# Joystick Controlled RC Robot

A four-wheel remote-controlled robot developed during the Summer Internship Programme at NIT Rourkela. The robot is controlled wirelessly using a custom-built joystick controller. Two ESP32 boards are used for communication, with ESP-NOW used to transmit movement commands from the joystick to the robot.

## Project Overview

The project consists of two main units:

1. **Joystick Transmitter** – A custom-built controller using an ESP32 and a two-axis joystick module.
2. **Robot Receiver** – A four-wheel robotic vehicle using an ESP32, L298N motor driver, and four 12V DC geared motors.

The joystick sends movement commands wirelessly to the robot using the ESP-NOW communication protocol.

### System Architecture

```text
              CUSTOM JOYSTICK CONTROLLER
                        
          ┌──────────────────────────┐
          │        Joystick          │
          │      Module (X/Y)        │
          └────────────┬─────────────┘
                       │
                       ▼
          ┌──────────────────────────┐
          │          ESP32           │
          │       Transmitter        │
          └────────────┬─────────────┘
                       │
                       │ ESP-NOW
                       │ Wireless
                       ▼
          ┌──────────────────────────┐
          │          ESP32           │
          │         Receiver         │
          └────────────┬─────────────┘
                       │
                       ▼
          ┌──────────────────────────┐
          │          L298N           │
          │      Motor Driver        │
          └────────────┬─────────────┘
                       │
              ┌────────┴────────┐
              ▼                 ▼
       Left-side Motors    Right-side Motors
              │                 │
              └────────┬────────┘
                       ▼
                 4-Wheel Robot



That's it. The **three backticks ` ``` `** end the flowchart/code block.
