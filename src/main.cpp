// =============================================================================
// DUNGEON RUSH ALPHA - Main Entry Point
// =============================================================================
// Step 7: Game State Manager - Full state machine implementation
// =============================================================================

#include <Arduino.h>
#include "Core/Config.h"
#include "Input/Input.h"
#include "UI/Display.h"
#include "States/StateManager.h"

// Core systems
Display display;
Input input;

// State manager
StateManager* stateManager;

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println();
    Serial.println("========================================");
    Serial.println("   DUNGEON RUSH ALPHA");
    Serial.println("   Version: 2.0 (Restructure)");
    Serial.println("========================================");
    Serial.println();
    
    // Initialize core systems
    Serial.println("Initializing Display...");
    display.init();
    
    Serial.println("Initializing Input...");
    input.init();
    
    // Seed random
    randomSeed(analogRead(0));
    
    // Create and initialize state manager
    Serial.println("Initializing State Manager...");
    stateManager = new StateManager(&display, &input);
    stateManager->init();
    
    Serial.println();
    Serial.println("=== SETUP COMPLETE ===");
    Serial.println();
}

void loop() {
    // Update input
    input.update();
    
    // Update state manager (handles all game logic)
    stateManager->update();
    
    delay(10);
}