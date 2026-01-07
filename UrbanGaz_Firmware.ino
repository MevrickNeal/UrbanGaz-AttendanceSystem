/*
 * URBANGAZ ATTENDANCE SYSTEM FIRMWARE
 * Author: Lian Mollick
 * Version: 0.5 (UI & Animation Test)
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Org_01.h> 

// Include modular files
#include "config.h"
#include "assets.h"
#include "animations.h"

// Initialize Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// System State Machine
enum SystemState { STATE_BOOT, STATE_MENU, STATE_ANIMATION };
SystemState currentState = STATE_BOOT;

// Menu Logic
int menuIndex = 0;
const char* menuItems[] = {"Test Success", "Test Fail"};

void setup() {
  Serial.begin(115200);

  // Pin Modes
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT); // Requires external pulldown resistor usually for GPIO 34
  pinMode(BTN_DOWN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT);

  // Init I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Init OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  runBootSequence();
}

void loop() {
  switch (currentState) {
    case STATE_MENU:
      handleMenu();
      break;
    case STATE_ANIMATION:
      // Animation handles its own loop blocking briefly
      currentState = STATE_MENU; 
      break;
  }
}

// --- LOGIC FUNCTIONS ---

void runBootSequence() {
  display.clearDisplay();
  
  // Draw Logo
  display.drawBitmap(47, 6, image_UGL_mono_bits, 36, 32, 1);

  // Draw Text
  display.setTextColor(SSD1306_WHITE);
  display.setFont(&Org_01);
  display.setCursor(28, 46); display.print("Urban GAZ LTD");
  display.setCursor(20, 54); display.print("Attendance Centre");
  display.display();

  // Robot Chirp Sound
  tone(BUZZER_PIN, 2000, 100); delay(120);
  tone(BUZZER_PIN, 2500, 100); delay(120);
  tone(BUZZER_PIN, 3500, 150); delay(1500); // Hold logo
  
  currentState = STATE_MENU;
}

void handleMenu() {
  display.clearDisplay();
  display.setFont(); // Reset font
  display.setCursor(0, 0); display.println("ANIMATION TEST");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Draw Menu Items
  for (int i = 0; i < 2; i++) {
    if (i == menuIndex) {
      display.fillRect(0, 20 + (i * 15), 128, 14, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(5, 23 + (i * 15));
    display.print(menuItems[i]);
  }
  display.display();

  // Button Inputs
  if (digitalRead(BTN_DOWN) == LOW) {
    menuIndex = !menuIndex; 
    playClick(); delay(200);
  }
  if (digitalRead(BTN_UP) == HIGH) { // Check your wiring logic for Pin 34
    menuIndex = !menuIndex; 
    playClick(); delay(200);
  }
  if (digitalRead(BTN_SELECT) == LOW) {
    playSelect(); delay(200);
    
    // Trigger Animation
    if (menuIndex == 0) playAnimation(anim_success, anim_success_frames);
    else playAnimation(anim_fail, anim_fail_frames);
  }
}

// Generic Animation Player
void playAnimation(const byte animArray[][512], int frames) {
  for (int i = 0; i < frames; i++) {
    display.clearDisplay();
    // Center 64x64 on 128x64 screen -> X=32, Y=0
    display.drawBitmap(32, 0, animArray[i], 64, 64, 1);
    display.display();
    delay(FRAME_DELAY);
  }
  delay(500); // Pause after animation
}

void playClick() { tone(BUZZER_PIN, 1000, 20); }
void playSelect() { tone(BUZZER_PIN, 2000, 50); }
