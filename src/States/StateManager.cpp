#include "StateManager.h"
#include "IGameState.h"
#include "TitleState.h"
#include "CombatState.h"
#include "TreasureState.h"
#include "GameOverState.h"
#include "VictoryState.h"
#include "FloorTransitionState.h"

// =============================================================================
// STATE MANAGER - Implementation
// =============================================================================

StateManager::StateManager(Display* disp, Input* inp) 
    : display(disp), input(inp) {
    
    player = nullptr;
    currentEnemy = nullptr;
    
    currentFloor = 1;
    enemiesOnFloor = 0;
    enemiesSinceTreasure = 0;
    enemiesSlain = 0;
    enemiesBefriended = 0;
    isBossBattle = false;
    
    currentState = nullptr;
    titleState = nullptr;
    combatState = nullptr;
    treasureState = nullptr;
    gameOverState = nullptr;
    victoryState = nullptr;
    floorTransitionState = nullptr;
    
    pendingState = GameStateID::TITLE;
    hasPendingTransition = false;
}

StateManager::~StateManager() {
    if (player) delete player;
    if (currentEnemy) delete currentEnemy;
    
    if (titleState) delete titleState;
    if (combatState) delete combatState;
    if (treasureState) delete treasureState;
    if (gameOverState) delete gameOverState;
    if (victoryState) delete victoryState;
    if (floorTransitionState) delete floorTransitionState;
}

void StateManager::init() {
    DEBUG_PRINTLN("StateManager: Initializing...");
    
    // Create player
    player = new Player();
    
    // Create all states
    titleState = new TitleState();
    combatState = new CombatState();
    treasureState = new TreasureState();
    gameOverState = new GameOverState();
    victoryState = new VictoryState();
    floorTransitionState = new FloorTransitionState();
    
    // Set state manager reference for all states
    titleState->setStateManager(this);
    combatState->setStateManager(this);
    treasureState->setStateManager(this);
    gameOverState->setStateManager(this);
    victoryState->setStateManager(this);
    floorTransitionState->setStateManager(this);
    
    // Start with title state
    currentState = titleState;
    currentState->enter();
    
    DEBUG_PRINTLN("StateManager: Ready");
}

void StateManager::update() {
    // Execute pending transition first
    if (hasPendingTransition) {
        executeTransition();
    }
    
    // Update current state
    if (currentState) {
        currentState->update();
    }
}

// =============================================================================
// STATE TRANSITIONS
// =============================================================================

void StateManager::requestTransition(GameStateID newState) {
    DEBUG_PRINT("StateManager: Transition requested to ");
    DEBUG_PRINTLN((int)newState);
    
    pendingState = newState;
    hasPendingTransition = true;
}

void StateManager::executeTransition() {
    if (!hasPendingTransition) return;
    
    hasPendingTransition = false;
    
    // Exit current state
    if (currentState) {
        currentState->exit();
    }
    
    // Get new state
    IGameState* newState = getStateByID(pendingState);
    
    if (newState) {
        currentState = newState;
        currentState->enter();
    } else {
        DEBUG_PRINTLN("StateManager: ERROR - Invalid state ID!");
    }
}

IGameState* StateManager::getStateByID(GameStateID id) {
    switch (id) {
        case GameStateID::TITLE:            return titleState;
        case GameStateID::COMBAT:           return combatState;
        case GameStateID::TREASURE:         return treasureState;
        case GameStateID::FLOOR_TRANSITION: return floorTransitionState;
        case GameStateID::GAME_OVER:        return gameOverState;
        case GameStateID::VICTORY:          return victoryState;
        default:                            return nullptr;
    }
}

GameStateID StateManager::getCurrentStateID() const {
    if (currentState) {
        return currentState->getStateID();
    }
    return GameStateID::TITLE;
}

// =============================================================================
// ENEMY MANAGEMENT
// =============================================================================

void StateManager::spawnEnemy(const char* name, int hp, int kindness, int atk, int def, EnemyAI ai) {
    // Clear existing enemy
    clearEnemy();
    
    // Create new enemy
    currentEnemy = new Enemy(name, hp, kindness, atk, def, ai);
    
    DEBUG_PRINT("StateManager: Spawned enemy - ");
    DEBUG_PRINTLN(name);
}

void StateManager::clearEnemy() {
    if (currentEnemy) {
        delete currentEnemy;
        currentEnemy = nullptr;
    }
}

// =============================================================================
// RUN MANAGEMENT
// =============================================================================

void StateManager::startNewRun() {
    DEBUG_PRINTLN("StateManager: Starting new run");
    
    // Reset player
    player->reset();
    player->initStarterAbilities();
    
    // Reset run stats
    currentFloor = 1;
    enemiesOnFloor = 0;
    enemiesSinceTreasure = 0;
    enemiesSlain = 0;
    enemiesBefriended = 0;
    isBossBattle = false;
    
    // Clear any existing enemy
    clearEnemy();
}

void StateManager::endRun(bool victory) {
    DEBUG_PRINT("StateManager: Run ended - ");
    DEBUG_PRINTLN(victory ? "VICTORY" : "DEFEAT");
    
    // Clear enemy
    clearEnemy();
    
    // Could save stats here for persistence
}

// =============================================================================
// FLOOR/ROOM TRACKING
// =============================================================================

void StateManager::advanceEnemy() {
    enemiesOnFloor++;
    enemiesSinceTreasure++;
    DEBUG_PRINT("StateManager: Enemy ");
    DEBUG_PRINT(enemiesOnFloor);
    DEBUG_PRINT(" of floor ");
    DEBUG_PRINT(currentFloor);
    DEBUG_PRINT(" (Treasure in ");
    DEBUG_PRINT(ENEMIES_PER_TREASURE - enemiesSinceTreasure);
    DEBUG_PRINTLN(")");
}

void StateManager::advanceFloor() {
    currentFloor++;
    enemiesOnFloor = 0;
    isBossBattle = false;
    
    DEBUG_PRINT("StateManager: Advanced to floor ");
    DEBUG_PRINTLN(currentFloor);
    
    // For now, no victory condition - just keep going
    // Could add: if (currentFloor > MAX_FLOORS) requestTransition(GameStateID::VICTORY);
}

// =============================================================================
// STATISTICS
// =============================================================================

void StateManager::recordSlay() {
    enemiesSlain++;
    DEBUG_PRINT("StateManager: Slay recorded. Total: ");
    DEBUG_PRINTLN(enemiesSlain);
}

void StateManager::recordBefriend() {
    enemiesBefriended++;
    DEBUG_PRINT("StateManager: Befriend recorded. Total: ");
    DEBUG_PRINTLN(enemiesBefriended);
}