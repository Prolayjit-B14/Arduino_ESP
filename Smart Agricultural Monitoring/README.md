
# 🌿 Smart Irrigation & Environmental Monitoring System

An intelligent, automated environmental monitoring and irrigation system powered by **Arduino UNO/Nano**, designed to optimize water usage, detect environmental conditions, and enhance precision agriculture. This system monitors **soil moisture**, **temperature**, **humidity**, and **light intensity**, and triggers irrigation automatically with visual and auditory alerts for critical conditions.

---

## 🚀 Project Objectives

- Reduce water wastage through automated irrigation.
- Provide real-time status of environmental parameters.
- Alert farmers/users to abnormal soil or temperature conditions.
- Lay the foundation for a low-cost, scalable smart farming solution.

---

## 📌 Core Features

- 🌱 **Smart Soil Moisture Analysis**: Automatically waters plants based on dryness levels.
- 🌡️ **Temperature & Humidity Monitoring**: Monitors ambient conditions using a DHT11 sensor.
- 💡 **Ambient Light Detection**: LDR-based system monitors sunlight levels.
- 💧 **Automated Pump Control**: Relay module drives the water pump based on moisture readings.
- 🚨 **Visual Alerts**: Color-coded LEDs indicate soil condition and temperature levels.
- 🔊 **Auditory Buzzer Warnings**: Sound alerts for dry or overly wet soil and high temperature.
- 📟 **Serial Monitor Feedback**: Detailed, continuous sensor logs for debugging and analysis.

---

## 🧰 Hardware Requirements


| Component                   | Description                               |
|-----------------------------|-------------------------------------------|
| Arduino UNO / Nano          | Core microcontroller                      |
| DHT11 Sensor                | Measures temperature and humidity         |
| Soil Moisture Sensor        | Detects water content in soil             |
| LDR (Light Dependent Resistor) | Detects ambient brightness             |
| Relay Module (Active LOW)   | Drives the water pump                     |
| Water Pump (optional)       | Irrigation actuator                       |
| Buzzer                      | Sound-based alert system                  |
| Red, Green, Yellow LEDs     | Moisture status indicators                |
| Blue LED (optional)         | Temperature alert                         |
| White LED (optional)        | Light level indicator                     |
| 9V Battery or USB Cable     | Power source                              |
| Breadboard & Jumper Wires   | For circuit assembly                      |


---

## 🔌 Pin Configuration


| Device                  | Arduino Pin |
|-------------------------|-------------|
| DHT11 Sensor            | D12         |
| Soil Moisture Sensor    | A1          |
| LDR Sensor              | A0          |
| Relay (Pump Control)    | D13         |
| Buzzer                  | D9          |
| Red LED (Dry Soil)      | D3          |
| Green LED (Moderate)    | D4          |
| Yellow LED (Wet Soil)   | D5          |
| Temp Alert LED          | D6          |
| Light Condition LED     | D7          |


---

## 🔁 Workflow Logic

1. **System Boot:**
   - Initializes sensors and pins.
   - Turns off pump relay initially for safety.

2. **Data Acquisition:**
   - Reads temperature and humidity from the DHT11 sensor.
   - Measures soil moisture using an analog soil sensor.
   - Measures ambient light using an LDR.

3. **Condition Evaluation:**
   - **Soil Moisture**:
     - < 300 → **Dry** → Red LED + Pump ON + Buzzer.
     - 300–699 → **Moderate** → Green LED + Pump OFF.
     - ≥ 700 → **Wet** → Yellow LED + Pump OFF + Low-tone buzzer.
   - **Temperature**:
     - > 35°C → Alert LED ON + Buzzer.
   - **Light**:
     - Bright (< 800) → Light LED ON.
     - Dark (> 800) → Light LED OFF.

4. **Action Execution:**
   - Activates/deactivates pump and alerts based on logic.
   - Logs detailed sensor readings to Serial Monitor every 2 seconds.

---

## 🧠 Required Libraries

Install the following libraries in Arduino IDE via Library Manager:

- [`DHT sensor library`](https://github.com/adafruit/DHT-sensor-library)
- `Adafruit Unified Sensor` (required by DHT library)

---

## 📟 Sample Serial Monitor Output

```
Temperature: 29.8 C  Humidity: 55.0%  Light: 690  Soil Moisture: 480
-> Moisture Level: MODERATE (Pump OFF)
-> Light Status: BRIGHT
```

---

## 📘 Future Enhancements

- 🌐 **IoT Integration**: ESP8266/ESP32-based cloud dashboard.
- 📲 **SMS Alerts** via GSM for remote notification.
- 📊 **OLED Display** for real-time data visualization.
- 🔋 **Battery & Solar Power** integration for field deployment.
- 🧠 **AI-based irrigation schedules** with historical learning.

---

## ⚠️ Precautions & Notes

- Relay module should be isolated if using external high-voltage pumps.
- Ensure DHT11 sensor stays dry for accurate readings.
- Soil sensor values may vary based on soil type and require calibration.
- Buzzer timing should be tuned for indoor vs. outdoor environments.

---

## 📝 License

This project is for Idea Lab & Innovation

---

## 🙌 Credits

- Developed using open-source tools and community-supported libraries.
- Special thanks to [Adafruit](https://www.adafruit.com/) for sensor libraries and hardware support.
