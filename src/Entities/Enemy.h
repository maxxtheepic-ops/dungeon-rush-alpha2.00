#ifndef ENEMY_H
#define ENEMY_H

#include <Arduino.h>
#include "../Core/Config.h"
#include "../Core/Types.h"

// =============================================================================
// ENEMY - Foes with HP (slay) and Kindness (befriend) bars
// =============================================================================

class Enemy {
private:
    // === IDENTITY ===
    String name;
    
    // === HEALTH (for slaying) ===
    int currentHP;
    int maxHP;
    
    // === KINDNESS (for befriending) ===
    int currentKindness;
    int maxKindness;
    
    // === COMBAT STATS ===
    int attack;
    int defense;
    
    // === ELEMENT MULTIPLIERS ===
    // Values are percentages: 100 = normal, <100 = resistant, >100 = weak
    uint8_t elementMultipliers[ELEM_COUNT];
    
    // === AI ===
    EnemyAI aiType;
    
    // === COMBAT STATE ===
    bool defendingDamage;
    bool defendingKindness;

public:
    // Constructors
    Enemy();
    Enemy(const char* enemyName, int hp, int kindness, int atk, int def, EnemyAI ai = EnemyAI::BALANCED);
    
    // === IDENTITY ===
    String getName() const { return name; }
    void setName(const char* newName) { name = newName; }
    
    // === HP GETTERS ===
    int getCurrentHP() const { return currentHP; }
    int getMaxHP() const { return maxHP; }
    
    // === KINDNESS GETTERS ===
    int getCurrentKindness() const { return currentKindness; }
    int getMaxKindness() const { return maxKindness; }
    
    // === STAT GETTERS ===
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    
    // === DAMAGE (for slaying) ===
    // Returns actual damage dealt after multipliers and defense
    int takeDamage(int amount, Element element);
    
    // === KINDNESS (for befriending) ===
    // Kindness can NEVER decrease - only accumulates
    void addKindness(int amount, Element element);
    
    // === VICTORY CONDITIONS ===
    bool isDefeated() const { return currentHP <= 0; }
    bool isBefriended() const { return currentKindness >= maxKindness; }
    
    // === ELEMENT MULTIPLIERS ===
    void setElementMultiplier(Element elem, uint8_t multiplier);
    uint8_t getElementMultiplier(Element elem) const;
    void setAllMultipliers(uint8_t value);  // Set all to same value
    
    // === DEFENSE STATE ===
    void setDefendingDamage(bool val) { defendingDamage = val; }
    void setDefendingKindness(bool val) { defendingKindness = val; }
    bool isDefendingDamage() const { return defendingDamage; }
    bool isDefendingKindness() const { return defendingKindness; }
    void resetDefense();
    
    // === AI ===
    EnemyAI getAIType() const { return aiType; }
    void setAIType(EnemyAI ai) { aiType = ai; }
    EnemyAction chooseAction();     // AI decides what to do this turn
    int performAttack();            // Returns damage to deal to player
    
    // === DEBUG ===
    void printStatus() const;
};

#endif // ENEMY_H