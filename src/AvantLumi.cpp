/*
 * AvantLumi Library - Implementation File
 * 
 * By: AvantMaker.com
 * 
 * A FastLED-based library for controlling LED strips with color palettes,
 * brightness control, waver effects, and more - without MQTT dependencies.
 * 
 * DYNAMIC PIN CONFIGURATION:
 * This library now supports runtime pin configuration on ESP32 platforms,
 * allowing users to use any GPIO pin as the data pin for LED strips.
 * This is implemented using ESP32's RMT (Remote Control) peripheral.
 * 
 * PLEASE NOTE THIS LIBRARY IS DESIGNED FOR ESP32 PLATFORMS ONLY. However,
 * if you want to try using this library on  non-ESP32 platforms, 
 * the library falls back to a switch statement
 * approach with predefined supported pins: 2, 3, 4, 5, 11, 12, 13
 * If you need additional pins on non-ESP32 platforms, you can easily add 
 * more cases to the switch statement.
 * 
 * ==================== CUSTOM PALETTE CUSTOMIZATION ====================
 * 
 * This library includes 10 user-customizable palettes (u01_p through u10_p) that
 * you can modify to create your own color schemes. These palettes are defined
 * below with default themes, but you can completely customize them.
 * 
 * HOW TO CUSTOMIZE:
 * 1. Locate the palette definitions below (search for "u01_p", "u02_p", etc.)
 * 2. Modify the CRGB color values in the palette definition
 * 3. Recompile your sketch
 * 
 * PALETTE STRUCTURE:
 * Each palette is a CRGBPalette16 with 16 color positions. You can use any
 * of the predefined FastLED colors (CRGB::Red, CRGB::Blue, etc.) or define
 * your own colors using CRGB(r, g, b) where r, g, b are values from 0-255.
 * 
 * EXAMPLE CUSTOMIZATION:
 * To create a custom rainbow palette for u01_p:
 * const CRGBPalette16 AvantLumi::u01_p = CRGBPalette16(
 *     CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green,
 *     CRGB::Cyan, CRGB::Blue, CRGB::Purple, CRGB::Pink,
 *     CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green,
 *     CRGB::Cyan, CRGB::Blue, CRGB::Purple, CRGB::Pink);
 * 
 * USING CUSTOM PALETTES:
 * You can use your custom palettes in two ways:
 * 1. By standardized name: setPalette("u01"), setPalette("u02"), etc.
 * 2. By nickname (if you keep the default theme): setPalette("christmas"), setPalette("autumn"), etc.
 * 
 * Note: When saving/loading configurations, only the standardized names (u01, u02, etc.)
 * are used to ensure consistency across reboots.
 * 
 * ==================== END CUSTOM PALETTE DOCUMENTATION ====================
 */

#include "AvantLumi.h"

#ifdef ARDUINO_ARCH_ESP32
// Include ESP32-specific FastLED headers
#include "platforms/esp/32/rmt_5/idf5_clockless_rmt_esp32.h"
#include "platforms/esp/32/rmt_5/idf5_rmt.h"

// Custom ClocklessController for ESP32 with dynamic pin configuration
// IMPORTANT: This controller is hardcoded to GRB color order for WS2812B strips.
// This is because this Library is designed for WS2812B strips only. However,
// if you want to try using this Library on 
// LED strip that uses a different color order (e.g., RGB, RGBW), you will need
// to modify the template parameter <GRB> below and recompile the library.
class CustomClocklessController : public CPixelLEDController<GRB> {
private:
    fl::RmtController5 mRMTController;
    
public:
    CustomClocklessController(uint8_t dataPin) 
        : mRMTController(dataPin, 300, 900, 600, fl::RmtController5::DMA_AUTO) {
        // WS2812B timing values: T1=300, T2=900, T3=600 (in nanoseconds)
    }
    
    void init() override {}
    
    virtual uint16_t getMaxRefreshRate() const override { 
        return 800; 
    }
    
protected:
    // Prepares data for the draw
    virtual void showPixels(PixelController<GRB> &pixels) override {
        fl::PixelIterator iterator = pixels.as_iterator(this->getRgbw());
        mRMTController.loadPixelData(iterator);
    }
    
    virtual void endShowLeds(void *data) override {
        CPixelLEDController<GRB>::endShowLeds(data);
        mRMTController.showPixels();
    }
};
#endif

// ESP32 platform detection macro for all ESP32 variants (classic, S2, S3, C3, C6, etc.)
// ARDUINO_ARCH_ESP32 is defined by all ESP32 Arduino cores.
#ifndef IS_ESP32
  #ifdef ARDUINO_ARCH_ESP32
    #define IS_ESP32 1
  #else
    #define IS_ESP32 0
  #endif
