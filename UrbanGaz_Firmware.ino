/*
 * URBANGAZ ATTENDANCE SYSTEM FIRMWARE v0.6
 * Flow: Boot Logo -> Network Check -> Menu
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Org_01.h> 

#include "config.h"
#include "assets.h"
#include "animations.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum SystemState { STATE_BOOT, STATE_NETWORK_CHECK, STATE_MENU, STATE_ANIMATION };
SystemState currentState = STATE_BOOT;

int menuIndex = 0;
const char* menuItems[] = {"Face Unlock", "System Info"};

void setup() {
  Serial.begin(115200);

  // Hardware Init
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT); 
  pinMode(BTN_DOWN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin(I2C_SDA, I2C_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 failed"));
    for(;;);
  }
  
  // 1. Start Boot Sequence
  runBootSequence();
  
  // 2. Run Network Check immediately after boot
  runNetworkCheck();
  
  // 3. Go to Menu
  currentState = STATE_MENU;
}

void loop() {
  switch (currentState) {
    case STATE_MENU:
      handleMenu();
      break;
    // Other states handled within their specific functions for now
  }
}

// --- SEQUENCES ---

void runBootSequence() {
  display.clearDisplay();
  display.drawBitmap(47, 6, image_UGL_mono_bits, 36, 32, 1);
  
  display.setTextColor(SSD1306_WHITE);
  display.setFont(&Org_01);
  display.setCursor(28, 46); display.print("Urban GAZ LTD");
  display.setCursor(20, 54); display.print("Attendance Centre");
  display.display();

  // Robot Chirp
  tone(BUZZER_PIN, 2000, 100); delay(120);
  tone(BUZZER_PIN, 2500, 100); delay(120);
  tone(BUZZER_PIN, 3500, 150); delay(2000); 
}

void runNetworkCheck() {
  // A. Simulation Loop: Play the animation 2 times to simulate "Connecting..."
  for(int loop = 0; loop < 2; loop++) {
    for (int i = 0; i < anim_wifi_frames; i++) {
      display.clearDisplay();
      
      // Draw Animation Centered
      display.drawBitmap(32, 0, anim_wifi[i], 64, 64, 1);
      
      // Draw "Connecting" Text at bottom
      display.setFont(); // System Font
      display.setCursor(35, 55); 
      display.println("Connecting");
      
      display.display();
      delay(FRAME_DELAY);
    }
  }

  // B. Connected Success Screen
  display.clearDisplay();
  display.drawBitmap(32, 0, anim_wifi[anim_wifi_frames-1], 64, 64, 1); // Show last frame (usually full signal)
  display.setCursor(30, 55);
  display.println("CONNECTED!");
  display.display();
  
  // Success Beep
  tone(BUZZER_PIN, 1000, 100); delay(100);
  tone(BUZZER_PIN, 2000, 200); delay(1000);
}

void handleMenu() {
  display.clearDisplay();
  display.setFont(); 
  display.setCursor(0, 0); display.println("URBANGAZ MAIN MENU");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

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

  // Button Logic
  if (digitalRead(BTN_DOWN) == LOW) {
    menuIndex = !menuIndex; delay(200);
  }
  if (digitalRead(BTN_UP) == HIGH) {
    menuIndex = !menuIndex; delay(200);
  }
  if (digitalRead(BTN_SELECT) == LOW) {
    tone(BUZZER_PIN, 2000, 50); delay(200);
    // Add logic here to enter Face ID mode
  }
}
