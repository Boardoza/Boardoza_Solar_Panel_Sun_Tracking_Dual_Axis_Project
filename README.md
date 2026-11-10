
# Boardoza Sun Tracking Dual-Axis Solar Panel Project

The **Boardoza Solar Panel Sun Tracking System** is a Borduino-based, dual-axis solar tracking demonstrator designed to automatically orient a solar panel toward the direction of maximum light intensity. The system uses four **Light Dependent Resistor (LDR)** sensors to monitor light levels from the left, right, top, and bottom directions. Two servo motors independently control the horizontal and vertical orientation of the panel, allowing the system to respond dynamically to changes in illumination.

Powered by a **5V adapter** through a **USB Type-C power board**, the system uses an **I²C hub** for VCC and GND distribution and a Borduino microcontroller for sensor processing and motion control. Each LDR is connected with a **10kΩ pull-down resistor**, and the controller averages eight analog readings per sensor to minimize measurement fluctuations. A hysteresis-based tracking algorithm starts movement when the light intensity difference reaches **15%** and stops correction when the difference decreases to **10% or below**. Servo control is implemented using **Timer1 Fast PWM Mode 14**, with pulse widths between **700µs and 2300µs** and movement limits of **0° to 180°**.


<h2 align="center">LED Matrix Panel</h2>
<p align="center">
  <img src="./assets/Sun Tracking Solar Panel.png" alt="Sun Tracking Solar Panel" width="400">
</p>

---

## Key Features

- **Dual-Axis Solar Tracking:** Automatically adjusts the solar panel's horizontal and vertical orientation using four LDR sensors and two independently controlled servo motors.
- **Real-Time Light Monitoring:** Continuously measures light intensity from four directions and averages sensor readings to reduce fluctuations and improve tracking stability.
- **Hysteresis-Based Motion Control:** Uses separate movement activation and stopping thresholds to prevent unnecessary servo adjustments when light intensity differences are small.
- **Adaptive Servo Movement:** Dynamically adjusts angular correction steps according to the measured light intensity difference, providing smaller movements for minor deviations and larger movements when necessary.
- **Hardware PWM Control:** Uses the microcontroller's Timer1 hardware timer to generate servo control signals with pulse widths of 700–2300µs and angular limits of 0°–180°.
- **Configurable Tracking Behavior:** Supports software adjustment of tracking sensitivity, servo angle limits, sensor thresholds, and correction intervals.

---

## System Architecture

The Solar Panel system integrates a microcontroller, solar panel, I²C hub, motor, LDR sensors, and USB Type-C interface to create a programmable solar tracking system. The LDR sensors detect changes in light intensity, enabling the system to adjust the solar panel's orientation for improved sunlight exposure.

| Component | Model |
|---|---|
| Microcontroller | [Boardoza Boarduino](https://www.ozdisan.com/p/arduino-gelistirme-kartlari-614/boardoza-boarduno-r3-c-1473947?tab=productDetails) |
| Solar Panel | [Solar Panel](https://www.ozdisan.com/) |
| I²C Hub | [Boardoza I²C Hub](https://github.com/Boardoza/Boardoza_USB_Type_C_Breakout_Board) |
| Motor | [DC Motor](https://www.ozdisan.com/) |
| USB Type-C Interface | [Boardoza USB Type-C](https://github.com/Boardoza/Boardoza_USB_Type_C_Breakout_Board) |

---

## Board Pinout

[Boardoza Sun Tracking Solar Panel Pinout Diagram](./assets/Sun%20Tracking%20Solar%20Panel%20Pinout%20Diagram.pdf)

---

## Step Files

[Boardoza Sun Tracking Solar Panel.step](./assets/Sun%20Tracking%20Solar%20Panel%20Step.step)

---

## Version History

- V1.0.0 - Initial Release

---

## Support

- If you have any questions or need support, please contact **support@boardoza.com**

---

## **License**

This repository contains both hardware and software components:

### **Hardware Design**

[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

All hardware design files are licensed under [Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

### **Software/Firmware**

[![BSD-3-Clause][bsd-shield]][bsd]

All software and firmware are licensed under [BSD 3-Clause License][bsd].

[bsd]: https://opensource.org/licenses/BSD-3-Clause
[bsd-shield]: https://img.shields.io/badge/License-BSD%203--Clause-blue.svg
