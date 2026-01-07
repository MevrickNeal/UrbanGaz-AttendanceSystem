#ifndef CONFIG_H
#define CONFIG_H

// --- OLED Display Configuration ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C  // Check 0x3C or 0x3D
#define OLED_RESET    -1     // Reset pin # (or -1 if sharing Arduino reset pin)

// --- I2C Pins (Freenove ESP32 Wrover) ---
#define I2C_SDA 21
#define I2C_SCL 22

// --- User Interface Pins ---
#define BTN_SELECT  19  // Active LOW (Pullup)
#define BTN_UP      34  // Active HIGH (Input only, needs resistor)
#define BTN_DOWN    33  // Active LOW (Pullup)
#define BUZZER_PIN  14  // Active HIGH

// --- Animation Settings ---
#define ANIM_FPS    30  // Target Frames Per Second
#define FRAME_DELAY 42  // ms delay between frames (approx 24fps)

#endif
