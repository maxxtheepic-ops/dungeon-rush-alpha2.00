#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include <vector>
#include "../Core/Config.h"
#include "../Core/Types.h"

// =============================================================================
// PLAYER - Main character with HP and spell unlocks
// =============================================================================

class Player {
private:
    // === STATS ===
    int currentHP;
    int maxHP;
    int attack;
    int defense;
    
    // === UNLOCKED ABILITIES ===
    std::vector<Element> unlockedElements;
    std::vector<SpellType> unlockedTypes;
    
    // === COMBAT STATE ===
    bool defending;

public:
    Player();
    
    // === STAT GETTERS ===
    int getCurrentHP() const { return currentHP; }
    int getMaxHP() const { return maxHP; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    
    // === HP MANAGEMENT ===
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const { return currentHP > 0; }
    
    // === DEFENSE ===
    void setDefending(bool val) { defending = val; }
    bool isDefending() const { return defending; }
    void resetDefense() { defending = false; }
    
    // === ELEMENT MANAGEMENT ===
    const std::vector<Element>& getUnlockedElements() const { return unlockedElements; }
    int getElementCount() const { return unlockedElements.size(); }
    bool hasElement(Element elem) const;
    bool unlockElement(Element elem);       // Returns true if newly unlocked
    Element unlockRandomElement();          // Returns element unlocked (or ELEM_COUNT if all owned)
    Element getElement(int index) const;    // Get element by index
    
    // === TYPE MANAGEMENT ===
    const std::vector<SpellType>& getUnlockedTypes() const { return unlockedTypes; }
    int getTypeCount() const { return unlockedTypes.size(); }
    bool hasType(SpellType type) const;
    bool unlockType(SpellType type);        // Returns true if newly unlocked
    SpellType unlockRandomType();           // Returns type unlocked (or TYPE_COUNT if all owned)
    SpellType getType(int index) const;     // Get type by index
    
    // === INITIALIZATION ===
    void initStarterAbilities();            // Random element + Missile type
    void reset();                           // Full reset for new run
    
    // === DEBUG ===
    void printStatus() const;
};

#endif // PLAYER_H