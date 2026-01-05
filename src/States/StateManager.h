#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>
#include "../Core/Types.h"
#include "../Core/Config.h"
#include "../UI/Display.h"
#include "../Input/Input.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"

// Forward declarations
class IGameState;
class TitleState;
class CombatState;
class TreasureState;
class GameOverState;
class VictoryState;
class FloorTransitionState;

// =============================================================================
// STATE MANAGER - Controls game state transitions
// =============================================================================

// Number of normal enemies before boss
#define NORMAL_ENEMIES_PER_FLOOR 4
// Number of enemies between treasure rewards
#define ENEMIES_PER_TREASURE 3

class StateManager {
private:
    // === CORE SYSTEMS ===
    Display* display;
    Input* input;
    
    // === GAME DATA ===
    Player* player;
    Enemy* currentEnemy;
    
    // === RUN TRACKING ===
    int currentFloor;
    int enemiesOnFloor;      // Count of enemies defeated on current floor
    int enemiesSinceTreasure; // Count since last treasure reward
    int enemiesSlain;
    int enemiesBefriended;
    bool isBossBattle;       // Is current combat a boss?
    
    // === STATES ===
    IGameState* currentState;
    TitleState* titleState;
    CombatState* combatState;
    TreasureState* treasureState;
    GameOverState* gameOverState;
    VictoryState* victoryState;
    FloorTransitionState* floorTransitionState;
    
    // === TRANSITION ===
    GameStateID pendingState;
    bool hasPendingTransition;
    
    // Execute the pending transition
    void executeTransition();
    
    // Get state by ID
    IGameState* getStateByID(GameStateID id);

public:
    StateManager(Display* disp, Input* inp);
    ~StateManager();
    
    // === LIFECYCLE ===
    void init();
    void update();
    
    // === STATE TRANSITIONS ===
    void requestTransition(GameStateID newState);
    GameStateID getCurrentStateID() const;
    
    // === SYSTEM ACCESS ===
    Display* getDisplay() const { return display; }
    Input* getInput() const { return input; }
    
    // === PLAYER ACCESS ===
    Player* getPlayer() const { return player; }
    
    // === ENEMY MANAGEMENT ===
    void spawnEnemy(const char* name, int hp, int kindness, int atk, int def, EnemyAI ai);
    void clearEnemy();
    Enemy* getEnemy() const { return currentEnemy; }
    
    // === RUN MANAGEMENT ===
    void startNewRun();
    void endRun(bool victory);
    
    // === FLOOR/ROOM TRACKING ===
    int getCurrentFloor() const { return currentFloor; }
    int getEnemiesOnFloor() const { return enemiesOnFloor; }
    bool isBossTime() const { return enemiesOnFloor >= NORMAL_ENEMIES_PER_FLOOR; }
    bool isCurrentBattleBoss() const { return isBossBattle; }
    void setIsBossBattle(bool boss) { isBossBattle = boss; }
    void advanceEnemy();     // Called after defeating an enemy
    void advanceFloor();     // Called after defeating boss
    
    // === TREASURE TRACKING ===
    bool isTreasureTime() const { return enemiesSinceTreasure >= ENEMIES_PER_TREASURE; }
    void resetTreasureCounter() { enemiesSinceTreasure = 0; }
    
    // === STATISTICS ===
    void recordSlay();
    void recordBefriend();
    int getSlayCount() const { return enemiesSlain; }
    int getBefriendCount() const { return enemiesBefriended; }
};

#endif // STATE_MANAGER_H