#ifndef SPELL_MATRIX_H
#define SPELL_MATRIX_H

#include <Arduino.h>
#include "../Core/Types.h"

// =============================================================================
// SPELL MATRIX - Lookup table for Element + Type combinations
// =============================================================================
// 16 Elements × 8 Types = 128 unique spells
// All data stored in PROGMEM for memory efficiency
// =============================================================================

// Spell data structure
struct SpellData {
    uint8_t damage;         // Fixed damage value
    uint8_t kindness;       // Fixed kindness value
    const char* description; // Flavor text displayed on screen
};

class SpellMatrix {
public:
    // Get spell data for an Element + Type combination
    static SpellData getSpell(Element elem, SpellType type);
    
    // Get individual values (convenience methods)
    static uint8_t getDamage(Element elem, SpellType type);
    static uint8_t getKindness(Element elem, SpellType type);
    static const char* getDescription(Element elem, SpellType type);
    
    // Get combined spell name (e.g., "Fire Missile")
    static String getSpellName(Element elem, SpellType type);
    
    // Debug: Print spell info
    static void printSpell(Element elem, SpellType type);
};

#endif // SPELL_MATRIX_H