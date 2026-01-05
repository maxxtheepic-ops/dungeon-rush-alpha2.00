#ifndef VICTORY_STATE_H
#define VICTORY_STATE_H

#include "IGameState.h"
#include "StateManager.h"

// =============================================================================
// VICTORY STATE - Run complete (placeholder for future expansion)
// =============================================================================

class VictoryState : public IGameState {
private:
    void drawScreen();

public:
    VictoryState();
    
    void enter() override;
    void update() override;
    void exit() override;
    
    GameStateID getStateID() const override { return GameStateID::VICTORY; }
};

#endif // VICTORY_STATE_H