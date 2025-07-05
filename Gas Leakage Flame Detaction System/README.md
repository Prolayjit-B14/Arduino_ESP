# 🛰️ Gas & Fire Detection System

A comprehensive Arduino-based safety monitoring system that detects gas leaks and fire hazards with visual and audio alerts.

## 📌 Features

- **Dual Sensor Detection**: Simultaneous monitoring of gas leaks and fire/flame presence
- **Multi-Level Alert System**: Different alert patterns for gas, fire, and combined hazards
- **Visual Feedback**: RGB LED indicators with color-coded status display
- **Audio Alerts**: Buzzer with distinct sound patterns for different threat levels
- **Real-time Monitoring**: Continuous sensor readings with serial output
- **Configurable Thresholds**: Adjustable sensitivity levels for both sensors
- **Emergency Response**: Immediate high-priority alerts for critical situations

## 🧰 Hardware Requirements

- **Microcontroller**: Arduino Uno/Nano/Pro Mini
- **Gas Sensor**: MQ-2, MQ-5, or MQ-9 (analog output)
- **Flame Sensor**: IR flame detection sensor (analog output)
- **Buzzer**: Active or passive buzzer (5V compatible)
- **RGB LED**: Common cathode RGB LED
- **Resistors**: 220Ω resistors for RGB LED (3x)
- **Breadboard**: For prototyping connections
- **Jumper Wires**: Male-to-male and male-to-female
- **Power Supply**: 5V DC (USB or external adapter)

### Software Requirements

- **Arduino IDE**: Version 1.8.0 or higher
- **Board Package**: Arduino AVR Boards
- **Serial Monitor**: For debugging and status monitoring

## 🔌 Wiring & Pinout

| Component | Arduino Pin | Connection Type |
|-----------|-------------|-----------------|
| Gas Sensor | A0 | Analog Input |
| Flame Sensor | A1 | Analog Input |
| Buzzer | Pin 5 | Digital Output (PWM) |
| RGB LED (Red) | Pin 6 | Digital Output (PWM) |
| RGB LED (Green) | Pin 7 | Digital Output |
| RGB LED (Blue) | Pin 8 | Digital Output |

**Additional Connections:**
- Gas Sensor: VCC to 5V, GND to GND
- Flame Sensor: VCC to 5V, GND to GND
- RGB LED: Common cathode to GND through appropriate resistors

## 📁 Code Overview

The system operates on a continuous monitoring loop with the following core components:

- **Sensor Reading**: Analog values from gas and flame sensors
- **Threshold Comparison**: Real-time hazard detection
- **Alert Management**: Coordinated visual and audio responses
- **Status Reporting**: Serial communication for monitoring

### 🔐 Configuration Parameters

```cpp
int gasThreshold = 50;      // Gas detection sensitivity
int flameThreshold = 500;   // Flame detection sensitivity
```

### Debug Levels

- **Level 1**: Basic sensor readings
- **Level 2**: Threshold comparison results
- **Level 3**: Alert system status
- **Level 4**: Detailed system diagnostics

## ⚙️ How It Works

The system continuously monitors environmental conditions and responds to detected hazards with appropriate alert mechanisms.

### 🔧 Setup Stage

1. **Pin Configuration**: Initialize all input/output pins
2. **Serial Communication**: Establish 9600 baud rate connection
3. **System Reset**: Clear all outputs and set safe state
4. **Startup Confirmation**: Display ready message

### 🔄 Loop Stage

1. **Sensor Reading**: Read analog values from both sensors
2. **Data Processing**: Compare readings against thresholds
3. **Hazard Classification**: Determine threat level and type
4. **Alert Activation**: Trigger appropriate response pattern
5. **Status Update**: Output current system state
6. **Delay Cycle**: 2-second monitoring interval

## 🧪 Status Codes

| Status | LED Color | Buzzer Pattern | Description |
|--------|-----------|----------------|-------------|
| Safe | Green | Silent | Normal operation |
| Gas Detected | Blue | Intermittent Beep | Gas leak detected |
| Fire Detected | Red | Continuous Siren | Flame/fire detected |
| Critical Alert | Red/Blue Blink | Loud Alarm | Both hazards present |

## 📟 Sample Serial Output

