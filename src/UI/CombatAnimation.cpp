#include "CombatAnimation.h"

// =============================================================================
// COMBAT ANIMATION - Implementation
// =============================================================================

CombatAnimation::CombatAnimation(Display* disp) : display(disp) {
    phase = AnimationPhase::IDLE;
    phaseStartTime = 0;
    canAdvance = false;
    spellElement = ELEM_NONE;
    spellType = TYPE_NONE;
    enemyAction = EnemyAction::ATTACK;
}

// =============================================================================
// START ANIMATIONS
// =============================================================================

void CombatAnimation::showPlayerSpell(Element elem, SpellType type, const char* description) {
    spellElement = elem;
    spellType = type;
    
    // Copy description from PROGMEM
    char buffer[100];
    strncpy_P(buffer, description, 99);
    buffer[99] = '\0';
    spellDescription = String(buffer);
    
    // Start animation
    phase = AnimationPhase::PLAYER_SPELL;
    phaseStartTime = millis();
    canAdvance = false;
    
    // Draw the scene
    drawBattleScene();
    drawSpellEffect();
    drawTextBox(spellDescription);
    
    DEBUG_PRINTLN("Animation: Player spell phase started");
}

void CombatAnimation::queueEnemyAction(EnemyAction action, const String& description) {
    enemyAction = action;
    enemyDescription = description;
}

// =============================================================================
// UPDATE
// =============================================================================

bool CombatAnimation::update(bool aPressed) {
    if (phase == AnimationPhase::IDLE || phase == AnimationPhase::COMPLETE) {
        return phase == AnimationPhase::COMPLETE;
    }
    
    unsigned long elapsed = millis() - phaseStartTime;
    
    // Check if minimum time has passed
    if (!canAdvance && elapsed >= MIN_DISPLAY_MS) {
        canAdvance = true;
        drawAdvancePrompt();
    }
    
    // Check for advance input
    if (canAdvance && aPressed) {
        if (phase == AnimationPhase::PLAYER_SPELL) {
            // Move to enemy action phase
            phase = AnimationPhase::ENEMY_ACTION;
            phaseStartTime = millis();
            canAdvance = false;
            
            // Draw enemy action
            drawBattleScene();
            drawEnemyActionEffect();
            drawTextBox(enemyDescription);
            
            DEBUG_PRINTLN("Animation: Enemy action phase started");
        }
        else if (phase == AnimationPhase::ENEMY_ACTION) {
            // Animation complete
            phase = AnimationPhase::COMPLETE;
            DEBUG_PRINTLN("Animation: Complete");
            return true;
        }
    }
    
    return false;
}

void CombatAnimation::reset() {
    phase = AnimationPhase::IDLE;
    canAdvance = false;
}

// =============================================================================
// DRAWING - BATTLE SCENE
// =============================================================================

void CombatAnimation::drawBattleScene() {
    // Clear entire screen
    display->clear(COLOR_BG);
    
    // Draw player and enemy
    drawPlayer();
    drawEnemy();
}

void CombatAnimation::drawPlayer() {
    // Placeholder: Simple rectangle for player (wizard)
    // Will be replaced with sprite later
    
    // Body (robe)
    display->fillRect(ANIM_PLAYER_X, ANIM_PLAYER_Y, ANIM_PLAYER_SIZE, ANIM_PLAYER_SIZE, COLOR_CYAN);
    display->drawRect(ANIM_PLAYER_X, ANIM_PLAYER_Y, ANIM_PLAYER_SIZE, ANIM_PLAYER_SIZE, COLOR_WHITE);
    
    // Head (circle approximation with small rect)
    int headSize = ANIM_PLAYER_SIZE / 3;
    int headX = ANIM_PLAYER_X + (ANIM_PLAYER_SIZE - headSize) / 2;
    int headY = ANIM_PLAYER_Y - headSize - 2;
    display->fillRect(headX, headY, headSize, headSize, COLOR_WHITE);
    
    // Hat (triangle approximation)
    int hatX = headX - 3;
    int hatY = headY - 12;
    display->fillRect(hatX, hatY, headSize + 6, 4, COLOR_MAGENTA);
    display->fillRect(hatX + 3, hatY - 4, headSize, 4, COLOR_MAGENTA);
    display->fillRect(hatX + 6, hatY - 8, headSize - 6, 4, COLOR_MAGENTA);
}

