#ifndef COMBAT_HUD_H
#define COMBAT_HUD_H

#include <Arduino.h>
#include "../UI/Display.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"

// =============================================================================
// COMBAT HUD - Manages all combat UI elements
// =============================================================================
// Handles:
// - Enemy HP and Kindness bars
// - Player HP bar
// - Partial updates for efficiency
// =============================================================================

class CombatHUD {
private:
    Display* display;
    
    // Layout constants (prefixed to avoid macro collision with Config.h)
    static const int HUD_ENEMY_SECTION_Y = 0;
    static const int HUD_ENEMY_NAME_Y = 5;
    static const int HUD_ENEMY_HP_Y = 28;
    static const int HUD_ENEMY_KIND_Y = 53;
    static const int HUD_DIVIDER1_Y = 78;
    
    static const int HUD_PLAYER_SECTION_Y = 85;
    static const int HUD_PLAYER_HP_Y = 100;
    static const int HUD_DIVIDER2_Y = 120;
    
    static const int HUD_BAR_X = 40;
    static const int HUD_BAR_HEIGHT = 12;
    static const int HUD_LABEL_X = 10;
    
    // Cached values for partial updates
    int lastEnemyHP;
    int lastEnemyMaxHP;
    int lastEnemyKindness;
    int lastEnemyMaxKindness;
    int lastPlayerHP;
    int lastPlayerMaxHP;
    String lastEnemyName;
    
    // Calculate bar width based on screen
    int getBarWidth() const;
    
    // Internal drawing helpers
    void drawEnemySection(const Enemy* enemy);
    void drawPlayerSection(const Player* player);
    void drawDividers();

public:
    CombatHUD(Display* disp);
    
    // === FULL DRAW ===
    // Call once when entering combat
    void drawFull(const Player* player, const Enemy* enemy);
    
    // === PARTIAL UPDATES ===
    // Call after any combat action - only redraws changed values
    void update(const Player* player, const Enemy* enemy);
    
    // === INDIVIDUAL BAR UPDATES ===
    void updateEnemyHP(int current, int max);
    void updateEnemyKindness(int current, int max);
    void updatePlayerHP(int current, int max);
    
    // === VICTORY/DEFEAT DISPLAY ===
    void showVictorySlain();
    void showVictoryBefriended();
    void showDefeat();
    
    // === LAYOUT INFO ===
    // Returns Y position where HUD ends (for other UI elements)
    int getBottomY() const { return HUD_DIVIDER2_Y + 2; }
};

#endif // COMBAT_HUD_H