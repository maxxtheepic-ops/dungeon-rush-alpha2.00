#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "IGameState.h"
#include "StateManager.h"

// =============================================================================
// TITLE STATE - Main menu / Title screen
// =============================================================================

class TitleState : public IGameState {
private:
    int selectedOption;
    static const int NUM_OPTIONS = 2;
    
    void drawScreen();
    void drawCursor();
    void clearCursor();
    
public:
    TitleState();
    
    void enter() override;
    void update() override;
    void exit() override;
    
    GameStateID getStateID() const override { return GameStateID::TITLE; }
};

#endif // TITLE_STATE_H