/*
 * AvantLumi Library - Header File
 * 
 * By: AvantMaker.com
 * 
 * A FastLED-based library for controlling LED strips with color palettes,
 * brightness control, waver effects, and more.
 * 
 * ==================== CUSTOM PALETTE CUSTOMIZATION ====================
 * 
 * This library includes 10 user-customizable palettes (u01_p through u10_p) that
 * you can modify to create your own color schemes. See AvantLumi.cpp for detailed
 * instructions on how to customize these palettes.
 * 
 * You can use these palettes in two ways:
 * 1. By standardized name: setPalette("u01"), setPalette("u02"), etc.
 * 2. By nickname (if you keep the default theme): setPalette("christmas"), setPalette("autumn"), etc.
 * 
 * Note: When saving/loading configurations, only the standardized names (u01, u02, etc.)
 * are used to ensure consistency across reboots.
 * 
 * ==================== END CUSTOM PALETTE DOCUMENTATION ====================
 * 
 * ==================== IMPORTANT USAGE NOTES ====================
 * 
 * SINGLE INSTANCE ONLY:
 * Only one AvantLumi instance should be created per LED channel in your sketch.
 * FastLED does not support de-registering LED controllers, so destroying and
 * recreating an AvantLumi object at runtime will result in a dangling pointer
 * inside the FastLED registry. Declare your AvantLumi object globally and
 * call begin() once in setup().
 *
 * BRIGHTNESS LEVEL 0:
 * setBright(0) turns the LEDs off via the brightness channel while keeping the
 * LED strip active. This is distinct from setSwitch("off"), which disables the
 * strip via the software switch. Both can be used independently.
 * Valid brightness levels: 0 (off), 1 (dim) through 5 (full).
 * 
 * ==================== END USAGE NOTES ====================
 */

#ifndef AVANTLUMI_H
#define AVANTLUMI_H

#include "FastLED.h"
#include <Preferences.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Forward declaration for ESP32 dynamic pin controller
#ifdef ARDUINO_ARCH_ESP32
class CustomClocklessController;
#endif

class AvantLumi {
private:
    // LED configuration
    CRGB* leds;
    uint8_t dataPin;
    uint16_t numLeds;
    
    // State variables
    bool waverEffect;  // Enables a waver/wave animation across LEDs
    bool useRandomPalette;
    bool useSolidColor;
    CRGB solidColor;
    bool ledEnabled;
    uint8_t currentBrightnessLevel;
    uint8_t targetBrightness;
    uint8_t actualBrightness;
    unsigned long lastBrightnessUpdate;
    String currentPaletteName;
    String solidColorName;
    
    // Brightness levels (0-5)
    // Level 0 = LEDs off via brightness (strip remains active)
    // Level 1 = dim ... Level 5 = full brightness
    static const uint8_t brightnessLevels[6];
    uint8_t blendSpeed;  // 1-5 scale, 1=slowest, 5=fastest
    
    // Palette variables
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    TBlendType currentBlending;
    
    // Custom palette definitions - User customizable palettes (u01_p through u10_p)
    // These palettes can be modified by users to create their own custom color schemes
    // To customize: modify the color definitions in AvantLumi.cpp and recompile
    static const CRGBPalette16 u01_p;  // Default: Christmas theme (red/green)
    static const CRGBPalette16 u02_p;  // Default: Autumn theme (oranges/browns)
    static const CRGBPalette16 u03_p;  // Default: Cyberpunk theme (magenta/cyan)
    static const CRGBPalette16 u04_p;  // Default: Halloween theme (orange/purple)
    static const CRGBPalette16 u05_p;  // Default: Winter theme (blues/whites)
    static const CRGBPalette16 u06_p;  // Default: Spring theme (greens/pinks)
    static const CRGBPalette16 u07_p;  // Default: Sunset theme (reds/oranges)
    static const CRGBPalette16 u08_p;  // Default: Deep Ocean theme (dark blues)
    static const CRGBPalette16 u09_p;  // Default: Neon theme (bright colors)
    static const CRGBPalette16 u10_p;  // Default: Fire theme (reds/oranges)
    
    // Private helper methods
    CRGBPalette16 createSolidPalette(CRGB color);
    void updateBrightness();
    void updateLEDs();
    CRGB parseColorName(String colorName);
    bool isValidColorName(String colorName);
    void generateRandomPalette();
    void getBlendParameters(uint8_t speedLevel, unsigned long& interval, uint8_t& maxChanges);

    // Initialization state
    bool initialized;
    
    // ESP32 dynamic pin controller
    #ifdef ARDUINO_ARCH_ESP32
    CustomClocklessController* controller;
    #endif

    uint8_t maxVolts;
    uint32_t maxMilliamps;

public:
    // Constructor
    AvantLumi(uint8_t dataPin, uint16_t numLeds);
    
    // Destructor
    ~AvantLumi();
    
    // Initialization
    // Returns true on first successful call.
    // Returns true (no-op) on subsequent calls — already initialized.
    // Returns false only if the pin is unsupported (non-ESP32 path).
    bool begin();
    
    // Main update loop (call this in Arduino loop())
    // Safe to call before begin() — returns immediately if not initialized.
    void update();
    
    // Setter methods
    bool setRGB(uint8_t rVal, uint8_t gVal, uint8_t bVal);
    bool setColor(String colorName);
    // Set brightness level. Valid range: 0-5.
    // Level 0 turns LEDs off via brightness (distinct from setSwitch(false)).
    // Levels 1-5 set increasing brightness from dim to full.
    bool setBright(uint8_t level);
    bool setSwitch(const char* state);
    bool setSwitch(bool state);
    bool setWaver(String state);
    bool setWaver(bool state);
    bool setPalette(String paletteName);
    bool setBlendSpeed(uint8_t speed_val);
    
    // Getter methods
    CRGB getRGB();
    String getColor();
    uint8_t getBright();
    bool getSwitch();
    bool getWaver();
    String getPalette();
    String getStatus();
    uint8_t getBlendSpeed();

    bool setMaxPower(uint8_t voltsVal, uint32_t milliamps);
    uint8_t getMaxVolts();
    uint32_t getMaxMilliamps();    

    // Preferences configuration
    bool saveConfig();
    bool loadConfig();
    bool checkConfig();
};

#endif // AVANTLUMI_H
