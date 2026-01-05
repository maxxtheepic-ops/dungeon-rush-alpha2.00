#ifndef TREASURE_STATE_H
#define TREASURE_STATE_H

#include "IGameState.h"
#include "StateManager.h"

// =============================================================================
// TREASURE STATE - Rewards after combat victory
// =============================================================================

enum class RewardType {
    NEW_ELEMENT,
    NEW_TYPE,
    NOTHING  // Already have everything
};

class TreasureState : public IGameState {
private:
    RewardType rewardType;
    Element newElement;
    SpellType newType;
    bool rewardGiven;
    
    void drawScreen();
    void giveReward();

public:
    TreasureState();
    
    void enter() override;
    void update() override;
    void exit() override;
    
    GameStateID getStateID() const override { return GameStateID::TREASURE; }
};

#endif // TREASURE_STATE_H