#endif

// Static member definitions
const uint8_t AvantLumi::brightnessLevels[6] = {0, 26, 64, 128, 192, 255};

// Custom palette definitions - User customizable palettes (u01_p through u10_p)
// These palettes can be modified by users to create their own custom color schemes
// To customize: modify these color definitions and recompile your sketch

// u01_p - Default: Christmas theme (red/green)
const CRGBPalette16 AvantLumi::u01_p = CRGBPalette16(
    CRGB::Red, CRGB::DarkRed, CRGB::Green, CRGB::DarkGreen,
    CRGB::Red, CRGB::Green, CRGB::DarkRed, CRGB::ForestGreen,
    CRGB::Crimson, CRGB::LimeGreen, CRGB::Red, CRGB::Green,
    CRGB::DarkRed, CRGB::SeaGreen, CRGB::Red, CRGB::DarkGreen);

// u02_p - Default: Autumn theme (oranges/browns)
const CRGBPalette16 AvantLumi::u02_p = CRGBPalette16(
    CRGB::DarkOrange, CRGB::OrangeRed, CRGB::Maroon, CRGB::SaddleBrown,
    CRGB::Orange, CRGB::Chocolate, CRGB::DarkGoldenrod, CRGB::FireBrick,
    CRGB::Peru, CRGB::Sienna, CRGB::DarkOrange, CRGB::Brown,
    CRGB::Goldenrod, CRGB::IndianRed, CRGB::Orange, CRGB::SaddleBrown);

// u03_p - Default: Cyberpunk theme (magenta/cyan)
const CRGBPalette16 AvantLumi::u03_p = CRGBPalette16(
    CRGB::Magenta, CRGB::DeepPink, CRGB::Purple, CRGB::Cyan,
    CRGB::HotPink, CRGB::DarkViolet, CRGB::Aqua, CRGB::Fuchsia,
    CRGB::MediumOrchid, CRGB::Turquoise, CRGB::Violet, CRGB::DeepSkyBlue,
    CRGB::Magenta, CRGB::Purple, CRGB::Cyan, CRGB::HotPink);

// u04_p - Default: Halloween theme (orange/purple)
const CRGBPalette16 AvantLumi::u04_p = CRGBPalette16(
    CRGB::Orange, CRGB::DarkOrange, CRGB::Purple, CRGB::Black,
    CRGB::OrangeRed, CRGB::DarkViolet, CRGB::Orange, CRGB::Indigo,
    CRGB::Chocolate, CRGB::Purple, CRGB::DarkOrange, CRGB::MidnightBlue,
    CRGB::Orange, CRGB::DarkSlateBlue, CRGB::SaddleBrown, CRGB::Purple);

// u05_p - Default: Winter theme (blues/whites)
const CRGBPalette16 AvantLumi::u05_p = CRGBPalette16(
    CRGB::Blue, CRGB::DarkBlue, CRGB::White, CRGB::LightSteelBlue,
    CRGB::SteelBlue, CRGB::PowderBlue, CRGB::LightBlue, CRGB::Navy,
    CRGB::CornflowerBlue, CRGB::AliceBlue, CRGB::RoyalBlue, CRGB::White,
    CRGB::DodgerBlue, CRGB::LightCyan, CRGB::Blue, CRGB::MidnightBlue);

// u06_p - Default: Spring theme (greens/pinks)
const CRGBPalette16 AvantLumi::u06_p = CRGBPalette16(
    CRGB::LightGreen, CRGB::Pink, CRGB::LightPink, CRGB::PaleGreen,
    CRGB::SpringGreen, CRGB::Thistle, CRGB::LemonChiffon, CRGB::LightCyan,
    CRGB::MediumSpringGreen, CRGB::Lavender, CRGB::LightYellow, CRGB::Aquamarine,
    CRGB::LightGreen, CRGB::Pink, CRGB::Honeydew, CRGB::PaleTurquoise);

// u07_p - Default: Sunset theme (reds/oranges)
const CRGBPalette16 AvantLumi::u07_p = CRGBPalette16(
    CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Pink,
    CRGB::Crimson, CRGB::DarkOrange, CRGB::Gold, CRGB::DeepPink,
    CRGB::OrangeRed, CRGB::Coral, CRGB::Orange, CRGB::LightPink,
    CRGB::Red, CRGB::Tomato, CRGB::Yellow, CRGB::HotPink);

