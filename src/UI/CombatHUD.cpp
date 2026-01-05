#include "CombatHUD.h"

// =============================================================================
// COMBAT HUD - Implementation
// =============================================================================

CombatHUD::CombatHUD(Display* disp) : display(disp) {
    // Initialize cached values to invalid state to force first draw
    lastEnemyHP = -1;
    lastEnemyMaxHP = -1;
    lastEnemyKindness = -1;
    lastEnemyMaxKindness = -1;
    lastPlayerHP = -1;
    lastPlayerMaxHP = -1;
    lastEnemyName = "";
}

int CombatHUD::getBarWidth() const {
    return SCREEN_WIDTH - HUD_BAR_X - 10;  // 10px right margin
}

// =============================================================================
// FULL DRAW
// =============================================================================

void CombatHUD::drawFull(const Player* player, const Enemy* enemy) {
    // Clear HUD area
    display->clearArea(0, 0, SCREEN_WIDTH, HUD_DIVIDER2_Y + 2, COLOR_BG);
    
    // Draw sections
    drawEnemySection(enemy);
    drawPlayerSection(player);
    drawDividers();
    
    // Cache current values
    lastEnemyName = enemy->getName();
    lastEnemyHP = enemy->getCurrentHP();
    lastEnemyMaxHP = enemy->getMaxHP();
    lastEnemyKindness = enemy->getCurrentKindness();
    lastEnemyMaxKindness = enemy->getMaxKindness();
    lastPlayerHP = player->getCurrentHP();
    lastPlayerMaxHP = player->getMaxHP();
}

void CombatHUD::drawEnemySection(const Enemy* enemy) {
    // Enemy name
    display->drawText(enemy->getName(), HUD_LABEL_X, HUD_ENEMY_NAME_Y, COLOR_WHITE, 2);
    
    // HP label and bar
    display->drawText("HP:", HUD_LABEL_X, HUD_ENEMY_HP_Y + 2, COLOR_RED, 1);
    updateEnemyHP(enemy->getCurrentHP(), enemy->getMaxHP());
    
    // Kindness label and bar
    display->drawText("Kind:", HUD_LABEL_X, HUD_ENEMY_KIND_Y + 2, COLOR_YELLOW, 1);
    updateEnemyKindness(enemy->getCurrentKindness(), enemy->getMaxKindness());
}

void CombatHUD::drawPlayerSection(const Player* player) {
    // Player label
    display->drawText("You:", HUD_LABEL_X, HUD_PLAYER_SECTION_Y, COLOR_CYAN, 1);
    
    // HP bar
    updatePlayerHP(player->getCurrentHP(), player->getMaxHP());
}

void CombatHUD::drawDividers() {
    display->fillRect(0, HUD_DIVIDER1_Y, SCREEN_WIDTH, 2, COLOR_GRAY);
    display->fillRect(0, HUD_DIVIDER2_Y, SCREEN_WIDTH, 2, COLOR_GRAY);
}

// =============================================================================
// PARTIAL UPDATES
// =============================================================================

void CombatHUD::update(const Player* player, const Enemy* enemy) {
    // Only update what changed
    
    // Check enemy name change (shouldn't happen mid-combat, but just in case)
    if (lastEnemyName != enemy->getName()) {
        display->clearArea(HUD_LABEL_X, HUD_ENEMY_NAME_Y, SCREEN_WIDTH - 20, 16, COLOR_BG);
        display->drawText(enemy->getName(), HUD_LABEL_X, HUD_ENEMY_NAME_Y, COLOR_WHITE, 2);
        lastEnemyName = enemy->getName();
    }
    
    // Check enemy HP
    if (lastEnemyHP != enemy->getCurrentHP() || lastEnemyMaxHP != enemy->getMaxHP()) {
        updateEnemyHP(enemy->getCurrentHP(), enemy->getMaxHP());
        lastEnemyHP = enemy->getCurrentHP();
        lastEnemyMaxHP = enemy->getMaxHP();
    }
    
    // Check enemy kindness
    if (lastEnemyKindness != enemy->getCurrentKindness() || 
        lastEnemyMaxKindness != enemy->getMaxKindness()) {
        updateEnemyKindness(enemy->getCurrentKindness(), enemy->getMaxKindness());
        lastEnemyKindness = enemy->getCurrentKindness();
        lastEnemyMaxKindness = enemy->getMaxKindness();
    }
    
    // Check player HP
    if (lastPlayerHP != player->getCurrentHP() || lastPlayerMaxHP != player->getMaxHP()) {
        updatePlayerHP(player->getCurrentHP(), player->getMaxHP());
        lastPlayerHP = player->getCurrentHP();
        lastPlayerMaxHP = player->getMaxHP();
    }
}

