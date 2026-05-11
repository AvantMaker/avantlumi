/*
 * AvantLumi Serial Control
 * 
 * Note: The AvantLumi Library is designed for ESP32 boards. It may not function 
 * properly on resource-constrained boards like Arduino Uno.
 * 
 * By: AvantMaker.com
 * Date: August, 2025
 * version 1.0.0
 * 
 * This example allows you to control an LED strip using serial commands.
 * Connect to the serial monitor to send commands and control the LEDs.
 * 
 * Available Commands:
 * switch:on|off        - Turn LED strip ON or OFF
 * bright:1-5           - Set brightness level (1=low, 5=high)
 * waver:on|off       - Enable/disable waver effects
 * rgb:R_G_B            - Set solid color (e.g., rgb:255_0_0 for red)
 * color:ColorName      - Set solid color by name (e.g., color:LightGreen)
 * palette:palette_name - Set a color palette (e.g., palette:rainbow, palette:u01)
 * blend:1-5            - Set blend speed level (1=slowest, 5=fastest)
 * power:V_mA           - Set max power (e.g., power:5_500 for 5V, 500mA)
 * status               - Get immediate status report
 * config:save|load|check - Save, load, or check NVS config
 * help                 - Show this help message
 */

#include "AvantLumi.h"

// LED Configuration
#define DATA_PIN 2
#define NUM_LEDS 24

// Create AvantLumi object
AvantLumi myLumi(DATA_PIN, NUM_LEDS);

void setup() {
    Serial.begin(115200);
    Serial.println(F("AvantLumi Library Serial Control Example"));

    // Set power limits (e.g., 5V, 500mA)
    if (myLumi.setMaxPower(5, 500)) {
        Serial.println(F("Power limit set to 5V, 500mA"));
    } else {
        Serial.println(F("Invalid power settings"));
    }

    // Initialize the LED controller
    if (myLumi.begin()) {
        Serial.println(F("AvantLumi initialized successfully!"));
    } else {
        Serial.println(F("AvantLumi initialization failed!"));
        return;
    }

    // Print initial status and help
    Serial.println("Initial Status: " + myLumi.getStatus());
    printHelp();
}

void loop() {
    // CRITICAL: Call update() continuously to keep LEDs responsive
    myLumi.update();
    
    // Handle serial commands
    if (Serial.available()) {
        handleSerialCommand();
    }
}

void handleSerialCommand() {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    String cmd;
    String value;
    int colonIndex = command.indexOf(':');
    if (colonIndex != -1) {
        cmd = command.substring(0, colonIndex);
        value = command.substring(colonIndex + 1);
    } else {
        cmd = command;
    }

    if (cmd == "switch") {
        if (myLumi.setSwitch(value)) {
            Serial.println("Switch set to: " + value);
        } else {
            Serial.println(F("Invalid switch state. Use 'on' or 'off'"));
        }
    } else if (cmd == "bright") {
        int level = value.toInt();
        if (myLumi.setBright(level)) {
            Serial.println("Brightness set to level: " + String(level));
        } else {
            Serial.println(F("Invalid brightness level. Use 1-5"));
        }
    } else if (cmd == "waver") {
        if (myLumi.setWaver(value)) {
            Serial.println("Waver set to: " + value);
        } else {
            Serial.println(F("Invalid waver state. Use 'on' or 'off'"));
        }
    } else if (cmd == "rgb") {
        int r, g, b;
        if (sscanf(value.c_str(), "%d_%d_%d", &r, &g, &b) == 3) {
            if (myLumi.setRGB(r, g, b)) {
                Serial.println("RGB set to: " + String(r) + "," + String(g) + "," + String(b));
            } else {
                Serial.println(F("Failed to set RGB. Values must be 0-255."));
            }
        } else {
            Serial.println(F("Invalid RGB format. Use R_G_B (e.g., 255_0_0)"));
        }
    } else if (cmd == "palette") {
        if (myLumi.setPalette(value)) {
            Serial.println("Palette set to: " + value);
        } else {
            Serial.println("Invalid palette name: " + value);
        }
    } else if (cmd == "color") {
        if (myLumi.setColor(value)) {
            Serial.println("Color set to: " + value);
        } else {
            Serial.println("Invalid color name: " + value);
        }
    } else if (cmd == "blend") {
        int speedLevel = value.toInt();
        if (myLumi.setBlendSpeed(speedLevel)) {
            Serial.println("Blend speed set to level: " + String(speedLevel));
        } else {
            Serial.println(F("Invalid blend speed level. Use 1-5"));
        }
    } else if (cmd == "status") {
        Serial.println("Status: " + myLumi.getStatus());
    } else if (cmd == "config") {
        if (value == "save") {
            if (myLumi.saveConfig()) {
                Serial.println(F("Configuration saved to NVS."));
            } else {
                Serial.println(F("Failed to save configuration."));
            }
        } else if (value == "load") {
            if (myLumi.loadConfig()) {
                Serial.println(F("Configuration loaded from NVS."));
                Serial.println("New Status: " + myLumi.getStatus());
            } else {
                Serial.println(F("No valid configuration found in NVS."));
            }
        } else if (value == "check") {
            if (myLumi.checkConfig()) {
                Serial.println(F("Valid configuration found in NVS."));
            } else {
                Serial.println(F("No valid configuration found in NVS."));
            }
        } else {
            Serial.println(F("Invalid config command. Use 'save', 'load', or 'check'."));
        }
    } else if (cmd == "power") {
        int v, ma;
        if (sscanf(value.c_str(), "%d_%d", &v, &ma) == 2) {
            if (myLumi.setMaxPower(v, ma)) {
                Serial.println("Max power set to: " + String(v) + "V, " + String(ma) + "mA");
            } else {
                Serial.println(F("Invalid power settings."));
            }
        } else {
            Serial.println(F("Invalid power format. Use V_mA (e.g., 5_500)"));
        }
    } else if (cmd == "help") {
        printHelp();
    } else {
        Serial.println(F("Unknown command. Type 'help' for available commands."));
    }
}

void printHelp() {
    Serial.println(F("\n--- AvantLumi Serial Control Commands ---"));
    Serial.println(F("switch:on|off        - Turn LED strip ON or OFF"));
    Serial.println(F("bright:1-5           - Set brightness level (1=low, 5=high)"));
    Serial.println(F("waver:on|off       - Enable/disable waver effects"));
    Serial.println(F("rgb:R_G_B            - Set solid color (e.g., rgb:255_0_0 for red)"));
    Serial.println(F("color:ColorName      - Set solid color by name (e.g., color:LightGreen)"));
    Serial.println(F("palette:palette_name - Set a color palette (e.g., palette:rainbow, palette:u01)"));
    Serial.println(F("blend:1-5            - Set blend speed level (1=slowest, 5=fastest)"));
    Serial.println(F("power:V_mA           - Set max power (e.g., power:5_500 for 5V, 500mA)"));
    Serial.println(F("status               - Get immediate status report"));
    Serial.println(F("config:save|load|check - Save, load, or check NVS config"));
    Serial.println(F("help                 - Show this help message"));
    Serial.println(F("-----------------------------------------"));
}