// u08_p - Default: Deep Ocean theme (dark blues)
const CRGBPalette16 AvantLumi::u08_p = CRGBPalette16(
    CRGB::DarkBlue, CRGB::MidnightBlue, CRGB::Navy, CRGB::DarkSlateBlue,
    CRGB::Indigo, CRGB::DarkTurquoise, CRGB::SteelBlue, CRGB::DarkCyan,
    CRGB::MediumBlue, CRGB::DarkSeaGreen, CRGB::CadetBlue, CRGB::DarkSlateGray,
    CRGB::DarkBlue, CRGB::Teal, CRGB::Navy, CRGB::MidnightBlue);

// u09_p - Default: Neon theme (bright colors)
const CRGBPalette16 AvantLumi::u09_p = CRGBPalette16(
    CRGB::Lime, CRGB::Cyan, CRGB::Magenta, CRGB::Yellow,
    CRGB::SpringGreen, CRGB::Aqua, CRGB::Fuchsia, CRGB::GreenYellow,
    CRGB::LimeGreen, CRGB::DeepSkyBlue, CRGB::HotPink, CRGB::Gold,
    CRGB::Lime, CRGB::Turquoise, CRGB::Violet, CRGB::Yellow);

// u10_p - Default: Fire theme (reds/oranges)
const CRGBPalette16 AvantLumi::u10_p = CRGBPalette16(
    CRGB::Red, CRGB::OrangeRed, CRGB::Orange, CRGB::DarkRed,
    CRGB::Crimson, CRGB::DarkOrange, CRGB::Yellow, CRGB::FireBrick,
    CRGB::Red, CRGB::Coral, CRGB::Gold, CRGB::Maroon,
    CRGB::Tomato, CRGB::Orange, CRGB::Red, CRGB::DarkRed);

// Constructor
AvantLumi::AvantLumi(uint8_t dataPin, uint16_t numLeds) {
    this->dataPin = dataPin;
    this->numLeds = numLeds;
    this->leds = new CRGB[numLeds];
    
    // Initialize state variables
    waverEffect = true;  // Waver/wave animation enabled by default
    useRandomPalette = true;
    useSolidColor = false;
    solidColor = CRGB::Red;
    ledEnabled = true;
    currentBrightnessLevel = 3;
    targetBrightness = brightnessLevels[currentBrightnessLevel];
    actualBrightness = brightnessLevels[currentBrightnessLevel];
    lastBrightnessUpdate = 0;
    currentPaletteName = "party";
    solidColorName = "";
    blendSpeed = 4;  // Default to level 4 (fast blending)
    
    // Initialize palettes
    currentPalette = PartyColors_p;
    targetPalette = PartyColors_p;
    currentBlending = LINEARBLEND;

    // Initialize power settings with defaults
    maxVolts = 5;        // Default 5V
    maxMilliamps = 500;  // Default 500mA

    // Initialize initialization state
    initialized = false;
    
    // Initialize controller pointer
    #if IS_ESP32
    controller = nullptr;
    #endif
}

// Destructor
// The ESP32 controller pointer is nulled BEFORE delete to prevent
// any re-entrant destruction in edge cases. Note: FastLED does not provide
// a removeLeds() API, so the FastLED registry will hold an invalid
// pointer after destruction. Do not destroy and recreate AvantLumi at runtime.
AvantLumi::~AvantLumi() {
    #if IS_ESP32
    if (controller != nullptr) {
        CustomClocklessController* tmp = controller;
        controller = nullptr;  // Null the member first to prevent re-entrant access
        delete tmp;
    }
    #endif
    delete[] leds;
    leds = nullptr;
}

