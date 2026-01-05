#include "GameOverState.h"
#include "StateManager.h"

// =============================================================================
// GAME OVER STATE - Implementation
// =============================================================================

GameOverState::GameOverState() {
}

void GameOverState::enter() {
    DEBUG_PRINTLN("Entering Game Over State");
    drawScreen();
}

void GameOverState::update() {
    Input* input = stateManager->getInput();
    
    if (input->wasPressed(Button::A) || input->wasPressed(Button::B)) {
        // Return to title
        stateManager->requestTransition(GameStateID::TITLE);
    }
}

void GameOverState::exit() {
    DEBUG_PRINTLN("Exiting Game Over State");
    stateManager->endRun(false);
}

void GameOverState::drawScreen() {
    Display* display = stateManager->getDisplay();
    
    display->clear(COLOR_BG);
    
    // Title
    display->drawTextCentered("GAME OVER", 50, COLOR_RED, 3);
    
    // Skull icon (simple)
    int skullX = (SCREEN_WIDTH - 40) / 2;
    int skullY = 100;
    display->fillRect(skullX, skullY, 40, 30, COLOR_WHITE);
    display->fillRect(skullX + 8, skullY + 8, 8, 8, COLOR_BG);   // Left eye
    display->fillRect(skullX + 24, skullY + 8, 8, 8, COLOR_BG);  // Right eye
    display->fillRect(skullX + 16, skullY + 20, 8, 10, COLOR_BG); // Nose
    
    // Divider
    display->fillRect(20, 150, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    // Run statistics
    display->drawTextCentered("- Run Stats -", 165, COLOR_YELLOW, 1);
    
    int floor = stateManager->getCurrentFloor();
    int slain = stateManager->getSlayCount();
    int befriended = stateManager->getBefriendCount();
    
    String floorText = "Floor: " + String(floor);
    String slainText = "Enemies Slain: " + String(slain);
    String befriendText = "Befriended: " + String(befriended);
    
    display->drawTextCentered(floorText, 190, COLOR_WHITE, 1);
    display->drawTextCentered(slainText, 210, COLOR_RED, 1);
    display->drawTextCentered(befriendText, 230, COLOR_YELLOW, 1);
    
    // Divider
    display->fillRect(20, 255, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    // Continue prompt
    display->drawTextCentered("Press A to return", 280, COLOR_GRAY, 1);
    display->drawTextCentered("to title", 295, COLOR_GRAY, 1);
}