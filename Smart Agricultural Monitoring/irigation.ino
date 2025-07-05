
#include <DHT.h>

// Define sensor pins
#define DHTPIN 12       // DHT sensor data pin
#define DHTTYPE DHT11   // DHT sensor type
#define LDR_PIN A0      // LDR sensor analog pin
#define SOIL_SENSOR A1  // Soil moisture sensor analog pin
#define RELAY_PIN 13    // Relay for Motor control (Pump) - FIXED LOGIC
#define BUZZER 9        // Buzzer pin

// LED Indicators
#define RED_LED 3       // Dry Soil (Low Moisture)
#define GREEN_LED 4     // Moderate Moisture
#define YELLOW_LED 5    // Wet Soil (High Moisture)
#define TEMP_LED 6      // High Temperature Alert LED
#define LIGHT_LED 7     // Light Condition LED

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
    
    // Set pin modes
    pinMode(LDR_PIN, INPUT);
    pinMode(SOIL_SENSOR, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(TEMP_LED, OUTPUT);
    pinMode(LIGHT_LED, OUTPUT);

    digitalWrite(RELAY_PIN, HIGH); // Ensure relay is OFF initially (Active LOW)
}

void loop() {
    // Read temperature and humidity
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    if (isnan(temp) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Read LDR value
    int lightValue = analogRead(LDR_PIN);
    
    // Read soil moisture (Invert the value)
    int soilValue = analogRead(SOIL_SENSOR);
    int invertedSoilValue = 1023 - soilValue;  // Fix for opposite readings

    // Print values to Serial Monitor
    Serial.print("Temperature: "); Serial.print(temp); Serial.print(" C  ");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.print("%  ");
    Serial.print("Light: "); Serial.print(lightValue); Serial.print("  ");
    Serial.print("Soil Moisture: "); Serial.println(invertedSoilValue);


    // **Soil Moisture Condition - Pump ON when soil is DRY**
    if (invertedSoilValue < 300) {  
        // Dry Soil - Need Water
        digitalWrite(RED_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        
        digitalWrite(RELAY_PIN, HIGH); // Turns on water pump (if relay is active LOW)
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
    } 
    else if (invertedSoilValue >= 300 && invertedSoilValue < 700) {  
        // Moderate Moisture - No need to water
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        
        digitalWrite(RELAY_PIN, LOW); // Turns off pump
    } 
    else {  
            // Wet Soil
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);
        
        digitalWrite(RELAY_PIN, LOW); // Turns off pump
        tone(BUZZER, 2050);  // Low Frequency Buzzer Sound for Wet Soil
        delay(500);
        noTone(BUZZER);
    }
    // **Temperature Condition**
    if (temp > 35) { 
        digitalWrite(TEMP_LED, HIGH);  // High temperature alert
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
    } else {
        digitalWrite(TEMP_LED, LOW);
    }

    // **Light Condition**
    if (lightValue > 800) {
        digitalWrite(LIGHT_LED, LOW); // It's dark, turn off
    } else {
        digitalWrite(LIGHT_LED, HIGH); // Bright, turn on
    }

    delay(2000); // Wait before next reading
}
