#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

// =============================================================================
// DUNGEON RUSH ALPHA - Shared Types
// =============================================================================
// All enums, structs, and type definitions used across the game
// =============================================================================

// === ELEMENTS (16 total) ===
// Player starts with 1 random element (not None), gains more from chests/bosses
enum Element {
    ELEM_NONE,          // "None" element - does nothing or silly effects
    ELEM_FIRE,
    ELEM_ICE,
    ELEM_WATER,
    ELEM_WOOD,
    ELEM_EARTH,
    ELEM_WIND,
    ELEM_LIGHTNING,
    ELEM_BUBBLE,
    ELEM_HAIR,
    ELEM_METAL,
    ELEM_TICKLE,
    ELEM_BONE,
    ELEM_PLASTIC,
    ELEM_LIGHT,
    ELEM_DARK,
    ELEM_COUNT          // Total count (16)
};

// === SPELL TYPES (8 total) ===
// Player always starts with MISSILE, gains more from chests/bosses
enum SpellType {
    TYPE_NONE,          // "None" type - does nothing or silly effects
    TYPE_MISSILE,       // Always unlocked at start
    TYPE_BALL,
    TYPE_EXPLOSION,
    TYPE_BEAM,
    TYPE_BOLT,
    TYPE_PUNCH,
    TYPE_WHIP,
    TYPE_COUNT          // Total count (8)
};

// === COMBAT RESULTS ===
enum class CombatResult {
    ONGOING,            // Combat continues
    SLAIN,              // Enemy HP reached 0
    BEFRIENDED,         // Enemy Kindness reached max
    PLAYER_DIED         // Player HP reached 0
};

// === ENEMY ACTIONS ===
enum class EnemyAction {
    ATTACK,             // Deal damage to player
    DEFEND_DAMAGE,      // Reduce incoming damage
    DEFEND_KINDNESS     // Reduce incoming kindness
};

// === ENEMY AI TYPES ===
enum class EnemyAI {
    BALANCED,           // Equal chance of all actions
    AGGRESSIVE,         // Prefers attacking
    DEFENSIVE,          // Prefers defending HP
    SHY                 // Prefers defending kindness
};

// === GAME STATES ===
enum class GameStateID {
    TITLE,
    DOOR_CHOICE,
    COMBAT,
    TREASURE,
    FLOOR_TRANSITION,
    VICTORY,
    GAME_OVER
};

// === ROOM TYPES ===
enum class RoomType {
    COMBAT,
    TREASURE,
    BOSS,
    EMPTY
};

// === HELPER FUNCTIONS ===

// Get element name as string
inline const char* getElementName(Element elem) {
    switch (elem) {
        case ELEM_NONE:      return "None";
        case ELEM_FIRE:      return "Fire";
        case ELEM_ICE:       return "Ice";
        case ELEM_WATER:     return "Water";
        case ELEM_WOOD:      return "Wood";
        case ELEM_EARTH:     return "Earth";
        case ELEM_WIND:      return "Wind";
        case ELEM_LIGHTNING: return "Lightning";
        case ELEM_BUBBLE:    return "Bubble";
        case ELEM_HAIR:      return "Hair";
        case ELEM_METAL:     return "Metal";
        case ELEM_TICKLE:    return "Tickle";
        case ELEM_BONE:      return "Bone";
        case ELEM_PLASTIC:   return "Plastic";
        case ELEM_LIGHT:     return "Light";
        case ELEM_DARK:      return "Dark";
        default:             return "Unknown";
    }
}

// Get spell type name as string
inline const char* getTypeName(SpellType type) {
    switch (type) {
        case TYPE_NONE:      return "None";
        case TYPE_MISSILE:   return "Missile";
        case TYPE_BALL:      return "Ball";
        case TYPE_EXPLOSION: return "Explosion";
        case TYPE_BEAM:      return "Beam";
        case TYPE_BOLT:      return "Bolt";
        case TYPE_PUNCH:     return "Punch";
        case TYPE_WHIP:      return "Whip";
        default:             return "Unknown";
    }
}

// Get element color for UI
inline uint16_t getElementColor(Element elem) {
    switch (elem) {
        case ELEM_NONE:      return 0x8410;  // Gray
        case ELEM_FIRE:      return 0xF800;  // Red
        case ELEM_ICE:       return 0x07FF;  // Cyan
        case ELEM_WATER:     return 0x001F;  // Blue
        case ELEM_WOOD:      return 0x07E0;  // Green
        case ELEM_EARTH:     return 0x8400;  // Brown
        case ELEM_WIND:      return 0xBFF7;  // Light cyan
        case ELEM_LIGHTNING: return 0xFFE0;  // Yellow
        case ELEM_BUBBLE:    return 0x7DFF;  // Light blue
        case ELEM_HAIR:      return 0xFBE0;  // Orange/tan
        case ELEM_METAL:     return 0xC618;  // Silver/gray
        case ELEM_TICKLE:    return 0xFE19;  // Pink
        case ELEM_BONE:      return 0xFFFF;  // White
        case ELEM_PLASTIC:   return 0xF81F;  // Magenta
        case ELEM_LIGHT:     return 0xFFF0;  // Bright yellow
        case ELEM_DARK:      return 0x8010;  // Purple
        default:             return 0xFFFF;  // White
    }
}

#endif // TYPES_H