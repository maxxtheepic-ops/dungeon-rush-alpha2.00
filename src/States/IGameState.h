#ifndef IGAME_STATE_H
#define IGAME_STATE_H

#include <Arduino.h>
#include "../Core/Types.h"

// =============================================================================
// IGAME STATE - Base interface for all game states
// =============================================================================

// Forward declaration
class StateManager;

class IGameState {
protected:
    StateManager* stateManager;
    
public:
    IGameState() : stateManager(nullptr) {}
    virtual ~IGameState() {}
    
    // Set reference to state manager (called by StateManager)
    void setStateManager(StateManager* sm) { stateManager = sm; }
    
    // === LIFECYCLE ===
    
    // Called when entering this state
    virtual void enter() = 0;
    
    // Called every frame while in this state
    virtual void update() = 0;
    
    // Called when leaving this state
    virtual void exit() = 0;
    
    // === OPTIONAL OVERRIDES ===
    
    // Called when returning to this state (e.g., after a sub-menu)
    virtual void resume() { enter(); }
    
    // Get state ID for debugging
    virtual GameStateID getStateID() const = 0;
};

#endif // IGAME_STATE_H