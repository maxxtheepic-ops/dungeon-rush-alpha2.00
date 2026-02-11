#ifndef SPELL_SELECTOR_H
#define SPELL_SELECTOR_H

#include <Arduino.h>
#include "../UI/Display.h"
#include "../Entities/Player.h"
#include "../Combat/SpellMatrix.h"

// =============================================================================
// SPELL SELECTOR - Two-step Element + Type selection UI
// =============================================================================

enum class SelectorPhase {
    SELECT_ELEMENT,
    SELECT_TYPE,
    READY_TO_CAST
};

class SpellSelector {
private:
    Display* display;
    const Player* player;
    
    // Layout constants
    int startY;  // Where selection UI begins
    static const int ELEMENT_ROW_Y_OFFSET = 15;
    static const int TYPE_ROW_Y_OFFSET = 55;
    static const int PREVIEW_Y_OFFSET = 105;
    static const int ITEM_WIDTH = 38;
    static const int ITEM_HEIGHT = 20;
    
    // Selection state
    SelectorPhase phase;
    int elementIndex;
    int typeIndex;
    
    // Remembered selection (persists between turns)
    int rememberedElementIndex;
    int rememberedTypeIndex;
    
    // Blink state for active cursor box
    unsigned long blinkTimer;
    bool blinkVisible;
    static const unsigned long BLINK_INTERVAL_MS = 350;
    
    // Cache for partial updates
    int lastElementIndex;
    int lastTypeIndex;
    SelectorPhase lastPhase;
    bool lastBlinkVisible;
    
    // Drawing helpers
    void drawElementRow();
    void drawTypeRow();
    void drawPhaseIndicator();
    void drawBox(int x, int y, uint16_t color);
    void clearBox(int x, int y);
    int getElementX(int index) const;
    int getTypeX(int index) const;

public:
    SpellSelector(Display* disp);
    
    // === LIFECYCLE ===
    void init(const Player* p, int yPosition);
    void reset();               // Resets phase but restores remembered indices
    void fullReset();           // Full reset including remembered indices
    void rememberSelection();   // Save current indices for next turn
    
    // === INPUT HANDLING ===
    void moveLeft();
    void moveRight();
    bool confirm();     // Returns true if spell is ready to cast
    bool cancel();      // Returns true if cancelled out of element selection
    
    // === DRAWING ===
    void drawFull();
    void update();      // Partial update for cursor movement and blink
    void drawSpellPreview();
    
    // === STATE ===
    SelectorPhase getPhase() const { return phase; }
    bool isReadyToCast() const { return phase == SelectorPhase::READY_TO_CAST; }
    
    // === SELECTION RESULTS ===
    Element getSelectedElement() const;
    SpellType getSelectedType() const;
    SpellData getSelectedSpell() const;
    String getSelectedSpellName() const;
    
    // === LAYOUT INFO ===
    int getBottomY() const { return startY + PREVIEW_Y_OFFSET + 70; }
};

#endif // SPELL_SELECTOR_H