// =============================================================================
// INDIVIDUAL BAR UPDATES
// =============================================================================

void CombatHUD::updateEnemyHP(int current, int max) {
    int barW = getBarWidth();
    
    // Draw bar
    display->drawBar(HUD_BAR_X, HUD_ENEMY_HP_Y, barW, HUD_BAR_HEIGHT, current, max, COLOR_HP);
    
    // Draw text value
    display->clearArea(HUD_BAR_X, HUD_ENEMY_HP_Y + HUD_BAR_HEIGHT + 2, 70, 10, COLOR_BG);
    String hpText = String(current) + "/" + String(max);
    display->drawText(hpText, HUD_BAR_X, HUD_ENEMY_HP_Y + HUD_BAR_HEIGHT + 2, COLOR_WHITE, 1);
}

void CombatHUD::updateEnemyKindness(int current, int max) {
    int barW = getBarWidth();
    
    // Draw bar
    display->drawBar(HUD_BAR_X, HUD_ENEMY_KIND_Y, barW, HUD_BAR_HEIGHT, current, max, COLOR_KINDNESS);
    
    // Draw text value
    display->clearArea(HUD_BAR_X, HUD_ENEMY_KIND_Y + HUD_BAR_HEIGHT + 2, 70, 10, COLOR_BG);
    String kindText = String(current) + "/" + String(max);
    display->drawText(kindText, HUD_BAR_X, HUD_ENEMY_KIND_Y + HUD_BAR_HEIGHT + 2, COLOR_WHITE, 1);
}

void CombatHUD::updatePlayerHP(int current, int max) {
    int barW = getBarWidth();
    
    // Draw bar
    display->drawBar(HUD_BAR_X, HUD_PLAYER_HP_Y, barW, HUD_BAR_HEIGHT, current, max, COLOR_HP);
    
    // Draw text value (to the right of the bar area)
    display->clearArea(HUD_BAR_X + barW + 5, HUD_PLAYER_HP_Y, 50, HUD_BAR_HEIGHT, COLOR_BG);
    // For player, show compact HP on same line
}

// =============================================================================
// VICTORY/DEFEAT DISPLAY
// =============================================================================

void CombatHUD::showVictorySlain() {
    // Flash the enemy HP bar area
    display->clearArea(HUD_LABEL_X, HUD_ENEMY_NAME_Y, SCREEN_WIDTH - 20, 70, COLOR_BG);
    display->drawTextCentered("SLAIN!", HUD_ENEMY_NAME_Y + 20, COLOR_RED, 2);
}

void CombatHUD::showVictoryBefriended() {
    // Flash the enemy kindness bar area  
    display->clearArea(HUD_LABEL_X, HUD_ENEMY_NAME_Y, SCREEN_WIDTH - 20, 70, COLOR_BG);
    display->drawTextCentered("BEFRIENDED!", HUD_ENEMY_NAME_Y + 20, COLOR_YELLOW, 2);
}

void CombatHUD::showDefeat() {
    // Flash the player HP bar area
    display->clearArea(HUD_LABEL_X, HUD_PLAYER_SECTION_Y, SCREEN_WIDTH - 20, 30, COLOR_BG);
    display->drawTextCentered("DEFEATED!", HUD_PLAYER_SECTION_Y + 5, COLOR_RED, 2);
}