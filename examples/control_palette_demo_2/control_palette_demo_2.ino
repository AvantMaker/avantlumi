/*
 * AvantLumi - Custom Palette Demo
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * Description:
 * This example demonstrates how to use and customize color palettes in the 
 * AvantLumi library. It showcases the 10 user-customizable palettes (u01-u10)
 * and explains how to modify them to create your own custom color schemes.
 * 
 * Author: AvantMaker <admin@avantmaker.com>
 * Author Website: https://www.AvantMaker.com
 * Date: May 10, 2026
 * Version: 1.0.0
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
 * This demo runs automatically and cycles through the 10 customizable 
 * palettes. Monitor the serial output at 115200 baud for explanations 
 * about each palette and how to customize it.
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

// Customizable palette names (u01 through u10) with their default themes
struct CustomPaletteInfo {
    String standardName;  // Standardized name (used for config storage)
    String nickName;      // Friendly nickname (based on default theme)
    String description;   // Description of the default theme
};

CustomPaletteInfo customPalettes[] = {
    {"u01", "christmas", "Red and green holiday colors"},
    {"u02", "autumn", "Orange and brown autumn tones"},
    {"u03", "cyberpunk", "Neon pink and cyan futuristic colors"},
    {"u04", "halloween", "Orange and purple spooky colors"},
    {"u05", "winter", "Blue and white winter colors"},
    {"u06", "spring", "Light green and pink spring colors"},
    {"u07", "sunset", "Warm sunset colors (reds/oranges)"},
    {"u08", "deep_ocean", "Deep sea blues"},
    {"u09", "neon", "Bright neon colors"},
    {"u10", "fire", "Fire red and orange colors"}
};
int numCustomPalettes = sizeof(customPalettes) / sizeof(customPalettes[0]);

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println(F("AvantLumi - Custom Palette Demo Starting..."));

    // Set power limits for LED safety
    if (myLumi.setMaxPower(5, 500)) {
        Serial.println(F("Power limit set to 5V, 500mA"));
    } else {
        Serial.println(F("Warning: Invalid power settings, using defaults"));
    }

    // Initialize the LED controller
    if (myLumi.begin()) {
        Serial.println(F("AvantLumi initialized successfully!"));
    } else {
        Serial.println(F("AvantLumi initialization failed!"));
        while (true) delay(1000);
    }

    // Set initial configuration
    myLumi.setBright(3);
    myLumi.setSwitch(true);
    myLumi.setWaver(true);
    myLumi.setBlendSpeed(4);

    // Print introduction
    Serial.println(F("\n=========================================="));
    Serial.println(F("  CUSTOM PALETTE DEMONSTRATION"));
    Serial.println(F("=========================================="));
    Serial.println(F("\nThis demo shows how to use the 10 user-"));
    Serial.println(F("customizable palettes (u01-u10) in the"));
    Serial.println(F("AvantLumi library."));
    Serial.println(F("\nEach palette can be customized by"));
    Serial.println(F("modifying the color definitions in"));
    Serial.println(F("AvantLumi.cpp source file."));
    Serial.println(F("==========================================\n"));

    // Start with the first palette
    showPaletteInfo(0);
    myLumi.setPalette(customPalettes[0].standardName);
    lastPaletteChange = millis();
}

void loop() {
    // CRITICAL: Call update() continuously
    myLumi.update();

    // Check if it's time to change palette
    unsigned long currentTime = millis();
    if (currentTime - lastPaletteChange >= paletteInterval) {
        lastPaletteChange = currentTime;
        
        // Cycle through custom palettes
        paletteIndex = (paletteIndex + 1) % numCustomPalettes;
        
        // Show palette info and set it
        showPaletteInfo(paletteIndex);
        myLumi.setPalette(customPalettes[paletteIndex].standardName);
        
        // Print current status
        Serial.print(F("Active palette: "));
        Serial.println(myLumi.getPalette());
        Serial.println();
    }
}

void showPaletteInfo(int index) {
    Serial.println(F("------------------------------------------"));
    Serial.print(F("Palette: "));
    Serial.print(customPalettes[index].standardName);
    Serial.print(F(" ("));
    Serial.print(customPalettes[index].nickName);
    Serial.println(F(")"));
    Serial.print(F("Description: "));
    Serial.println(customPalettes[index].description);
    Serial.println(F("------------------------------------------"));
    
    // Explain how to customize this palette
    Serial.print(F("To customize "));
    Serial.print(customPalettes[index].standardName);
    Serial.println(F(":"));
    Serial.println(F("1. Open AvantLumi.cpp in a text editor"));
    Serial.print(F("2. Find the definition for "));
    Serial.println(customPalettes[index].standardName + "_p");
    Serial.println(F("3. Modify the CRGB color values"));
    Serial.println(F("4. Recompile and upload your sketch"));
    Serial.println();
    
    // Show usage examples
    Serial.println(F("Usage Examples:"));
    Serial.print(F("  myLumi.setPalette(\""));
    Serial.print(customPalettes[index].standardName);
    Serial.println(F("\");  // Using standard name"));
    Serial.print(F("  myLumi.setPalette(\""));
    Serial.print(customPalettes[index].nickName);
    Serial.println(F("\");  // Using nickname"));
    Serial.println();
}

/*
 * Key Concepts Demonstrated:
 * 
 * 1. Custom Palettes Overview:
 *    - AvantLumi includes 10 user-customizable palettes (u01_p through u10_p)
 *    - These are defined in AvantLumi.cpp as CRGBPalette16 arrays
 *    - Each palette contains 16 color positions
 * 
 * 2. How to Customize a Palette:
 *    - Locate the palette definition in AvantLumi.cpp (search for "u01_p", "u02_p", etc.)
 *    - Modify the CRGB color values
 *    - Recompile the library/sketch
 * 
 * 3. Palette Structure:
 *    Each palette is defined as:
 *    const CRGBPalette16 AvantLumi::uXX_p = CRGBPalette16(
 *        CRGB::Color1, CRGB::Color2, CRGB::Color3, CRGB::Color4,
 *        CRGB::Color5, CRGB::Color6, CRGB::Color7, CRGB::Color8,
 *        CRGB::Color9, CRGB::Color10, CRGB::Color11, CRGB::Color12,
 *        CRGB::Color13, CRGB::Color14, CRGB::Color15, CRGB::Color16);
 * 
 * 4. Using Custom Palettes:
 *    - By standardized name: setPalette("u01"), setPalette("u02"), etc.
 *    - By nickname: setPalette("christmas"), setPalette("autumn"), etc.
 *    - Both methods work identically
 * 
 * 5. Important Notes:
 *    - Configuration storage uses only standardized names (u01-u10)
 *    - Nicknames are based on the default themes and may not match
 *      if you customize the palette
 *    - You can use any CRGB color values (0-255 for R, G, B)
 *    - Predefined FastLED colors are available (CRGB::Red, CRGB::Blue, etc.)
 * 
 * 6. Example Customization:
 *    To create a custom rainbow palette for u01_p:
 *    const CRGBPalette16 AvantLumi::u01_p = CRGBPalette16(
 *        CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green,
 *        CRGB::Cyan, CRGB::Blue, CRGB::Purple, CRGB::Pink,
 *        CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green,
 *        CRGB::Cyan, CRGB::Blue, CRGB::Purple, CRGB::Pink);
 * 
 * 7. Default Palette Themes:
 *    u01: Christmas (red/green)
 *    u02: Autumn (orange/brown)
 *    u03: Cyberpunk (magenta/cyan)
 *    u04: Halloween (orange/purple)
 *    u05: Winter (blue/white)
 *    u06: Spring (green/pink)
 *    u07: Sunset (red/orange)
 *    u08: Deep Ocean (dark blue)
 *    u09: Neon (bright colors)
 *    u10: Fire (red/orange)
 */
