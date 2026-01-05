#include "FloorTransitionState.h"
#include "StateManager.h"

// =============================================================================
// FLOOR TRANSITION STATE - Implementation
// =============================================================================

FloorTransitionState::FloorTransitionState() {
    fromFloor = 1;
    toFloor = 2;
    startTime = 0;
    canAdvance = false;
}

void FloorTransitionState::enter() {
    DEBUG_PRINTLN("Entering Floor Transition State");
    
    // Get floor info from state manager
    // We just completed fromFloor, advancing to toFloor
    toFloor = stateManager->getCurrentFloor();
    fromFloor = toFloor - 1;
    if (fromFloor < 1) fromFloor = 1;
    
    startTime = millis();
    canAdvance = false;
    
    drawScreen();
}

void FloorTransitionState::update() {
    Input* input = stateManager->getInput();
    
    unsigned long elapsed = millis() - startTime;
    
    // Check if minimum time has passed
    if (!canAdvance && elapsed >= MIN_DISPLAY_MS) {
        canAdvance = true;
        drawAdvancePrompt();
    }
    
    // Simple animation update (placeholder - just flashing)
    static unsigned long lastAnimUpdate = 0;
    if (elapsed - lastAnimUpdate > 200) {
        drawAnimation();
        lastAnimUpdate = elapsed;
    }
    
    // Check for advance
    if (canAdvance && input->wasPressed(Button::A)) {
        // Continue to next floor's first combat
        stateManager->requestTransition(GameStateID::COMBAT);
    }
}

void FloorTransitionState::exit() {
    DEBUG_PRINTLN("Exiting Floor Transition State");
}

void FloorTransitionState::drawScreen() {
    Display* display = stateManager->getDisplay();
    
    display->clear(COLOR_BG);
    
    // Title
    display->drawTextCentered("FLOOR CLEAR!", 30, COLOR_GREEN, 2);
    
    // Floor numbers
    String fromText = "Floor " + String(fromFloor);
    String toText = "Floor " + String(toFloor);
    
    display->drawTextCentered(fromText, 80, COLOR_WHITE, 2);
    
    // Arrow area (will be animated)
    // Leave space at y=110-160 for animation
    
    display->drawTextCentered(toText, 180, COLOR_YELLOW, 2);
    
    // Stats summary
    display->fillRect(20, 220, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    int slain = stateManager->getSlayCount();
    int befriended = stateManager->getBefriendCount();
    
    String slainText = "Slain: " + String(slain);
    String befriendText = "Befriended: " + String(befriended);
    
    display->drawTextCentered(slainText, 235, COLOR_RED, 1);
    display->drawTextCentered(befriendText, 255, COLOR_YELLOW, 1);
    
    // Draw initial animation frame
    drawAnimation();
}

void FloorTransitionState::drawAnimation() {
    Display* display = stateManager->getDisplay();
    
    // Clear animation area
    display->fillRect(0, 105, SCREEN_WIDTH, 70, COLOR_BG);
    
    // Placeholder animation: Animated arrow pointing down
    static int animFrame = 0;
    animFrame = (animFrame + 1) % 4;
    
    int centerX = SCREEN_WIDTH / 2;
    int baseY = 115 + (animFrame * 5);  // Bouncing effect
    
    // Draw downward arrow
    uint16_t arrowColor = (animFrame % 2 == 0) ? COLOR_YELLOW : COLOR_WHITE;
    
    // Arrow shaft
    display->fillRect(centerX - 5, baseY, 10, 30, arrowColor);
    
    // Arrow head (triangle approximation)
    display->fillRect(centerX - 15, baseY + 30, 30, 8, arrowColor);
    display->fillRect(centerX - 10, baseY + 38, 20, 8, arrowColor);
    display->fillRect(centerX - 5, baseY + 46, 10, 8, arrowColor);
    
    // Sparkles around arrow
    if (animFrame == 0) {
        display->fillRect(centerX - 30, baseY + 10, 4, 4, COLOR_CYAN);
        display->fillRect(centerX + 28, baseY + 25, 4, 4, COLOR_MAGENTA);
    } else if (animFrame == 2) {
        display->fillRect(centerX + 25, baseY + 5, 4, 4, COLOR_CYAN);
        display->fillRect(centerX - 28, baseY + 35, 4, 4, COLOR_MAGENTA);
    }
}

void FloorTransitionState::drawAdvancePrompt() {
    Display* display = stateManager->getDisplay();
    display->drawTextCentered("Press A to continue", 290, COLOR_GRAY, 1);
}