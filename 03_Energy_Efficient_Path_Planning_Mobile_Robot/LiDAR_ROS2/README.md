# YDLIDAR X2 and ROS 2 — Media

This folder contains photographs, screenshots, videos, and mapping data from the **YDLIDAR X2 and ROS 2 experiments** conducted as part of the project:

> **Energy-Efficient Path Planning for Mobile Robots using Genetic Algorithms**

The LiDAR experiments were performed separately from the final two-wheel mobile robot.

---

## Contents

### YDLIDAR X2

Media showing the YDLIDAR X2 hardware and its setup for 2D laser scanning.

### ROS 2 and RViz2

Screenshots and videos showing:

* ROS 2 operation
* `/scan` laser scan data
* RViz2 visualization
* 2D environment mapping
* Laser scan points and mapped walls

### Mapping Data

The folder also contains the generated 2D map files used during the mapping experiments.

---

## Experimental Setup

The LiDAR testing setup consisted of:

```text
YDLIDAR X2
      │
      │ USB / Serial
      ▼
Raspberry Pi
      │
      │ ROS 2
      ▼
YDLIDAR ROS 2 Driver
      │
      ▼
/scan
      │
      ▼
SLAM / Mapping
      │
      ▼
RViz2
```

---

## Important Project Note

The YDLIDAR X2 was successfully tested as a separate sensing and mapping setup using a Raspberry Pi, Ubuntu, ROS 2, and RViz2.

However, due to hardware and integration issues, the YDLIDAR X2 was **not successfully integrated onto the final two-wheel mobile robot**.

Therefore, the media in this folder represents the **independent LiDAR and ROS 2 experimental setup**, and should not be interpreted as a demonstration of LiDAR operating directly on the final mobile robot.

---

## Files

Typical media in this folder includes:

* YDLIDAR X2 setup photographs
* ROS 2 / RViz2 screenshots
* Mapping visualization
* LiDAR demonstration video
* Generated 2D map (`.pgm`)
* Map configuration (`.yaml`)

These files provide visual and experimental evidence of the LiDAR sensing and mapping stage of the project.
