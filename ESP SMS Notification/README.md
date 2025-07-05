# 🛰️ WiFi SMS Notification System

An ESP32-based wireless communication system that sends SMS notifications via HTTP API using WiFi connectivity and secure HTTPS protocols.

## 📌 Features

- **WiFi Connectivity**: Automatic connection to wireless networks
- **SMS API Integration**: Cloud-based SMS service integration
- **Secure HTTPS Communication**: SSL/TLS encrypted API calls
- **Template-Based Messaging**: Customizable SMS templates with variables
- **Real-time Status Monitoring**: Serial output for connection and transmission status
- **Error Handling**: Comprehensive error reporting and debugging
- **Automated Notifications**: Trigger-based SMS sending capability
- **Mobile Number Management**: Support for multiple recipient numbers

## 🧰 Hardware Requirements

- **Microcontroller**: ESP32 DevKit v1 or ESP32-WROOM-32
- **WiFi Module**: Built-in ESP32 WiFi (802.11 b/g/n)
- **Antenna**: PCB trace antenna or external antenna
- **Power Supply**: 5V USB or 3.3V regulated power
- **Development Board**: ESP32 development board with USB-to-Serial converter
- **Status LEDs**: Optional for visual connection indicators
- **Reset Button**: Hardware reset capability

### Software Requirements

- **Arduino IDE**: Version 1.8.19 or higher with ESP32 board package
- **ESP32 Board Package**: Version 2.0.0 or higher
- **WiFi Library**: ESP32 WiFi (built-in)
- **HTTPClient Library**: ESP32 HTTPClient (built-in)
- **WiFiClientSecure Library**: ESP32 secure client (built-in)

## 🔌 Wiring & Pinout

| Component | ESP32 Pin | Connection Type |
|-----------|-----------|-----------------|
| Power | 5V/3.3V | Power Input |
| Ground | GND | Ground Connection |
| Status LED (Optional) | GPIO2 | Digital Output |
| Reset Button | EN | Hardware Reset |
| Programming | GPIO0 | Boot Mode Selection |

**WiFi Configuration:**
- Built-in WiFi module (no external wiring required)
- PCB antenna or U.FL connector for external antenna

## 📁 Code Overview

The system establishes WiFi connectivity and sends SMS notifications through a cloud-based API service with the following architecture:

- **Network Connection**: WiFi authentication and connection management
- **API Communication**: HTTPS POST requests to SMS service
- **Data Formatting**: JSON payload construction for API calls
- **Response Handling**: HTTP status code processing and error management

### 🔐 Configuration Parameters

```cpp
const char* ssid = "Redmi Note 11 Pro+ 5G";     // WiFi network name
const char* password = "@polu1411P";            // WiFi password
const char* apiKey = "jDzlBeB3enDB";            // SMS API authorization key
const char* templateID = "104";                 // SMS template identifier
const char* mobileNumber = "917364098822";      // Recipient phone number
const char* var1 = "GPS TRACKER";               // Template variable 1
const char* var2 = "20";                        // Template variable 2
```

### Debug Levels

- **Level 0**: Silent operation (no serial output)
- **Level 1**: Basic connection status
- **Level 2**: HTTP response codes and messages
- **Level 3**: Full payload and response debugging
- **Level 4**: Detailed network diagnostics

## ⚙️ How It Works

The system connects to WiFi and sends SMS notifications through a secure API endpoint using template-based messaging.

### 🔧 Setup Stage

1. **Serial Initialization**: Configure 9600 baud rate for debugging
2. **WiFi Connection**: Authenticate with network credentials
3. **Connection Verification**: Wait for successful network association
4. **Initial SMS**: Send test message to verify system functionality

### 🔄 Loop Stage

1. **Standby Mode**: System waits for trigger events
2. **Event Detection**: Monitor for notification triggers
3. **SMS Preparation**: Format message with current variables
4. **API Communication**: Send HTTPS POST request
5. **Response Processing**: Handle success/failure responses
6. **Status Reporting**: Log transmission results

## 🧪 Status Codes

| HTTP Code | Status | Description |
|-----------|--------|-------------|
| 200 | Success | SMS sent successfully |
| 400 | Bad Request | Invalid payload or parameters |
| 401 | Unauthorized | Invalid API key |
| 403 | Forbidden | Insufficient permissions |
| 429 | Rate Limited | Too many requests |
| 500 | Server Error | API service unavailable |

## 📟 Sample Serial Output

```
Connecting to wifi......
Connected!
SMS sent successfully!
{
  "status": "success",
  "message": "SMS queued for delivery",
  "message_id": "MSG_12345",
  "credits_remaining": 47
}
```

