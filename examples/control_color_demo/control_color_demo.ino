/*
 * AvantLumi - Color Control Demo
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * Description:
 * This example demonstrates how to control LED colors using the AvantLumi library.
 * It showcases three key methods: setRGB() for direct RGB values, setColor() for
 * named colors, and setWaver() for enabling/disabling wave animation effects.
 * The demo cycles through different color settings to illustrate these features.
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
 * This demo runs automatically and does not require any user interaction.
 * It will cycle through various color settings. Monitor the serial output
 * at 115200 baud to see status messages for each step.
 * 
 * Compatibility: Tested with ESP32 DevKitC and DOIT ESP32 DevKit boards.
 */

#include "AvantLumi.h"

// LED Configuration
// Define the data pin connected to your LED strip
#define DATA_PIN 2

// Define the number of LEDs in your strip
#define NUM_LEDS 17

// Create an AvantLumi object
// Parameters: dataPin, numLeds
AvantLumi myLumi(DATA_PIN, NUM_LEDS);

// Demo state variables
int currentDemoPhase = 0;        // Tracks current demo phase
unsigned long lastPhaseChange = 0; // Tracks the last time we changed phase
const unsigned long phaseDuration = 10000; // Each phase lasts 10 seconds 

// Array of color names for demonstration
String colorNames[] = {"red", "green", "blue", "yellow", "purple", "cyan", "pink", "white"};
int numColors = sizeof(colorNames) / sizeof(colorNames[0]);
int colorIndex = 0;

void setup() {
    // Initialize serial communication for debugging
    // Baud rate: 115200
    Serial.begin(115200);
    Serial.println(F("AvantLumi - Color Control Demo Starting..."));

    // (Optional) Set power limits for LED safety
    // Parameters: volts, milliamps
    // Example: 5V, 500mA
    if (myLumi.setMaxPower(5, 500)) {
        Serial.println(F("Power limit set to 5V, 500mA"));
    } else {
        Serial.println(F("Warning: Invalid power settings, using defaults"));
    }

    // Initialize the LED controller
    // begin() must be called before any other methods
    if (myLumi.begin()) {
        Serial.println(F("AvantLumi initialized successfully!"));
    } else {
        Serial.println(F("AvantLumi initialization failed! Check your wiring."));
        // If initialization fails, we can't proceed
        while (true) {
            delay(1000);
        }
    }

    // Set initial configuration
    myLumi.setBright(3);          // Set brightness level (1-5)
    myLumi.setSwitch(true);       // Turn LEDs on
    myLumi.setWaver(false);       // Start with waver effect disabled

    // Print initial status to serial monitor
    Serial.println(F("\nInitial Status:"));
    Serial.println(myLumi.getStatus());
    
    Serial.println(F("\nStarting color control demo..."));
}

void loop() {
    // CRITICAL: Call update() continuously to keep LEDs responsive
    // This method must be called in the main loop
    myLumi.update();

    // Check if it's time to change demo phase
    unsigned long currentTime = millis();
    if (currentTime - lastPhaseChange >= phaseDuration) {
        // Update the last phase change time
        lastPhaseChange = currentTime;
        
        // Move to next phase
        currentDemoPhase = (currentDemoPhase + 1) % 5;
        
        // Execute current phase
        executeDemoPhase(currentDemoPhase);
    }
}

void executeDemoPhase(int phase) {
    switch(phase) {
        case 0:
            // Phase 1: Set color using RGB values
            Serial.println(F("\n=== Phase 1: Setting color with RGB values ==="));
            Serial.println(F("Using setRGB(255, 0, 0) - Pure Red"));
            myLumi.setRGB(255, 0, 0);  // Red
            myLumi.setWaver(false);     // Disable waver effect
            break;
            
        case 1:
            // Phase 2: Set color using RGB values (cyan)
            Serial.println(F("\n=== Phase 2: Setting color with RGB values ==="));
            Serial.println(F("Using setRGB(0, 255, 255) - Cyan"));
            myLumi.setRGB(0, 255, 255);  // Cyan
            break;
            
        case 2:
            // Phase 3: Set color using named color
            Serial.println(F("\n=== Phase 3: Setting color with named color ==="));
            colorIndex = (colorIndex + 1) % numColors;
            Serial.print(F("Using setColor(\""));
            Serial.print(colorNames[colorIndex]);
            Serial.println(F("\")"));
            myLumi.setColor(colorNames[colorIndex]);
            break;
            
        case 3:
            // Phase 4: Enable waver effect with current color
            Serial.println(F("\n=== Phase 4: Enabling waver effect ==="));
            Serial.println(F("Using setWaver(true)"));
            myLumi.setWaver(true);  // Enable wave animation
            break;
            
        case 4:
            // Phase 5: Another RGB color with waver
            Serial.println(F("\n=== Phase 5: RGB color with waver ==="));
            Serial.println(F("Using setRGB(128, 0, 255) - Purple with waver"));
            myLumi.setRGB(128, 0, 255);  // Purple
            break;
    }
    
    // Print current status after each phase change
    Serial.println(F("Current Status:"));
    Serial.println(myLumi.getStatus());
}

/*
 * Key Concepts Demonstrated:
 * 
 * 1. setRGB(r, g, b):
 *    - Sets color using direct RGB values (0-255 for each channel)
 *    - Example: setRGB(255, 0, 0) = Red
 *    - Example: setRGB(0, 255, 255) = Cyan
 *    - Example: setRGB(128, 0, 255) = Purple
 * 
 * 2. setColor(colorName):
 *    - Sets color using predefined named colors
 *    - Over 80 named colors supported including:
 *      - Basic: red, green, blue, white, black, yellow, cyan, magenta
 *      - Light: lightgreen, lightblue, lightpink
 *      - Dark: darkred, darkgreen, darkblue
 *      - Special: springgreen, forestgreen, hotpink, crimson, navy
 *    - Example: setColor("red"), setColor("hotpink"), setColor("springgreen")
 * 
 * 3. setWaver(state):
 *    - Enables or disables the wave/waver animation effect
 *    - When enabled, LEDs animate with a wave-like brightness variation
 *    - Accepts boolean: setWaver(true) or setWaver(false)
 *    - Accepts string: setWaver("on") or setWaver("off")
 *    - When disabled, all LEDs maintain constant brightness
 * 
 * 4. Color vs Palette:
 *    - setRGB() and setColor() set a solid color across all LEDs
 *    - setPalette() sets a color palette for gradient effects
 *    - When using solid colors, getPalette() returns "solid_color"
 * 
 * 5. Getting Color Information:
 *    - getRGB() returns current CRGB color value
 *    - getColor() returns current color name (if set via setColor())
 *    - getStatus() includes color information in JSON format
 * 
 * Important Notes:
 * - setRGB() and setColor() both set useSolidColor to true
 * - To use color palettes again, call setPalette() with a palette name
 * - The waver effect works with both solid colors and palettes
 * - Valid RGB values range from 0 to 255
 */
