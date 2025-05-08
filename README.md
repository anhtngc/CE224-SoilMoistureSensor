# CE224 - Embedded System Design

## Abstract 
This research focuses on developing a temperature and humidity measurement system using commonly available electronic components such as the DHT11 sensor, ESP32, SH1106 OLED display, and a piezo buzzer. The primary objective of the system is to provide an accurate, stable, and user-friendly device with the ability to monitor environmental conditions and issue alerts when temperature thresholds are exceeded.

The system operates by utilizing the DHT11 sensor to collect real-time data on temperature and humidity. This data is processed by the ESP32, which then sends it to the SH1106 OLED display for real-time monitoring. When the temperature exceeds a predefined safe threshold, the piezo buzzer emits an audible alarm, alerting the user to abnormal environmental conditions.

Power for the system is supplied by a 3.7V Li-po battery, which is charged and protected via the TP4056 module. The MT3608 boost converter increases the voltage to a stable 5V to power the components. The system also features Wi-Fi connectivity, allowing remote monitoring and notifications about temperature or humidity changes. 

(Final Design showed in file ***result.mov***)

## 📑 Table of Contents
- [About Me](#about-me)
- [Introduction To Our Team](#introduction-to-our-team)
- [Key Features](#key-features)
- [System Architecture](#system-architecture)
- [References](#references)

## About Me
👋 **Name:** Trần Ngọc Ánh  
👋 **Responsibility:**  Firmware Development, Sensor Data Processing, Research System Control (Connection)  
👋 **MSSV:** 22520077  
👋 **Gmail:** 22520077@gm.uit.edu.vn

## Introduction To Our Team
- **Course Name:** Embedded System Design
- **Course Code:** CS224
- **Class Code:** CS224.P14
- **Academic Year:** Semester 1 (2024 - 2025)
- **Lecturer:**
 
| No. | Full Name | Email |
| :-: | --------- | ----- |
| 1   | **TS. Đoàn Duy** | *duyd@uit.edu.vn* |

## Team Members
| No. | Full Name        | MSSV      | Email                     |
| :-: | ---------------- | --------- | ------------------------- |
| 1   | **Trần Ngọc Ánh** | *22520077* | *22520077@gm.uit.edu.vn* |
| 2   | **Dương Anh Khôi**    | *22520696* | *22520696@gm.uit.edu.vn* |
| 3   | **Nguyễn Đăng Thanh Tuệ**      | *22521616* | *22521616@gm.uit.edu.vn* |
| 4   | **Tống Viết Trường**      | *22521583* | *22521583@gm.uit.edu.vn* |

## Key Features
- **ESP32**: The microcontroller that controls the entire system and provides network
connectivity.
- **DHT11**: The sensor that collects temperature and humidity data.
- **SH1106 OLED Display**: Displays temperature, humidity, and system status
information.
- **Piezo Buzzer**: Emits an alarm when the temperature exceeds a threshold.
- **TP4056 and MT3608**: Manage and supply power to the system.

The system can be applied in various fields such as environmental monitoring, healthcare, and home automation, with advantages including compact design, ease of use, and energy efficiency. However, there are some limitations in terms of the accuracy of the DHT11 sensor, battery life, and compatibility with other technologies.

## System Architecture
![image](https://github.com/user-attachments/assets/6a0853da-b2d3-4b90-9984-35065f405d37)

## References
[Design and Implementation of a Smart Weather Monitoring System Using DHT11
Sensor](https://www.scribd.com/document/638134702/Design-and-Implementation-of-a-Weather-Monitoring-System-Using-Internet-of-Things-Copy)  
[Arduino-based Environmental Monitoring System using DHT11 Sensor](https://docs.cirkitdesigner.com/project/published/f8d64794-3616-442a-8eed-5fc65f76c463/arduino-uno-based-environmental-monitoring-system-with-dht11-bh1750-and-gps)  
[ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)  
[SH1106 OLED Display Datasheet](https://cdn.velleman.eu/downloads/29/infosheets/sh1106_datasheet.pdf)  
[TP4056 Lithium Battery Charger IC Datasheet](https://www.mouser.vn/c/?tool%20is%20for%20evaluation%20of=TP4056)  
[MT3608 Boost Converter Datasheet](https://www.olimex.com/Products/Breadboarding/BB-PWR-3608/resources/MT3608.pdf)  
