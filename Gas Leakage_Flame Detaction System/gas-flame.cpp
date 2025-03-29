// Define Sensor and Buzzer Pins
#define GAS_SENSOR_A A0
#define FLAME_SENSOR_A A1
#define BUZZER 5
#define RGB_RED 6
#define RGB_GREEN 7
#define RGB_BLUE 8

int gasThreshold = 50;
int flameThreshold = 500;

void setup() {
    Serial.begin(9600);
    pinMode(GAS_SENSOR_A, INPUT);
    pinMode(FLAME_SENSOR_A, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(RGB_RED, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
    pinMode(RGB_BLUE, OUTPUT);

    setRGB(0, 0, 0);
    noTone(BUZZER);

    Serial.println("🔥 Gas & Fire Detection System Ready!");
}

void loop() {
    int gasValue = analogRead(GAS_SENSOR_A);
    int flameValue = analogRead(FLAME_SENSOR_A);

    Serial.print("Gas: "); Serial.print(gasValue);
    Serial.print(" | Flame: "); Serial.println(flameValue);

    if (gasValue > gasThreshold && flameValue < flameThreshold) {
        blinkRedBlue();
        tone(BUZZER, 1500);  // 🚨 Loud continuous alarm
        Serial.println("🚨 BOTH GAS & FIRE DETECTED! IMMEDIATE ACTION REQUIRED!");
    } 
    else if (gasValue > gasThreshold) {
        setRGB(0, 0, 255);
        beepBeepSound();
        Serial.println("⚠️ Gas Leak Detected!");
    } 
    else if (flameValue < flameThreshold) {
        setRGB(255, 0, 0);
        highSiren();
        Serial.println("🔥 Fire Detected!");
    } 
    else {
        setRGB(0, 255, 0);
        noTone(BUZZER);
        Serial.println("✅ Safe Environment");
    }

    delay(2000);
}

// Function to set RGB LED color
void setRGB(int r, int g, int b) {
    analogWrite(RGB_RED, r);
    analogWrite(RGB_GREEN, g);
    analogWrite(RGB_BLUE, b);
}

// 🚨 Blinking Red & Blue for both Gas & Fire
void blinkRedBlue() {
    for (int i = 0; i < 10; i++) {
        setRGB(255, 0, 0);
        delay(200);
        setRGB(0, 0, 255);
        delay(200);
    }
}

// 🔊 High Siren for Fire (Continuous)
void highSiren() {
    tone(BUZZER, 1000);  // Keep alarm ON
}

// 🔉 Beep Beep for Gas Leak (Intermittent)
void beepBeepSound() {
    for (int i = 0; i < 5; i++) {
        tone(BUZZER, 700);
        delay(300);
        noTone(BUZZER);
        delay(300);
    }
}
