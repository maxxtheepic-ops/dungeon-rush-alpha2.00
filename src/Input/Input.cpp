#include "Input.h"

// =============================================================================
// INPUT SYSTEM - Implementation
// =============================================================================

Input::Input() {
    // Initialize all states to false
    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttonStates[i] = false;
        prevButtonStates[i] = false;
    }
    lastDebounceTime = 0;
}

void Input::init() {
    // Configure button pins with internal pull-up resistors
    // Buttons connect to GND when pressed
    pinMode(PIN_BTN_UP, INPUT_PULLUP);
    pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
    pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
    pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_BTN_A, INPUT_PULLUP);
    pinMode(PIN_BTN_B, INPUT_PULLUP);
    pinMode(PIN_BTN_X, INPUT_PULLUP);
    pinMode(PIN_BTN_Y, INPUT_PULLUP);
    pinMode(PIN_BTN_OPTIONS, INPUT_PULLUP);
    
    DEBUG_PRINTLN("Input: Initialized");
    DEBUG_PRINT("  UP=GPIO");
    DEBUG_PRINTLN(PIN_BTN_UP);
    DEBUG_PRINT("  DOWN=GPIO");
    DEBUG_PRINTLN(PIN_BTN_DOWN);
    DEBUG_PRINT("  LEFT=GPIO");
    DEBUG_PRINTLN(PIN_BTN_LEFT);
    DEBUG_PRINT("  RIGHT=GPIO");
    DEBUG_PRINTLN(PIN_BTN_RIGHT);
    DEBUG_PRINT("  A=GPIO");
    DEBUG_PRINTLN(PIN_BTN_A);
    DEBUG_PRINT("  B=GPIO");
    DEBUG_PRINTLN(PIN_BTN_B);
    DEBUG_PRINT("  X=GPIO");
    DEBUG_PRINTLN(PIN_BTN_X);
    DEBUG_PRINT("  Y=GPIO");
    DEBUG_PRINTLN(PIN_BTN_Y);
    DEBUG_PRINT("  OPTIONS=GPIO");
    DEBUG_PRINTLN(PIN_BTN_OPTIONS);
}

void Input::update() {
    // Store previous states for edge detection
    for (int i = 0; i < BUTTON_COUNT; i++) {
        prevButtonStates[i] = buttonStates[i];
    }
    
    // Read current states
    // Buttons are active LOW (pulled up, connect to GND when pressed)
    // So we invert: pressed = true when digitalRead returns LOW
    buttonStates[0] = !digitalRead(PIN_BTN_UP);
    buttonStates[1] = !digitalRead(PIN_BTN_DOWN);
    buttonStates[2] = !digitalRead(PIN_BTN_LEFT);
    buttonStates[3] = !digitalRead(PIN_BTN_RIGHT);
    buttonStates[4] = !digitalRead(PIN_BTN_A);
    buttonStates[5] = !digitalRead(PIN_BTN_B);
    buttonStates[6] = !digitalRead(PIN_BTN_X);
    buttonStates[7] = !digitalRead(PIN_BTN_Y);
    buttonStates[8] = !digitalRead(PIN_BTN_OPTIONS);
}

bool Input::wasPressed(Button btn) const {
    // Debounce check
    if (millis() - lastDebounceTime < INPUT_DEBOUNCE_MS) {
        return false;
    }
    
    int idx = getIndex(btn);
    if (idx < 0) return false;
    
    // Edge detection: currently pressed AND wasn't pressed last frame
    bool pressed = buttonStates[idx] && !prevButtonStates[idx];
    
    if (pressed) {
        // Update debounce timer (cast away const for this - or make mutable)
        const_cast<Input*>(this)->lastDebounceTime = millis();
    }
    
    return pressed;
}

bool Input::isHeld(Button btn) const {
    int idx = getIndex(btn);
    if (idx < 0) return false;
    
    return buttonStates[idx];
}

bool Input::wasReleased(Button btn) const {
    int idx = getIndex(btn);
    if (idx < 0) return false;
    
    // Edge detection: not pressed now AND was pressed last frame
    return !buttonStates[idx] && prevButtonStates[idx];
}

int Input::getIndex(Button btn) const {
    switch (btn) {
        case Button::UP:      return 0;
        case Button::DOWN:    return 1;
        case Button::LEFT:    return 2;
        case Button::RIGHT:   return 3;
        case Button::A:       return 4;
        case Button::B:       return 5;
        case Button::X:       return 6;
        case Button::Y:       return 7;
        case Button::OPTIONS: return 8;
        default:              return -1;
    }
}

int Input::getPin(Button btn) const {
    switch (btn) {
        case Button::UP:      return PIN_BTN_UP;
        case Button::DOWN:    return PIN_BTN_DOWN;
        case Button::LEFT:    return PIN_BTN_LEFT;
        case Button::RIGHT:   return PIN_BTN_RIGHT;
        case Button::A:       return PIN_BTN_A;
        case Button::B:       return PIN_BTN_B;
        case Button::X:       return PIN_BTN_X;
        case Button::Y:       return PIN_BTN_Y;
        case Button::OPTIONS: return PIN_BTN_OPTIONS;
        default:              return -1;
    }
}

void Input::debugPrint() const {
    Serial.print("Buttons: ");
    Serial.print(buttonStates[0] ? "[UP]" : "[ ]");
    Serial.print(buttonStates[1] ? "[DN]" : "[ ]");
    Serial.print(buttonStates[2] ? "[LT]" : "[ ]");
    Serial.print(buttonStates[3] ? "[RT]" : "[ ]");
    Serial.print(buttonStates[4] ? "[A]" : "[ ]");
    Serial.print(buttonStates[5] ? "[B]" : "[ ]");
    Serial.print(buttonStates[6] ? "[X]" : "[ ]");
    Serial.print(buttonStates[7] ? "[Y]" : "[ ]");
    Serial.print(buttonStates[8] ? "[OPT]" : "[ ]");
    Serial.println();
}