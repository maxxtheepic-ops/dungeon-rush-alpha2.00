#include "TitleState.h"

// =============================================================================
// TITLE STATE - Implementation
// =============================================================================

TitleState::TitleState() : selectedOption(0) {
}

void TitleState::enter() {
    DEBUG_PRINTLN("Entering Title State");
    selectedOption = 0;
    drawScreen();
}

void TitleState::update() {
    Input* input = stateManager->getInput();
    
    // Navigation
    if (input->wasPressed(Button::UP)) {
        clearCursor();
        selectedOption--;
        if (selectedOption < 0) selectedOption = NUM_OPTIONS - 1;
        drawCursor();
    }
    
    if (input->wasPressed(Button::DOWN)) {
        clearCursor();
        selectedOption++;
        if (selectedOption >= NUM_OPTIONS) selectedOption = 0;
        drawCursor();
    }
    
    // Selection
    if (input->wasPressed(Button::A)) {
        switch (selectedOption) {
            case 0:  // New Game
                DEBUG_PRINTLN("Starting new game...");
                stateManager->startNewRun();
                stateManager->requestTransition(GameStateID::COMBAT);
                break;
            case 1:  // (placeholder for future options)
                DEBUG_PRINTLN("Option 2 selected (placeholder)");
                break;
        }
    }
}

void TitleState::exit() {
    DEBUG_PRINTLN("Exiting Title State");
}

void TitleState::drawScreen() {
    Display* display = stateManager->getDisplay();
    
    display->clear(COLOR_BG);
    
    // Title
    display->drawTextCentered("DUNGEON", 40, COLOR_YELLOW, 3);
    display->drawTextCentered("RUSH", 75, COLOR_YELLOW, 3);
    display->drawTextCentered("ALPHA", 115, COLOR_WHITE, 2);
    
    // Version
    display->drawTextCentered("v2.0", 145, COLOR_GRAY, 1);
    
    // Divider
    display->fillRect(20, 170, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    // Menu options
    display->drawText("New Game", 50, 190, COLOR_WHITE, 2);
    display->drawText("Options", 50, 220, COLOR_DARK_GRAY, 2);  // Grayed out for now
    
    // Instructions
    display->drawTextCentered("Press A to select", 280, COLOR_GRAY, 1);
    
    // Draw initial cursor
    drawCursor();
}

void TitleState::drawCursor() {
    Display* display = stateManager->getDisplay();
    int y = 190 + (selectedOption * 30);
    display->drawText(">", 30, y, COLOR_CURSOR, 2);
}

void TitleState::clearCursor() {
    Display* display = stateManager->getDisplay();
    int y = 190 + (selectedOption * 30);
    display->fillRect(30, y, 16, 16, COLOR_BG);
}