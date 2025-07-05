# 🛰️ GeoLinker GPS Tracker – ESP32 Real-Time Location Logger

A comprehensive real-time GPS tracking solution for ESP32 microcontrollers with cloud connectivity and offline storage capabilities.

## 📌 Features

- **Real-time GPS tracking** with NMEA data parsing
- **WiFi connectivity** for seamless data transmission to GeoLinker cloud
- **Offline data storage** with configurable buffer limits
- **Automatic reconnection** capabilities for network resilience
- **Timezone offset** configuration for accurate timestamps
- **Multiple debug levels** for development and troubleshooting
- **Status monitoring** with detailed error codes and feedback
- **Configurable update intervals** for optimized battery usage
- **Memory-efficient** offline buffering for resource-constrained devices

## 🧰 Hardware Requirements

| Component | Specification | Purpose |
|-----------|---------------|---------|
| **ESP32 Development Board** | Any ESP32 variant | Main microcontroller |
| **GPS Module** | NMEA compatible (e.g., NEO-6M, NEO-8M) | Location data source |
| **Jumper Wires** | Male-to-female | GPIO connections |
| **Breadboard** (Optional) | Half/full size | Prototype connections |
| **Power Supply** | 3.3V/5V | GPS module power |
| **WiFi Network** | 2.4GHz compatible | Data transmission |

### Software Requirements
- Arduino IDE with ESP32 board support
- GeoLinker library
- ESP32 Arduino Core

## 🔌 Wiring & Pinout

| ESP32 GPIO | GPS Module Pin | Signal | Wire Color (Suggested) |
|------------|----------------|--------|------------------------|
| **GPIO16** | TX | GPS → ESP32 Data | Yellow |
| **GPIO17** | RX | ESP32 → GPS Data | Orange |
| **3.3V** | VCC | Power Supply | Red |
| **GND** | GND | Ground | Black |

```
ESP32                    GPS Module
┌─────────────┐         ┌─────────────┐
│    GPIO16   │◄────────┤     TX      │
│    GPIO17   │─────────►│     RX      │
│     3.3V    │─────────►│     VCC     │
│     GND     │─────────►│     GND     │
└─────────────┘         └─────────────┘
```

## 📁 Code Overview

The project is structured as a single Arduino sketch with comprehensive configuration options and robust error handling. Key components include GPS serial interface, WiFi connectivity, cloud API integration, and offline data management.

### 🔐 Configuration Parameters

```cpp
// WiFi Network Credentials
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

// GeoLinker Cloud API
const char* apiKey = "jDzlBeB3enDB";
const char* deviceID = "ESP-32_Tracker";

// Tracking Settings
const uint16_t updateInterval = 2;        // Seconds between updates
const uint8_t offlineBufferLimit = 20;    // Max offline records
const bool enableOfflineStorage = true;   // Offline capability
const bool enableAutoReconnect = true;    // Auto WiFi reconnect

// Timezone Configuration
const int8_t timeOffsetHours = 5;         // UTC+5:30 (IST)
const int8_t timeOffsetMinutes = 30;

// Hardware Settings
#define GPS_RX 16                         // ESP32 pin for GPS data
#define GPS_TX 17                         // ESP32 pin for GPS commands
#define GPS_BAUD 9600                     // Standard NMEA baud rate
```

### Debug Levels
- `DEBUG_NONE`: No debug output
- `DEBUG_BASIC`: Essential status information
- `DEBUG_VERBOSE`: Detailed debugging information

## ⚙️ How It Works

The GeoLinker GPS Tracker operates in two main stages: initialization and continuous monitoring.

### 🔧 Setup Stage

1. **Serial Communication Init**: Establishes debug output (115200 baud) and GPS communication (9600 baud)
2. **GeoLinker Library Init**: Configures the tracking library with API credentials and device settings
3. **GPS Module Setup**: Initializes hardware serial communication on custom GPIO pins
4. **WiFi Connection**: Attempts to connect to configured network with fallback to offline mode
5. **Configuration Apply**: Sets update intervals, debug levels, timezone, and storage parameters

### 🔄 Loop Stage

1. **GPS Data Reading**: Continuously monitors GPS serial port for NMEA sentences
2. **Data Validation**: Parses and validates GPS coordinates, timestamp, and satellite information
3. **Network Check**: Verifies WiFi connectivity and attempts reconnection if needed
4. **Cloud Transmission**: Sends location data to GeoLinker cloud service via HTTP/HTTPS
5. **Offline Storage**: Buffers data locally when network is unavailable
6. **Status Reporting**: Provides real-time feedback via serial monitor
7. **Error Handling**: Manages GPS errors, network issues, and API response codes

## 🧪 Status Codes

| Status Code | Symbol | Description | Recommended Action |
|-------------|--------|-------------|-------------------|
| `STATUS_SENT` | ✓ | Data transmitted successfully | Normal operation |
| `STATUS_GPS_ERROR` | ✗ | GPS module connection error | Check wiring and power |
| `STATUS_NETWORK_ERROR` | ⚠ | Network connectivity issue | Verify WiFi settings |
| `STATUS_BAD_REQUEST_ERROR` | ✗ | Server rejected request | Check API key validity |
| `STATUS_PARSE_ERROR` | ✗ | GPS data parsing error | Verify GPS module function |
| `STATUS_INTERNAL_SERVER_ERROR` | ✗ | GeoLinker server error | Retry after delay |

## 📟 Sample Serial Output

