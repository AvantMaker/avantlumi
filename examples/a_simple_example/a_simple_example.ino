/*
 * AvantLumi - Simple Example
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * Description:
 * This is a minimal example that demonstrates the basic usage of the AvantLumi
 * library: initializing the library, setting a solid RGB color, disabling the
 * waver (flickering) effect, and turning the LED on. 
 * It serves as a quick starting point for new users.
 * 
 * Author: AvantMaker <admin@avantmaker.com>
 * Author Website: https://www.AvantMaker.com
 * Date: June 24, 2026
 * Version: 1.0.1
 * 
 * Hardware Requirements:
 * - ESP32-based microcontroller
 * - WS2812B LED strip (or compatible addressable LEDs)
 * - Appropriate power supply for your LED strip
 *
 * Dependencies:
 * - FastLED library (available at https://github.com/FastLED/FastLED)
 * 
 * License: MIT License
 * Repository: https://github.com/AvantMaker/AvantLumi
 * 
 * Usage Notes:
 * Monitor the serial output at 115200 baud to see initialization messages.
 * The update() function must be called continuously in loop() to keep the LEDs
 * responsive.
 * 
 * Compatibility: Tested with ESP32 DevKitC and DOIT ESP32 DevKit boards.
 */

#include "AvantLumi.h"

// LED Configuration
#define DATA_PIN 2
#define NUM_LEDS 17

// Initialize AvantLumi
AvantLumi myLumi(DATA_PIN, NUM_LEDS);

void setup() {
  Serial.begin(115200);
  Serial.println(F("AvantLumi Library Simple Example..."));

  // Initialize the LED controller
  if (myLumi.begin()) {
      Serial.println(F("AvantLumi initialized successfully!"));
  } else {
      Serial.println(F("AvantLumi initialization failed!"));
      return;
  }

  myLumi.setRGB(255, 255, 255);  // Set LED Color RGB Value 
  // setRGB(255, 255, 255) sets the LED to the RGB color (255, 255, 255).

  myLumi.setBright(3);           // Set LED brightness to level 3 (medium brightness level)
  // The setBright sets the LED brightness on a scale from 1 (darkest) to 5 (brightest).

  myLumi.setWaver(false);        // SetWaver Effect Off
  /*
  Note: The AvantLumi library is designed for smart home devices. 
  By default, the LEDs controlled by the AvantLumi library will 
  produce a flickering effect. This effect can be enabled or disabled 
  via the setWaver method. 
  Executing the statement myLumi.setWaver(false) will turn off the flickering effect;
  And use myLumi.setWaver(true); to turn flickering effect on.
  */

  myLumi.setSwitch("on");        // Set LED on
}

void loop() {
  // CRITICAL: Call update() continuously to update LEDs status and make LEDs responsive
  myLumi.update();
}
