#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Define GPS serial connection (UART1 on ESP32)
HardwareSerial gpsSerial(1);  // UART1
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // GPS TX → GPIO16, RX ← GPIO17
  Serial.println("📡 GPS Test Started - Waiting for satellite fix...");
}

void loop() {
  // Read data from GPS
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // When new location data is available
  if (gps.location.isUpdated()) {
    Serial.println("=== GPS Location Update ===");

    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    Serial.print("Date: ");
    if (gps.date.isValid()) {
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.println(gps.date.year());
    } else {
      Serial.println("Invalid");
    }

    Serial.print("Time (UTC): ");
    if (gps.time.isValid()) {
      char timeBuffer[10];
      sprintf(timeBuffer, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
      Serial.println(timeBuffer);
    } else {
      Serial.println("Invalid");
    }

    // Google Maps Link
    Serial.print("🗺️ Google Maps: https://maps.google.com/?q=");
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.println(gps.location.lng(), 6);

    Serial.println("===========================\n");
    delay(1000);  // Delay to prevent flooding the serial
  }
}