// Initialization
// Returns true if already initialized (system is in a valid state),
// rather than false, which was ambiguous with a real failure.
// On non-ESP32, an unsupported pin now causes an immediate return false
// WITHOUT registering any FastLED controller — previously a spurious
// controller was added on pin 2 before the function returned false.
bool AvantLumi::begin() {
    // Guard against multiple initialization calls.
    // Return true: the system is already correctly initialized; this is not an error.
    if (initialized) {
        return true;
    }
    
    // For ESP32, we can use the RMT controller to support dynamic pin configuration
    // This allows users to use any pin as the data pin
    #if IS_ESP32
        // GPIO48 uses template-based static binding for compatibility
        // (RmtController5 does not initialize correctly on this pin)
        if (dataPin == 48) {
            FastLED.addLeds<WS2812B, 48, GRB>(leds, numLeds);
        } else {
            // Create a custom ClocklessController with the dynamic pin
            // WS2812B timing values: T1=300, T2=900, T3=600 (in nanoseconds)
            if (controller == nullptr) {
                controller = new CustomClocklessController(dataPin);
            }
            FastLED.addLeds(controller, leds, numLeds);
        }
    #else
        // For non-ESP32 platforms, fall back to the switch statement approach.
        // Note: FastLED addLeds template requires compile-time constants for pin numbers.
        // This is a known limitation of the FastLED library template system.
        //
        // IMPORTANT: The default case does not register a controller before
        // returning false. If the pin is unsupported, we return false immediately without
        // touching FastLED, leaving the registry in a clean state.
        // Users can add more cases below for additional pin support.
        switch(dataPin) {
            case 2:
                FastLED.addLeds<WS2812B, 2, GRB>(leds, numLeds);
                break;
            case 3:
                FastLED.addLeds<WS2812B, 3, GRB>(leds, numLeds);
                break;
            case 4:
                FastLED.addLeds<WS2812B, 4, GRB>(leds, numLeds);
                break;
            case 5:
                FastLED.addLeds<WS2812B, 5, GRB>(leds, numLeds);
                break;
            case 11:
                FastLED.addLeds<WS2812B, 11, GRB>(leds, numLeds);
                break;
            case 12:
                FastLED.addLeds<WS2812B, 12, GRB>(leds, numLeds);
                break;
            case 13:
                FastLED.addLeds<WS2812B, 13, GRB>(leds, numLeds);
                break;
            default:
                // [Fix 2] Unsupported pin: return false immediately.
                // No FastLED controller is registered, leaving the registry clean.
                // initialized remains false so update() will safely no-op.
                return false;
        }
    #endif
    
    FastLED.setBrightness(brightnessLevels[currentBrightnessLevel]);
    FastLED.setMaxPowerInVoltsAndMilliamps(maxVolts, maxMilliamps);
    
    initialized = true;
    return true;
}

// Main update loop
// Returns immediately if begin() has not been successfully called.
// This prevents undefined behavior (e.g., calling FastLED.show() on
// uninitialized state) if update() is called before begin() or after
// begin() returned false.
void AvantLumi::update() {
    if (!initialized) {
        return;
    }

    static unsigned long lastPaletteBlend = 0;
    static unsigned long lastRandomPalette = 0;
    
    updateBrightness();
    
    // Blend palettes
    unsigned long blendInterval;
    uint8_t maxBlendChanges;
    getBlendParameters(blendSpeed, blendInterval, maxBlendChanges);

    if (millis() - lastPaletteBlend >= blendInterval) {
        lastPaletteBlend = millis();
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxBlendChanges);
    }
    
    // Generate random palette
    if (millis() - lastRandomPalette >= 5000) {
        lastRandomPalette = millis();
        if (useRandomPalette && !useSolidColor) {
            generateRandomPalette();
        }
    }
    
    updateLEDs();
    FastLED.show();
}

// Setter methods
bool AvantLumi::setRGB(uint8_t rVal, uint8_t gVal, uint8_t bVal) {
    rVal = constrain(rVal, 0, 255);
    gVal = constrain(gVal, 0, 255);
    bVal = constrain(bVal, 0, 255);
    
    solidColor = CRGB(rVal, gVal, bVal);
    targetPalette = createSolidPalette(solidColor);
    useSolidColor = true;
    useRandomPalette = false;
    currentPaletteName = "solid_color";
    solidColorName = "";
    
    return true;
}

bool AvantLumi::setColor(String colorName) {
    colorName.trim();
    colorName.toLowerCase();
    
    if (colorName.length() > 0 && isValidColorName(colorName)) {
        solidColor = parseColorName(colorName);
        solidColorName = colorName;
        targetPalette = createSolidPalette(solidColor);
        useSolidColor = true;
        useRandomPalette = false;
        currentPaletteName = "solid_color";
        return true;
    }
    return false;
}

// Valid range is now 0-5.
// Level 0 maps to brightnessLevels[0] = 0, switching LEDs off via the
// brightness channel. This is independent of setSwitch() — both the
// switch state and brightness level are preserved separately.
bool AvantLumi::setBright(uint8_t level) {
    if (level <= 5) {  // uint8_t is always >= 0, so only upper bound needed
        currentBrightnessLevel = level;
        return true;
    }
    return false;
}

bool AvantLumi::setSwitch(String state) {
    state.toLowerCase();
    state.trim();
    
    if (state == "on") {
        ledEnabled = true;
        return true;
    } else if (state == "off") {
        ledEnabled = false;
        return true;
    }
    return false;
}

bool AvantLumi::setSwitch(bool state) {
    ledEnabled = state;
    return true;
}

