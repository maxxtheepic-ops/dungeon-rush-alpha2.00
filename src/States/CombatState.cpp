#include "CombatState.h"
#include "StateManager.h"
#include "../Combat/SpellMatrix.h"

// =============================================================================
// COMBAT STATE - Implementation
// =============================================================================

CombatState::CombatState() {
    combatHUD = nullptr;
    spellSelector = nullptr;
    combatAnimation = nullptr;
    phase = CombatPhase::PLAYER_TURN;
    
    pendingElement = ELEM_NONE;
    pendingType = TYPE_NONE;
    pendingDamage = 0;
    pendingKindness = 0;
    pendingEnemyAction = EnemyAction::ATTACK;
    pendingEnemyDamage = 0;
}

CombatState::~CombatState() {
    if (combatHUD) delete combatHUD;
    if (spellSelector) delete spellSelector;
    if (combatAnimation) delete combatAnimation;
}

void CombatState::enter() {
    DEBUG_PRINTLN("Entering Combat State");
    
    // Create UI components if needed
    if (!combatHUD) {
        combatHUD = new CombatHUD(stateManager->getDisplay());
    }
    if (!spellSelector) {
        spellSelector = new SpellSelector(stateManager->getDisplay());
    }
    if (!combatAnimation) {
        combatAnimation = new CombatAnimation(stateManager->getDisplay());
    }
    
    // Reset combat phase
    phase = CombatPhase::PLAYER_TURN;
    combatAnimation->reset();
    
    // Spawn an enemy if there isn't one
    if (!stateManager->getEnemy()) {
        int floor = stateManager->getCurrentFloor();
        
        // Check if it's boss time (after 4 normal enemies)
        if (stateManager->isBossTime()) {
            // BOSS BATTLE!
            stateManager->setIsBossBattle(true);
            
            // Boss stats scale with floor
            int bossHP = 100 + (floor * 30);
            int bossKindness = 60 + (floor * 20);
            int bossAtk = 12 + (floor * 3);
            int bossDef = 5 + floor;
            
            stateManager->spawnEnemy("BOSS Slime King", bossHP, bossKindness, bossAtk, bossDef, EnemyAI::AGGRESSIVE);
            
            Enemy* enemy = stateManager->getEnemy();
            enemy->setElementMultiplier(ELEM_FIRE, 150);
            enemy->setElementMultiplier(ELEM_WATER, 50);
            enemy->setElementMultiplier(ELEM_LIGHTNING, 120);
            
            DEBUG_PRINTLN("*** BOSS BATTLE ***");
        } else {
            // Normal enemy
            stateManager->setIsBossBattle(false);
            
            // Stats scale slightly with floor
            int enemyHP = 50 + (floor * 10);
            int enemyKindness = 30 + (floor * 10);
            int enemyAtk = 6 + (floor * 2);
            int enemyDef = 2 + floor;
            
            stateManager->spawnEnemy("Slime", enemyHP, enemyKindness, enemyAtk, enemyDef, EnemyAI::BALANCED);
            
            Enemy* enemy = stateManager->getEnemy();
            enemy->setElementMultiplier(ELEM_FIRE, 150);
            enemy->setElementMultiplier(ELEM_WATER, 50);
            enemy->setElementMultiplier(ELEM_BUBBLE, 120);
        }
    }
    
    // Draw the combat screen
    drawCombatScreen();
}

void CombatState::update() {
    Input* input = stateManager->getInput();
    
    switch (phase) {
        case CombatPhase::PLAYER_TURN:
            handlePlayerTurn();
            break;
            
        case CombatPhase::ANIMATING:
            handleAnimation();
            break;
            
        case CombatPhase::VICTORY_SLAIN:
        case CombatPhase::VICTORY_BEFRIEND:
            if (input->wasPressed(Button::A) || input->wasPressed(Button::B)) {
                // Record the outcome
                if (phase == CombatPhase::VICTORY_SLAIN) {
                    stateManager->recordSlay();
                } else {
                    stateManager->recordBefriend();
                }
                
                // Advance enemy counter
                stateManager->advanceEnemy();
                
                // Check if we get treasure (every 3 enemies OR boss)
                if (stateManager->isCurrentBattleBoss() || stateManager->isTreasureTime()) {
                    stateManager->resetTreasureCounter();
                    stateManager->requestTransition(GameStateID::TREASURE);
                } else {
                    // Skip treasure, go straight to next combat
                    stateManager->requestTransition(GameStateID::COMBAT);
                }
            }
            break;
            
        case CombatPhase::DEFEAT:
            if (input->wasPressed(Button::A) || input->wasPressed(Button::B)) {
                stateManager->requestTransition(GameStateID::GAME_OVER);
            }
            break;
    }
}