```
🔥 Gas & Fire Detection System Ready!
Gas: 25 | Flame: 800
✅ Safe Environment
Gas: 75 | Flame: 750
⚠️ Gas Leak Detected!
Gas: 30 | Flame: 300
🔥 Fire Detected!
Gas: 85 | Flame: 250
🚨 BOTH GAS & FIRE DETECTED! IMMEDIATE ACTION REQUIRED!
```

## 🛠 Setup Instructions

1. **Hardware Assembly**:
   - Connect sensors to analog pins A0 and A1
   - Wire RGB LED to pins 6, 7, and 8 with current-limiting resistors
   - Connect buzzer to pin 5
   - Ensure proper power and ground connections

2. **Software Installation**:
   - Open Arduino IDE
   - Copy and paste the provided code
   - Select appropriate board and port
   - Upload the sketch to your Arduino

3. **Calibration**:
   - Monitor serial output for baseline readings
   - Adjust threshold values based on your environment
   - Test with controlled gas and flame sources

## 🌍 Example GeoLinker Payload (Cloud Format)

```json
{
  "device_id": "gas_fire_detector_01",
  "timestamp": "2025-07-05T14:30:00Z",
  "location": {
    "lat": 23.4057,
    "lon": 88.3741
  },
  "sensors": {
    "gas_level": 75,
    "flame_level": 300,
    "status": "gas_detected"
  },
  "alerts": {
    "active": true,
    "type": "gas_leak",
    "priority": "medium"
  }
}
```

## 🔧 Customization Options

### Adjust Update Frequency

Modify the delay value in the main loop:
```cpp
delay(2000);  // Change to desired interval (milliseconds)
```

### Modify Offline Storage

Add EEPROM storage for threshold values:
```cpp
#include <EEPROM.h>
// Store thresholds in EEPROM for persistence
```

### Change Debug Level

Add debug level control:
```cpp
int debugLevel = 2;  // 0=Silent, 1=Basic, 2=Detailed
```

### Configure Timezone

For timestamped logging:
```cpp
#define TIMEZONE_OFFSET +5.5  // GMT+5:30 for India
```

## 🛠️ Troubleshooting

### GPS Issues

*Note: This system doesn't use GPS, but for location-based variants:*
- Ensure clear sky visibility
- Check antenna connections
- Verify power supply stability

### WiFi Issues

*For IoT-enabled versions:*
- Verify network credentials
- Check signal strength
- Ensure proper antenna placement

### Data Transmission Issues

- **Serial Communication**: Check baud rate settings
- **Sensor Readings**: Verify analog pin connections
- **Power Issues**: Ensure stable 5V supply

## 📘 Future Improvements

- **IoT Integration**: WiFi connectivity for remote monitoring
- **Data Logging**: SD card storage for historical data
- **Mobile Alerts**: SMS/email notifications
- **Multiple Sensors**: Support for additional gas types
- **Calibration Mode**: Automated sensor calibration
- **Battery Backup**: Uninterrupted operation during power outages
- **Web Dashboard**: Real-time monitoring interface

## ❗ Notes & Best Practices

- **Sensor Warm-up**: Allow 2-3 minutes for gas sensors to stabilize
- **Ventilation**: Ensure proper airflow around sensors
- **Calibration**: Regular calibration in clean air environment
- **Power Supply**: Use stable power source for accurate readings
- **Safety**: This system is for monitoring only - not a replacement for certified safety equipment
- **Testing**: Regular testing with known gas sources (safely)

## 📚 Library Dependencies

- **Arduino Core Libraries**: Built-in functions only
- **No External Libraries**: Uses standard Arduino functions
- **Optional Enhancements**:
  - WiFi library for IoT features
  - EEPROM library for persistent storage
  - Time library for timestamps

## 🧾 License

This project is released under the MIT License. Feel free to modify and distribute according to your needs.

## 🙌 Credits

- **Arduino Community**: For extensive documentation and examples
- **Sensor Manufacturers**: For detailed datasheets and specifications
- **Open Source Contributors**: For inspiration and code examples

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for:
- Bug fixes and improvements
- New sensor support
- Additional alert mechanisms
- Documentation enhancements

## 📞 Support

For technical support and questions:
- **GitHub Issues**: Report bugs and request features
- **Arduino Forums**: Community support and discussions
- **Documentation**: Refer to sensor datasheets for specific details
- **Email**: Contact project maintainers for direct support

---

**⚠️ Safety Disclaimer**: This system is designed for educational and monitoring purposes. It should not be used as the sole safety system in critical applications. Always follow local safety regulations and use certified safety equipment for life-critical applications.
