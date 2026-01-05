#include "Enemy.h"

// =============================================================================
// ENEMY - Implementation
// =============================================================================

Enemy::Enemy() {
    name = "Unknown";
    currentHP = 50;
    maxHP = 50;
    currentKindness = 0;
    maxKindness = 50;
    attack = 10;
    defense = 5;
    aiType = EnemyAI::BALANCED;
    defendingDamage = false;
    defendingKindness = false;
    
    // Default all multipliers to 100 (normal damage)
    setAllMultipliers(100);
}

Enemy::Enemy(const char* enemyName, int hp, int kindness, int atk, int def, EnemyAI ai) {
    name = enemyName;
    currentHP = hp;
    maxHP = hp;
    currentKindness = 0;
    maxKindness = kindness;
    attack = atk;
    defense = def;
    aiType = ai;
    defendingDamage = false;
    defendingKindness = false;
    
    // Default all multipliers to 100 (normal damage)
    setAllMultipliers(100);
}

// === DAMAGE ===

int Enemy::takeDamage(int amount, Element element) {
    // Apply element multiplier
    int multiplier = getElementMultiplier(element);
    int modifiedDamage = (amount * multiplier) / 100;
    
    // Apply defense reduction if defending
    if (defendingDamage) {
        modifiedDamage = modifiedDamage / 2;  // 50% reduction
    }
    
    // Apply base defense
    int actualDamage = modifiedDamage - defense;
    if (actualDamage < 1) actualDamage = 1;  // Minimum 1 damage
    
    currentHP -= actualDamage;
    if (currentHP < 0) currentHP = 0;
    
    DEBUG_PRINT(name);
    DEBUG_PRINT(" took ");
    DEBUG_PRINT(actualDamage);
    DEBUG_PRINT(" ");
    DEBUG_PRINT(getElementName(element));
    DEBUG_PRINT(" damage (");
    DEBUG_PRINT(multiplier);
    DEBUG_PRINT("%) [");
    DEBUG_PRINT(currentHP);
    DEBUG_PRINT("/");
    DEBUG_PRINT(maxHP);
    DEBUG_PRINTLN(" HP]");
    
    return actualDamage;
}

// === KINDNESS ===

void Enemy::addKindness(int amount, Element element) {
    // Apply element multiplier (some enemies more receptive to certain elements)
    int multiplier = getElementMultiplier(element);
    int modifiedKindness = (amount * multiplier) / 100;
    
    // Apply defense reduction if defending kindness
    if (defendingKindness) {
        modifiedKindness = modifiedKindness / 2;  // 50% reduction
    }
    
    // Minimum 1 kindness
    if (modifiedKindness < 1) modifiedKindness = 1;
    
    // Kindness can NEVER decrease
    currentKindness += modifiedKindness;
    if (currentKindness > maxKindness) currentKindness = maxKindness;
    
    DEBUG_PRINT(name);
    DEBUG_PRINT(" received ");
    DEBUG_PRINT(modifiedKindness);
    DEBUG_PRINT(" ");
    DEBUG_PRINT(getElementName(element));
    DEBUG_PRINT(" kindness [");
    DEBUG_PRINT(currentKindness);
    DEBUG_PRINT("/");
    DEBUG_PRINT(maxKindness);
    DEBUG_PRINTLN("]");
}

// === ELEMENT MULTIPLIERS ===

void Enemy::setElementMultiplier(Element elem, uint8_t multiplier) {
    if (elem < ELEM_COUNT) {
        elementMultipliers[elem] = multiplier;
    }
}

uint8_t Enemy::getElementMultiplier(Element elem) const {
    if (elem < ELEM_COUNT) {
        return elementMultipliers[elem];
    }
    return 100;  // Default normal damage
}

void Enemy::setAllMultipliers(uint8_t value) {
    for (int i = 0; i < ELEM_COUNT; i++) {
        elementMultipliers[i] = value;
    }
}

// === DEFENSE STATE ===

void Enemy::resetDefense() {
    defendingDamage = false;
    defendingKindness = false;
}

// === AI ===

EnemyAction Enemy::chooseAction() {
    int roll = random(1, 101);  // 1-100
    
    switch (aiType) {
        case EnemyAI::AGGRESSIVE:
            // 70% attack, 20% defend damage, 10% defend kindness
            if (roll <= 70) return EnemyAction::ATTACK;
            if (roll <= 90) return EnemyAction::DEFEND_DAMAGE;
            return EnemyAction::DEFEND_KINDNESS;
            
        case EnemyAI::DEFENSIVE:
            // 30% attack, 50% defend damage, 20% defend kindness
            if (roll <= 30) return EnemyAction::ATTACK;
            if (roll <= 80) return EnemyAction::DEFEND_DAMAGE;
            return EnemyAction::DEFEND_KINDNESS;
            
        case EnemyAI::SHY:
            // 30% attack, 20% defend damage, 50% defend kindness
            if (roll <= 30) return EnemyAction::ATTACK;
            if (roll <= 50) return EnemyAction::DEFEND_DAMAGE;
            return EnemyAction::DEFEND_KINDNESS;
            
        case EnemyAI::BALANCED:
        default:
            // 40% attack, 30% defend damage, 30% defend kindness
            if (roll <= 40) return EnemyAction::ATTACK;
            if (roll <= 70) return EnemyAction::DEFEND_DAMAGE;
            return EnemyAction::DEFEND_KINDNESS;
    }
}

int Enemy::performAttack() {
    // Base attack with small variance
    int variance = random(-2, 3);  // -2 to +2
    int damage = attack + variance;
    if (damage < 1) damage = 1;
    
    DEBUG_PRINT(name);
    DEBUG_PRINT(" attacks for ");
    DEBUG_PRINT(damage);
    DEBUG_PRINTLN(" damage!");
    
    return damage;
}

// === DEBUG ===

void Enemy::printStatus() const {
    Serial.println("=== ENEMY STATUS ===");
    Serial.print("Name: ");
    Serial.println(name);
    Serial.print("HP: ");
    Serial.print(currentHP);
    Serial.print("/");
    Serial.println(maxHP);
    Serial.print("Kindness: ");
    Serial.print(currentKindness);
    Serial.print("/");
    Serial.println(maxKindness);
    Serial.print("ATK: ");
    Serial.print(attack);
    Serial.print(" DEF: ");
    Serial.println(defense);
    
    Serial.print("AI: ");
    switch (aiType) {
        case EnemyAI::AGGRESSIVE: Serial.println("Aggressive"); break;
        case EnemyAI::DEFENSIVE:  Serial.println("Defensive"); break;
        case EnemyAI::SHY:        Serial.println("Shy"); break;
        default:                  Serial.println("Balanced"); break;
    }
    
    Serial.println("Element Multipliers:");
    for (int i = 0; i < ELEM_COUNT; i++) {
        Element e = static_cast<Element>(i);
        Serial.print("  ");
        Serial.print(getElementName(e));
        Serial.print(": ");
        Serial.print(elementMultipliers[i]);
        Serial.println("%");
    }
}