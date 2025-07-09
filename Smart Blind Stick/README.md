
# 👨‍🦯 Smart Blind Stick with Obstacle Detection & SOS Alert

A comprehensive smart assistive device tailored for the visually impaired. It combines real-time **ultrasonic obstacle detection**, **GPS location tracking**, and **GSM-based SOS messaging** to enhance mobility, safety, and emergency responsiveness.

---

## 🚀 Project Overview

This project enables a blind or visually impaired person to:

- **Detect obstacles** ahead and receive alerts via sound and vibration.
- **Send SOS alerts** in emergencies via a single button press.
- **Transmit real-time location** using GPS, shared over SMS using a GSM module.

Whether used indoors or outdoors, this device increases independence and improves overall safety.

---

## 📌 Key Features

- 🔊 **Ultrasonic Obstacle Detection:** Alerts user when an object is within 50 cm.
- 📳 **Dual Alert System:** Buzzer and vibration motor for sensory feedback.
- 🆘 **SOS Button Integration:** Instantly sends SMS with real-time GPS coordinates.
- 📡 **Live GPS Tracking:** Captures and formats accurate latitude and longitude.
- 📲 **GSM Connectivity:** Sends messages using SIM800L or compatible GSM modules.
- 🖥️ **Serial Monitor Feedback:** Debug and monitor actions in real-time.

---

## 🧰 Hardware Requirements

| Component                     | Description                          |
|-------------------------------|--------------------------------------|
| Arduino UNO / Nano            | Microcontroller board                |
| Ultrasonic Sensor (HC-SR04)   | Detects obstacles                    |
| Buzzer                        | Provides audible alert               |
| Vibration Motor (via transistor) | Provides tactile feedback         |
| Push Button                   | Triggers SOS alert                   |
| GPS Module (e.g., NEO-6M)     | Captures location data               |
| GSM Module (e.g., SIM800L)    | Sends SMS messages                   |
| Power Supply                  | 9V battery or USB                    |
| Jumper Wires & Breadboard     | For circuit prototyping              |

---

## 🔌 Arduino Pin Configuration

| Device               | Arduino Pin       |
|----------------------|-------------------|
| Ultrasonic Trig      | D9                |
| Ultrasonic Echo      | D10               |
| Buzzer               | D5                |
| Vibration Motor      | D6                |
| SOS Button           | D7 (INPUT_PULLUP) |
| GPS Module (TX/RX)   | D4 (TX), D3 (RX)  |
| GSM Module (TX/RX)   | D8 (TX), D2 (RX)  |

---

## 🔁 Functional Flow

### 1. Initialization
- System initializes all components and shows status on the serial monitor.

### 2. Obstacle Monitoring
- Continuously checks for nearby obstacles using ultrasonic sensor.
- If an object is detected within 50 cm:
  - Activates buzzer and vibration motor.

### 3. SOS Emergency Alert
- When the SOS button is pressed:
  - Retrieves GPS coordinates.
  - Sends an SOS SMS message to the predefined emergency contact.

### 4. Status Logging
- Displays “All Clear” every 5 seconds if no obstacle is detected.

---

## 🧠 Arduino Libraries Required

Install the following libraries via **Arduino Library Manager**:

- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)
- SoftwareSerial (preinstalled with Arduino IDE)

---

## 🛠️ Setup Instructions

1. **Build the circuit** using the components listed above.
2. **Connect** all modules as per the pin mapping table.
3. In the code, **replace** the placeholder phone number:
   ```cpp
   gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\"");
   ```
4. **Upload** the code to your Arduino board via USB.
5. **Open Serial Monitor** at 9600 baud for live debug output.
6. **Test**:
   - Place an object close to trigger alerts.
   - Press SOS button to send location SMS.

---

## 📦 Sample Serial Monitor Output

```
🔰 Blind Smart SOS System Initializing...
✅ Ready! Monitoring started...

✅ Status: All clear. No obstacles nearby.
⏱️ Waiting for next scan...

⚠️ Obstacle Detected!
📏 Distance: 35 cm

🆘 SOS Button Pressed!
📲 Sending SMS...
🚨 SOS ALERT!
📍 Location: Lat: 22.5726, Lon: 88.3639
✅ Message sent!
```

---

## 🚨 Safety Notes

- Use a **regulated 3.7V–4.2V** power source for the GSM module to avoid damage.
- Make sure GSM SIM card has **valid SMS balance** and is properly inserted.
- GPS modules need **open sky** for accurate positioning (may take 30–60 sec to lock).

---

## 📘 Future Enhancements

- 🔋 Battery level monitoring and auto-shutdown.
- 🌐 Cloud sync via WiFi for location logs.
- 🧭 Digital compass and route guidance.
- 🎤 Audio output via text-to-speech module.

---

## 📝 License

This project is made during Project-A-Thon.

---

## 🙌 Acknowledgements

- SIM800L documentation and GSM AT command reference

