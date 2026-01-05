#ifndef FLOOR_TRANSITION_STATE_H
#define FLOOR_TRANSITION_STATE_H

#include "IGameState.h"

// Forward declaration
class StateManager;

// =============================================================================
// FLOOR TRANSITION STATE - Shows progression between floors
// =============================================================================

class FloorTransitionState : public IGameState {
private:
    int fromFloor;
    int toFloor;
    unsigned long startTime;
    bool canAdvance;
    
    static const unsigned long MIN_DISPLAY_MS = 1000;  // 1 second minimum
    
    void drawScreen();
    void drawAnimation();
    void drawAdvancePrompt();

public:
    FloorTransitionState();
    
    void enter() override;
    void update() override;
    void exit() override;
    
    GameStateID getStateID() const override { return GameStateID::FLOOR_TRANSITION; }
};

#endif // FLOOR_TRANSITION_STATE_H