bool AvantLumi::setWaver(String state) {
    state.toLowerCase();
    state.trim();
    
    if (state == "on") {
        waverEffect = true;
        return true;
    } else if (state == "off") {
        waverEffect = false;
        return true;
    }
    return false;
}

bool AvantLumi::setWaver(bool state) {
    waverEffect = state;
    return true;
}

bool AvantLumi::setPalette(String paletteName) {
    paletteName.toLowerCase();
    paletteName.trim();
    useSolidColor = false;
    
    // Built-in FastLED palettes
    if (paletteName == "rainbow") {
        targetPalette = RainbowColors_p;
        useRandomPalette = false;
        currentPaletteName = "rainbow";
    }
    else if (paletteName == "party") {
        targetPalette = PartyColors_p;
        useRandomPalette = false;
        currentPaletteName = "party";
    }
    else if (paletteName == "ocean") {
        targetPalette = OceanColors_p;
        useRandomPalette = false;
        currentPaletteName = "ocean";
    }
    else if (paletteName == "forest") {
        targetPalette = ForestColors_p;
        useRandomPalette = false;
        currentPaletteName = "forest";
    }
    else if (paletteName == "heat") {
        targetPalette = HeatColors_p;
        useRandomPalette = false;
        currentPaletteName = "heat";
    }
    else if (paletteName == "cloud") {
        targetPalette = CloudColors_p;
        useRandomPalette = false;
        currentPaletteName = "cloud";
    }
    else if (paletteName == "lava") {
        targetPalette = LavaColors_p;
        useRandomPalette = false;
        currentPaletteName = "lava";
    }
    // User customizable palettes (u01_p through u10_p)
    // These can be customized by modifying the palette definitions in AvantLumi.cpp
    // Accept both standardized names (u01, u02, etc.) and nicknames (christmas, autumn, etc.)
    else if (paletteName == "u01" || paletteName == "christmas") {
        targetPalette = u01_p;  // Default: Christmas theme (red/green)
        useRandomPalette = false;
        currentPaletteName = "u01";  // Always store standardized name
    }
    else if (paletteName == "u02" || paletteName == "autumn") {
        targetPalette = u02_p;  // Default: Autumn theme (oranges/browns)
        useRandomPalette = false;
        currentPaletteName = "u02";  // Always store standardized name
    }
    else if (paletteName == "u03" || paletteName == "cyberpunk") {
        targetPalette = u03_p;  // Default: Cyberpunk theme (magenta/cyan)
        useRandomPalette = false;
        currentPaletteName = "u03";  // Always store standardized name
    }
    else if (paletteName == "u04" || paletteName == "halloween") {
        targetPalette = u04_p;  // Default: Halloween theme (orange/purple)
        useRandomPalette = false;
        currentPaletteName = "u04";  // Always store standardized name
    }
    else if (paletteName == "u05" || paletteName == "winter") {
        targetPalette = u05_p;  // Default: Winter theme (blues/whites)
        useRandomPalette = false;
        currentPaletteName = "u05";  // Always store standardized name
    }
    else if (paletteName == "u06" || paletteName == "spring") {
        targetPalette = u06_p;  // Default: Spring theme (greens/pinks)
        useRandomPalette = false;
        currentPaletteName = "u06";  // Always store standardized name
    }
    else if (paletteName == "u07" || paletteName == "sunset") {
        targetPalette = u07_p;  // Default: Sunset theme (reds/oranges)
        useRandomPalette = false;
        currentPaletteName = "u07";  // Always store standardized name
    }
    else if (paletteName == "u08" || paletteName == "deep_ocean") {
        targetPalette = u08_p;  // Default: Deep Ocean theme (dark blues)
        useRandomPalette = false;
        currentPaletteName = "u08";  // Always store standardized name
    }
    else if (paletteName == "u09" || paletteName == "neon") {
        targetPalette = u09_p;  // Default: Neon theme (bright colors)
        useRandomPalette = false;
        currentPaletteName = "u09";  // Always store standardized name
    }
    else if (paletteName == "u10" || paletteName == "fire") {
        targetPalette = u10_p;  // Default: Fire theme (reds/oranges)
        useRandomPalette = false;
        currentPaletteName = "u10";  // Always store standardized name
    }
    else if (paletteName == "random") {
        useRandomPalette = true;
        currentPaletteName = "random";
    }
    else {
        return false; // Unknown palette
    }
    
    return true;
}

// Getter methods
CRGB AvantLumi::getRGB() {
    return solidColor;
}

String AvantLumi::getColor() {
    return solidColorName;
}

