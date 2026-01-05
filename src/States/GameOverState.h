#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include "IGameState.h"

// Forward declaration - avoid circular dependency
class StateManager;

// =============================================================================
// GAME OVER STATE - Death screen with run statistics
// =============================================================================

class GameOverState : public IGameState {
private:
    void drawScreen();

public:
    GameOverState();
    
    void enter() override;
    void update() override;
    void exit() override;
    
    GameStateID getStateID() const override { return GameStateID::GAME_OVER; }
};

#endif // GAME_OVER_STATE_H