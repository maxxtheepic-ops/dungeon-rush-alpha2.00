#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "../Core/Config.h"

// =============================================================================
// INPUT SYSTEM - Button Handling
// =============================================================================

// Button identifiers
enum class Button {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A,
    B,
    X,
    Y,
    OPTIONS,
    NONE
};

class Input {
private:
    // Button states (current frame)
    bool buttonStates[BUTTON_COUNT];
    
    // Button states (previous frame) - for edge detection
    bool prevButtonStates[BUTTON_COUNT];
    
    // Debounce timing
    unsigned long lastDebounceTime;
    
    // Helper to convert Button enum to array index
    int getIndex(Button btn) const;
    
    // Helper to get pin for button
    int getPin(Button btn) const;

public:
    Input();
    
    // Initialize button pins
    void init();
    
    // Call once per frame to update button states
    void update();
    
    // Returns true on the frame the button was pressed (edge detection)
    bool wasPressed(Button btn) const;
    
    // Returns true while button is held down
    bool isHeld(Button btn) const;
    
    // Returns true on the frame the button was released
    bool wasReleased(Button btn) const;
    
    // Debug: print current button states
    void debugPrint() const;
};

#endif // INPUT_H