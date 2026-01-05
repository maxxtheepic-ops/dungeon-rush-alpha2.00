#include "TreasureState.h"
#include "StateManager.h"

// =============================================================================
// TREASURE STATE - Implementation
// =============================================================================

TreasureState::TreasureState() {
    rewardType = RewardType::NOTHING;
    newElement = ELEM_COUNT;
    newType = TYPE_COUNT;
    rewardGiven = false;
}

void TreasureState::enter() {
    DEBUG_PRINTLN("Entering Treasure State");
    
    rewardGiven = false;
    
    // Determine and give reward
    giveReward();
    
    // Draw the screen
    drawScreen();
}

void TreasureState::update() {
    Input* input = stateManager->getInput();
    
    if (input->wasPressed(Button::A) || input->wasPressed(Button::B)) {
        // Check if we just beat a boss
        if (stateManager->isCurrentBattleBoss()) {
            // Boss defeated! Advance to next floor
            stateManager->advanceFloor();
            stateManager->requestTransition(GameStateID::FLOOR_TRANSITION);
        } else {
            // Normal treasure, continue to next combat
            stateManager->requestTransition(GameStateID::COMBAT);
        }
    }
}

void TreasureState::exit() {
    DEBUG_PRINTLN("Exiting Treasure State");
}

void TreasureState::giveReward() {
    Player* player = stateManager->getPlayer();
    
    // Randomly decide element or type (50/50)
    bool tryElement = random(0, 2) == 0;
    
    if (tryElement) {
        // Try to unlock a new element
        newElement = player->unlockRandomElement();
        if (newElement != ELEM_COUNT) {
            rewardType = RewardType::NEW_ELEMENT;
            rewardGiven = true;
            DEBUG_PRINT("Reward: New element - ");
            DEBUG_PRINTLN(getElementName(newElement));
            return;
        }
    }
    
    // Try to unlock a new type
    newType = player->unlockRandomType();
    if (newType != TYPE_COUNT) {
        rewardType = RewardType::NEW_TYPE;
        rewardGiven = true;
        DEBUG_PRINT("Reward: New type - ");
        DEBUG_PRINTLN(getTypeName(newType));
        return;
    }
    
    // If type failed, try element (if we didn't try it first)
    if (!tryElement) {
        newElement = player->unlockRandomElement();
        if (newElement != ELEM_COUNT) {
            rewardType = RewardType::NEW_ELEMENT;
            rewardGiven = true;
            DEBUG_PRINT("Reward: New element - ");
            DEBUG_PRINTLN(getElementName(newElement));
            return;
        }
    }
    
    // Player has everything!
    rewardType = RewardType::NOTHING;
    DEBUG_PRINTLN("Reward: Nothing (player has all abilities)");
}

void TreasureState::drawScreen() {
    Display* display = stateManager->getDisplay();
    Player* player = stateManager->getPlayer();
    
    display->clear(COLOR_BG);
    
    // Title - different for boss vs normal
    if (stateManager->isCurrentBattleBoss()) {
        display->drawTextCentered("BOSS DEFEATED!", 30, COLOR_GREEN, 2);
    } else {
        display->drawTextCentered("TREASURE!", 30, COLOR_YELLOW, 2);
    }
    
    // Chest icon (simple box)
    int chestX = (SCREEN_WIDTH - 60) / 2;
    int chestY = 70;
    display->fillRect(chestX, chestY, 60, 40, COLOR_ORANGE);
    display->drawRect(chestX, chestY, 60, 40, COLOR_WHITE);
    display->fillRect(chestX + 25, chestY + 15, 10, 10, COLOR_YELLOW);  // Lock
    
    // Divider
    display->fillRect(20, 130, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    // Reward text
    switch (rewardType) {
        case RewardType::NEW_ELEMENT:
            display->drawTextCentered("New Element!", 150, COLOR_GREEN, 2);
            display->drawTextCentered(getElementName(newElement), 180, getElementColor(newElement), 2);
            break;
            
        case RewardType::NEW_TYPE:
            display->drawTextCentered("New Type!", 150, COLOR_GREEN, 2);
            display->drawTextCentered(getTypeName(newType), 180, COLOR_CYAN, 2);
            break;
            
        case RewardType::NOTHING:
            display->drawTextCentered("You have", 150, COLOR_WHITE, 1);
            display->drawTextCentered("mastered all!", 170, COLOR_YELLOW, 2);
            break;
    }
    
    // Current inventory summary
    display->fillRect(20, 210, SCREEN_WIDTH - 40, 2, COLOR_GRAY);
    
    String elemCount = "Elements: " + String(player->getElementCount()) + "/" + String(ELEM_COUNT);
    String typeCount = "Types: " + String(player->getTypeCount()) + "/" + String(TYPE_COUNT);
    
    display->drawTextCentered(elemCount, 225, COLOR_WHITE, 1);
    display->drawTextCentered(typeCount, 245, COLOR_WHITE, 1);
    
    // Continue prompt
    display->drawTextCentered("Press A to continue", 290, COLOR_GRAY, 1);
}