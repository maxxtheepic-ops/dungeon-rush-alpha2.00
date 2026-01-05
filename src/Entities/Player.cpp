#include "Player.h"

// =============================================================================
// PLAYER - Implementation
// =============================================================================

Player::Player() {
    reset();
}

// === HP MANAGEMENT ===

void Player::takeDamage(int amount) {
    // Apply defense reduction if defending
    if (defending) {
        amount = amount / 2;  // 50% damage reduction when defending
    }
    
    // Apply base defense
    int actualDamage = amount - defense;
    if (actualDamage < 1) actualDamage = 1;  // Minimum 1 damage
    
    currentHP -= actualDamage;
    if (currentHP < 0) currentHP = 0;
    
    DEBUG_PRINT("Player took ");
    DEBUG_PRINT(actualDamage);
    DEBUG_PRINT(" damage (");
    DEBUG_PRINT(currentHP);
    DEBUG_PRINT("/");
    DEBUG_PRINT(maxHP);
    DEBUG_PRINTLN(" HP)");
}

void Player::heal(int amount) {
    currentHP += amount;
    if (currentHP > maxHP) currentHP = maxHP;
    
    DEBUG_PRINT("Player healed ");
    DEBUG_PRINT(amount);
    DEBUG_PRINT(" (");
    DEBUG_PRINT(currentHP);
    DEBUG_PRINT("/");
    DEBUG_PRINT(maxHP);
    DEBUG_PRINTLN(" HP)");
}

// === ELEMENT MANAGEMENT ===

bool Player::hasElement(Element elem) const {
    for (Element e : unlockedElements) {
        if (e == elem) return true;
    }
    return false;
}

bool Player::unlockElement(Element elem) {
    if (hasElement(elem)) return false;  // Already owned
    
    unlockedElements.push_back(elem);
    DEBUG_PRINT("Player unlocked element: ");
    DEBUG_PRINTLN(getElementName(elem));
    return true;
}

Element Player::unlockRandomElement() {
    // Build list of locked elements (skip ELEM_NONE - that's always given at start)
    std::vector<Element> locked;
    for (int i = 1; i < ELEM_COUNT; i++) {  // Start at 1 to skip ELEM_NONE
        Element e = static_cast<Element>(i);
        if (!hasElement(e)) {
            locked.push_back(e);
        }
    }
    
    // If all elements owned, return invalid
    if (locked.empty()) {
        DEBUG_PRINTLN("Player already has all elements!");
        return ELEM_COUNT;
    }
    
    // Pick random locked element
    int idx = random(0, locked.size());
    Element chosen = locked[idx];
    unlockElement(chosen);
    return chosen;
}

Element Player::getElement(int index) const {
    if (index < 0 || index >= (int)unlockedElements.size()) {
        return ELEM_COUNT;  // Invalid
    }
    return unlockedElements[index];
}

// === TYPE MANAGEMENT ===

bool Player::hasType(SpellType type) const {
    for (SpellType t : unlockedTypes) {
        if (t == type) return true;
    }
    return false;
}

bool Player::unlockType(SpellType type) {
    if (hasType(type)) return false;  // Already owned
    
    unlockedTypes.push_back(type);
    DEBUG_PRINT("Player unlocked type: ");
    DEBUG_PRINTLN(getTypeName(type));
    return true;
}

SpellType Player::unlockRandomType() {
    // Build list of locked types (skip TYPE_NONE - that's always given at start)
    std::vector<SpellType> locked;
    for (int i = 1; i < TYPE_COUNT; i++) {  // Start at 1 to skip TYPE_NONE
        SpellType t = static_cast<SpellType>(i);
        if (!hasType(t)) {
            locked.push_back(t);
        }
    }
    
    // If all types owned, return invalid
    if (locked.empty()) {
        DEBUG_PRINTLN("Player already has all types!");
        return TYPE_COUNT;
    }
    
    // Pick random locked type
    int idx = random(0, locked.size());
    SpellType chosen = locked[idx];
    unlockType(chosen);
    return chosen;
}

SpellType Player::getType(int index) const {
    if (index < 0 || index >= (int)unlockedTypes.size()) {
        return TYPE_COUNT;  // Invalid
    }
    return unlockedTypes[index];
}

// === INITIALIZATION ===

void Player::initStarterAbilities() {
    // Clear any existing abilities
    unlockedElements.clear();
    unlockedTypes.clear();
    
    // Always start with None element (silly/flavor spells)
    unlockElement(ELEM_NONE);
    
    // Always start with None type (silly/flavor spells)
    unlockType(TYPE_NONE);
    
    // Always start with Missile type
    unlockType(TYPE_MISSILE);
    
    // Random starting element (from ELEM_FIRE to ELEM_DARK)
    Element startElem = static_cast<Element>(random(1, ELEM_COUNT));
    unlockElement(startElem);
    
    DEBUG_PRINTLN("Player starter abilities initialized:");
    DEBUG_PRINT("  Elements: None, ");
    DEBUG_PRINTLN(getElementName(startElem));
    DEBUG_PRINTLN("  Types: None, Missile");
}

void Player::reset() {
    // Reset stats to starting values
    maxHP = PLAYER_START_MAX_HP;
    currentHP = maxHP;
    attack = PLAYER_START_ATTACK;
    defense = PLAYER_START_DEFENSE;
    
    // Clear abilities
    unlockedElements.clear();
    unlockedTypes.clear();
    
    // Reset combat state
    defending = false;
    
    DEBUG_PRINTLN("Player reset to starting state");
}

// === DEBUG ===

void Player::printStatus() const {
    Serial.println("=== PLAYER STATUS ===");
    Serial.print("HP: ");
    Serial.print(currentHP);
    Serial.print("/");
    Serial.println(maxHP);
    Serial.print("ATK: ");
    Serial.print(attack);
    Serial.print(" DEF: ");
    Serial.println(defense);
    
    Serial.print("Elements (");
    Serial.print(unlockedElements.size());
    Serial.print("): ");
    for (size_t i = 0; i < unlockedElements.size(); i++) {
        if (i > 0) Serial.print(", ");
        Serial.print(getElementName(unlockedElements[i]));
    }
    Serial.println();
    
    Serial.print("Types (");
    Serial.print(unlockedTypes.size());
    Serial.print("): ");
    for (size_t i = 0; i < unlockedTypes.size(); i++) {
        if (i > 0) Serial.print(", ");
        Serial.print(getTypeName(unlockedTypes[i]));
    }
    Serial.println();
}