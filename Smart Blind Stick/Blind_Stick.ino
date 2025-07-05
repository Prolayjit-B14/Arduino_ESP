#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Ultrasonic
const int trigPin = 9;
const int echoPin = 10;

// Outputs
const int buzzerPin = 5;
const int motorControlPin = 6; // Transistor control
const int buttonPin = 7;       // SOS button (2-leg pull-up)

// GPS & GSM
SoftwareSerial gpsSerial(4, 3);   // GPS: RX, TX
SoftwareSerial gsmSerial(8, 2);   // GSM: TX, RX
TinyGPSPlus gps;

// Variables
unsigned long lastSafeMsgTime = 0;
bool obstacleDetected = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(motorControlPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("🔰 Blind Smart SOS System Initializing...");
  delay(3000);
  Serial.println("✅ Ready! Monitoring started...\n");
}

void loop() {
  checkObstacle();
  checkSOSButton();
  readGPS();

  // Print safe status every 5 seconds if no obstacle
  if (!obstacleDetected && millis() - lastSafeMsgTime > 5000) {
    Serial.println("✅ Status: All clear. No obstacles nearby.");
    Serial.println("⏱️ Waiting for next scan...\n");
    lastSafeMsgTime = millis();
  }

  obstacleDetected = false;
  delay(100);
}

// ---- Check Ultrasonic Obstacle ----
void checkObstacle() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > 0 && distance <= 50) {
    obstacleDetected = true;
    Serial.println("⚠️ Obstacle Detected!");
    Serial.print("📏 Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Alert
    tone(buzzerPin, 1000);
    digitalWrite(motorControlPin, HIGH);
    delay(300);
    noTone(buzzerPin);
    digitalWrite(motorControlPin, LOW);
    delay(200);
  }
}

// ---- Check SOS Button ----
void checkSOSButton() {
  if (digitalRead(buttonPin) == LOW) {
    delay(50);
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("🆘 SOS Button Pressed!");
      sendSOS();
      delay(3000); // Avoid rapid repeat
    }
  }
}

// ---- Read GPS Data ----
void readGPS() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
}

// ---- Send SOS via GSM ----
void sendSOS() {
  String gpsData = "";
  if (gps.location.isValid()) {
    gpsData = "Lat: " + String(gps.location.lat(), 6) + 
              ", Lon: " + String(gps.location.lng(), 6);
  } else {
    gpsData = "GPS not available.";
  }

  String msg = "🚨 SOS ALERT!\n📍 Location: " + gpsData;

  Serial.println("📲 Sending SMS...");
  Serial.println(msg);

  gsmSerial.println("AT+CMGF=1"); delay(1000);
  gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\""); delay(1000); // Replace with phone number
  gsmSerial.print(msg);
  gsmSerial.write(26); // End SMS with Ctrl+Z
  Serial.println("✅ Message sent!\n");
}
