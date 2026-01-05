#include "VictoryState.h"

// =============================================================================
// VICTORY STATE - Implementation
// =============================================================================

VictoryState::VictoryState() {
}

void VictoryState::enter() {
    DEBUG_PRINTLN("Entering Victory State");
    drawScreen();
}

void VictoryState::update() {
    Input* input = stateManager->getInput();
    
    if (input->wasPressed(Button::A) || input->wasPressed(Button::B)) {
        // Return to title
        stateManager->requestTransition(GameStateID::TITLE);
    }
}

void VictoryState::exit() {
    DEBUG_PRINTLN("Exiting Victory State");
    stateManager->endRun(true);
}

void VictoryState::drawScreen() {
    Display* display = stateManager->getDisplay();
    
    display->clear(COLOR_BG);
    
    // Title
    display->drawTextCentered("VICTORY!", 40, COLOR_GREEN, 3);
    display->drawTextCentered("Dungeon Complete!", 80, COLOR_YELLOW, 2);
    
    // Divider
    display->fillRect(20, 115, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    // Run statistics
    display->drawTextCentered("- Final Stats -", 130, COLOR_CYAN, 1);
    
    int floor = stateManager->getCurrentFloor();
    int slain = stateManager->getSlayCount();
    int befriended = stateManager->getBefriendCount();
    int total = slain + befriended;
    
    String floorText = "Floors Cleared: " + String(floor);
    String totalText = "Total Battles: " + String(total);
    String slainText = "Enemies Slain: " + String(slain);
    String befriendText = "Befriended: " + String(befriended);
    
    display->drawTextCentered(floorText, 155, COLOR_WHITE, 1);
    display->drawTextCentered(totalText, 175, COLOR_WHITE, 1);
    display->drawTextCentered(slainText, 200, COLOR_RED, 1);
    display->drawTextCentered(befriendText, 220, COLOR_YELLOW, 1);
    
    // Befriend percentage
    if (total > 0) {
        int befriendPercent = (befriended * 100) / total;
        String percentText = "Befriend Rate: " + String(befriendPercent) + "%";
        display->drawTextCentered(percentText, 245, COLOR_MAGENTA, 1);
    }
    
    // Divider
    display->fillRect(20, 270, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    // Continue prompt
    display->drawTextCentered("Press A to return", 285, COLOR_GRAY, 1);
    display->drawTextCentered("to title", 300, COLOR_GRAY, 1);
}