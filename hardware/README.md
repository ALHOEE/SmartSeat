
# 🛠️ SmartSeat - Hardware Architecture & Circuit Design

This directory houses the electrical schematics, sensor integrations, pin configurations, and power management considerations for the SmartSeat IoT safety system.

## 🔌 Component Specifications (BOM)

| Component | Description | Interface / Protocol | Purpose |
| :--- | :--- | :--- | :--- |
| **ESP32 NodeMCU** | Dual-core Wi-Fi/BLE MCU | SPI, I2C, UART, ADC | Central processing, cloud sync & logic execution |
| **FSR (Force Sensitive Resistor)** | Analog pressure sensor | Analog (ADC) | Detects child's presence/weight in the seat |
| **DHT11 Temperature Sensor** | Environmental sensor | Digital (1-Wire) | Monitors vehicle cabin temperature and humidity |
| **Active Buzzer & LED** | Local alert system | Digital Output (GPIO) | Immediate localized physical warning system |
| **Resistors & Passives** | 10kΩ pull-down, 220Ω current-limiting | Passive Elements | Voltage division and current regulation |

---

## 📐 Circuit Design & Pin Mapping

The hardware architecture safely bridges analog sensor data into the ESP32's internal 12-bit Successive Approximation Register (SAR) ADC.

### Microcontroller Pinout Configuration:
* **FSR Pressure Sensor:** Connected to **GPIO34** (ADC1_CH6) through a voltage divider network.
* **DHT11 Sensor:** Connected to **GPIO23** (Digital I/O) with an internal pull-up.
* **Active Buzzer:** Driven via an NPN transistor configuration connected to **GPIO18**.
* **Status LED:** Connected to **GPIO19** via a 220Ω current-limiting resistor.

---

## 🧠 Electrical Engineering Considerations

### 1. Voltage Divider Signal Conditioning
Since the FSR changes its resistance based on mechanical pressure, it cannot be read directly by the MCU. A voltage divider network utilizing a stable 10kΩ resistor maps the variable resistance into a readable 0V to 3.3V analog voltage range ($V_{out}$). 

The voltage calculation follows the standard transfer function:
$$V_{out} = V_{in} \cdot \frac{R_{pull-down}}{R_{FSR} + R_{pull-down}}$$

### 2. Power Optimization & Low-Power Management
Automotive applications require strict power budgets to prevent vehicle battery drain. 
* When the vehicle ignition is off and the seat is unoccupied, the ESP32 is programmed to enter **Deep Sleep Mode**, dropping power consumption to the microampere range.
* The system relies on external hardware interrupts on the FSR channel to wake the MCU instantly upon weight detection.

### 3. Noise Reduction & Stabilization
* **Decoupling Capacitors:** 0.1µF ceramic capacitors are placed across the ESP32 VCC and GND lines to filter high-frequency noise from the vehicle's electrical grid.

---

## 🖼️ Schematic Diagram
> **
> *Tip: Once you complete your circuit schematic (in SPICE / Multisim / Altium), export it as a PNG/PDF, drop it into the `media/` directory, and link it here using: `![Schematic](../media/schematics.png)`*