void CombatAnimation::drawEnemy() {
    // Placeholder: Simple blob for enemy (slime)
    // Will be replaced with sprite later
    
    // Main body (oval approximation)
    display->fillRect(ANIM_ENEMY_X, ANIM_ENEMY_Y + 10, ANIM_ENEMY_SIZE, ANIM_ENEMY_SIZE - 10, COLOR_GREEN);
    display->fillRect(ANIM_ENEMY_X + 5, ANIM_ENEMY_Y + 5, ANIM_ENEMY_SIZE - 10, 10, COLOR_GREEN);
    display->fillRect(ANIM_ENEMY_X + 10, ANIM_ENEMY_Y, ANIM_ENEMY_SIZE - 20, 10, COLOR_GREEN);
    
    // Eyes
    int eyeY = ANIM_ENEMY_Y + 20;
    int leftEyeX = ANIM_ENEMY_X + 12;
    int rightEyeX = ANIM_ENEMY_X + ANIM_ENEMY_SIZE - 18;
    display->fillRect(leftEyeX, eyeY, 6, 8, COLOR_WHITE);
    display->fillRect(rightEyeX, eyeY, 6, 8, COLOR_WHITE);
    
    // Pupils
    display->fillRect(leftEyeX + 2, eyeY + 3, 3, 4, COLOR_BG);
    display->fillRect(rightEyeX + 2, eyeY + 3, 3, 4, COLOR_BG);
    
    // Outline
    display->drawRect(ANIM_ENEMY_X, ANIM_ENEMY_Y, ANIM_ENEMY_SIZE, ANIM_ENEMY_SIZE, COLOR_WHITE);
}

// =============================================================================
// DRAWING - SPELL EFFECTS
// =============================================================================

