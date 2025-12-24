# 🤖 Autonomous Smart Line Follower & Obstacle Avoidance Robot

An advanced Arduino-based autonomous robot that combines **Line Following** and **Obstacle Avoidance** capabilities. This project features a custom-built logic for obstacle scanning and pathfinding without relying on external servo libraries.

---

## 🚀 Key Features

*   **Dual-Mode Operation:** Seamlessly switches between following a black line and navigating around obstacles.
*   **Library-Free Servo Control:** Uses manual PWM signal generation to control the SG90 servo, reducing memory footprint and avoiding timer conflicts.
*   **Smart Scanning Logic:** When an obstacle is detected, the robot performs a 180-degree scan to compare left vs. right distances and chooses the clearest path.
*   **Smooth Motion Profiling:** Implements gradual servo movement to prevent jitter and protect the sensor mount.
*   **Noise Filtering:** Includes a multi-sample averaging algorithm for the Ultrasonic sensor to ensure stable distance readings.

---

## 🛠 Hardware Requirements

*   **Microcontroller:** Arduino Uno (or compatible).
*   **Motor Driver:** L298N Dual H-Bridge.
*   **Motors:** 2x DC Gear Motors.
*   **Sensors:**
    *   HC-SR04 Ultrasonic Sensor (Distance measurement).
    *   2x Infrared (IR) Line Sensors (Path tracking).
*   **Actuator:** SG90 Micro Servo (Scanner mount).
*   **Power:** 7.4V Li-ion or 9V Battery.

---

## 🔌 Pin Connections

### Motors (L298N)
| Label | Arduino Pin | Function |
| :--- | :--- | :--- |
| **enA** | 10 | Left Motor Speed (PWM) |
| **in1** | 9 | Left Motor Direction |
| **in2** | 8 | Left Motor Direction |
| **in3** | 7 | Right Motor Direction |
| **in4** | 6 | Right Motor Direction |
| **enB** | 5 | Right Motor Speed (PWM) |

### Sensors & Servo
| Component | Arduino Pin |
| :--- | :--- |
| **Left IR Sensor** | A0 |
| **Right IR Sensor** | A1 |
| **Ultrasonic Echo** | A2 |
| **Ultrasonic Trigger** | A3 |
| **Servo Signal** | A5 |

---

## 🧠 Navigation Logic

1.  **Line Following:** The robot monitors the IR sensors. If both sensors see white, it moves forward. If a sensor hits the black line, the robot adjusts its direction to stay centered.
2.  **Obstacle Detection:** If the Ultrasonic sensor detects an object within **15cm**:
    *   The robot stops immediately.
    *   The Servo rotates the sensor to the **Right** to measure distance.
    *   The Servo rotates the sensor to the **Left** to measure distance.
    *   The robot compares the two distances and executes a turn towards the side with more clearance.
3.  **Recovery:** After bypassing the obstacle, the robot resumes its search for the line to continue its primary mission.

---

## ⚙️ Configuration & Calibration

You can easily tune the performance by modifying these constants in the code:

*   `vspeed (110)`: Default cruising speed.
*   `tspeed (160)`: Turning speed for sharper maneuvers.
*   `SAFE_DISTANCE (15)`: Minimum distance (cm) to trigger obstacle avoidance.
*   `centerAngle (50)`: Adjust this value to calibrate the "Straight Forward" position of your servo.
*   `TURN_TIME (350)`: Adjust based on your battery voltage to get perfect 90-degree turns.

---

---
*Feel free to star ⭐ this repository if you found it helpful!*
