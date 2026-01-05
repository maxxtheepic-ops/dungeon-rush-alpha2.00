#include "Display.h"

// =============================================================================
// DISPLAY SYSTEM - Implementation
// =============================================================================

Display::Display() : tft() {
    needsFullRedraw = true;
}

void Display::init() {
    DEBUG_PRINTLN("Display: Initializing TFT...");
    
    // Initialize TFT_eSPI
    tft.init();
    
    // Set rotation (2 = 180 degrees, USB port at bottom typically)
    tft.setRotation(SCREEN_ROTATION);
    
    // Turn on backlight (INVERTED: LOW = ON)
    pinMode(TFT_BL, OUTPUT);
    setBacklight(true);
    
    // Clear screen to black
    clear(COLOR_BG);
    
    // Set default text properties
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.setTextSize(2);
    tft.setTextWrap(false);
    
    DEBUG_PRINT("Display: Ready (");
    DEBUG_PRINT(SCREEN_WIDTH);
    DEBUG_PRINT("x");
    DEBUG_PRINT(SCREEN_HEIGHT);
    DEBUG_PRINTLN(")");
}

// === SCREEN CONTROL ===

void Display::clear(uint16_t color) {
    tft.fillScreen(color);
    needsFullRedraw = false;
}

void Display::setBacklight(bool on) {
    // Backlight is INVERTED: LOW = ON, HIGH = OFF
    digitalWrite(TFT_BL, on ? LOW : HIGH);
}

// === TEXT DRAWING ===

void Display::drawText(const String& text, int x, int y, uint16_t color, uint8_t size) {
    tft.setTextColor(color, COLOR_BG);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(text);
}

void Display::drawTextCentered(const String& text, int y, uint16_t color, uint8_t size) {
    tft.setTextSize(size);
    
    // Calculate text width (approximate: 6 pixels per char at size 1)
    int charWidth = 6 * size;
    int textWidth = text.length() * charWidth;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    
    drawText(text, x, y, color, size);
}

void Display::drawTextRight(const String& text, int y, uint16_t color, uint8_t size) {
    tft.setTextSize(size);
    
    // Calculate text width
    int charWidth = 6 * size;
    int textWidth = text.length() * charWidth;
    int x = SCREEN_WIDTH - textWidth - 4; // 4px margin
    
    drawText(text, x, y, color, size);
}

// === SHAPE DRAWING ===

void Display::fillRect(int x, int y, int w, int h, uint16_t color) {
    tft.fillRect(x, y, w, h, color);
}

void Display::drawRect(int x, int y, int w, int h, uint16_t color) {
    tft.drawRect(x, y, w, h, color);
}

void Display::fillScreen(uint16_t color) {
    tft.fillScreen(color);
}

// === BAR DRAWING ===

void Display::drawBar(int x, int y, int w, int h, int current, int max, 
                      uint16_t fillColor, uint16_t bgColor) {
    // Draw background (empty portion)
    tft.fillRect(x, y, w, h, bgColor);
    
    // Calculate fill width
    int fillWidth = 0;
    if (max > 0 && current > 0) {
        fillWidth = (current * w) / max;
        if (fillWidth > w) fillWidth = w;
        if (fillWidth < 0) fillWidth = 0;
    }
    
    // Draw filled portion
    if (fillWidth > 0) {
        tft.fillRect(x, y, fillWidth, h, fillColor);
    }
    
    // Draw border
    tft.drawRect(x, y, w, h, COLOR_WHITE);
}

// === PARTIAL UPDATE HELPERS ===

void Display::clearArea(int x, int y, int w, int h, uint16_t color) {
    tft.fillRect(x, y, w, h, color);
}