✋ Gesture-to-Text-and-Speech Glove
A wearable system that converts finger gestures into text on an LCD and audio output through a DFPlayer Mini.

📘 Project Overview
This project provides an intuitive communication aid by mapping specific hand and finger gestures to pre-defined phrases. The core functionality relies on measuring the change in resistance when fingers are bent, translating these values into digital commands for dual output:

Visual Output: Displaying the phrase on an I2C LCD.
Auditory Output: Playing a corresponding MP3 file through a speaker.

The system supports:
5 single-finger gestures
3 combined multi-finger gestures
Real-time text display
Speech output

📦 Components List
Component	Quantity	Description
Arduino Nano (ATmega328P)	1	Core microcontroller
Flex Sensors	5	For all five fingers
10 kΩ Resistors	5	For voltage divider with each sensor
I2C 16×2 LCD (PCF8574)	1	Displays detected gesture text
DFPlayer Mini MP3 Module	1	Plays audio files
220 Ω Resistor	1	Protects DFPlayer RX pin
8Ω Speaker	1	Audio output
microSD Card (FAT32)	1	Stores MP3 files (0001.mp3…)
Breadboard	1	For prototyping
Jumper Wires	20–30	Connections
5V Power Source	1	USB or external 5V


🧩 System Architecture

The signal flow moves from physical gesture input to dual digital outputs:
graph TD
    A[Finger Gestures] --> B(Flex Sensors);
    B --> C(Voltage Divider);
    C --> D[Arduino Nano ATmega328P];
    D --> E{I2C LCD};
    D --> F{DFPlayer Mini};
    E --> G[Text Output];
    F --> H[Speaker/Speech Output];

🧠 How the System Works

Finger bends → flex sensor resistance increases.
Voltage divider converts resistance to analog voltage.
Arduino reads the voltage and identifies gesture levels.

Selected gesture maps to:
Text message → I2C LCD
Audio file → DFPlayer Mini
Speaker plays the corresponding MP3.

Supports both:
Single-finger gestures
Dual-finger combined gestures


📌 Wiring Connections (Arduino Nano – Sensors – LCD – DFPlayer – Speaker)

Below is the complete wiring table for the gesture-to-text + audio playback system.

## 1. Flex Sensor Connections (5 Sensors — Thumb to Little Finger)

Each flex sensor uses a voltage divider with a 10 kΩ resistor.

Flex Sensor Wiring Table
Finger	Flex Sensor Terminal → Arduino	Flex Sensor Terminal → 5V	Divider Resistor (10kΩ) → GND
Thumb	Output → A0	One terminal → 5V	A0 → 10kΩ → GND
Index	Output → A1	One terminal → 5V	A1 → 10kΩ → GND
Middle	Output → A2	One terminal → 5V	A2 → 10kΩ → GND
Ring	Output → A3	One terminal → 5V	A3 → 10kΩ → GND
Little	Output → A6	One terminal → 5V	A6 → 10kΩ → GND

Voltage Divider Layout (for each sensor):

5V ---- Flex Sensor ---- A(n) ---- 10kΩ ---- GND

## 2. I2C LCD (16x2) Connections
LCD Pin	Connects To	Notes
VCC	5V	Power
GND	GND	Ground
SDA	A4	I2C Data
SCL	A5	I2C Clock

Only two data wires needed due to I2C interface.

## 3. DFPlayer Mini Connections
DFPlayer Pin	Arduino Nano Pin	Notes
VCC	5V	Power
GND	GND	Common ground
RX	D2 (TX of SoftwareSerial) via 220Ω resistor	Protects DFPlayer 3.3V RX
TX	D3 (RX of SoftwareSerial)	Direct connection
SPK_1	Speaker +	Built-in amplifier output
SPK_2	Speaker –	Built-in amplifier output

Important: Use FAT32 microSD, file names:
0001.mp3, 0002.mp3, etc.

## 4. Speaker Connections
Speaker Pin	Connects To
+	DFPlayer SPK_1
–	DFPlayer SPK_2

No amplifier needed — DFPlayer drives small 8Ω speakers directly.

## 5. Power Distribution
Component	Power Source
Flex Sensors	5V, GND
LCD Module	5V, GND
DFPlayer Mini	5V, GND
Arduino Nano	USB or 5V supply

Ensure common ground for all modules.

## 6. Complete Wiring Diagram (Text Form)
Arduino Nano
│
├── Flex Sensor 1 → A0 → 10kΩ → GND
├── Flex Sensor 2 → A1 → 10kΩ → GND
├── Flex Sensor 3 → A2 → 10kΩ → GND
├── Flex Sensor 4 → A3 → 10kΩ → GND
├── Flex Sensor 5 → A6 → 10kΩ → GND
│
├── LCD SDA → A4
├── LCD SCL → A5
│
├── DFPlayer RX ← D2 (via 220Ω)
├── DFPlayer TX → D3
│
└── DFPlayer SPK_1/SPK_2 → 8Ω Speaker



## 📄 Notes

Use stable 5V supply; voltage dips cause DFPlayer issues.
Keep speaker wires short to avoid noise.
Use heat shrink or tape to secure flex sensors on glove.
Ensure all modules share common GND.


## CODE
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
