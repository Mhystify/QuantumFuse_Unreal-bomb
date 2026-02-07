# QuantumFuse ⚡  
**Interactive Prop Bomb Simulation (Educational / Escape Room / Demo Project)**

QuantumFuse is a **safe, non-functional prop simulation** designed for educational purposes, escape rooms, and embedded systems demonstrations.  
It simulates a movie-style control terminal using RFID authentication, keypad input, countdown logic, and visual/audio feedback.

> ❗ This project contains **NO real explosives, detonators, chemicals, or hazardous components**.  
> It is a **pure simulation** intended for learning, presentation, and entertainment purposes only.

---

## 🎯 Project Goals
- Demonstrate embedded systems logic using Arduino
- Practice RFID-based authentication
- Implement state machines and timed events
- Create an immersive, cinematic terminal-style interaction
- Learn clean project structure and documentation for GitHub

---

## 🚀 Quick Start
1. Upload the sketch to your Arduino
2. Register your RFID card UID
3. Power the system and scan an authorized card
4. Enter the correct PIN before the timer reaches zero


## 🧠 Core Scenario (How It Works)

1. **Idle State**
   - System waits for user interaction
   - Display shows standby message
   - Countdown is inactive

2. **RFID Authentication**
   - User scans an authorized RFID card
   - If the card UID matches a registered card:
     - System unlocks
     - Countdown begins
   - Unauthorized cards trigger a warning state

3. **Countdown Phase**
   - Timer starts decreasing (configurable)
   - Buzzer beeps at intervals
   - LEDs update based on remaining time

4. **Code Entry (Disarm Simulation)**
   - User enters a predefined PIN via keypad
   - Correct PIN:
     - Countdown stops
     - System enters “Disarmed” state
   - Incorrect PIN:
     - Time penalty applied
     - Warning feedback triggered

5. **Fail State**
   - If time reaches zero:
     - Alarm sound plays
     - LEDs flash
     - System locks and requires reset
   - No physical action occurs (visual/audio only)

---

## 🔌 Hardware Components
- Arduino Uno / Nano / compatible board
- MFRC522 RFID Reader
- RFID cards or key fobs
- 4x4 or 3x4 Keypad
- Active or passive buzzer
- LEDs (Red / Yellow / Green)
- 16x2 LCD (I2C recommended)
- Jumper wires
- Breadboard or enclosure (optional)

---

## 📌 Pin Configuration (Example – Arduino Uno)

### RFID (MFRC522 – SPI)
| RFID Pin | Arduino Pin |
|--------|------------|
| SDA    | D10        |
| SCK    | D13        |
| MOSI   | D11        |
| MISO   | D12        |
| RST    | D9         |
| VCC    | 3.3V       |
| GND    | GND        |

### Keypad (4x4 Example)
| Keypad | Arduino |
|------|---------|
| R1   | D2      |
| R2   | D3      |
| R3   | D4      |
| R4   | D5      |
| C1   | D6      |
| C2   | D7      |
| C3   | D8      |
| C4   | A0      |

### Output Devices
| Device | Arduino Pin |
|------|-------------|
| Buzzer | A1 |
| Red LED | A2 |
| Yellow LED | A3 |
| Green LED | A4 |

> ⚠️ Pins are configurable. Update them in the code if needed.

---

## 🪪 RFID Card Registration (Important)

1. Upload the **RFID UID reader sketch**
2. Open Serial Monitor (9600 baud)
3. Scan your RFID card
4. Copy the UID printed in Serial Monitor
5. Paste the UID into the main code:

🔐 PIN Configuration

The disarm PIN is defined in the code:

const String correctPIN = "1234";

Length is configurable
Wrong attempts can:
Reduce remaining time
Trigger warnings
Lock the system (optional)


🛡️ License

This project uses a custom restrictive license:
Others may view and learn from the code
Commercial use is not allowed
Claiming the project as their own is not allowed
Original authorship remains with Mhystify
See LICENSE for full terms.


⚠️ Disclaimer

This project is a fictional simulation.
Any resemblance to real-world devices is purely aesthetic.
Do not use this project in public spaces, schools, or environments where it could cause fear or confusion.

🚀 Final Notes

QuantumFuse is about logic, control flow, and presentation — not danger.
Think of it as a hands-on lesson in embedded systems with style.
Build smart. Stay ethical. Look cool doing it.


## 👤 Author
Developed by Mhystify 
Embedded systems & simulation project