## 🛠 Setup Instructions

1. **Hardware Preparation**:
   - Connect ESP32 to computer via USB cable
   - Ensure proper power supply (5V/1A minimum)
   - Verify antenna connection (if using external antenna)

2. **Software Configuration**:
   - Install Arduino IDE with ESP32 board package
   - Select "ESP32 Dev Module" as board type
   - Configure appropriate COM port and upload speed

3. **Code Customization**:
   - Update WiFi credentials (SSID and password)
   - Replace API key with your SMS service credentials
   - Modify template ID and variables as needed
   - Update recipient mobile number

4. **Testing**:
   - Upload code to ESP32
   - Monitor serial output for connection status
   - Verify SMS delivery to recipient number


## 🔧 Customization Options

### Adjust Update Frequency

Add periodic SMS sending:
```cpp
unsigned long lastSMS = 0;
unsigned long smsInterval = 300000; // 5 minutes

if (millis() - lastSMS > smsInterval) {
  sendSMS();
  lastSMS = millis();
}
```

### Modify Offline Storage

Store SMS queue in SPIFFS:
```cpp
#include <SPIFFS.h>
// Store failed SMS for retry when connection restored
```

### Change Debug Level

Control serial output verbosity:
```cpp
#define DEBUG_LEVEL 2  // 0=Silent, 1=Basic, 2=Detailed, 3=Full
```

### Configure Timezone

Add timezone support for timestamps:
```cpp
#include <time.h>
configTime(19800, 0, "pool.ntp.org"); // GMT+5:30 for India
```

## 🛠️ Troubleshooting


### WiFi Issues

- **Connection Failures**: Verify SSID and password accuracy
- **Weak Signal**: Check router proximity and antenna orientation
- **IP Assignment**: Ensure DHCP is enabled on router
- **Firewall**: Check if outbound HTTPS is blocked
  
### GPS Issues

*For location-based SMS systems:*
- Ensure clear sky visibility for GPS module
- Check GPS module connections and power
- Verify GPS baud rate and serial configuration
  
### Data Transmission Issues

- **API Errors**: Verify API key validity and account balance
- **HTTPS Issues**: Check certificate validation settings
- **Timeout Problems**: Increase HTTP client timeout values
- **JSON Formatting**: Validate payload structure

## 📘 Future Improvements

- **Multiple Recipients**: Support for SMS distribution lists
- **Message Queuing**: Offline message storage and retry mechanism
- **Sensor Integration**: Automatic SMS based on sensor triggers
- **Two-way Communication**: SMS command processing capability
- **Encryption**: End-to-end message encryption
- **Location Services**: GPS coordinates in SMS messages
- **Battery Monitoring**: Low power alerts and deep sleep mode
- **Web Interface**: Configuration portal for settings management

## ❗ Notes & Best Practices

- **API Limits**: Monitor SMS credit balance and rate limits
- **Security**: Keep API keys secure and rotate regularly
- **Network Stability**: Implement connection retry mechanisms
- **Power Management**: Use deep sleep for battery-powered applications
- **Testing**: Test with different network conditions and locations
- **Compliance**: Ensure SMS content complies with local regulations
- **Error Handling**: Implement comprehensive error logging

## 📚 Library Dependencies

- **WiFi.h**: ESP32 WiFi connectivity (built-in)
- **HTTPClient.h**: HTTP/HTTPS client functionality (built-in)
- **WiFiClientSecure.h**: Secure socket layer communication (built-in)
- **ArduinoJson.h**: JSON parsing and generation (optional)
- **SPIFFS.h**: File system for offline storage (optional)

## 🧾 License

This project is released under the MIT License. Commercial use permitted with proper attribution.

## 🙌 Credits

- **ESP32 Community**: For extensive WiFi and HTTP examples
- **Arduino Foundation**: For comprehensive library documentation
- **SMS Service Providers**: For API integration examples
- **Open Source Contributors**: For security and connectivity improvements

## 🤝 Contributing

Contributions welcome for:
- Additional SMS service provider integrations
- Enhanced error handling and retry mechanisms
- Power optimization features
- Security improvements
- Documentation updates

## 📞 Support

For technical assistance:
- **GitHub Issues**: Bug reports and feature requests
- **ESP32 Forums**: Community support and troubleshooting
- **SMS Provider Support**: API-specific issues and limitations

---

**⚠️ Security Notice**: This system transmits sensitive data over WiFi networks. Ensure proper network security, use strong authentication, and keep API credentials secure. Monitor API usage to prevent unauthorized access and unexpected charges.