uint8_t AvantLumi::getBright() {
    return currentBrightnessLevel;
}

bool AvantLumi::getSwitch() {
    return ledEnabled;
}

bool AvantLumi::getWaver() {
    return waverEffect;
}

String AvantLumi::getPalette() {
    if (useSolidColor) {
        return "solid_color";
    }
    return currentPaletteName;
}

String AvantLumi::getStatus() {
    String status = "{";
    
    // Switch status - changed from "sw" to "switch"
    status += "\"switch\":\"" + String(ledEnabled ? "on" : "off") + "\",";
    
    // Brightness level - changed from "br" to "bright"
    status += "\"bright\":" + String(currentBrightnessLevel) + ",";
    
    // Waver effect status
    status += "\"waver\":\"" + String(waverEffect ? "on" : "off") + "\",";
    
    // Color/Palette information
    if (useSolidColor) {
        // Changed from "cr" to "rgb" and "cn" to "color"
        status += "\"rgb\":{";
        status += "\"r\":" + String(solidColor.r) + ",";
        status += "\"g\":" + String(solidColor.g) + ",";
        status += "\"b\":" + String(solidColor.b);
        if (solidColorName.length() > 0) {
            status += ",\"color\":\"" + solidColorName + "\"";
        }
        status += "}";
    } else {
        // Changed from "pal" to "palette"
        status += "\"palette\":\"" + currentPaletteName + "\"";
    }

    // Changed from "pwr" to "power"
    status += ",\"power\":{\"v\":" + String(maxVolts) + ",\"ma\":" + String(maxMilliamps) + "}";
    
    // Keep blend_spd as is since it's not a command name, it's a parameter name
    status += ",\"blend_spd\":" + String(blendSpeed);
    
    status += "}";
    
    return status;
}

// Private helper methods
CRGBPalette16 AvantLumi::createSolidPalette(CRGB color) {
    return CRGBPalette16(color, color, color, color, 
                         color, color, color, color,
                         color, color, color, color,
                         color, color, color, color);
}

void AvantLumi::updateBrightness() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastBrightnessUpdate >= 20) {
        lastBrightnessUpdate = currentTime;
        
        uint8_t desiredBrightness = ledEnabled ? brightnessLevels[currentBrightnessLevel] : 0;
        
        if (targetBrightness != desiredBrightness) {
            targetBrightness = desiredBrightness;
        }
        
        if (actualBrightness < targetBrightness) {
            actualBrightness = min((int)actualBrightness + 3, (int)targetBrightness);
        }
        else if (actualBrightness > targetBrightness) {
            actualBrightness = max((int)actualBrightness - 3, (int)targetBrightness);
        }
        
        FastLED.setBrightness(actualBrightness);
    }
}

void AvantLumi::updateLEDs() {
    for (int i = 0; i < numLeds; i++) {
        uint8_t brightness;
        
        if (waverEffect) {
            // Use a local pseudo-random approach to avoid corrupting global RNG state
            uint16_t localSeed = 535 + i * 17;
            uint8_t randomDivisor = (localSeed % 11) + 10; // Range 10-20
            brightness = sin8(millis() / randomDivisor);
        } else {
            brightness = 255;
        }
        
        leds[i] = ColorFromPalette(currentPalette, i * 20, brightness, currentBlending);
    }
}

void AvantLumi::generateRandomPalette() {
    uint8_t baseC = random8(255);
    targetPalette = CRGBPalette16(CHSV(baseC + random8(0, 32), 255, random8(128, 255)), 
                                  CHSV(baseC + random8(0, 32), 255, random8(128, 255)), 
                                  CHSV(baseC + random8(0, 32), 192, random8(128, 255)), 
                                  CHSV(baseC + random8(0, 32), 255, random8(128, 255)));
}

// Unified color lookup table: maps lowercase color name string to CRGB value
// This single table is used by both parseColorName() and isValidColorName()
// to ensure they are always in sync.
struct ColorEntry {
    const char* name;
    CRGB color;
};

