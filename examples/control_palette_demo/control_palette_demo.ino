/*
 * AvantLumi - Palette Control Demo (Simplified)
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * Description:
 * This simplified example demonstrates how to use color palettes with the 
 * AvantLumi library. It cycles through the built-in color palettes, showing
 * how to use the setPalette() method to create beautiful gradient effects.
 * 
 * Author: AvantMaker <admin@avantmaker.com>
 * Author Website: https://www.AvantMaker.com
 * Date: May 10, 2026
 * Version: 1.0.1
 * 
 * Hardware Requirements:
 * - ESP32-based microcontroller (e.g., ESP32 DevKitC, DOIT ESP32 DevKit, etc.)
 * - WS2812B LED strip (or compatible addressable LEDs)
 * - Appropriate power supply for your LED strip
 * 
 * Dependencies:
 * - FastLED library (available at https://github.com/FastLED/FastLED)
 * 
 * License: MIT License
 * Repository: https://github.com/AvantMaker/avantlumi
 * 
 * Usage Notes:
 * This demo runs automatically and cycles through built-in color palettes.
 * Monitor the serial output at 115200 baud to see which palette is active.
 * 
 * Compatibility: Tested with ESP32 DevKitC and DOIT ESP32 DevKit boards.
 */

#include "AvantLumi.h"

// LED Configuration
#define DATA_PIN 2
#define NUM_LEDS 17

// Create an AvantLumi object
AvantLumi myLumi(DATA_PIN, NUM_LEDS);

// Demo state variables
int paletteIndex = 0;
unsigned long lastPaletteChange = 0;
const unsigned long paletteInterval = 10000; // Change palette every 10 seconds

// Built-in palette names
String builtinPalettes[] = {
    "rainbow",   // Classic rainbow colors
    "party",     // Vibrant party colors
    "ocean",     // Ocean blue tones
    "forest",    // Forest green tones
    "heat",      // Heat/fire colors
    "cloud",     // Soft cloud colors
    "lava"       // Lava red/orange tones
};
int numPalettes = sizeof(builtinPalettes) / sizeof(builtinPalettes[0]);

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    Serial.println(F("AvantLumi - Palette Control Demo (Simplified)"));

    // Set power limits for LED safety (5V, 500mA)
    myLumi.setMaxPower(5, 500);

    // Initialize the LED controller
    if (myLumi.begin()) {
        Serial.println(F("AvantLumi initialized successfully!"));
    } else {
        Serial.println(F("AvantLumi initialization failed! Check wiring."));
        while (true) delay(1000); // Halt if initialization fails
    }

    // Set initial configuration
    myLumi.setBright(3);      // Medium brightness (1-5)
    myLumi.setSwitch(true);   // Turn LEDs on
    myLumi.setWaver(true);    // Enable wave animation effect

    // Print initial status
    Serial.println(F("\nInitial Status:"));
    Serial.println(myLumi.getStatus());
    Serial.println(F("\nStarting palette demo..."));
}

void loop() {
    // CRITICAL: Call update() continuously to update LEDs
    myLumi.update();

    // Check if it's time to change palette
    unsigned long currentTime = millis();
    if (currentTime - lastPaletteChange >= paletteInterval) {
        lastPaletteChange = currentTime;
        
        // Get current palette name
        String currentPalette = builtinPalettes[paletteIndex];
        
        // Print status message
        Serial.print(F("\nSetting palette to: "));
        Serial.println(currentPalette);
        
        // Set the palette using setPalette()
        if (myLumi.setPalette(currentPalette)) {
            Serial.println(F("Palette changed successfully"));
        } else {
            Serial.println(F("Failed to change palette"));
        }
        
        // Print current palette
        Serial.print(F("Active palette: "));
        Serial.println(myLumi.getPalette());
        
        // Move to next palette (loop back when reaching the end)
        paletteIndex = (paletteIndex + 1) % numPalettes;
    }
}

/*
 * Key Concepts Demonstrated:
 * 
 * 1. What is a Color Palette?
 *    - A palette is a collection of colors arranged in a specific order
 *    - Unlike solid colors, palettes create gradient effects across LEDs
 *    - Each LED gets its color from the palette based on its position
 *    - Palettes enable smooth color transitions and visual effects
 * 
 * 2. setPalette(paletteName):
 *    - Sets the active color palette
 *    - Built-in palettes available:
 *      - "rainbow"  : Classic rainbow colors
 *      - "party"    : Vibrant party colors  
 *      - "ocean"    : Ocean blue tones
 *      - "forest"   : Forest green tones
 *      - "heat"     : Heat/fire colors
 *      - "cloud"    : Soft cloud colors
 *      - "lava"     : Lava red/orange tones
 *    - Returns true if palette was set successfully
 * 
 * 3. getPalette():
 *    - Returns the name of the currently active palette
 *    - Returns "solid_color" if using solid colors (setColor/setRGB)
 * 
 * 4. Waver Effect:
 *    - When enabled (setWaver(true)), LEDs animate with wave-like brightness
 *    - Works well with palettes to create dynamic effects
 *    - Disable with setWaver(false) for static colors
 * 
 * Basic Usage Pattern:
 *   AvantLumi myLumi(DATA_PIN, NUM_LEDS);
 *   myLumi.begin();
 *   myLumi.setBright(3);
 *   myLumi.setSwitch(true);
 *   myLumi.setWaver(true);
 *   myLumi.setPalette("rainbow");
 *   
 *   while(1) {
 *       myLumi.update();
 *   }
 */