void CombatState::exit() {
    DEBUG_PRINTLN("Exiting Combat State");
    
    // Clear the enemy
    stateManager->clearEnemy();
}

void CombatState::drawCombatScreen() {
    Display* display = stateManager->getDisplay();
    Player* player = stateManager->getPlayer();
    Enemy* enemy = stateManager->getEnemy();
    
    display->clear(COLOR_BG);
    
    // Draw HUD
    combatHUD->drawFull(player, enemy);
    
    // Initialize spell selector below HUD
    // init() calls reset() which restores remembered indices
    spellSelector->init(player, combatHUD->getBottomY() + 5);
    spellSelector->drawFull();
}

void CombatState::handlePlayerTurn() {
    Input* input = stateManager->getInput();
    
    // Navigation (Left/Right to cycle through elements and types)
    if (input->wasPressed(Button::LEFT)) {
        spellSelector->moveLeft();
        spellSelector->update();
    }
    
    if (input->wasPressed(Button::RIGHT)) {
        spellSelector->moveRight();
        spellSelector->update();
    }
    
    // Confirm
    if (input->wasPressed(Button::A)) {
        if (spellSelector->confirm()) {
            // Spell selected - remember choice for next turn, then animate
            spellSelector->rememberSelection();
            startSpellAnimation();
            return;  // Don't let spellSelector->update() redraw over animation
        } else {
            // Just moved to type selection
            spellSelector->update();
        }
    }
    
    // Cancel / Back
    if (input->wasPressed(Button::B)) {
        if (!spellSelector->cancel()) {
            // Just went back to element selection
            spellSelector->update();
        }
        // Note: B at element selection does nothing (can't flee combat)
    }
    
    // Update blink animation even when no input
    spellSelector->update();
}

void CombatState::startSpellAnimation() {
    Enemy* enemy = stateManager->getEnemy();
    
    // Store spell data for later application
    pendingElement = spellSelector->getSelectedElement();
    pendingType = spellSelector->getSelectedType();
    SpellData spell = spellSelector->getSelectedSpell();
    pendingDamage = spell.damage;
    pendingKindness = spell.kindness;
    
    // Determine enemy action now (before animation)
    pendingEnemyAction = enemy->chooseAction();
    if (pendingEnemyAction == EnemyAction::ATTACK) {
        pendingEnemyDamage = enemy->getAttack();
    } else {
        pendingEnemyDamage = 0;
    }
    
    // Queue enemy action for animation
    String enemyDesc = getEnemyActionDescription(pendingEnemyAction, pendingEnemyDamage);
    combatAnimation->queueEnemyAction(pendingEnemyAction, enemyDesc);
    
    // Start player spell animation
    combatAnimation->showPlayerSpell(pendingElement, pendingType, spell.description);
    
    // Enter animation phase
    phase = CombatPhase::ANIMATING;
    
    DEBUG_PRINT("Starting animation - ");
    DEBUG_PRINT(getElementName(pendingElement));
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(getTypeName(pendingType));
}

void CombatState::handleAnimation() {
    Input* input = stateManager->getInput();
    
    // Update animation - check if complete
    bool aPressed = input->wasPressed(Button::A);
    
    if (combatAnimation->update(aPressed)) {
        // Animation complete - apply all pending actions
        applyPendingActions();
        
        // Check combat end
        checkCombatEnd();
        
        // If combat continues, return to player turn
        if (phase == CombatPhase::ANIMATING) {
            phase = CombatPhase::PLAYER_TURN;
            combatAnimation->reset();
            
            // Redraw combat screen (spell selector will restore remembered indices)
            drawCombatScreen();
        }
    }
}