static const ColorEntry colorLookupTable[] = {
    // Basic colors
    {"red", CRGB::Red},
    {"green", CRGB::Green},
    {"blue", CRGB::Blue},
    {"white", CRGB::White},
    {"black", CRGB::Black},
    {"yellow", CRGB::Yellow},
    {"cyan", CRGB::Cyan},
    {"magenta", CRGB::Magenta},
    {"orange", CRGB::Orange},
    {"purple", CRGB::Purple},
    {"pink", CRGB::Pink},
    {"brown", CRGB::Brown},
    // Light variations
    {"lightgreen", CRGB::LightGreen},
    {"lightblue", CRGB::LightBlue},
    {"lightpink", CRGB::LightPink},
    {"lightcyan", CRGB::LightCyan},
    {"lightyellow", CRGB::LightYellow},
    {"lightsteelblue", CRGB::LightSteelBlue},
    // Dark variations
    {"darkred", CRGB::DarkRed},
    {"darkgreen", CRGB::DarkGreen},
    {"darkblue", CRGB::DarkBlue},
    {"darkorange", CRGB::DarkOrange},
    {"darkviolet", CRGB::DarkViolet},
    {"darkgray", CRGB::DarkGray},
    {"darkgrey", CRGB::DarkGray},
    {"darkcyan", CRGB::DarkCyan},
    {"darkgoldenrod", CRGB::DarkGoldenrod},
    {"darkslateblue", CRGB::DarkSlateBlue},
    {"darkturquoise", CRGB::DarkTurquoise},
    {"darkseagreen", CRGB::DarkSeaGreen},
    // Medium variations
    {"mediumblue", CRGB::MediumBlue},
    {"mediumorchid", CRGB::MediumOrchid},
    {"mediumspringgreen", CRGB::MediumSpringGreen},
    // Special colors
    {"springgreen", CRGB::SpringGreen},
    {"forestgreen", CRGB::ForestGreen},
    {"seagreen", CRGB::SeaGreen},
    {"limegreen", CRGB::LimeGreen},
    {"lime", CRGB::Lime},
    {"aqua", CRGB::Aqua},
    {"aquamarine", CRGB::Aquamarine},
    {"turquoise", CRGB::Turquoise},
    {"palegreen", CRGB::PaleGreen},
    {"paleturquoise", CRGB::PaleTurquoise},
    {"powderblue", CRGB::PowderBlue},
    {"steelblue", CRGB::SteelBlue},
    {"royalblue", CRGB::RoyalBlue},
    {"cornflowerblue", CRGB::CornflowerBlue},
    {"deepskyblue", CRGB::DeepSkyBlue},
    {"dodgerblue", CRGB::DodgerBlue},
    {"midnightblue", CRGB::MidnightBlue},
    {"navy", CRGB::Navy},
    {"indigo", CRGB::Indigo},
    {"violet", CRGB::Violet},
    {"fuchsia", CRGB::Fuchsia},
    {"hotpink", CRGB::HotPink},
    {"deeppink", CRGB::DeepPink},
    {"crimson", CRGB::Crimson},
    {"firebrick", CRGB::FireBrick},
    {"maroon", CRGB::Maroon},
    {"orangered", CRGB::OrangeRed},
    {"tomato", CRGB::Tomato},
    {"coral", CRGB::Coral},
    {"chocolate", CRGB::Chocolate},
    {"saddlebrown", CRGB::SaddleBrown},
    {"sienna", CRGB::Sienna},
    {"peru", CRGB::Peru},
    {"goldenrod", CRGB::Goldenrod},
    {"gold", CRGB::Gold},
    {"greenyellow", CRGB::GreenYellow},
    {"lemonchiffon", CRGB::LemonChiffon},
    {"honeydew", CRGB::Honeydew},
    {"lavender", CRGB::Lavender},
    {"thistle", CRGB::Thistle},
    {"aliceblue", CRGB::AliceBlue},
    {"gray", CRGB::Gray},
    {"grey", CRGB::Gray},
    {"silver", CRGB::Silver},
    {"teal", CRGB::Teal},
    {"cadetblue", CRGB::CadetBlue},
    {"darkslategray", CRGB::DarkSlateGray},
    {"darkslategrey", CRGB::DarkSlateGray},
    {"indianred", CRGB::IndianRed}
};

static const int colorLookupTableSize = sizeof(colorLookupTable) / sizeof(colorLookupTable[0]);

CRGB AvantLumi::parseColorName(String colorName) {
    // Note: colorName should already be lowercased by the caller (setColor)
    for (int i = 0; i < colorLookupTableSize; i++) {
        if (colorName == colorLookupTable[i].name) {
            return colorLookupTable[i].color;
        }
    }
    // If color name not found, return Red as default
    return CRGB::Red;
}

bool AvantLumi::isValidColorName(String colorName) {
    // Note: colorName should already be lowercased by the caller (setColor)
    for (int i = 0; i < colorLookupTableSize; i++) {
        if (colorName == colorLookupTable[i].name) {
            return true;
        }
    }
    return false;
}

