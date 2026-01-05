#ifndef COMBAT_ANIMATION_H
#define COMBAT_ANIMATION_H

#include <Arduino.h>
#include "../UI/Display.h"
#include "../Core/Types.h"

// =============================================================================
// COMBAT ANIMATION - Visual battle scene between player and enemy actions
// =============================================================================

enum class AnimationPhase {
    IDLE,
    PLAYER_SPELL,
    ENEMY_ACTION,
    COMPLETE
};

class CombatAnimation {
private:
    Display* display;
    
    // Layout constants (prefixed to avoid macro collision with Config.h)
    static const int ANIM_PLAYER_X = 25;
    static const int ANIM_PLAYER_Y = 180;
    static const int ANIM_PLAYER_SIZE = 40;
    
    static const int ANIM_ENEMY_X = 105;
    static const int ANIM_ENEMY_Y = 40;
    static const int ANIM_ENEMY_SIZE = 50;
    
    static const int ANIM_EFFECT_X = 85;
    static const int ANIM_EFFECT_Y = 100;
    static const int ANIM_EFFECT_SIZE = 60;
    
    static const int ANIM_TEXTBOX_Y = 240;
    static const int ANIM_TEXTBOX_HEIGHT = 70;
    static const int ANIM_TEXTBOX_PADDING = 8;
    
    static const unsigned long MIN_DISPLAY_MS = 1000;  // 1 second minimum
    
    // Animation state
    AnimationPhase phase;
    unsigned long phaseStartTime;
    bool canAdvance;
    
    // Player spell data
    Element spellElement;
    SpellType spellType;
    String spellDescription;
    
    // Enemy action data
    EnemyAction enemyAction;
    String enemyDescription;
    
    // Drawing methods
    void drawBattleScene();
    void drawPlayer();
    void drawEnemy();
    void drawSpellEffect();
    void drawEnemyActionEffect();
    void drawTextBox(const String& text);
    void drawAdvancePrompt();
    
    // Effect helpers (placeholder shapes based on element)
    uint16_t getEffectColor() const;
    void drawEffectShape();

public:
    CombatAnimation(Display* disp);
    
    // === START ANIMATIONS ===
    
    // Begin player spell animation
    void showPlayerSpell(Element elem, SpellType type, const char* description);
    
    // Set up enemy action (shown after player spell)
    void queueEnemyAction(EnemyAction action, const String& description);
    
    // === UPDATE ===
    
    // Call each frame - returns true when entire sequence is complete
    // Pass true for aPressed when A button was just pressed
    bool update(bool aPressed);
    
    // === STATE ===
    AnimationPhase getPhase() const { return phase; }
    bool isComplete() const { return phase == AnimationPhase::COMPLETE; }
    bool isIdle() const { return phase == AnimationPhase::IDLE; }
    
    // Reset to idle state
    void reset();
};

#endif // COMBAT_ANIMATION_H