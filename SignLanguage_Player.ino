#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pins for DFPlayer Mini
static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX

// Initialize DFPlayer Mini
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SIGN LANGUAGE");

  pinMode(13, OUTPUT);

  // Initialize DFPlayer Mini
  softwareSerial.begin(9600);
  if (player.begin(softwareSerial)) {
    Serial.println("DFPlayer Mini initialized.");
    player.volume(30);  // Set volume to 20 (out of 30)
  } 
  else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

void loop() {
  int s1 = analogRead(A0);
  int s2 = analogRead(A1);
  int s3 = analogRead(A2);
  int s4 = analogRead(A3);
  int s5 = analogRead(A6);

  if (s1 > 15) {
    digitalWrite(13, LOW);
    lcd.setCursor(0, 1);
    lcd.print(" WASHROOM ");
    player.play(2);
    delay(3000);

  } else if (s2 > 15) {
    digitalWrite(13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" DRINK WATER ");
    player.play(4);
    delay(3000);

  } else if (s3 > 15) {
    digitalWrite(13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" YES ");
    player.play(3);
    delay(3000);

  } else if (s4 > 15) {
    digitalWrite(13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" NO ");
    player.play(1);
    delay(3000);

  } else if (s5 > 15) {
    digitalWrite(13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" MEDICAL EMERGENCY ");
    player.play(9);
    delay(3000);
  }
}
