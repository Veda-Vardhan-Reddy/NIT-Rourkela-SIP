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



---

## Features

- Four-wheel remote-controlled robot
- Custom-built joystick controller
- Wireless control using ESP-NOW
- ESP32-based transmitter and receiver
- Forward movement
- Backward movement
- Left movement
- Right movement
- Stop command
- Four 12V DC motors
- L298N dual H-bridge motor driver
- PWM-based motor speed control
- Battery-powered operation

---

## Hardware Components

### Robot / Receiver

| Component | Quantity |
|---|---:|
| ESP32 | 1 |
| 12V DC motors | 4 |
| L298N Dual H-Bridge Motor Driver | 1 |
| 80 mm robotic wheels | 4 |
| XL4015 Buck Converter | 1 |
| 18650 Li-ion cells | 3 |
| Battery holder | 1 |
| Robot chassis/frame | 1 |
| Motor mounting brackets | 4 |
| Power switch | 1 |
| Breadboard / terminal connections | As required |
| Jumper/connecting wires | As required |
| Screws, nuts and spacers | As required |

### Joystick / Transmitter

| Component | Quantity |
|---|---:|
| ESP32 | 1 |
| 2-axis joystick module | 1 |
| 18650 Li-ion cells | 2 |
| Buck converter | 1 |
| Battery holder | 1 |
| Power switch | 1 |
| Connecting wires | As required |
| Custom controller body/enclosure | 1 |

---

## Power Supply

### Robot

The robot is powered using three 18650 Li-ion cells connected as a 3-cell battery pack.

The battery pack provides a 12V-class supply for the robot system.

The battery supply is used for the motor driver and robot power system. An XL4015 buck converter is used to provide regulated voltage where required.

> A 3-cell Li-ion pack has a nominal voltage of approximately 11.1V and can reach approximately 12.6V when fully charged.

### Joystick Controller

The joystick controller is powered using two 18650 Li-ion cells.

A buck converter is used to regulate the battery voltage to approximately 5V for the joystick controller electronics.

---

## Wireless Communication

The joystick and robot communicate using **ESP-NOW**.

ESP-NOW allows the two ESP32 boards to communicate directly without requiring a Wi-Fi router or internet connection.

The transmitter reads the joystick position and converts it into a movement command.

The commands used in the project are:

| Command | Function |
|---|---|
| `F` | Forward |
| `B` | Backward |
| `L` | Left |
| `R` | Right |
| `S` | Stop |

The command is transmitted from the joystick ESP32 to the robot ESP32 using ESP-NOW.

---

## Joystick Transmitter

The transmitter consists of an ESP32 connected to a two-axis joystick module.

The joystick provides X-axis and Y-axis analog values. These values are processed by the ESP32 to determine the required movement direction.

### Joystick Pin Connections

| Joystick Signal | ESP32 GPIO |
|---|---:|
| X-axis | GPIO 34 |
| Y-axis | GPIO 35 |
| Switch | GPIO 32 |

### Movement Detection

The transmitter checks the joystick values and generates a corresponding command:

```text
Joystick Movement
       │
       ▼
Read X and Y Values
       │
       ▼
Determine Direction
       │
       ├── Forward  → F
       ├── Backward → B
       ├── Left     → L
       ├── Right    → R
       └── Neutral  → S
