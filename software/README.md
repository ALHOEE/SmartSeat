
# 💻 SmartSeat - Firmware Architecture & Software Logic

This directory contains the core source code and firmware logic for the ESP32 microcontroller powering the SmartSeat system. The codebase is optimized for real-time sensor polling, multi-threaded task management, and low-power efficiency.

## 🧠 Software Architecture: Finite State Machine (FSM)

The firmware is designed around a strict Finite State Machine (FSM) to ensure predictable, deterministic behavior under all automotive conditions. 

The system transitions between four operational states:
1. **`STATE_IDLE`**: The vehicle seat is unoccupied. The MCU operates in light sleep mode or low-frequency clock polling to conserve battery power.
2. **`STATE_OCCUPIED_SAFE`**: Weight is detected on the FSR ($> \text{Threshold}$), but environmental parameters (temperature) are within safety limits and the driver is nearby.
3. **`STATE_WARNING`**: The child is in the seat, and either the temperature exceeds 36°C OR the driver's smartphone disconnects (BLE/Wi-Fi RSSI drops). Local alerts (Buzzer/LED) begin pulsing intermittently.
4. **`STATE_CRITICAL_ALERT`**: Critical conditions reached (Temperature $> 38^{\circ}\text{C}$ or disconnection time $> 2\text{ minutes}$). Continuous local sirens are activated, and cloud API payloads are dispatched to trigger mobile notifications and emergency SMS via the Blynk/Twilio network.

---

## 🛠️ Main Logic Loop (Pseudo-C++)

The embedded software utilizes the following logical structure to optimize resource execution and avoid blocking operations (`delay()` is strictly forbidden; `millis()` time-stamping is used instead):

```cpp
#include <WiFi.h>
#include <DHT.h>

// Pin Definitions
#define FSR_PIN 34
#define DHT_PIN 23
#define BUZZER_PIN 18

// Thresholds
const int WEIGHT_THRESHOLD = 500; // ADC value
const float TEMP_THRESHOLD = 38.0; // Celsius

void setup() {
    Serial.begin(115200);
    pinMode(FSR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    initSensors();
    connectToCloud();
}

void loop() {
    int weightRaw = analogRead(FSR_PIN);
    float currentTemp = dht.readTemperature();
    bool isOccupied = (weightRaw > WEIGHT_THRESHOLD);

    // Finite State Machine Logic
    if (isOccupied) {
        if (currentTemp >= TEMP_THRESHOLD || isDriverAway()) {
            triggerCriticalAlert();
        } else {
            maintainSafeState();
        }
    } else {
        enterLowPowerMode();
    }
    
    // Maintain Cloud Keep-Alives
    Blynk.run(); 
}
