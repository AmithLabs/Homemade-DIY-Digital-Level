# Homemade DIY Digital Level

[![Watch the video](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)](https://youtu.be/YOUR_VIDEO_LINK)

📺 **Watch the Full Build Video on YouTube**
https://youtu.be/YOUR_VIDEO_LINK

---

## Project Overview

This project demonstrates a simple and practical **Homemade DIY Digital Level** using **NodeMCU ESP8266**, **MPU6050 Gyroscope + Accelerometer Sensor**, and a **0.96" OLED Display**.

This system converts a normal spirit level into a smart digital level tool that displays the real-time tilt angle in degrees on the OLED screen.

The MPU6050 sensor reads the tilt angle of the device, and the OLED displays the angle clearly for easy measurement and alignment work.

This project is ideal for learning **ESP8266 projects, I2C communication, MPU6050 interfacing, OLED display control, and digital angle measurement systems**.

The full build process is available on the **AmithLabs YouTube channel**.

---

## Main Features

* Real-Time Angle Measurement
* OLED Live Angle Display
* X-Axis Tilt Detection
* Digital Spirit Level Function
* Simple and Compact Design
* Portable Smart Tool Project
* Beginner Friendly Electronics Project
* Professional DIY Measurement Tool

---

## Hardware Components

* NodeMCU ESP8266
* MPU6050 Gyroscope + Accelerometer Module
* 0.96" OLED Display (I2C SSD1306)
* Spirit Level Body
* Jumper Wires
* USB Cable
* Breadboard (Optional)
* 18650 Battery (Optional)
* TP4056 Charging Module (Optional)
* MT3608 Boost Converter (Optional)

---

## Pin Configuration

| Component   | NodeMCU Pin |
| ----------- | ----------- |
| OLED SDA    | D2 (GPIO4)  |
| OLED SCL    | D1 (GPIO5)  |
| MPU6050 SDA | D2 (GPIO4)  |
| MPU6050 SCL | D1 (GPIO5)  |
| OLED VCC    | 3V3         |
| MPU6050 VCC | 3V3         |
| GND         | GND         |

---

## System Operation

1. NodeMCU powers on and starts I2C communication.
2. OLED display initializes using I2C address 0x3C.
3. MPU6050 sensor starts and checks connection.
4. Accelerometer values are continuously read from the sensor.
5. The system calculates the X-axis tilt angle.
6. The angle is converted into degrees.
7. The OLED screen displays the live angle value in large text.
8. The device works as a portable Digital Spirit Level.

---

## Arduino Code

The complete ESP8266 program is included in this repository:

```text
Digital_Level.ino
```

This code controls:

* MPU6050 Sensor Reading
* OLED Display Output
* Angle Calculation
* I2C Communication
* Live Digital Level Display

---

## Project Images

```text
Schematic Diagram.jpg
Project Build Photos.jpg
```

---

## Required Libraries

Install the following libraries from Arduino IDE:

```text
Adafruit SSD1306
Adafruit GFX Library
MPU6050
Wire
```

Library Path:

```text
Sketch → Include Library → Manage Libraries
```

Search and install:

```text
Adafruit SSD1306
Adafruit GFX
MPU6050 by Electronic Cats
```

---

## Angle Calculation Formula

```text
angleX = atan2(ay, az) * 180 / PI
```

This formula converts accelerometer values into angle degrees for accurate level measurement.

---

## YouTube Channel

This project was created for the **AmithLabs** YouTube channel.

Subscribe for more DIY electronics, smart tools, automation projects, and creative engineering builds.

---

## Disclaimer

This project is created for **educational and demonstration purposes only**.

Please use proper safety precautions when working with batteries, charging modules, and electronic circuits.

For best results, use proper mounting and calibration before practical usage.

---

## Author

**Amith Wijekoon**
Automation & Electrical Engineering Specialist
Creator of the **AmithLabs** YouTube Channel
