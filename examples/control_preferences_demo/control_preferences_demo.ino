/*
 * AvantLumi - Preferences Demo
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * Description:
 * This example demonstrates how to use the configuration management features
 * of the AvantLumi library. It shows how to save, load, and check preferences
 * using the ESP32's Non-Volatile Storage (NVS) through the Preferences library.
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
 * - ESP32 Preferences library (built into ESP32 Arduino core)
 * 
 * License: MIT License
 * Repository: https://github.com/AvantMaker/avantlumi
 * 
 * Usage Notes:
 * This demo runs automatically and demonstrates configuration persistence.
 * It will first check for saved configuration, load it if available, and then
 * periodically save the current configuration. Monitor the serial output at 
 * 115200 baud to see the configuration management in action.
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
bool configLoaded = false;
unsigned long lastSaveTime = 0;
const unsigned long saveInterval = 10000; // Save every 10 seconds
int demoPhase = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println(F("AvantLumi - Preferences Demo Starting..."));

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

    // Print introduction about Preferences
    Serial.println(F("\n=========================================="));
    Serial.println(F("  PREFERENCES DEMONSTRATION"));
    Serial.println(F("=========================================="));
    Serial.println(F("\nAbout Preferences:"));
    Serial.println(F("The ESP32 has Non-Volatile Storage (NVS)"));
    Serial.println(F("which allows data to persist even when"));
    Serial.println(F("power is removed. The Preferences library"));
    Serial.println(F("provides an easy interface to store key-value"));
    Serial.println(F("pairs in NVS."));
    Serial.println(F("\nAvantLumi uses Preferences to store:"));
    Serial.println(F("- LED switch state (on/off)"));
    Serial.println(F("- Brightness level"));
    Serial.println(F("- Waver effect state"));
    Serial.println(F("- Current palette/color"));
    Serial.println(F("- Blend speed"));
    Serial.println(F("- Power limits (voltage/current)"));
    Serial.println(F("==========================================\n"));

    // Step 1: Check if configuration exists
    Serial.println(F("Step 1: Checking for saved configuration..."));
    if (myLumi.checkConfig()) {
        Serial.println(F("  ✓ Configuration found in NVS"));
        
        // Step 2: Load the configuration
        Serial.println(F("\nStep 2: Loading configuration..."));
        if (myLumi.loadConfig()) {
            Serial.println(F("  ✓ Configuration loaded successfully"));
            configLoaded = true;
        } else {
            Serial.println(F("  ✗ Failed to load configuration"));
            setDefaultConfig();
        }
    } else {
        Serial.println(F("  ✗ No saved configuration found"));
        
        // Set default configuration
        Serial.println(F("\nStep 2: Setting default configuration..."));
        setDefaultConfig();
        Serial.println(F("  ✓ Default configuration set"));
    }

    // Print current status after configuration
    Serial.println(F("\nCurrent Configuration:"));
    Serial.println(myLumi.getStatus());
    
    // Start the demo
    Serial.println(F("\nStarting preferences demo..."));
    lastSaveTime = millis();
}

void loop() {
    // CRITICAL: Call update() continuously
    myLumi.update();

    // Get current time
    unsigned long currentTime = millis();

    // Change configuration periodically to demonstrate saving
    if (currentTime - lastSaveTime >= saveInterval) {
        lastSaveTime = currentTime;
        
        // Cycle through different configurations
        demoPhase = (demoPhase + 1) % 4;
        
        Serial.println(F("\n------------------------------------------"));
        Serial.print(F("Phase "));
        Serial.println(demoPhase);
        Serial.println(F("------------------------------------------"));
        
        switch(demoPhase) {
            case 0:
                // Set rainbow palette with medium brightness
                Serial.println(F("Setting rainbow palette, brightness level 3"));
                myLumi.setPalette("rainbow");
                myLumi.setBright(3);
                break;
                
            case 1:
                // Set fire palette with higher brightness
                Serial.println(F("Setting fire palette, brightness level 4"));
                myLumi.setPalette("fire");
                myLumi.setBright(4);
                break;
                
            case 2:
                // Set solid color
                Serial.println(F("Setting solid blue color"));
                myLumi.setColor("blue");
                break;
                
            case 3:
                // Toggle waver effect and save
                Serial.println(F("Toggling waver effect"));
                myLumi.setWaver(!myLumi.getWaver());
                break;
        }
        
        // Step 3: Save the configuration
        Serial.println(F("\nStep 3: Saving configuration to NVS..."));
        if (myLumi.saveConfig()) {
            Serial.println(F("  ✓ Configuration saved successfully"));
        } else {
            Serial.println(F("  ✗ Failed to save configuration"));
        }
        
        // Print current status
        Serial.println(F("\nCurrent Status after change:"));
        Serial.println(myLumi.getStatus());
        
        // Verify the save by checking again
        Serial.println(F("\nVerifying save..."));
        if (myLumi.checkConfig()) {
            Serial.println(F("  ✓ Configuration exists in NVS"));
        } else {
            Serial.println(F("  ✗ Configuration not found!"));
        }
    }
}

void setDefaultConfig() {
    // Set default configuration values
    myLumi.setBright(3);           // Medium brightness
    myLumi.setSwitch(true);        // LEDs on
    myLumi.setWaver(true);         // Waver effect enabled
    myLumi.setPalette("rainbow");   // Rainbow palette
    myLumi.setBlendSpeed(4);       // Fast blending
}

/*
 * Key Concepts Demonstrated:
 * 
 * 1. What is Preferences/NVS?
 *    - NVS (Non-Volatile Storage) is a partition on the ESP32 flash memory
 *    - Data stored in NVS persists even when power is removed
 *    - The Preferences library provides a key-value store interface
 *    - Each "namespace" acts like a separate storage container
 *    - AvantLumi uses "AvantLumi" as its namespace
 * 
 * 2. checkConfig():
 *    - Checks if a valid configuration exists in NVS
 *    - Returns true if configuration exists, false otherwise
 *    - Uses Preferences.isKey() to check for the "ledEnabled" key
 *    - Always call this before loadConfig() to avoid errors
 * 
 * 3. loadConfig():
 *    - Loads saved configuration from NVS
 *    - Restores all saved settings:
 *      - ledEnabled: Switch state
 *      - brightness: Brightness level (1-5)
 *      - waver: Waver effect state
 *      - palette/color settings
 *      - blendSpeed: Palette blending speed
 *      - maxVolts/maxMilliamps: Power limits
 *    - Returns true on success, false on failure
 * 
 * 4. saveConfig():
 *    - Saves current configuration to NVS
 *    - Stores all current settings in key-value pairs
 *    - Creates the "AvantLumi" namespace if it doesn't exist
 *    - Returns true on success, false on failure
 * 
 * 5. Configuration Persistence:
 *    - Saved configurations survive power cycles and reboots
 *    - Useful for maintaining user settings
 *    - Automatically restores last state on power-up
 * 
 * 6. Usage Pattern:
 *    void setup() {
 *        myLumi.begin();
 *        if (myLumi.checkConfig()) {
 *            myLumi.loadConfig();
 *        } else {
 *            // Set default configuration
 *            myLumi.setBright(3);
 *            myLumi.setPalette("rainbow");
 *        }
 *    }
 *    
 *    void loop() {
 *        myLumi.update();
 *        // ... make changes ...
 *        myLumi.saveConfig(); // Save when settings change
 *    }
 * 
 * 7. Important Notes:
 *    - NVS has limited space (typically 512KB or 1MB)
 *    - Avoid saving too frequently to reduce flash wear
 *    - Always check if config exists before loading
 *    - The Preferences library is built into ESP32 Arduino core
 *    - Configuration uses standardized palette names (u01-u10) for consistency
 */
