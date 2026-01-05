#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Core/Config.h"

// =============================================================================
// DISPLAY SYSTEM - TFT_eSPI Wrapper
// =============================================================================

class Display {
private:
    TFT_eSPI tft;
    
    // Track if display needs full redraw
    bool needsFullRedraw;

public:
    Display();
    
    // === LIFECYCLE ===
    void init();
    
    // === SCREEN CONTROL ===
    void clear(uint16_t color = COLOR_BG);
    void setBacklight(bool on);
    
    // === TEXT DRAWING ===
    void drawText(const String& text, int x, int y, uint16_t color = COLOR_TEXT, uint8_t size = 2);
    void drawTextCentered(const String& text, int y, uint16_t color = COLOR_TEXT, uint8_t size = 2);
    void drawTextRight(const String& text, int y, uint16_t color = COLOR_TEXT, uint8_t size = 2);
    
    // === SHAPE DRAWING ===
    void fillRect(int x, int y, int w, int h, uint16_t color);
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void fillScreen(uint16_t color);
    
    // === BAR DRAWING (for HP/Kindness) ===
    void drawBar(int x, int y, int w, int h, int current, int max, 
                 uint16_t fillColor, uint16_t bgColor = COLOR_DARK_GRAY);
    
    // === PARTIAL UPDATE HELPERS ===
    void clearArea(int x, int y, int w, int h, uint16_t color = COLOR_BG);
    
    // === RAW ACCESS (for advanced usage) ===
    TFT_eSPI* getTFT() { return &tft; }
    
    // === DIMENSIONS ===
    int width() const { return SCREEN_WIDTH; }
    int height() const { return SCREEN_HEIGHT; }
};

#endif // DISPLAY_H