void CombatState::applyPendingActions() {
    Player* player = stateManager->getPlayer();
    Enemy* enemy = stateManager->getEnemy();
    
    // Apply player spell
    if (pendingDamage > 0) {
        enemy->takeDamage(pendingDamage, pendingElement);
        DEBUG_PRINT("Applied ");
        DEBUG_PRINT(pendingDamage);
        DEBUG_PRINTLN(" damage");
    }
    if (pendingKindness > 0) {
        enemy->addKindness(pendingKindness, pendingElement);
        DEBUG_PRINT("Applied ");
        DEBUG_PRINT(pendingKindness);
        DEBUG_PRINTLN(" kindness");
    }
    
    // Apply enemy action (only if enemy not defeated/befriended)
    if (!enemy->isDefeated() && !enemy->isBefriended()) {
        switch (pendingEnemyAction) {
            case EnemyAction::ATTACK:
                player->takeDamage(pendingEnemyDamage);
                DEBUG_PRINT("Enemy dealt ");
                DEBUG_PRINT(pendingEnemyDamage);
                DEBUG_PRINTLN(" damage");
                break;
                
            case EnemyAction::DEFEND_DAMAGE:
                enemy->setDefendingDamage(true);
                DEBUG_PRINTLN("Enemy defending damage");
                break;
                
            case EnemyAction::DEFEND_KINDNESS:
                enemy->setDefendingKindness(true);
                DEBUG_PRINTLN("Enemy defending kindness");
                break;
        }
    }
    
    // Reset enemy defense at end of turn
    enemy->resetDefense();
}

String CombatState::getEnemyActionDescription(EnemyAction action, int damage) {
    switch (action) {
        case EnemyAction::ATTACK:
            return "The enemy attacks you for " + String(damage) + " damage!";
            
        case EnemyAction::DEFEND_DAMAGE:
            return "The enemy braces itself, reducing incoming damage!";
            
        case EnemyAction::DEFEND_KINDNESS:
            return "The enemy hardens its heart against your kindness!";
            
        default:
            return "The enemy does something mysterious...";
    }
}

void CombatState::checkCombatEnd() {
    Enemy* enemy = stateManager->getEnemy();
    Player* player = stateManager->getPlayer();
    
    if (enemy->isDefeated()) {
        phase = CombatPhase::VICTORY_SLAIN;
        showVictoryScreen(false);
        DEBUG_PRINTLN("*** ENEMY SLAIN ***");
    }
    else if (enemy->isBefriended()) {
        phase = CombatPhase::VICTORY_BEFRIEND;
        showVictoryScreen(true);
        DEBUG_PRINTLN("*** ENEMY BEFRIENDED ***");
    }
    else if (!player->isAlive()) {
        phase = CombatPhase::DEFEAT;
        showDefeatScreen();
        DEBUG_PRINTLN("*** PLAYER DIED ***");
    }
}

void CombatState::showVictoryScreen(bool befriended) {
    Display* display = stateManager->getDisplay();
    Player* player = stateManager->getPlayer();
    Enemy* enemy = stateManager->getEnemy();
    
    // Redraw combat HUD to show final state
    display->clear(COLOR_BG);
    combatHUD->drawFull(player, enemy);
    
    if (befriended) {
        combatHUD->showVictoryBefriended();
    } else {
        combatHUD->showVictorySlain();
    }
    
    // Show message in lower area
    display->clearArea(0, combatHUD->getBottomY(), SCREEN_WIDTH, 100, COLOR_BG);
    display->drawTextCentered("VICTORY!", combatHUD->getBottomY() + 20, COLOR_GREEN, 2);
    
    if (befriended) {
        display->drawTextCentered("Enemy BEFRIENDED!", combatHUD->getBottomY() + 45, COLOR_YELLOW, 1);
    } else {
        display->drawTextCentered("Enemy SLAIN!", combatHUD->getBottomY() + 45, COLOR_RED, 1);
    }
    
    display->drawTextCentered("Press A to continue", combatHUD->getBottomY() + 70, COLOR_GRAY, 1);
}

void CombatState::showDefeatScreen() {
    Display* display = stateManager->getDisplay();
    Player* player = stateManager->getPlayer();
    Enemy* enemy = stateManager->getEnemy();
    
    // Redraw combat HUD to show final state
    display->clear(COLOR_BG);
    combatHUD->drawFull(player, enemy);
    
    combatHUD->showDefeat();
    
    // Show message in lower area
    display->clearArea(0, combatHUD->getBottomY(), SCREEN_WIDTH, 100, COLOR_BG);
    display->drawTextCentered("DEFEATED!", combatHUD->getBottomY() + 20, COLOR_RED, 2);
    display->drawTextCentered("You have fallen...", combatHUD->getBottomY() + 45, COLOR_WHITE, 1);
    display->drawTextCentered("Press A to continue", combatHUD->getBottomY() + 70, COLOR_GRAY, 1);
}