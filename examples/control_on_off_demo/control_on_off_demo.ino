/*
 * AvantLumi - Control On/Off Demo
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * Description:
 * This example demonstrates how to turn LEDs on and off using the AvantLumi library.
 * It provides a simple demonstration of the setSwitch() method with both string
 * and boolean parameters. The LEDs will cycle between on and off states every
 * few seconds, showing the basic on/off control functionality.
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
 * This demo runs automatically and does not require any user interaction.
 * It will cycle the LEDs between on and off states. Monitor the serial output
 * at 115200 baud to see status messages.
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
bool ledState = false;          // Tracks current LED state (on/off)
unsigned long lastToggleTime = 0; // Tracks the last time we toggled the state
const unsigned long toggleInterval = 3000; // Toggle every 3 seconds (3000ms)

void setup() {
    // Initialize serial communication for debugging
    // Baud rate: 115200
    Serial.begin(115200);
    Serial.println(F("AvantLumi - Control On/Off Demo Starting..."));

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

    // Set initial LED configuration
    // Set a solid color for better visibility
    myLumi.setColor("blue");
    
    // Set brightness level (1-5, where 1=dimmest, 5=brightest)
    myLumi.setBright(3);

    // Print initial status to serial monitor
    Serial.println(F("\nInitial Status:"));
    Serial.println(myLumi.getStatus());
    
    Serial.println(F("\nStarting LED on/off cycle..."));
}

void loop() {
    // CRITICAL: Call update() continuously to keep LEDs responsive
    // This method must be called in the main loop
    myLumi.update();

    // Check if it's time to toggle the LED state
    // Using millis() for non-blocking timing
    unsigned long currentTime = millis();
    if (currentTime - lastToggleTime >= toggleInterval) {
        // Update the last toggle time
        lastToggleTime = currentTime;
        
        // Toggle the LED state
        ledState = !ledState;
        
        // Turn LEDs on or off using setSwitch() method
        
        // Option 1: Using boolean parameter
        // myLumi.setSwitch(ledState);
        
        // Option 2: Using string parameter ("on" or "off")
        if (ledState) {
            myLumi.setSwitch("on");
        } else {
            myLumi.setSwitch("off");
        }
        
        // Print status message
        Serial.print(F("LEDs turned "));
        Serial.println(ledState ? F("ON") : F("OFF"));
        
        // Print current status
        Serial.println(F("Current Status: "));
        Serial.println(myLumi.getStatus());
        Serial.println();
    }
}

/*
 * Key Concepts Demonstrated:
 * 
 * 1. Initialization:
 *    - Include the AvantLumi library
 *    - Create an AvantLumi object with data pin and LED count
 *    - Call begin() to initialize the LED controller
 * 
 * 2. Power Management (Optional):
 *    - Use setMaxPower() to set voltage and current limits
 *    - This helps protect your power supply and LED strip
 * 
 * 3. Basic Configuration:
 *    - setColor() or setRGB() to set LED color
 *    - setBright() to set brightness level (1-5)
 * 
 * 4. On/Off Control:
 *    - setSwitch(true) or setSwitch("on") to turn LEDs on
 *    - setSwitch(false) or setSwitch("off") to turn LEDs off
 *    - getSwitch() to check current state (returns boolean)
 * 
 * 5. Status Monitoring:
 *    - getStatus() returns JSON string with current configuration
 *    - Useful for debugging and monitoring
 * 
 * 6. Important Notes:
 *    - Always call update() in the main loop for responsive LEDs
 *    - The switch state is independent of brightness level
 *    - setBright(0) can also turn off LEDs via brightness channel
 *      while setSwitch(false) disables the strip completely
 */
