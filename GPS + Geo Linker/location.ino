/* 
 * ===============================================================
 *              ESP32 GPS TRACKER WITH ALERTS SYSTEM
 * ===============================================================
 * 
 * FEATURES:
 * - Real-time GPS tracking with NMEA data parsing
 * - WiFi connectivity for data transmission
 * - Email alerts with Google Maps preview
 * - SMS alerts via CircuitDigest API
 * - GeoLinker cloud integration
 * - Offline data storage capability
 * - Automatic reconnection capabilities
 * - Timezone offset configuration
 * - Periodic location updates
 * 
 * HARDWARE REQUIREMENTS:
 * - ESP32 development board
 * - GPS module (NMEA compatible)
 * - WiFi network access
 * 
 * PIN CONNECTIONS:
 * - GPS RX: GPIO16
 * - GPS TX: GPIO17
 * 
 * Author: SEMICOLON
 * Version: 2.2 (Fixed SMS/Email alerts)
 * Last Modified: 09/07/2025
 * 
 * ===============================================================
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP_Mail_Client.h>
#include <GeoLinker.h>
#include <time.h>
#include <TinyGPS++.h>
// ==================================================================
//                    HARDWARE CONFIGURATION
// ==================================================================

// GPS Serial Communication Setup
HardwareSerial gpsSerial(1);
#define GPS_RX 16
#define GPS_TX 17
#define GPS_BAUD 9600

// ==================================================================
//                    NETWORK CONFIGURATION
// ==================================================================

// WiFi Network Credentials
const char* ssid = "Redmi Note 11 Pro+ 5G";
const char* password = "@polu1411P";

// ==================================================================
//                   EMAIL CONFIGURATION (Gmail SMTP)
// ==================================================================

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "prolayjitbiswas14112004@gmail.com"
#define AUTHOR_PASSWORD "lspi lxjl lkcb jajj"

// Google Maps API Key
#define GOOGLE_MAPS_API_KEY "YOUR_API_KEY_HERE"

// Email Recipients
const char* emailRecipients[][2] = {
  {"Primary Contact", "prolayjitbiswas14112004@gmail.com"},
  {"Family Member", "bubun15072006@gmail.com"},
  {"Friend", "ankanbhowmik11@gmail.com"}
};

// ==================================================================
//                   GEOLINKER CONFIGURATION
// ==================================================================

const char* geoApiKey = "jDzlBeB3enDB";
const char* deviceID = "ESP-32_GPS_Tracker";
const uint16_t updateInterval = 5;  // seconds
const bool enableOfflineStorage = true;
const uint8_t offlineBufferLimit = 20;
const bool enableAutoReconnect = true;
const int8_t timeOffsetHours = 5;
const int8_t timeOffsetMinutes = 30;

// ==================================================================
//                    FUNCTION DECLARATIONS
// ==================================================================

void connectToWiFi();
bool syncTime();
void initializeGeoLinker();
void updateGPSData();
void parseGPSData(String data);
void sendEmailAlert(String alertType, String additionalInfo = "");
String createHTMLEmailBody(String alertType, String timestamp, String locationURL, String additionalInfo);
String createEmailBody(String alertType, String timestamp, String locationURL, String additionalInfo);
String getCurrentTimestamp();
void handleGeoLinkerStatus(uint8_t status);

// ==================================================================
//                    GLOBAL VARIABLES
// ==================================================================

GeoLinker geo;
SMTPSession smtp;

// GPS Data Storage
float currentLatitude = 0.0;
float currentLongitude = 0.0;
bool gpsDataValid = false;
bool firstGPSLock = true;  // Track first GPS lock
String currentLocationName = "Unknown Location";

// Timing Variables
unsigned long lastGPSUpdate = 0;
unsigned long lastLocationSend = 0;
unsigned long lastSMSAlert = 0;
const unsigned long GPS_UPDATE_INTERVAL = 2000;  // 2 seconds
const unsigned long LOCATION_SEND_INTERVAL = 60000;  // 1 minute
const unsigned long SMS_ALERT_INTERVAL = 300000;  // 5 minutes (to avoid spam)

// ==================================================================
//                    INITIALIZATION SETUP
// ==================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== ESP32 GPS Tracker System ===");
  Serial.println("Initializing components...");

  // Initialize GPS
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("✓ GPS Serial initialized");

  // Initialize WiFi
  connectToWiFi();

  // Initialize time synchronization
  if (!syncTime()) {
    Serial.println("⚠ Time sync failed - continuing without proper timestamps");
  }

  // Initialize GeoLinker
  initializeGeoLinker();

  Serial.println("\n=== System Ready ===");
  Serial.println("GPS tracking active...");
  Serial.println("Sending periodic location updates every 1 minute");
  Serial.println("SMS alerts every 5 minutes\n");
}

// ==================================================================
//                    WIFI CONNECTION FUNCTIONS
// ==================================================================

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to WiFi");
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi connection failed! Continuing in offline mode.");
  }
}

bool syncTime() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("⏳ Syncing time");
  
  time_t now = time(nullptr);
  unsigned long start = millis();
  
  while (now < 8 * 3600 * 2 && millis() - start < 10000) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  
  if (now < 8 * 3600 * 2) {
    Serial.println("\n❌ Time sync failed.");
    return false;
  }
  
  Serial.println("\n✅ Time synced successfully!");
  return true;
}

// ==================================================================
//                    GEOLINKER INITIALIZATION
// ==================================================================

void initializeGeoLinker() {
  geo.begin(gpsSerial);
  geo.setApiKey(geoApiKey);
  geo.setDeviceID(deviceID);
  geo.setUpdateInterval_seconds(updateInterval);
  geo.setDebugLevel(DEBUG_BASIC);
  geo.enableOfflineStorage(enableOfflineStorage);
  geo.enableAutoReconnect(enableAutoReconnect);
  geo.setOfflineBufferLimit(offlineBufferLimit);
  geo.setTimeOffset(timeOffsetHours, timeOffsetMinutes);
  geo.setNetworkMode(GEOLINKER_WIFI);
  geo.setWiFiCredentials(ssid, password);
  
  if (WiFi.status() == WL_CONNECTED) {
    if (geo.connectToWiFi()) {
      Serial.println("✓ GeoLinker WiFi connected");
    } else {
      Serial.println("⚠ GeoLinker WiFi connection failed");
    }
  }
  
  Serial.println("✓ GeoLinker initialized");
}

// ==================================================================
//                    GPS DATA PROCESSING
// ==================================================================

void updateGPSData() {
  if (millis() - lastGPSUpdate > GPS_UPDATE_INTERVAL) {
    // Read GPS data from serial
    while (gpsSerial.available()) {
      String gpsData = gpsSerial.readStringUntil('\n');
      if (gpsData.startsWith("$GPGGA") || gpsData.startsWith("$GPRMC")) {
        parseGPSData(gpsData);
      }
    }
    lastGPSUpdate = millis();
  }
}

void parseGPSData(String data) {
  // Simple NMEA parsing for demonstration
  // In production, use a proper NMEA library
  if (data.startsWith("$GPGGA")) {
    // Parse GGA sentence for lat/lon
    int commaIndex = 0;
    String parts[15];
    int partIndex = 0;
    
    for (int i = 0; i < data.length() && partIndex < 15; i++) {
      if (data[i] == ',') {
        partIndex++;
      } else {
        parts[partIndex] += data[i];
      }
    }
    
    if (parts[2].length() > 0 && parts[4].length() > 0) {
      // Convert DDMM.MMMM to decimal degrees
      float lat = parts[2].substring(0, 2).toFloat() + 
                  parts[2].substring(2).toFloat() / 60.0;
      float lon = parts[4].substring(0, 3).toFloat() + 
                  parts[4].substring(3).toFloat() / 60.0;
      
      if (parts[3] == "S") lat = -lat;
      if (parts[5] == "W") lon = -lon;
      
      currentLatitude = lat;
      currentLongitude = lon;
      gpsDataValid = true;
      
      Serial.print("📍 GPS: ");
      Serial.print(currentLatitude, 6);
      Serial.print(", ");
      Serial.println(currentLongitude, 6);
      
      // Send first GPS lock alert
      if (firstGPSLock) {
        firstGPSLock = false;
        Serial.println("🎯 First GPS lock acquired! Sending initial alerts...");
        sendEmailAlert("GPS_LOCK", "First GPS lock acquired successfully");
      }
    }
  }
}


// ==================================================================
//                WOMEN SAFETY SOS EMAIL ALERT SYSTEM
// ==================================================================

// Global variables for location caching
String lastKnownAddress = "";
unsigned long lastAddressUpdateTime = 0;
const unsigned long ADDRESS_UPDATE_INTERVAL = 300000; // 5 minutes

// Enhanced reverse geocoding with fallback services
String getAddressFromCoordinates(double lat, double lng) {
  if (WiFi.status() != WL_CONNECTED) {
    return "Network unavailable";
  }
  
  // Check cache first
  unsigned long currentTime = millis();
  if (currentTime - lastAddressUpdateTime < ADDRESS_UPDATE_INTERVAL && lastKnownAddress.length() > 0) {
    return lastKnownAddress;
  }
  
  // Try multiple services for better reliability
  String address = tryNominatimGeocoding(lat, lng);
  if (address.length() > 10) {
    lastKnownAddress = address;
    lastAddressUpdateTime = currentTime;
    return address;
  }
  

}

String tryNominatimGeocoding(double lat, double lng) {
  WiFiClientSecure client;
  client.setInsecure();
  
  String url = "/reverse?format=json&lat=" + String(lat, 6) + "&lon=" + String(lng, 6) + "&zoom=16&addressdetails=1";
  
  if (client.connect("nominatim.openstreetmap.org", 443)) {
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: nominatim.openstreetmap.org");
    client.println("User-Agent: SOS-Emergency-System/1.0");
    client.println("Connection: close");
    client.println();
    
    String response = "";
    bool headersEnded = false;
    
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        if (!headersEnded) {
          if (line == "\r") {
            headersEnded = true;
          }
        } else {
          response += line;
        }
      }
    }
    client.stop();
    
    // Parse JSON response
    int addressStart = response.indexOf("\"display_name\":\"");
    if (addressStart != -1) {
      addressStart += 16;
      int addressEnd = response.indexOf("\"", addressStart);
      if (addressEnd != -1) {
        return response.substring(addressStart, addressEnd);
      }
    }
  }
  
  return "";
}

void sendEmergencyAlert(String alertType, String emergencyDetails) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected - Cannot send emergency alert");
    return;
  }

  Serial.println("🚨 EMERGENCY ALERT: Preparing to send SOS message...");

  // Get current location
  String currentAddress = "Location unavailable";
  if (gpsDataValid) {
    Serial.println("📍 Fetching emergency location...");
    currentAddress = getAddressFromCoordinates(currentLatitude, currentLongitude);
    currentLocationName = currentAddress;
  }

  // Configure SMTP for emergency
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
  session.secure.startTLS = false;
  
  // Create emergency message
  SMTP_Message message;
  
  String locationURL = "";
  if (gpsDataValid) {
    locationURL = "https://www.google.com/maps/search/?api=1&query=" +
                  String(currentLatitude, 6) + "," + String(currentLongitude, 6);
  }

  String timestamp = getCurrentTimestamp();
  String deviceName = "SOS-DEVICE-" + String(deviceID);
  
  // URGENT subject line
  message.subject = "🚨 EMERGENCY ALERT : " + alertType + " =>  IMMEDIATE ASSISTANCE NEEDED";
  
  // Sender details
  message.sender.name = "Women Safety SOS System";
  message.sender.email = AUTHOR_EMAIL;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high; // HIGH PRIORITY
  
  // Add emergency contacts
  int recipientCount = sizeof(emailRecipients) / sizeof(emailRecipients[0]);
  for (int i = 0; i < recipientCount; i++) {
    message.addRecipient(emailRecipients[i][0], emailRecipients[i][1]);
    Serial.println("🚨 Adding emergency contact: " + String(emailRecipients[i][1]));
  }
  
  // Create professional emergency email
  String htmlBody = createEmergencyHTMLEmail(alertType, timestamp, locationURL, emergencyDetails, currentAddress);
  message.html.content = htmlBody;
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  

  // Send immediately
  Serial.println("📡 Connecting to emergency services...");
  if (!smtp.connect(&session)) {
    Serial.println("❌ Emergency connection failed: " + smtp.errorReason());
    return;
  }
  
  Serial.println("🚨 Sending emergency alert...");
  if (!MailClient.sendMail(&smtp, &message, true)) {
    Serial.println("❌ Emergency alert failed: " + smtp.errorReason());
  } else {
    Serial.println("✅ EMERGENCY ALERT SENT SUCCESSFULLY!");
  }
  
  smtp.closeSession();
}


String createEmergencyHTMLEmail(String alertType, String timestamp, String locationURL, String emergencyDetails, String currentAddress) {
  const String locationIQ_API_KEY = //"pk.6505e3b91d2cd482cf3aa33b71926917"; // Replace with secure token
  String staticMapURL = "https://maps.locationiq.com/v3/staticmap?key=" + locationIQ_API_KEY +
                        "&center=" + String(currentLatitude, 6) + "," + String(currentLongitude, 6) +
                        "&zoom=16&size=400x160&markers=icon:large-red-cutout|" +
                        String(currentLatitude, 6) + "," + String(currentLongitude, 6) + "&format=png";

  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Emergency Alert</title>";
  html += "<style>";
  // [Insert your full optimized CSS block here — you already have it well-structured.]
  // Core styling - professional but urgent
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Arial, sans-serif; background: #1a1a1a; color: #333; line-height: 1.4; }";
  html += ".email-container { max-width: 480px; margin: 0 auto; background: #ffffff; min-height: 100vh; position: relative; }";
  
  // Critical header - immediate attention grabber
  html += ".emergency-header { background: linear-gradient(135deg, #ff1744 0%, #d50000 100%); color: white; padding: 24px 20px; text-align: center; position: relative; box-shadow: 0 4px 20px rgba(255, 23, 68, 0.4); }";
  html += ".emergency-header::before { content: '🚨'; font-size: 28px; position: absolute; left: 20px; top: 50%; transform: translateY(-50%); animation: pulse 1s infinite; }";
  html += ".emergency-header h1 { font-size: 22px; font-weight: 800; margin-bottom: 4px; text-shadow: 0 2px 4px rgba(0,0,0,0.3); }";
  html += ".emergency-header .subtitle { font-size: 13px; opacity: 0.95; font-weight: 500; }";
  
  // Alert badge - emotional impact
  html += ".alert-status { background: #fff3e0; border-left: 6px solid #ff6d00; padding: 16px 20px; margin: 0; }";
  html += ".alert-status h2 { color: #e65100; font-size: 18px; font-weight: 700; margin-bottom: 4px; }";
  html += ".alert-status p { color: #bf360c; font-size: 14px; font-weight: 500; }";
  
  // Emergency info - scannable layout
  html += ".info-block { padding: 20px; background: #fafafa; border-bottom: 1px solid #e0e0e0; }";
  html += ".info-row { display: flex; align-items: center; margin-bottom: 12px; }";
  html += ".info-row:last-child { margin-bottom: 0; }";
  html += ".info-icon { font-size: 16px; margin-right: 12px; width: 20px; }";
  html += ".info-label { font-weight: 600; color: #424242; min-width: 70px; margin-right: 12px; font-size: 14px; }";
  html += ".info-value { color: #1a1a1a; font-size: 14px; flex: 1; }";
  
  // Location section - most critical
  html += ".location-section { background: #e8f5e8; border-left: 6px solid #2e7d32; padding: 20px; }";
  html += ".location-header { display: flex; align-items: center; margin-bottom: 16px; }";
  html += ".location-header h3 { color: #1b5e20; font-size: 16px; font-weight: 700; margin-left: 8px; }";
  html += ".address-text { background: white; padding: 12px; border-radius: 6px; margin-bottom: 16px; border: 1px solid #c8e6c9; }";
  html += ".address-text strong { color: #1b5e20; }";
  
  // Map preview - clickable and functional
  html += ".map-preview { position: relative; margin-bottom: 16px; border-radius: 8px; overflow: hidden; box-shadow: 0 2px 8px rgba(0,0,0,0.15); }";
  html += ".map-preview img { width: 100%; height: 160px; object-fit: cover; display: block; }";
  html += ".map-overlay { position: absolute; top: 0; left: 0; right: 0; bottom: 0; background: rgba(0,0,0,0.1); display: flex; align-items: center; justify-content: center; opacity: 0; transition: opacity 0.3s; }";
  html += ".map-preview:hover .map-overlay { opacity: 1; }";
  html += ".map-overlay span { background: rgba(0,0,0,0.8); color: white; padding: 8px 16px; border-radius: 20px; font-size: 12px; font-weight: 600; }";
  
  // Action buttons - immediate response
  html += ".action-section { padding: 20px; background: #fff8e1; border-left: 6px solid #f57c00; }";
  html += ".action-header { display: flex; align-items: center; margin-bottom: 16px; }";
  html += ".action-header h3 { color: #e65100; font-size: 16px; font-weight: 700; margin-left: 8px; }";
  html += ".action-buttons { display: grid; gap: 12px; }";
  html += ".action-btn { display: flex; align-items: center; justify-content: center; padding: 14px 16px; border-radius: 8px; text-decoration: none; font-weight: 600; font-size: 14px; transition: all 0.2s; border: none; cursor: pointer; }";
  html += ".btn-primary { background: #d32f2f; color: white; box-shadow: 0 2px 8px rgba(211, 47, 47, 0.3); }";
  html += ".btn-primary:hover { background: #c62828; transform: translateY(-1px); }";
  html += ".btn-secondary { background: #1976d2; color: white; box-shadow: 0 2px 8px rgba(25, 118, 210, 0.3); }";
  html += ".btn-secondary:hover { background: #1565c0; transform: translateY(-1px); }";
  html += ".btn-icon { margin-right: 8px; font-size: 16px; }";
  
  // Emergency contacts - quick access
  html += ".contacts-section { padding: 20px; background: #f3e5f5; border-left: 6px solid #7b1fa2; }";
  html += ".contacts-header { display: flex; align-items: center; margin-bottom: 16px; }";
  html += ".contacts-header h3 { color: #4a148c; font-size: 16px; font-weight: 700; margin-left: 8px; }";
  html += ".contacts-grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 10px; }";
  html += ".contact-item { background: white; padding: 10px; border-radius: 6px; text-align: center; border: 1px solid #e1bee7; }";
  html += ".contact-item h4 { color: #4a148c; font-size: 12px; font-weight: 600; margin-bottom: 2px; }";
  html += ".contact-item p { color: #6a1b9a; font-size: 14px; font-weight: 700; }";
  
  // Footer - reassuring
  html += ".footer { padding: 16px 20px; background: #f5f5f5; text-align: center; border-top: 1px solid #e0e0e0; }";
  html += ".footer p { color: #666; font-size: 11px; margin-bottom: 4px; }";
  html += ".footer .brand { color: #d32f2f; font-weight: 600; }";
  
  // Animations
  html += "@keyframes pulse { 0%, 100% { transform: translateY(-50%) scale(1); } 50% { transform: translateY(-50%) scale(1.1); } }";
  
  // Mobile optimizations
  html += "@media (max-width: 480px) { .email-container { margin: 0; } .contacts-grid { grid-template-columns: repeat(3, 1fr); } .contact-item { padding: 8px; } }";
  
  html += "</style></head><body><div class='email-container'>";

  // HEADER
  html += "<header class='emergency-header'><h1>EMERGENCY ALERT</h1>";
  html += "<div class='subtitle'>Women Safety SOS • Immediate Response Required</div></header>";

  // ALERT TYPE + TIME
  html += "<section class='alert-status'><h2>" + alertType + " Emergency</h2>";
  html += "<p>Time: " + timestamp + "</p></section>";

  // DEVICE + EMERGENCY DETAILS
  html += "<section class='info-block'>";
  html += "<div class='info-row'><span class='info-icon'>📱</span><span class='info-label'>Device:</span><span class='info-value'>SOS-" + String(deviceID) + "</span></div>";
  if (emergencyDetails.length() > 0) {
    html += "<div class='info-row'><span class='info-icon'>📝</span><span class='info-label'>Details:</span><span class='info-value'>" + emergencyDetails + "</span></div>";
  }
  html += "</section>";

  // LOCATION BLOCK
  if (gpsDataValid) {
    html += "<section class='location-section'><div class='location-header'>";
    html += "<span style='font-size: 18px;'>📍</span><h3>Current Location</h3></div>";
    html += "<div class='address-text'><strong>Address:</strong> " + currentAddress + "</div>";

    // MAP PREVIEW
    html += "<a href='" + locationURL + "' class='map-preview' aria-label='Map preview. Click to open in Google Maps'>";
    html += "<img src='" + staticMapURL + "' alt='Emergency Location Map'>";
    html += "<div class='map-overlay'><span>Tap to open in Google Maps</span></div></a></section>";

  }

  // EMERGENCY CONTACTS GRID
  html += "<section class='contacts-section'><div class='contacts-header'>";
  html += "<span style='font-size: 18px;'>🆘</span><h3>Emergency Numbers</h3></div><div class='contacts-grid'>";
  html += "<a href='tel:100' class='contact-item'><h4>Police</h4><p>100</p></a>";
  html += "<a href='tel:108' class='contact-item'><h4>Ambulance</h4><p>102</p></a>";
  html += "<a href='tel:1091' class='contact-item'><h4>Women Help</h4><p>1091</p></a>";
  html += "<a href='tel:112' class='contact-item'><h4>Emergency</h4><p>112</p></a>";
  html += "</div></section>";

  // FOOTER
  html += "<footer class='footer'><p><span class='brand'>Women Safety SOS System</span></p>";
  html += "<p>Automated Emergency Alert • Respond Immediately</p>";
  html += "<p>Support : " + String(AUTHOR_EMAIL) + "</p></footer></div></body></html>";

  return html;
}

void sendEmailAlert(String alertType, String emergencyDetails) {
  // Wrapper function for compatibility
  sendEmergencyAlert(alertType, emergencyDetails);
}

// Alternative function for regular location updates (non-emergency)
void sendLocationUpdate(String updateType, String additionalInfo) {
  // Use the same structure but with different styling/urgency
  // This would be for regular check-ins, not emergencies
  sendEmergencyAlert(updateType, additionalInfo);
}
// ==================================================================
//                    UTILITY FUNCTIONS
// ==================================================================

String getCurrentTimestamp() {
  time_t now = time(nullptr);
  if (now < 8 * 3600 * 2) {
    return "Time not synchronized";
  }
  
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  
  char timeStr[50];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  
  return String(timeStr);
}

// ==================================================================
//                    MAIN PROGRAM LOOP
// ==================================================================

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠ WiFi disconnected - attempting reconnection...");
    connectToWiFi();
  }
  
  // Update GPS data
  updateGPSData();
  
  // Run GeoLinker loop for cloud tracking
  uint8_t geoStatus = geo.loop();
  if (geoStatus > 0) {
    handleGeoLinkerStatus(geoStatus);
  }
  
  // Send periodic location updates via email (every 1 minute)
  if (millis() - lastLocationSend > LOCATION_SEND_INTERVAL && gpsDataValid) {
    Serial.println("⏰ Sending periodic location update...");
    sendEmailAlert("LOCATION_UPDATE", "Periodic location update from GPS tracker");
    lastLocationSend = millis();
  }
  
  
  delay(100);
}

void handleGeoLinkerStatus(uint8_t status) {
  switch(status) {
    case STATUS_SENT:
      Serial.println("✓ Location data sent to GeoLinker cloud");
      break;
    case STATUS_GPS_ERROR:
      Serial.println("✗ GPS module error - check connections");
      break;
    case STATUS_NETWORK_ERROR:
      Serial.println("⚠ Network error - data buffered offline");
      break;
    case STATUS_BAD_REQUEST_ERROR:
      Serial.println("✗ Server rejected request - check API key");
      break;
    case STATUS_PARSE_ERROR:
      Serial.println("✗ GPS data parsing error");
      break;
    case STATUS_INTERNAL_SERVER_ERROR:
      Serial.println("✗ GeoLinker server error");
      break;
    default:
      Serial.println("? Unknown GeoLinker status: " + String(status));
      break;
  }
}