void CombatAnimation::drawSpellEffect() {
    uint16_t color = getEffectColor();
    
    // Draw effect based on spell type (placeholder shapes)
    switch (spellType) {
        case TYPE_MISSILE:
            // Small fast projectile - arrow shape
            display->fillRect(ANIM_EFFECT_X, ANIM_EFFECT_Y + 25, 40, 10, color);
            display->fillRect(ANIM_EFFECT_X + 40, ANIM_EFFECT_Y + 20, 10, 20, color);
            break;
            
        case TYPE_BALL:
            // Large sphere - filled circle approximation
            display->fillRect(ANIM_EFFECT_X + 10, ANIM_EFFECT_Y, 40, 60, color);
            display->fillRect(ANIM_EFFECT_X, ANIM_EFFECT_Y + 10, 60, 40, color);
            display->fillRect(ANIM_EFFECT_X + 5, ANIM_EFFECT_Y + 5, 50, 50, color);
            break;
            
        case TYPE_EXPLOSION:
            // Starburst pattern
            display->fillRect(ANIM_EFFECT_X + 20, ANIM_EFFECT_Y, 20, 60, color);
            display->fillRect(ANIM_EFFECT_X, ANIM_EFFECT_Y + 20, 60, 20, color);
            display->fillRect(ANIM_EFFECT_X + 10, ANIM_EFFECT_Y + 10, 40, 40, color);
            // Add some "debris"
            display->fillRect(ANIM_EFFECT_X - 10, ANIM_EFFECT_Y + 10, 8, 8, color);
            display->fillRect(ANIM_EFFECT_X + 55, ANIM_EFFECT_Y + 40, 8, 8, color);
            display->fillRect(ANIM_EFFECT_X + 5, ANIM_EFFECT_Y + 50, 6, 6, color);
            break;
            
        case TYPE_BEAM:
            // Horizontal beam
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE, ANIM_EFFECT_Y + 25, 
                            ANIM_ENEMY_X - ANIM_PLAYER_X - ANIM_PLAYER_SIZE + 20, 12, color);
            // Glow effect
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE, ANIM_EFFECT_Y + 22, 
                            ANIM_ENEMY_X - ANIM_PLAYER_X - ANIM_PLAYER_SIZE + 20, 3, color);
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE, ANIM_EFFECT_Y + 37, 
                            ANIM_ENEMY_X - ANIM_PLAYER_X - ANIM_PLAYER_SIZE + 20, 3, color);
            break;
            
        case TYPE_BOLT:
            // Lightning bolt - zigzag
            display->fillRect(ANIM_EFFECT_X + 25, ANIM_EFFECT_Y, 15, 15, color);
            display->fillRect(ANIM_EFFECT_X + 15, ANIM_EFFECT_Y + 12, 20, 12, color);
            display->fillRect(ANIM_EFFECT_X + 25, ANIM_EFFECT_Y + 22, 20, 12, color);
            display->fillRect(ANIM_EFFECT_X + 15, ANIM_EFFECT_Y + 32, 20, 12, color);
            display->fillRect(ANIM_EFFECT_X + 22, ANIM_EFFECT_Y + 42, 15, 18, color);
            break;
            
        case TYPE_PUNCH:
            // Fist impact - POW style
            display->fillRect(ANIM_EFFECT_X + 5, ANIM_EFFECT_Y + 10, 50, 40, color);
            display->fillRect(ANIM_EFFECT_X + 15, ANIM_EFFECT_Y + 5, 30, 50, color);
            // Impact lines
            display->fillRect(ANIM_EFFECT_X - 15, ANIM_EFFECT_Y + 25, 15, 4, COLOR_WHITE);
            display->fillRect(ANIM_EFFECT_X + 60, ANIM_EFFECT_Y + 25, 15, 4, COLOR_WHITE);
            display->fillRect(ANIM_EFFECT_X + 25, ANIM_EFFECT_Y - 10, 4, 12, COLOR_WHITE);
            break;
            
        case TYPE_WHIP:
            // Curved whip line
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE - 5, ANIM_EFFECT_Y + 40, 20, 6, color);
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE + 12, ANIM_EFFECT_Y + 30, 25, 6, color);
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE + 32, ANIM_EFFECT_Y + 20, 30, 6, color);
            display->fillRect(ANIM_PLAYER_X + ANIM_PLAYER_SIZE + 55, ANIM_EFFECT_Y + 15, 20, 6, color);
            break;
            
        case TYPE_NONE:
        default:
            // Generic sparkle/nothing
            display->fillRect(ANIM_EFFECT_X + 25, ANIM_EFFECT_Y + 25, 10, 10, color);
            display->fillRect(ANIM_EFFECT_X + 20, ANIM_EFFECT_Y + 28, 20, 4, color);
            display->fillRect(ANIM_EFFECT_X + 28, ANIM_EFFECT_Y + 20, 4, 20, color);
            break;
    }
    
    // Draw spell name above effect
    String spellName = "";
    if (spellElement != ELEM_NONE) {
        spellName += getElementName(spellElement);
        spellName += " ";
    }
    if (spellType != TYPE_NONE) {
        spellName += getTypeName(spellType);
    }
    if (spellName.length() == 0) {
        spellName = "Nothing";
    }
    
    display->drawTextCentered(spellName, ANIM_EFFECT_Y - 20, color, 1);
}

uint16_t CombatAnimation::getEffectColor() const {
    return getElementColor(spellElement);
}

// =============================================================================
// DRAWING - ENEMY ACTION EFFECTS
// =============================================================================

