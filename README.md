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
- Maintain clean, professional GitHub documentation

---

## 🚀 Quick Start
1. Upload the sketch to your Arduino Mega
2. Register your RFID card UID in the code
3. Power the system and arm it
4. Enter the correct PIN and scan the authorized RFID card before the timer reaches zero

---

## 🧠 Core Scenario (How It Works)

1. **Idle State**
   - System is powered but inactive
   - Display shows system status
   - Countdown is stopped

2. **System Armed**
   - System is turned on via button or Bluetooth
   - User is prompted to enter the PIN

3. **RFID Authentication**
   - Authorized RFID card must be scanned
   - UID is validated against stored value

4. **Countdown Phase**
   - Countdown timer starts
   - Beep rate increases as time decreases
   - LEDs and buzzer provide feedback

5. **Disarm Procedure**
   - Correct PIN + valid RFID card
   - System transitions to “Defused” state

6. **Fail State**
   - Timer reaches zero or remote trigger is received
   - Alarm feedback is activated (visual/audio only)
   - No physical action occurs

---

## 🔧 Hardware Wiring  
### **Arduino Mega 2560 – REQUIRED**

> ⚠️ Arduino Uno is **NOT supported**  
> This project requires Arduino Mega due to pin count and hardware serial usage.

### 🧠 Microcontroller
- **Board:** Arduino Mega 2560  
- **Logic Voltage:** 5V  
- **RFID Voltage:** 3.3V (IMPORTANT)

---

## 📟 Module Connections (Exact – Based on Code)

### 📡 RFID Reader (MFRC522 – SPI)

| MFRC522 Pin | Arduino Mega |
|------------|--------------|
| SDA (SS) | 53 |
| SCK | 52 |
| MOSI | 51 |
| MISO | 50 |
| RST | 49 |
| VCC | 3.3V |
| GND | GND |


#define SS_PIN 53
#define RST_PIN 49
⚠️ Do NOT connect RFID VCC to 5V

🔢 Keypad (4x4 Matrix)
Keypad Pin	Arduino Mega
R1	22
R2	23
R3	24
R4	25
C1	26
C2	27
C3	28
C4	29

cpp
Copy code
byte rowPins[4] = {22, 23, 24, 25};
byte colPins[4] = {26, 27, 28, 29};
📺 LCD Display (16x2 I2C)
LCD Pin	Arduino Mega
VCC	5V
GND	GND
SDA	20
SCL	21

cpp
Copy code
LiquidCrystal_I2C lcd(0x27, 16, 2);
Default I2C address: 0x27

💡 LEDs
LED	Arduino Mega
Green LED	2
Red LED	3

Use 220Ω resistors in series

cpp
Copy code
#define GREEN_LED 2
#define RED_LED 3
🔊 Buzzer
Buzzer	Arduino Mega
Signal	4
GND	GND

cpp
Copy code
#define BUZZER 4
🔘 Arm / System Button
Button	Arduino Mega
Signal	5
Other Side	GND

cpp
Copy code
#define ARM_BUTTON 5
Uses INPUT_PULLUP

Button is active LOW

📶 Bluetooth Module (HC‑05 / HC‑06)
Bluetooth	Arduino Mega
TX	RX1 (19)
RX	TX1 (18)
VCC	5V
GND	GND

cpp
Copy code
Serial1.begin(9600);
Hardware Serial1 is required → Uno cannot support this

🪪 RFID Card Registration
Upload the code

Open Serial Monitor

Scan your RFID card

Replace the UID in the code:

cpp
Copy code
byte authorizedUID[4] = {0x23, 0x25, 0xF9, 0x12};
🔐 PIN & EEPROM Behavior
Master PIN is stored in EEPROM

Default PIN: 1234

PIN persists after power loss

Can be changed via:

Keypad secret combination

Bluetooth command

🛡️ License
This project is released under a custom restrictive license:

Source code may be viewed and studied

Commercial use is not allowed

Claiming the project as your own is not allowed

Original authorship remains with Mhystify

See LICENSE for full terms.

⚠️ Disclaimer
This project is a fictional simulation.
It must not be used in public spaces, schools, or environments where it could cause fear or confusion.
All effects are visual and audio only.

🚀 Final Notes
QuantumFuse focuses on logic, control flow, and presentation — not danger.
It is best used as a learning tool, demo system, or escape room prop.

Build smart. Stay ethical. Look cool doing it.

👤 Author
Developed by Mhystify
Embedded systems & simulation project

yaml
Copy code
