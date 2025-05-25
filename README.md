# 🌐 IoT-Based Home Automation System 🏠

This project was developed as part of a **1-month internship** in **Digital Electronics & VLSI Design** conducted by **Codectechnologies**, under the guidance of **Ms. Vaishali Shrivastava**.

## ✅ Project Description

This smart home automation system uses an **ESP32 microcontroller** to integrate multiple sensors and actuators, allowing real-time monitoring and control of environmental parameters via the **Blynk IoT platform** and simulation using **Wokwi**.

## 🔧 Features

- 🌡️ **Temperature & Humidity Monitoring** (DHT22)
- 🚨 **Gas Leakage Detection** (MQ2)
- 🚪 **Motion-Based Door Automation** (PIR + Servo)
- 💧 **Water Tank Level Monitoring** (Ultrasonic Sensor)
- 💡 **Smart Lighting System**
  - Manual mode via push button or Blynk
  - Automatic mode using LDR sensor
- 🌬️ **Fan Control** based on environmental conditions or manual override
- 🖥️ **Dual Display Output**
  - **LCD I2C** for sensor values and status
  - **OLED (U8g2)** for time and door status

## 🔌 Components Used

| Component         | Function                              |
|------------------|---------------------------------------|
| ESP32            | Main controller (WiFi-enabled)        |
| DHT22            | Temperature & Humidity sensing        |
| MQ2              | Gas leak detection                    |
| PIR Sensor       | Motion detection                      |
| Servo Motor      | For door and fan control              |
| Ultrasonic Sensor| Water level detection                 |
| LDR              | Ambient light sensing                 |
| Push Buttons     | Manual control of light and fan       |
| RGB LED          | Status indication                     |
| I2C LCD          | Sensor display                        |
| OLED Display     | System time & door status             |
| Buzzer           | Alert when tank is full and gas leak  |

## 🌐 IoT Integration

- **Blynk Virtual Pins:**
  - `V0` – Temperature
  - `V1` – Humidity
  - `V2` – Gas status
  - `V3` – LDR value
  - `V4` – Tank level
  - `V5` – Motion status
  - `V6` – Room light status (manual/auto)
  - `V7` – Fan control
  - `V8` – Light control mode (auto/manual)

## 🛠️ Tools & Platforms

- **Wokwi Simulator** – Virtual testing and simulation
- **Blynk IoT** – Mobile control interface
- **Arduino IDE** – Programming environment
- **GitHub** – Version control and project documentation

## 👨‍🏫 Internship Info

- 🎓 **Internship Domain:** Digital Electronics & VLSI
- 🏢 **Organization:** Codectechnologies
- 🧑‍🏫 **Mentor & Guide:** Ms. Vaishali Shrivastava
- ⏳ **Duration:** 1 Month

## 📸 Screenshots / Demo

![Image](https://github.com/user-attachments/assets/32453553-605e-41b0-9b14-3aa8925bb442)

![Image](https://github.com/user-attachments/assets/38c77e78-f5f3-4066-946b-3ec994101542)

![Image](https://github.com/user-attachments/assets/8835b8b8-9fc9-4f00-9101-f9b9942a30b8)

![Image](https://github.com/user-attachments/assets/0443c723-a019-4d71-91ec-4ad8f653ad4d)

![Image](https://github.com/user-attachments/assets/77e1a9b0-3803-4e5a-a224-95211b89e8e5)




## 📁 How to Run the Project

1. Clone this repository.
2. Open the `.ino` file in **Arduino IDE**.
3. Install the required libraries:
   - `Blynk`
   - `DHT`
   - `LiquidCrystal_I2C`
   - `ESP32Servo`
   - `U8g2lib`
   - `NTPClient`
4. Upload the code to **ESP32** (or simulate on **Wokwi**).
5. Connect Blynk with the template ID, name, and auth token.
6. Monitor real-time sensor data and control devices via your phone.

## 📜 License

This project is for educational purposes under the internship program by **Codectechnologies**. All rights reserved to the original contributors.

---

> 💬 For suggestions or collaborations, feel free to connect with me on [LinkedIn](https://www.linkedin.com/in/suheal-ahamed-n-047625299/) 