bool AvantLumi::setMaxPower(uint8_t voltsVal, uint32_t milliamps) {
    // Validate voltage (common values: 3, 5, 12, 24V)
    if (voltsVal < 3 || voltsVal > 24) {
        return false;
    }
    
    // Validate current (reasonable range: 50mA to 20A)
    if (milliamps < 50 || milliamps > 20000) {
        return false;
    }
    
    maxVolts = voltsVal;
    maxMilliamps = milliamps;
    
    // Apply the new power settings to FastLED
    FastLED.setMaxPowerInVoltsAndMilliamps(maxVolts, maxMilliamps);
    
    return true;
}

uint8_t AvantLumi::getMaxVolts() {
    return maxVolts;
}

uint32_t AvantLumi::getMaxMilliamps() {
    return maxMilliamps;
}

bool AvantLumi::setBlendSpeed(uint8_t speed_val) {
    if (speed_val >= 1 && speed_val <= 5) {
        blendSpeed = speed_val;
        return true;
    }
    return false;
}

uint8_t AvantLumi::getBlendSpeed() {
    return blendSpeed;
}

void AvantLumi::getBlendParameters(uint8_t speedLevel, unsigned long& interval, uint8_t& maxChanges) {
    switch(speedLevel) {
        case 1:  // Slowest
            interval = 200;   // 200ms
            maxChanges = 25;
            break;
        case 2:  // Slow
            interval = 100;   // 100ms  
            maxChanges = 50;
            break;
        case 3:  // Faster
            interval = 50;    // 50ms
            maxChanges = 75;
            break;
        case 4:  // Fast
            interval = 25;    // 25ms
            maxChanges = 100;
            break;
        case 5:  // Fastest
            interval = 10;    // 10ms
            maxChanges = 150;
            break;
        default:
            interval = 100;   // Default to level 2
            maxChanges = 50;
            break;
    }
}

bool AvantLumi::saveConfig() {
    Preferences prefs;
    if (!prefs.begin("AvantLumi", false)) {
        return false;
    }

    prefs.putBool("ledEnabled", this->ledEnabled);
    prefs.putUChar("brightness", this->currentBrightnessLevel);
    prefs.putBool("waver", this->waverEffect);
    prefs.putBool("useSolid", this->useSolidColor);
    prefs.putUChar("solidR", this->solidColor.r);
    prefs.putUChar("solidG", this->solidColor.g);
    prefs.putUChar("solidB", this->solidColor.b);
    prefs.putString("palette", this->currentPaletteName);
    prefs.putString("colorName", this->solidColorName);
    prefs.putBool("randomPal", this->useRandomPalette);
    prefs.putUChar("blendSpeed", this->blendSpeed);
    prefs.putUChar("maxVolts", this->maxVolts);
    prefs.putUInt("maxMilliamps", this->maxMilliamps);

    prefs.end();
    return true;
}

bool AvantLumi::loadConfig() {
    Preferences prefs;
    if (!prefs.begin("AvantLumi", true)) {
        return false;
    }

    if (!prefs.isKey("ledEnabled")) {
        prefs.end();
        return false;
    }

    this->ledEnabled = prefs.getBool("ledEnabled", false);
    this->currentBrightnessLevel = prefs.getUChar("brightness", 3);
    this->waverEffect = prefs.getBool("waver", true);
    this->useSolidColor = prefs.getBool("useSolid", false);
    this->solidColor.r = prefs.getUChar("solidR", 0);
    this->solidColor.g = prefs.getUChar("solidG", 0);
    this->solidColor.b = prefs.getUChar("solidB", 0);
    this->currentPaletteName = prefs.getString("palette", "u01");
    this->solidColorName = prefs.getString("colorName", "black");
    this->useRandomPalette = prefs.getBool("randomPal", false);
    this->blendSpeed = prefs.getUChar("blendSpeed", 4);
    this->maxVolts = prefs.getUChar("maxVolts", 5);
    this->maxMilliamps = prefs.getUInt("maxMilliamps", 500);

    prefs.end();

    // Restore palette state
    if (this->useSolidColor) {
        this->targetPalette = createSolidPalette(this->solidColor);
    } else {
        setPalette(this->currentPaletteName);
    }

    // Restore brightness levels
    this->targetBrightness = brightnessLevels[this->currentBrightnessLevel];
    this->actualBrightness = brightnessLevels[this->currentBrightnessLevel];

    return true;
}

bool AvantLumi::checkConfig() {
    Preferences prefs;
    if (!prefs.begin("AvantLumi", true)) {
        return false;
    }
    bool hasConfig = prefs.isKey("ledEnabled");
    prefs.end();
    return hasConfig;
}
