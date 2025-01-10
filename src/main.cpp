#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// กำหนดพินสำหรับ I2C LCD
#define I2C_SDA 21
#define I2C_SCL 22

// สร้างอินสแตนซ์ LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD Address 0x27, 16 columns, 2 rows

// กำหนดพินสำหรับ Switch และ LED
#define SWITCH_UP 15
#define SWITCH_SELECT 2

#define LED_1 23
#define LED_2 19
#define LED_3 18
#define LED_4 5
#define LED_5 17
#define LED_6 16
#define LED_7 4
#define LED_8 0

int ledPins[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};

// ตัวแปรเมนู
int menu = 1;
int selectedMenu = -1;

// ตัวแปรสำหรับ debounce
unsigned long lastDebounceTime_UP = 0;
unsigned long lastDebounceTime_SELECT = 0;
const unsigned long debounceDelay = 50; // หน่วงเวลา 50 ms

bool lastSwitchState_UP = HIGH;
bool lastSwitchState_SELECT = HIGH;

// ฟังก์ชันตั้งค่า
void setup() {
  // ตั้งค่า LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Select Menu");

  // ตั้งค่า Switch เป็น INPUT พร้อม PULLUP
  pinMode(SWITCH_UP, INPUT_PULLUP);
  pinMode(SWITCH_SELECT, INPUT_PULLUP);

  // ตั้งค่า LED เป็น OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // เริ่มต้นปิด LED
  }
}

// ฟังก์ชันสำหรับการควบคุม LED ตามเมนู
void controlLEDs(int menu) {
  switch (menu) {
    case 1:
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      break;

    case 2:
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], (i % 2 == 0) ? HIGH : LOW);
      }
      delay(500);
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], (i % 2 != 0) ? HIGH : LOW);
      }
      delay(500);
      break;

    case 3:
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(200);
        digitalWrite(ledPins[i], LOW);
      }
      break;

    case 4:
      for (int i = 0; i < 8; i += 2) {
        digitalWrite(ledPins[i], HIGH);
        digitalWrite(ledPins[i + 1], HIGH);
        delay(500);
        digitalWrite(ledPins[i], LOW);
        digitalWrite(ledPins[i + 1], LOW);
      }
      break;

    case 5:
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(200);
      }
      for (int i = 7; i >= 0; i--) {
        digitalWrite(ledPins[i], LOW);
        delay(200);
      }
      break;

    case 6:
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      delay(500);
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      delay(500);
      break;

    default:
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      break;
  }
}

// ฟังก์ชันหลัก
void loop() {
  // อ่านสถานะของ Switch UP
  bool currentSwitchState_UP = digitalRead(SWITCH_UP);
  if (currentSwitchState_UP == LOW && lastSwitchState_UP == HIGH && millis() - lastDebounceTime_UP > debounceDelay) {
    menu++;
    if (menu > 6) {
      menu = 1; // วนกลับไปที่เมนูแรก
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select Menu:");
    lcd.setCursor(0, 1);
    lcd.print("Menu ");
    lcd.print(menu);
    lastDebounceTime_UP = millis();
  }
  lastSwitchState_UP = currentSwitchState_UP;

  // อ่านสถานะของ Switch SELECT
  bool currentSwitchState_SELECT = digitalRead(SWITCH_SELECT);
  if (currentSwitchState_SELECT == LOW && lastSwitchState_SELECT == HIGH && millis() - lastDebounceTime_SELECT > debounceDelay) {
    selectedMenu = menu;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Running Menu:");
    lcd.setCursor(0, 1);
    lcd.print("Menu ");
    lcd.print(selectedMenu);
    lastDebounceTime_SELECT = millis();
  }
  lastSwitchState_SELECT = currentSwitchState_SELECT;

  // ควบคุม LED ตามเมนูที่เลือก
  if (selectedMenu > 0) {
    controlLEDs(selectedMenu);
  }
}
