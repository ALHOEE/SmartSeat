# 📸 SmartSeat - Visual Gallery & Demonstrations

Welcome to the visual repository of the SmartSeat project. This gallery documents the engineering journey from conceptual diagrams to a functional hardware prototype integrated with cloud services.

## 📺 Video Demonstration (Proof of Concept)

> Click the image below to watch the full system demonstration on YouTube. The video showcases real-time sensor polling, cloud sync, and the triggering of critical alerts.

[![SmartSeat Demo Video](https://img.youtube.com/vi/YOUR_VIDEO_ID_HERE/0.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID_HERE)
*(Replace `YOUR_VIDEO_ID_HERE` with your actual YouTube video ID in both links above to embed properly).*

---

## 🏗️ Engineering Diagrams & System Flow

Clear documentation of the system logic and electrical flow.

| Diagram Type | Description | Preview |
| :--- | :--- | :--- |
| **High-Level Architecture** | Shows the interaction between ESP32, Sensors, Blynk Cloud, and Smartphone. | `![Architecture Diagram](./architecture.png)` |
| **Firmware FSM Logic** | Finite State Machine flowchart detailing state transitions (Idle, Safe, Alert). | `![FSM Flowchart](./fsm_flow.png)` |
| **Electrical Schematic** | Complete circuit schematic created in SPICE/Altium showing pin mappings. | `![Circuit Schematic](./schematic.png)` |

---

## 📸 Hardware Prototype Iterations

Visual documentation of the physical build and car integration.

### 1. Breadboard POC (Proof of Concept)
The initial setup focusing on validating the analog-to-digital (ADC) logic for the FSR sensor and calibrating the DHT11 temperature readings.
> `![Breadboard Prototype](./media/breadboard_poc.jpg)`

### 2. Prototype Integration & Car Installation
Photos of the final prototype housed in a custom enclosure and integrated into a real vehicle child seat.
> `![Car Installation 1](./media/car_setup_1.jpg)`
> `![Car Installation 2](./media/car_setup_2.jpg)`

---

## 📱 User Interface (Mobile App)

Screenshots of the smartphone interface (Blynk IoT) showing real-time telemetry and the alert system in action.

| Safe State Monitoring | Critical Alert Notification |
| :---: | :---: |
| `![App Safe State](./media/app_safe.jpg)` | `![App Alert State](./media/app_alert.jpg)` |
| Monitoring active occupancy and safe temperature. | Critical alert triggered due to driver disconnection. |

---

### 💡 How to update this gallery:
1.  Upload your images/diagrams directly into this `media/` directory.
2.  Edit this `README.md` file.
3.  Replace the placeholders like `./media/breadboard_poc.jpg` with the actual filename of the image you uploaded.
