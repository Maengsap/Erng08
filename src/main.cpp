#include <Arduino.h>
// กำหนดขา GPIO สำหรับ LED แต่ละดวง
int ledPins[] = {23, 19, 18, 5, 17, 16, 4, 0}; 
const int ledCount = 8; // จำนวนหลอดไฟทั้งหมด
int potPin = 36;        // ขา GPIO36 สำหรับ Potentiometer
// ประกาศ prototype ของฟังก์ชัน
void controlLEDs(int ledOnCount);

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // กำหนดความละเอียด ADC เป็น 12 บิต (0-4095)

  // กำหนดขา GPIO ทั้งหมดเป็น OUTPUT
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  int potValue = analogRead(potPin); // อ่านค่าสัญญาณจาก Potentiometer (0 - 4095)
  Serial.println(potValue); // แสดงค่าที่อ่านได้ผ่าน Serial Monitor

  // ควบคุมการติด/ดับหลอดไฟตามช่วงที่กำหนด
  if (potValue >= 0 && potValue <= 500) {
    controlLEDs(1); // ติด 1 หลอด
  } else if (potValue > 500 && potValue <= 1000) {
    controlLEDs(3); // ติด 3 หลอด
  } else if (potValue > 1000 && potValue <= 2000) {
    controlLEDs(5); // ติด 5 หลอด
  } else {
    controlLEDs(8); // ติดทั้งหมด
  }

  delay(100); // หน่วงเวลาเล็กน้อยเพื่อความเสถียร
}


// ฟังก์ชันควบคุมจำนวนหลอดไฟที่ติด
void controlLEDs(int ledOnCount) {
  for (int i = 0; i < ledCount; i++) {
    if (i < ledOnCount) {
      digitalWrite(ledPins[i], HIGH); // เปิดหลอดไฟ
    } else {
      digitalWrite(ledPins[i], LOW);  // ปิดหลอดไฟ
    }
  }
}
