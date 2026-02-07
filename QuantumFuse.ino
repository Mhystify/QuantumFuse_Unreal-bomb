#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

// ---------- PIN DEFINES ----------
#define SS_PIN 53
#define RST_PIN 49
#define GREEN_LED 2
#define RED_LED 3
#define BUZZER 4
#define ARM_BUTTON 5

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
byte authorizedUID[4] = {0x23, 0x25, 0xF9, 0x12}; 

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'}
};
byte rowPins[ROWS] = {22,23,24,25};
byte colPins[COLS] = {26,27,28,29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------- VARIABLES---------
String MASTER_PIN = "";
String enteredPIN = "";
bool systemOn = false;
bool bombActive = false;
int countdown = 60;
unsigned long lastBeep = 0, lastLCD = 0;
int beepInterval = 1000;

char secretCombo[] = {'B', 'D', 'C', '*'};
int comboIndex = 0;
unsigned long lastComboTime = 0;

// ---------- EEPROM OPERATIONS ----------
void savePinToEEPROM(String p) {
  for (int i = 1; i < 15; i++) EEPROM.write(i, 0xFF); 
  for (int i = 0; i < p.length(); i++) EEPROM.write(i + 1, p[i]);
  EEPROM.write(p.length() + 1, '\0'); 
  EEPROM.write(0, 77); 
}

String getPinFromEEPROM() {
  if (EEPROM.read(0) != 77) return "1234"; 
  String p = "";
  for (int i = 1; i < 12; i++) {
    char c = EEPROM.read(i);
    if (c == '\0' || (byte)c == 255) break;
    if (c >= '0' && c <= '9') { 
      p += c;
    }
  }
  return (p.length() > 0) ? p : "1234"; //Default pin is 1234#
}

// ---------- HELPFUL FUNCTIONS ----------
String inputSecret(String title) {
  String input = "";
  lcd.clear(); lcd.setCursor(0, 0); lcd.print(title);
  while (true) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9' && input.length() < 10) {
      input += key; lcd.setCursor(input.length() - 1, 1); lcd.print("*");
    } else if (key == 'A' && input.length() > 0) {
      input.remove(input.length() - 1); lcd.setCursor(input.length(), 1); lcd.print(" ");
    } else if (key == '#' && input.length() > 0) return input;
  }
}

bool checkRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return false;
  for (byte i = 0; i < 4; i++) if (rfid.uid.uidByte[i] != authorizedUID[i]) return false;
  return true;
}

// ---------- MAIN BOMB CYCLE ----------
void activateBomb() {
  bombActive = true; countdown = 60; beepInterval = 1000;
  digitalWrite(GREEN_LED, LOW);
  lcd.clear();
  Serial1.println("\n>>> STATUS: BOMB ACTIVE! <<<");
  String defusePIN = ""; bool enteringPin = false, remoteExplode = false;

  while (bombActive && countdown > 0) {
    unsigned long now = millis();

    if (countdown > 30) beepInterval = 1000;
    else if (countdown > 15) beepInterval = 500;
    else if (countdown > 5) beepInterval = 250;
    else beepInterval = 100;

    // Bluetooth Dinleme
    if (Serial1.available() > 0) {
      char cmd = Serial1.read();
      if (cmd == 'P' || cmd == 'p') { remoteExplode = true; bombActive = false; break; }
      if (cmd == 'I' || cmd == 'i') { 
        bombActive = false; systemOn = false; 
        lcd.clear(); 
        lcd.setCursor(0, 0); 
        lcd.print("REMOTE DEFUSED "); 
        Serial1.println("Time: --");
        Serial1.println(">>> STATUS: System Off (Remote Defused)");
        delay(2000); 
        lcd.clear();
        return; 
      }
    }

    // LCD UPDATE
    if (now - lastLCD >= 1000) {
      lastLCD = now; countdown--;
      if (!enteringPin) { lcd.setCursor(2, 0); lcd.print("BOMB ACTIVE!"); }
      lcd.setCursor(7, 1); if (countdown < 10) lcd.print("0"); lcd.print(countdown);
      Serial1.print("Time: "); Serial1.println(countdown);
    }

    // Sound and Light
    if (now - lastBeep >= beepInterval) {
      lastBeep = now;
      digitalWrite(RED_LED, HIGH); digitalWrite(BUZZER, HIGH); delay(40); 
      digitalWrite(RED_LED, LOW); digitalWrite(BUZZER, LOW);
    }

    // Keypad Operations
    char key = keypad.getKey();
    if (key == '#') {
       if (!enteringPin) { enteringPin = true; defusePIN = ""; lcd.clear(); lcd.setCursor(0, 0); lcd.print("PIN:"); }
       else if (defusePIN == MASTER_PIN) { 
         lcd.clear(); lcd.setCursor(3, 0); lcd.print("SCAN CARD!");
         unsigned long scanStart = millis(); bool cardOk = false;
         while(millis() - scanStart < 5000) {
           if(checkRFID()) { cardOk = true; break; }
           if(millis() - lastBeep > beepInterval) {
             digitalWrite(RED_LED, HIGH); digitalWrite(BUZZER, HIGH); delay(40);
             digitalWrite(RED_LED, LOW); digitalWrite(BUZZER, LOW); lastBeep = millis();
           }
         }
         if(cardOk) {
            lcd.clear(); lcd.setCursor(4, 0); lcd.print("DEFUSED!"); 
            bombActive = false; systemOn = false; 
            Serial1.println("Time: --");
            Serial1.println(">>> STATUS: System Off (Manual Defuse)");
            delay(2000); 
            lcd.clear();
            return; 
         } else {
            lcd.clear(); lcd.setCursor(2, 0); lcd.print("CARD TIMEOUT"); 
            delay(1000); lcd.clear(); lcd.setCursor(0, 0); lcd.print("PIN:"); defusePIN = "";
         }
       } else { 
         lcd.clear(); lcd.setCursor(3, 0); lcd.print("WRONG PIN!"); delay(1000); 
         lcd.clear(); lcd.setCursor(0, 0); lcd.print("PIN:"); defusePIN = ""; 
       }
    }
    if (enteringPin && key == 'A' && defusePIN.length() > 0) {
        lcd.setCursor(4 + defusePIN.length(), 0); lcd.print(" "); defusePIN.remove(defusePIN.length() - 1);
    }
    if (enteringPin && key >= '0' && key <= '9' && defusePIN.length() < 10) {
        defusePIN += key; lcd.setCursor(4 + defusePIN.length(), 0); lcd.print("*");
    }
  }

  if (countdown <= 0 || remoteExplode) {
    lcd.clear(); lcd.setCursor(4, 0); lcd.print("BOOM!!!"); 
    Serial1.println("Time: --");
    Serial1.println(">>> STATUS: EXPLODED! <<<");
    digitalWrite(RED_LED, HIGH); tone(BUZZER, 100, 3000); delay(5000); digitalWrite(RED_LED, LOW);
  }
  bombActive = false; systemOn = false;
  lcd.clear();
}

