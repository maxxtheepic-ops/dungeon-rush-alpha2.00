#ifndef COMBAT_STATE_H
#define COMBAT_STATE_H

#include "IGameState.h"
#include "../UI/CombatHUD.h"
#include "../UI/SpellSelector.h"
#include "../UI/CombatAnimation.h"

// Forward declaration - avoid circular dependency
class StateManager;

// =============================================================================
// COMBAT STATE - Turn-based combat encounter
// =============================================================================

enum class CombatPhase {
    PLAYER_TURN,
    ANIMATING,
    VICTORY_SLAIN,
    VICTORY_BEFRIEND,
    DEFEAT
};

class CombatState : public IGameState {
private:
    // UI Components
    CombatHUD* combatHUD;
    SpellSelector* spellSelector;
    CombatAnimation* combatAnimation;
    
    // Combat state
    CombatPhase phase;
    
    // Pending spell data (stored during animation)
    Element pendingElement;
    SpellType pendingType;
    uint8_t pendingDamage;
    uint8_t pendingKindness;
    
    // Pending enemy action data
    EnemyAction pendingEnemyAction;
    int pendingEnemyDamage;
    
    // Methods
    void drawCombatScreen();
    void handlePlayerTurn();
    void startSpellAnimation();
    void handleAnimation();
    void applyPendingActions();
    void checkCombatEnd();
    void showVictoryScreen(bool befriended);
    void showDefeatScreen();
    
    // Helper to get enemy action description
    String getEnemyActionDescription(EnemyAction action, int damage);

public:
    CombatState();
    ~CombatState();
    
    void enter() override;
    void update() override;
    void exit() override;
    
    GameStateID getStateID() const override { return GameStateID::COMBAT; }
};

#endif // COMBAT_STATE_H