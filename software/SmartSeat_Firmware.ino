#define BLYNK_TEMPLATE_ID   "TMPL000000"
#define BLYNK_TEMPLATE_NAME "SmartSeat"
#define BLYNK_AUTH_TOKEN    "YourAuthTokenPlaceholder"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// --- Hardware Pin Assignments ---
const int FSR_PIN = 34;       // Analog input for Force Sensitive Resistor (ADC1_CH6)
const int DHT_PIN = 23;       // Digital input for DHT11 Temperature & Humidity Sensor
const int BUZZER_PIN = 18;    // Digital output for Alarm Buzzer Driver
const int LED_PIN = 19;       // Digital output for Status Indicator LED

// --- Calibration & Thresholds ---
const int WEIGHT_THRESHOLD = 500;   // Calibrated raw ADC value for child presence
const float TEMP_THRESHOLD = 38.0;  // Critical temperature threshold in Celsius

// --- Timing Variables (Non-blocking execution) ---
unsigned long lastSensorReadTime = 0;
const unsigned long SENSOR_INTERVAL = 100; // Poll sensors every 100ms

// --- Object Initializations ---
DHT dht(DHT_PIN, DHT11);

// --- Network Credentials ---
char ssid[] = "YourNetworkSSID";
char pass[] = "YourNetworkPassword";

void setup() {
    // Initialize Hardware Serial Communication
    Serial.begin(115200);
    delay(100);
    Serial.println("\n--- SmartSeat Firmware Initialization Started ---");

    // Configure GPIO Pin Modes
    pinMode(FSR_PIN, INPUT); 
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // Set initial actuator states to safe (OFF)
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    // Initialize Environmental Sensor
    dht.begin();
    Serial.println("[INFO] DHT11 Sensor initialized successfully.");

    // Configure ESP32 Successive Approximation Register (SAR) ADC
    // 12-bit resolution provides values from 0 to 4095
    analogReadResolution(12);        
    // 11dB attenuation allows reading voltages up to ~3.3V safely
    analogSetAttenuation(ADC_11db);  
    Serial.println("[INFO] ADC configured to 12-bit resolution with 11dB attenuation.");

    // Initialize Cloud Connectivity via Blynk IoT Platform
    Serial.println("[INFO] Establishing connection to Blynk IoT Cloud...");
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
    // Maintain cloud keep-alives and process incoming network events
    Blynk.run();

    // Event-driven sensor polling execution (Non-blocking design without delay())
    unsigned long currentMillis = millis();
    if (currentMillis - lastSensorReadTime >= SENSOR_INTERVAL) {
        lastSensorReadTime = currentMillis;
        processSystemSensors();
    }
}

void processSystemSensors() {
    // Read analog voltage from FSR voltage divider network
    int rawADC = analogRead(FSR_PIN);
    float voltage = (rawADC * 3.3) / 4095.0; // Map 12-bit ADC value to physical voltage

    // Read climate metrics from the DHT11 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check for sensor readout failures (Hardware disconnected or noise interference)
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("[ERROR] Failed to read from DHT sensor! Checking hardware line...");
        return;
    }

    // Evaluate child occupancy condition based on physical weight
    bool isOccupied = (rawADC > WEIGHT_THRESHOLD);

    // Print operational telemetry to hardware Serial Monitor for debugging purposes
    Serial.printf("FSR ADC: %4d | Volts: %2.2fV | Temp: %2.1fC | Hum: %2.1f%% | State: %s\n", 
                  rawADC, voltage, temperature, humidity, isOccupied ? "OCCUPIED" : "EMPTY");

    // Transmit telemetry payloads to Blynk Cloud Virtual Pins
    Blynk.virtualWrite(V1, rawADC);
    Blynk.virtualWrite(V2, temperature);
    Blynk.virtualWrite(V3, humidity);
    Blynk.virtualWrite(V4, isOccupied ? 1 : 0);

    // System Safety Logic Verification (Finite State Machine execution)
    if (isOccupied) {
        if (temperature >= TEMP_THRESHOLD) {
            // Threshold exceeded - Trigger critical safety protocols
            executeCriticalAlarm();
        } else {
            // Occupied but within medical safety thresholds (Safe State)
            digitalWrite(LED_PIN, HIGH); 
            digitalWrite(BUZZER_PIN, LOW);
        }
    } else {
        // Seat is vacant - safe standby mode
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
    }
}

void executeCriticalAlarm() {
    Serial.println("[CRITICAL ALERT] Severe condition! Seat occupied under high temperature!");
    
    // Actuate localized physical alarms (NPN transistor driven buzzer)
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    
    // Trigger remote cloud notifications via Blynk event handlers (Push/SMS)
    Blynk.logEvent("critical_heat_alert", "Warning! Critical temperature detected in occupied child seat!");
}
