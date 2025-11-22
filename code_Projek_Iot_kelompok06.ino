#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// IR Sensors
int ir_s1 = A1;
int ir_s2 = A0;

// Buzzer (positif ke 5V, negatif ke pin 12)
int buzzer = 12;

// Timing
unsigned long timer1;
unsigned long timer2;

float Time;
float distance = 10.16;   // 10.16 cm
float speed;

// Flags
int flag1 = 0;
int flag2 = 0;

// Sensor filter
unsigned long blockStart1 = 0;
unsigned long blockStart2 = 0;
const int MIN_BLOCK_TIME = 100; // ms

void setup() {
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);  // buzzer OFF (karena aktif HIGH)

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO OUR PROJECT");
  lcd.setCursor(0, 1);
  lcd.print("By Kelompok 06");
  delay(2000);
  lcd.clear();
}

void loop() {

  // SENSOR 1
  if (digitalRead(ir_s1) == LOW) {
    if (blockStart1 == 0) blockStart1 = millis();
  } else {
    blockStart1 = 0;
  }

  // Valid baca
  if (blockStart1 > 0 && (millis() - blockStart1) >= MIN_BLOCK_TIME && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }

  // SENSOR 2
  if (digitalRead(ir_s2) == LOW) {
    if (blockStart2 == 0) blockStart2 = millis();
  } else {
    blockStart2 = 0;
  }

  // Valid baca
  if (blockStart2 > 0 && (millis() - blockStart2) >= MIN_BLOCK_TIME && flag2 == 0) {
    timer2 = millis();
    flag2 = 1;
  }

  // Jika kedua sensor aktif
  if (flag1 == 1 && flag2 == 1) {

    Time = abs((long)timer1 - (long)timer2) / 1000.0;
    speed = (distance / Time) * 5.6;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.print(speed, 1);
    lcd.print(" km/h");

    lcd.setCursor(0, 1);

    if (speed >= 50) {
      lcd.print("Over Speeding!");
      digitalWrite(buzzer, HIGH);   // buzzer ON
    } else {
      lcd.print("Normal Speed   ");
      digitalWrite(buzzer, LOW);  // buzzer OFF
    }

    delay(3000);
    // Setelah selesai tampilkan, matikan buzzer
    digitalWrite(buzzer, LOW);

    // Reset
    speed = 0;
    flag1 = 0;
    flag2 = 0;
    blockStart1 = 0;
    blockStart2 = 0;

    lcd.clear();
    return;
  }

  // MODE STANDBY
  lcd.setCursor(0, 0);
  lcd.print("Waiting... ");

  lcd.setCursor(0, 1);
  lcd.print("No car detected ");

  // STANDBY BUZZER = OFF W
  digitalWrite(buzzer, LOW);
}