void setup() {
  pinMode(GREEN_LED, OUTPUT); pinMode(RED_LED, OUTPUT); pinMode(BUZZER, OUTPUT); pinMode(ARM_BUTTON, INPUT_PULLUP);
  lcd.init(); lcd.backlight(); SPI.begin(); rfid.PCD_Init();
  Serial.begin(9600); Serial1.begin(9600);
  MASTER_PIN = getPinFromEEPROM();
  if (MASTER_PIN == "") { MASTER_PIN = inputSecret("SET INITIAL PIN"); savePinToEEPROM(MASTER_PIN); }
  Serial1.println(">>> STATUS: System Off (Boot)");
}

void loop() {
  char key = keypad.getKey();

  if (Serial1.available() > 0) {
    char btCmd = Serial1.read();
    if (btCmd == '\n' || btCmd == '\r') return;

    if (btCmd == 'S' || btCmd == 's') { Serial1.print("CURRENT PIN: "); Serial1.println(MASTER_PIN); }
    
    if (btCmd == 'N' || btCmd == 'n') {
        String newPin = Serial1.readStringUntil('\n');
        newPin.trim();
        if(newPin.length() > 0) {
           MASTER_PIN = newPin; savePinToEEPROM(MASTER_PIN);
           Serial1.print(">>> STATUS: PIN Changed to: "); Serial1.println(MASTER_PIN);
        }
    }

    if ((btCmd == '1') && !systemOn) { systemOn = true; enteredPIN = ""; lcd.clear(); Serial1.println(">>> STATUS: System On"); } 
    else if ((btCmd == '0') && systemOn && !bombActive) { systemOn = false; lcd.clear(); Serial1.println(">>> STATUS: System Off"); Serial1.println("Time: --"); }
    else if ((btCmd == 'L' || btCmd == 'l') && systemOn && !bombActive) { activateBomb(); }
  }

  if (key && !systemOn) {
    if (millis() - lastComboTime > 3000) comboIndex = 0;
    if (key == secretCombo[comboIndex]) {
      comboIndex++; lastComboTime = millis();
      if (comboIndex == 4) {
        comboIndex = 0;
        if (inputSecret("OLD PIN:") == MASTER_PIN) {
          MASTER_PIN = inputSecret("NEW PIN:"); savePinToEEPROM(MASTER_PIN);
          lcd.clear(); lcd.setCursor(4, 0); lcd.print("SUCCESS!");
        } else { lcd.clear(); lcd.setCursor(5, 0); lcd.print("WRONG!"); }
        delay(2000); lcd.clear();
      }
    } else { comboIndex = 0; }
  }

  if (digitalRead(ARM_BUTTON) == LOW) { 
    delay(300); systemOn = !systemOn; lcd.clear(); 
    if(systemOn) Serial1.println(">>> STATUS: System On"); else Serial1.println(">>> STATUS: System Off");
    while(digitalRead(ARM_BUTTON) == LOW); 
  }

  if (!systemOn) { 
    digitalWrite(GREEN_LED, LOW); 
    digitalWrite(RED_LED, LOW); 
    lcd.setCursor(3, 0); 
    lcd.print("SYSTEM OFF"); 
    return; 
  }

  if (bombActive) digitalWrite(GREEN_LED, LOW); else digitalWrite(GREEN_LED, HIGH);

  lcd.setCursor(0, 0); lcd.print("PIN:");
  if (key) {
    if (key >= '0' && key <= '9' && enteredPIN.length() < 10) { 
      enteredPIN += key; lcd.setCursor(4 + enteredPIN.length(), 0); lcd.print("*");
    }
    else if (key == 'A' && enteredPIN.length() > 0) { 
      lcd.setCursor(4 + enteredPIN.length(), 0); lcd.print(" "); enteredPIN.remove(enteredPIN.length() - 1); 
    }
    else if (key == '#' && enteredPIN.length() > 0) {
      if (enteredPIN == MASTER_PIN) {
        lcd.clear(); lcd.setCursor(3, 0); lcd.print("SCAN CARD!"); unsigned long s = millis(); bool r = false;
        while(millis() - s < 5000) { if(checkRFID()) { r = true; break; } }
        if (r) activateBomb(); else { lcd.clear(); lcd.setCursor(4, 0); lcd.print("TIMEOUT"); delay(1000); }
      } else { lcd.clear(); lcd.setCursor(5, 0); lcd.print("WRONG!"); delay(1000); }
      enteredPIN = ""; lcd.clear();
    }
  }
}