```
Starting GeoLinker GPS Tracker...
GPS Serial initialized on pins 16(RX) and 17(TX)
GeoLinker library initialized
API key configured
Device ID set
Update interval set to 2 seconds
Debug level set to BASIC
Offline storage enabled
Auto-reconnect enabled
Offline buffer limit set to 20 records
Timezone offset set to UTC+5:30
Network mode set to WiFi
WiFi credentials configured
Connecting to WiFi network: YourWiFiName
WiFi connected successfully!

==================================================
GeoLinker GPS Tracker setup complete!
Starting main tracking loop...
==================================================

[STATUS] GeoLinker Operation: ✓ Data transmitted successfully to cloud!
[STATUS] GeoLinker Operation: ✓ Data transmitted successfully to cloud!
[STATUS] GeoLinker Operation: ⚠ Network connectivity issue - Data buffered offline
[STATUS] GeoLinker Operation: ✓ Data transmitted successfully to cloud!
```

## 🛠 Setup Instructions

1. **Install Arduino IDE** with ESP32 board support
2. **Install GeoLinker Library** through Library Manager or manual installation
3. **Wire Hardware** according to pinout diagram above
4. **Configure Code**:
   - Update WiFi credentials (`ssid`, `password`)
   - Set GeoLinker API key (`apiKey`)
   - Customize device ID (`deviceID`)
   - Adjust timezone offset if needed
5. **Upload Code** to ESP32 board
6. **Open Serial Monitor** (115200 baud) to view status
7. **Test GPS Fix** - Place device with clear sky view
8. **Verify Cloud Connection** - Check GeoLinker dashboard for incoming data

## 🌍 Example GeoLinker Payload (Cloud Format)

```json
{
  "device_id": "ESP-32_Tracker",
  "timestamp": "2025-06-28T10:30:45+05:30",
  "location": {
    "latitude": 22.5726,
    "longitude": 88.3639,
    "altitude": 12.5,
    "accuracy": 3.2
  },
  "gps_info": {
    "satellites": 8,
    "hdop": 1.2,
    "fix_quality": 1
  },
  "device_status": {
    "battery_level": 85,
    "signal_strength": -45,
    "offline_records": 0
  }
}
```

## 🔧 Customization Options

### Adjust Update Frequency
```cpp
const uint16_t updateInterval = 5; // Send data every 5 seconds
```

### Modify Offline Storage
```cpp
const uint8_t offlineBufferLimit = 50; // Store up to 50 records
```

### Change Debug Level
```cpp
geo.setDebugLevel(DEBUG_VERBOSE); // More detailed output
```

### Configure Timezone
```cpp
const int8_t timeOffsetHours = -5;   // UTC-5 (EST)
const int8_t timeOffsetMinutes = 0;  // No minute offset
```

## 🛠️ Troubleshooting

### GPS Issues
- Verify GPS module connections
- Ensure GPS module has clear sky view
- Check baud rate settings (9600 bps)
- Verify power supply to GPS module

### WiFi Issues
- Check WiFi credentials
- Verify network connectivity
- Ensure ESP32 is within WiFi range
- Check for network firewall restrictions

### Data Transmission Issues
- Verify GeoLinker API key
- Check internet connectivity
- Ensure proper device ID format
- Monitor serial output for error messages

## 📘 Future Improvements

- **Battery Monitoring**: Add voltage sensing for low-power alerts
- **GSM/LTE Support**: Cellular connectivity for areas without WiFi
- **Geofencing**: Trigger alerts when entering/leaving defined areas
- **Data Encryption**: Secure data transmission with SSL/TLS
- **Mobile App**: Companion app for real-time tracking visualization
- **Multi-Sensor Support**: Temperature, humidity, accelerometer integration
- **Power Management**: Sleep modes for extended battery life
- **SD Card Logging**: Extended offline storage capability

## ❗ Notes & Best Practices

- **GPS Cold Start**: Initial GPS fix may take 30-60 seconds outdoors
- **Memory Management**: Keep `offlineBufferLimit` low (≤50) to prevent ESP32 crashes
- **WiFi Range**: Ensure stable WiFi signal for consistent data transmission
- **API Rate Limits**: Respect GeoLinker API rate limits to avoid service suspension
- **Power Supply**: Use stable 3.3V supply for GPS module to prevent data corruption
- **Debugging**: Start with `DEBUG_BASIC` level, use `DEBUG_VERBOSE` only for troubleshooting
- **Network Security**: Use WPA2/WPA3 encrypted WiFi networks for security
- **Backup Configuration**: Save API keys and device IDs in secure location

## 📚 Library Dependencies

- **GeoLinker.h**: Main GPS tracking and cloud connectivity library
- **ESP32 Arduino Core**: Hardware abstraction layer
- **Built-in WiFi libraries**: For network connectivity

## 🧾 License

This project is provided as-is for educational and commercial use. Please check the GeoLinker library license for specific terms and conditions.

## 🙌 Credits

- **Author**: CircuitDigest/Rithik_Krisna_M
- **Version**: 1.0
- **Last Modified**: 28/06/2025
- **GeoLinker Library**: GPS tracking and cloud connectivity framework
- **ESP32 Arduino Core**: Hardware abstraction layer for ESP32 platform
- **NMEA GPS Protocol**: Standardized GPS data format specification

## 🤝 Contributing

Feel free to contribute improvements, bug fixes, or additional features through pull requests.

## 📞 Support

For technical support and questions:
- Check the troubleshooting section
- Review serial monitor output for error codes
- Verify hardware connections and configurations

---

*Happy tracking with GeoLinker GPS Tracker! 🛰️📍*