void CombatAnimation::drawEnemyActionEffect() {
    switch (enemyAction) {
        case EnemyAction::ATTACK: {
            // Enemy attack effect - claw/slam toward player
            uint16_t color = COLOR_RED;
            
            // Impact lines from enemy toward player
            int startX = ANIM_ENEMY_X;
            int startY = ANIM_ENEMY_Y + ANIM_ENEMY_SIZE;
            int endX = ANIM_PLAYER_X + ANIM_PLAYER_SIZE;
            int endY = ANIM_PLAYER_Y;
            
            // Draw slashing lines
            display->fillRect(startX - 10, startY + 20, 40, 8, color);
            display->fillRect(startX - 25, startY + 40, 45, 8, color);
            display->fillRect(startX - 40, startY + 60, 50, 8, color);
            
            // Impact burst near player
            display->fillRect(endX - 10, endY - 20, 30, 30, color);
            display->fillRect(endX - 5, endY - 25, 20, 40, color);
            
            // "ATTACK!" text
            display->drawTextCentered("ATTACK!", ANIM_EFFECT_Y, COLOR_RED, 2);
            break;
        }
        
        case EnemyAction::DEFEND_DAMAGE: {
            // Shield effect around enemy
            uint16_t color = COLOR_CYAN;
            
            // Shield bubble around enemy
            display->drawRect(ANIM_ENEMY_X - 8, ANIM_ENEMY_Y - 8, ANIM_ENEMY_SIZE + 16, ANIM_ENEMY_SIZE + 16, color);
            display->drawRect(ANIM_ENEMY_X - 5, ANIM_ENEMY_Y - 5, ANIM_ENEMY_SIZE + 10, ANIM_ENEMY_SIZE + 10, color);
            display->drawRect(ANIM_ENEMY_X - 2, ANIM_ENEMY_Y - 2, ANIM_ENEMY_SIZE + 4, ANIM_ENEMY_SIZE + 4, color);
            
            // "DEFEND!" text
            display->drawTextCentered("DEFEND!", ANIM_EFFECT_Y + 30, COLOR_CYAN, 2);
            break;
        }
        
        case EnemyAction::DEFEND_KINDNESS: {
            // Emotional shield - heart barrier
            uint16_t color = COLOR_MAGENTA;
            
            // Shield around enemy (different style)
            display->drawRect(ANIM_ENEMY_X - 8, ANIM_ENEMY_Y - 8, ANIM_ENEMY_SIZE + 16, ANIM_ENEMY_SIZE + 16, color);
            
            // Small hearts around enemy
            int hx = ANIM_ENEMY_X - 15;
            int hy = ANIM_ENEMY_Y + 10;
            display->fillRect(hx, hy, 6, 6, color);
            display->fillRect(hx + 5, hy, 6, 6, color);
            display->fillRect(hx + 2, hy + 4, 8, 6, color);
            
            hx = ANIM_ENEMY_X + ANIM_ENEMY_SIZE + 10;
            display->fillRect(hx, hy, 6, 6, color);
            display->fillRect(hx + 5, hy, 6, 6, color);
            display->fillRect(hx + 2, hy + 4, 8, 6, color);
            
            // "GUARD!" text
            display->drawTextCentered("GUARD HEART!", ANIM_EFFECT_Y + 30, COLOR_MAGENTA, 1);
            break;
        }
    }
}

// =============================================================================
// DRAWING - TEXT BOX
// =============================================================================

void CombatAnimation::drawTextBox(const String& text) {
    // Draw text box background
    display->fillRect(0, ANIM_TEXTBOX_Y, SCREEN_WIDTH, ANIM_TEXTBOX_HEIGHT, COLOR_DARK_GRAY);
    display->drawRect(0, ANIM_TEXTBOX_Y, SCREEN_WIDTH, ANIM_TEXTBOX_HEIGHT, COLOR_WHITE);
    display->drawRect(2, ANIM_TEXTBOX_Y + 2, SCREEN_WIDTH - 4, ANIM_TEXTBOX_HEIGHT - 4, COLOR_GRAY);
    
    // Word wrap and draw text (with dark gray background to match the box)
    int textX = ANIM_TEXTBOX_PADDING;
    int textY = ANIM_TEXTBOX_Y + ANIM_TEXTBOX_PADDING;
    int lineHeight = 14;
    int maxCharsPerLine = 26;  // Approximate for size 1 font
    
    String remaining = text;
    int line = 0;
    int maxLines = 3;
    
    while (remaining.length() > 0 && line < maxLines) {
        String lineText;
        
        if ((int)remaining.length() <= maxCharsPerLine) {
            lineText = remaining;
            remaining = "";
        } else {
            // Find last space within limit
            int splitPos = remaining.lastIndexOf(' ', maxCharsPerLine);
            if (splitPos < 5) splitPos = maxCharsPerLine;
            
            lineText = remaining.substring(0, splitPos);
            remaining = remaining.substring(splitPos + 1);
        }
        
        display->drawText(lineText, textX, textY + (line * lineHeight), COLOR_WHITE, 1, COLOR_DARK_GRAY);
        line++;
    }
}

void CombatAnimation::drawAdvancePrompt() {
    // Draw "Press A" prompt at bottom right of text box
    int promptY = ANIM_TEXTBOX_Y + ANIM_TEXTBOX_HEIGHT - 16;
    display->drawTextRight("Press A >", promptY, COLOR_YELLOW, 1, COLOR_DARK_GRAY);
}