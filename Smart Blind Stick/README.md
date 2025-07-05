# 👨‍🦯 Smart Blind Stick with Obstacle Detection & SOS Alert

A smart assistive device designed for the visually impaired, integrating ultrasonic obstacle detection, GPS tracking, and GSM-based emergency SOS messaging.

---

## 📌 Features

- 🔊 **Ultrasonic Obstacle Detection:** Detects objects within 50 cm using an ultrasonic sensor and alerts via buzzer and vibration motor.
- 🆘 **SOS Emergency Button:** Sends an emergency SMS with real-time GPS location via GSM when the SOS button is pressed.
- 📡 **GPS Location Tracking:** Acquires real-time latitude and longitude coordinates using a GPS module.
- 📲 **GSM SMS Integration:** Sends formatted SOS alerts over SMS using a SIM800L or compatible GSM module.

---

## 🧰 Hardware Requirements

| Component                  | Description                          |
|----------------------------|--------------------------------------|
| Arduino UNO / Nano         | Microcontroller                      |
| Ultrasonic Sensor (HC-SR04)| For obstacle detection               |
| Buzzer                     | Audio alert for obstacles            |
| Vibration Motor (via transistor) | Haptic feedback for alerts    |
| Push Button                | SOS trigger (connected to pull-up)  |
| GPS Module (NEO-6M)        | Location tracking                    |
| GSM Module (SIM800L)       | SMS communication                    |
| Power Supply               | 9V battery / USB                     |
| Jumper Wires               | For circuit connections              |
| Breadboard or PCB          | Optional for prototyping             |

---

## 🔌 Pin Configuration

| Module           | Arduino Pin |
|------------------|-------------|
| Trig (Ultrasonic) | D9         |
| Echo (Ultrasonic) | D10        |
| Buzzer           | D5          |
| Vibration Motor  | D6          |
| SOS Button       | D7 (INPUT_PULLUP) |
| GPS RX/TX        | D4 (TX), D3 (RX) |
| GSM TX/RX        | D8 (TX), D2 (RX) |

---

## 🔁 Working Process

1. **Startup:**
   - Initializes serial, GPS, GSM, and I/O pins.
   - Displays status messages on Serial Monitor.

2. **Obstacle Detection:**
   - Constantly checks for nearby objects using the ultrasonic sensor.
   - If object detected ≤ 50 cm:
     - Buzzer sounds.
     - Vibration motor pulses.

3. **SOS Alert:**
   - Pressing the SOS button:
     - Checks GPS location.
     - Formats location string.
     - Sends SMS via GSM to emergency contact.

4. **Status Logging:**
   - If no obstacle is detected for 5 seconds:
     - Logs "All clear" status on Serial Monitor.

---

## 🧠 Libraries Required

Install the following libraries via Arduino Library Manager:

- [`TinyGPS++`](https://github.com/mikalhart/TinyGPSPlus)
- `SoftwareSerial` (built-in)

---

## 🛠️ Setup Instructions

1. Connect the hardware as per the pin configuration above.
2. Replace the placeholder phone number in the code:
   ```cpp
   gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\"");
3. Upload the code to your Arduino.

4. Open the Serial Monitor at 9600 baud.

5. Press the SOS button or trigger obstacle detection to test.

📦 Sample Output

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



🚨 Disclaimer
Always test your hardware with care, especially GSM and GPS modules which require proper voltage levels (GSM: 3.7–4.2V recommended).

SMS charges may apply depending on your GSM provider.