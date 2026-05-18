# 📄 SmartSeat - System Documentation & Engineering Logic

This directory contains the detailed engineering documentation, logic flowcharts, system requirements, and state-machine transitions that govern the SmartSeat safety architecture.

## 📋 System Requirements & Technical Specifications

To ensure maximum safety and reliability, the system is designed to adhere to strict timing and environmental constraints:

* **Sensing Latency:** Sensor polling interval $\le 100\text{ms}$.
* **False Positive Mitigation:** Weight detection requires a continuous stable reading above the threshold for at least 3 seconds before transitioning states to avoid false triggers caused by vehicle movement or road bumps.
* **Power Budget:** Operating voltage of 3.3V, utilization of ESP32 automatic modem-sleep and light-sleep states when the vehicle ignition is turned off.
* **Communication Range:** Bluetooth Low Energy (BLE) RSSI tracking calibrated to trigger a "Driver Away" warning when signal attenuation drops below $-85\text{dBm}$ (approximately 10–12 meters from the vehicle).

---

## 🔄 Finite State Machine (FSM) Detailed Transitions

The firmware relies on an event-driven Finite State Machine. The table below details the exact input conditions required to trigger state transitions:

| Current State | Input Event / Condition | Next State | System Action |
| :--- | :--- | :--- | :--- |
| **`STATE_IDLE`** | Weight Detected ($Raw\_ADC > 500$) | **`STATE_OCCUPIED_SAFE`** | Initialize system telemetry; start continuous environmental monitoring. |
| **`STATE_OCCUPIED_SAFE`** | Temperature $\ge 36^{\circ}\text{C}$ OR Driver Signal $< -85\text{dBm}$ | **`STATE_WARNING`** | Flash local status LED; pulse buzzer intermittently (1Hz duty cycle). |
| **`STATE_WARNING`** | Temperature $< 36^{\circ}\text{C}$ AND Driver Signal Returns | **`STATE_OCCUPIED_SAFE`** | Stop buzzer/LED; return system to safe tracking mode. |
| **`STATE_WARNING`** | Temperature $\ge 38^{\circ}\text{C}$ OR Warning Duration $> 2\text{ min}$ | **`STATE_CRITICAL_ALERT`** | Activate continuous hardware siren; dispatch API payload to cloud gateways. |
| **`STATE_CRITICAL_ALERT`** | Weight Removed ($Raw\_ADC < 500$) | **`STATE_IDLE`** | Deactivate all alarms; update cloud status to empty seat; enter low-power sleep. |

---

## 📉 Mathematical & Signal Models

### 1. Temperature Warning Delta
The car cabin acts as a greenhouse environment. The rate of temperature change ($\Delta T$) is modeled based on external solar radiation and cabin insulation. The system uses a predictive derivative check:
$$\frac{dT}{dt} > \theta_{critical}$$
If the temperature is rising too rapidly, the system decreases the state-transition delay to accelerate emergency notifications.

### 2. RSSI Signal Filtering
Raw RSSI data from Bluetooth beacons is highly susceptible to multipath fading and noise within the car's metallic chassis. To prevent false alarms, a digital **Exponential Moving Average (EMA) Filter** is implemented in the firmware:
$$RSSI_{filtered}[n] = \alpha \cdot RSSI_{raw}[n] + (1 - \alpha) \cdot RSSI_{filtered}[n-1]$$
*Where $\alpha = 0.2$ is chosen to smooth out transient signal drops while maintaining rapid response when the driver exits the vehicle perimeter.*

---

## 🗺️ System Logic Flowchart
> **
> *Tip: You can use tools like Lucidchart or Visio to create a clean system logic diagram, export it as a PNG, upload it to the `media/` folder, and link it here using: `![System Flowchart](../media/fsm_flowchart.png)`*
