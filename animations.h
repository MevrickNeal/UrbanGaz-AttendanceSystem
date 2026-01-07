#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>
#include <pgmspace.h>

// --- Animation 1: Face ID Success ---
// Width: 64, Height: 64
const int anim_success_frames = 29; // Update this number based on your specific array count!
const byte PROGMEM anim_success[][512] = {
    // PASTE YOUR "Face Detection Confirm" ARRAY DATA HERE
    // Copy everything inside the { } from your first animation code block
    // Example: {0x00, ... }, {0x00, ... }
};

// --- Animation 2: Face ID Fail (Try Again) ---
// Width: 64, Height: 64
const int anim_fail_frames = 29; // Update this number based on your specific array count!
const byte PROGMEM anim_fail[][512] = {
    // PASTE YOUR "Restart/Fail" ARRAY DATA HERE
    // Copy everything inside the { } from your second animation code block
};

#endif
