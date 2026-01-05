#include "SpellMatrix.h"
#include <pgmspace.h>

// =============================================================================
// SPELL MATRIX - All 128 spell definitions
// =============================================================================
// Format: { damage, kindness, "description" }
// Organized as [Element][Type] - 16 elements × 8 types
// =============================================================================

// === SPELL DESCRIPTIONS (stored in PROGMEM) ===

// --- NONE Element ---
static const char DESC_NONE_NONE[] PROGMEM = "you sit there doing nothing";
static const char DESC_NONE_MISSILE[] PROGMEM = "nothing";
static const char DESC_NONE_BALL[] PROGMEM = "you play ball with the opponent";
static const char DESC_NONE_EXPLOSION[] PROGMEM = "nothing";
static const char DESC_NONE_BEAM[] PROGMEM = "nothing";
static const char DESC_NONE_BOLT[] PROGMEM = "you should get a nut for that";
static const char DESC_NONE_PUNCH[] PROGMEM = "you try your best but you aren't a boxer";
static const char DESC_NONE_WHIP[] PROGMEM = "you spanked the opponent";

// --- FIRE Element ---
static const char DESC_FIRE_NONE[] PROGMEM = "you and the opponent enjoy the warmth of a fire you lit";
static const char DESC_FIRE_MISSILE[] PROGMEM = "a fire missile whizs forward";
static const char DESC_FIRE_BALL[] PROGMEM = "a powerful fireball shoots forward";
static const char DESC_FIRE_EXPLOSION[] PROGMEM = "the opponent erupts in fiery explosions";
static const char DESC_FIRE_BEAM[] PROGMEM = "you shoot a fire beam from your pointer finger";
static const char DESC_FIRE_BOLT[] PROGMEM = "nothing";
static const char DESC_FIRE_PUNCH[] PROGMEM = "you rain down blazing fists";
static const char DESC_FIRE_WHIP[] PROGMEM = "nothing";

// --- ICE Element ---
static const char DESC_ICE_NONE[] PROGMEM = "you suck on an ice cube yumm";
static const char DESC_ICE_MISSILE[] PROGMEM = "an ice missile whizs forward";
static const char DESC_ICE_BALL[] PROGMEM = "you get in a snowball fight with the opponent";
static const char DESC_ICE_EXPLOSION[] PROGMEM = "nothing";
static const char DESC_ICE_BEAM[] PROGMEM = "ice grows on the opponent and theyre frozen solid";
static const char DESC_ICE_BOLT[] PROGMEM = "a bolt of ice launches forward";
static const char DESC_ICE_PUNCH[] PROGMEM = "you rain down freezing fists";
static const char DESC_ICE_WHIP[] PROGMEM = "you lash the opponent with links of ice";

// --- WATER Element ---
static const char DESC_WATER_NONE[] PROGMEM = "its important to hydrate!";
static const char DESC_WATER_MISSILE[] PROGMEM = "a water missile whizs forward";
static const char DESC_WATER_BALL[] PROGMEM = "the opponent suffocates in an orb of water";
static const char DESC_WATER_EXPLOSION[] PROGMEM = "you and the opponent splash each other with water";
static const char DESC_WATER_BEAM[] PROGMEM = "a water jet bursts from your palm";
static const char DESC_WATER_BOLT[] PROGMEM = "the opponent is hit in the face by a column of water";
static const char DESC_WATER_PUNCH[] PROGMEM = "your punches are softened by water";
static const char DESC_WATER_WHIP[] PROGMEM = "tails of water slash and whip the opponent";

// --- WOOD Element ---
static const char DESC_WOOD_NONE[] PROGMEM = "what a nice bench!";
static const char DESC_WOOD_MISSILE[] PROGMEM = "a wood missile whizs forward";
static const char DESC_WOOD_BALL[] PROGMEM = "a large ball of wood slams into the opponent";
static const char DESC_WOOD_EXPLOSION[] PROGMEM = "flowers explode at the opponents feet";
static const char DESC_WOOD_BEAM[] PROGMEM = "you give the opponent a sunflower";
static const char DESC_WOOD_BOLT[] PROGMEM = "a sharpened log hurls toward the opponent";
static const char DESC_WOOD_PUNCH[] PROGMEM = "you smack your opponent with a two by four";
static const char DESC_WOOD_WHIP[] PROGMEM = "you entangle and whip your opponent with greenery";

// --- EARTH Element ---
static const char DESC_EARTH_NONE[] PROGMEM = "mmm dirt";
static const char DESC_EARTH_MISSILE[] PROGMEM = "an earth missile whizs forward";
static const char DESC_EARTH_BALL[] PROGMEM = "a ball of mud splats in the opponents face";
static const char DESC_EARTH_EXPLOSION[] PROGMEM = "the earth erupts around the opponent";
static const char DESC_EARTH_BEAM[] PROGMEM = "nothing";
static const char DESC_EARTH_BOLT[] PROGMEM = "a pillar of rock soars toward the opponent";
static const char DESC_EARTH_PUNCH[] PROGMEM = "you and the opponent wrestle in the mud";
static const char DESC_EARTH_WHIP[] PROGMEM = "nothing";

// --- WIND Element ---
static const char DESC_WIND_NONE[] PROGMEM = "you let out a toot whoops";
static const char DESC_WIND_MISSILE[] PROGMEM = "a wind missile whizs forward";
static const char DESC_WIND_BALL[] PROGMEM = "the air around the opponent is quickly pressurized";
static const char DESC_WIND_EXPLOSION[] PROGMEM = "a burst of air launches the opponent high into the air";
static const char DESC_WIND_BEAM[] PROGMEM = "you fart a powerful stream of air toward the opponent";
static const char DESC_WIND_BOLT[] PROGMEM = "nothing";
static const char DESC_WIND_PUNCH[] PROGMEM = "you play the air guitar for the opponent";
static const char DESC_WIND_WHIP[] PROGMEM = "wind whips the opponent";

// --- LIGHTNING Element ---
static const char DESC_LIGHTNING_NONE[] PROGMEM = "lightning strikes in the distance";
static const char DESC_LIGHTNING_MISSILE[] PROGMEM = "a lightning missile whizs forward";
static const char DESC_LIGHTNING_BALL[] PROGMEM = "a lightning ball whizzes toward the opponent";
static const char DESC_LIGHTNING_EXPLOSION[] PROGMEM = "nothing";
static const char DESC_LIGHTNING_BEAM[] PROGMEM = "you put on a show shooting lightning from your fingertips";
static const char DESC_LIGHTNING_BOLT[] PROGMEM = "a powerful lightning bolt strikes the opponent";
static const char DESC_LIGHTNING_PUNCH[] PROGMEM = "the opponent is shocked with every punch";
static const char DESC_LIGHTNING_WHIP[] PROGMEM = "you wield a lightning bolt to whip the opponent";

// --- BUBBLE Element ---
static const char DESC_BUBBLE_NONE[] PROGMEM = "you blow some bubbles the opponent is amused";
static const char DESC_BUBBLE_MISSILE[] PROGMEM = "a bubble missile whizs forward";
static const char DESC_BUBBLE_BALL[] PROGMEM = "you blow a giant bubble";
static const char DESC_BUBBLE_EXPLOSION[] PROGMEM = "bubbles explode and cover the floor causing the opponent to slip";
static const char DESC_BUBBLE_BEAM[] PROGMEM = "you shoot a bubble beam toward the opponent";
static const char DESC_BUBBLE_BOLT[] PROGMEM = "nothing";
static const char DESC_BUBBLE_PUNCH[] PROGMEM = "you scrub the opponent";
static const char DESC_BUBBLE_WHIP[] PROGMEM = "nothing";

// --- HAIR Element ---
static const char DESC_HAIR_NONE[] PROGMEM = "you spit a hair out bleh";
static const char DESC_HAIR_MISSILE[] PROGMEM = "a hair missile whizs forward";
static const char DESC_HAIR_BALL[] PROGMEM = "the opponent chokes on a hair ball";
static const char DESC_HAIR_EXPLOSION[] PROGMEM = "the opponent has a bad hair day";
static const char DESC_HAIR_BEAM[] PROGMEM = "you launch needle like hairs toward the opponent";
static const char DESC_HAIR_BOLT[] PROGMEM = "you style your hair into a mohawk";
static const char DESC_HAIR_PUNCH[] PROGMEM = "you style your opponents hair making them feel sexy";
static const char DESC_HAIR_WHIP[] PROGMEM = "you whip your hair back and forth";

// --- METAL Element ---
static const char DESC_METAL_NONE[] PROGMEM = "cold to the touch hm";
static const char DESC_METAL_MISSILE[] PROGMEM = "a metal missile whizs forward";
static const char DESC_METAL_BALL[] PROGMEM = "you fire a cannon ball at your opponent";
static const char DESC_METAL_EXPLOSION[] PROGMEM = "a car explodes right next to the opponent";
static const char DESC_METAL_BEAM[] PROGMEM = "an i beam falls onto the opponent";
static const char DESC_METAL_BOLT[] PROGMEM = "you fix the opponents car for them";
static const char DESC_METAL_PUNCH[] PROGMEM = "you smack the opponent with brass knuckles";
static const char DESC_METAL_WHIP[] PROGMEM = "nothing";

// --- TICKLE Element ---
static const char DESC_TICKLE_NONE[] PROGMEM = "you tickle the opponent they giggle";
static const char DESC_TICKLE_MISSILE[] PROGMEM = "a tickle missile whizs forward";
static const char DESC_TICKLE_BALL[] PROGMEM = "you tickle the opponent so hard they roll into a ball";
static const char DESC_TICKLE_EXPLOSION[] PROGMEM = "hands explode from the ground and start grabbing the opponent";
static const char DESC_TICKLE_BEAM[] PROGMEM = "you tickle the opponent with a feather";
static const char DESC_TICKLE_BOLT[] PROGMEM = "nothing";
static const char DESC_TICKLE_PUNCH[] PROGMEM = "tickling with fists isn't very effective";
static const char DESC_TICKLE_WHIP[] PROGMEM = "the opponent is tied up and tickled";

// --- BONE Element ---
static const char DESC_BONE_NONE[] PROGMEM = "nope don't even think about it";
static const char DESC_BONE_MISSILE[] PROGMEM = "a bone missile whizs forward";
static const char DESC_BONE_BALL[] PROGMEM = "you bowl a ball into your opponent";
static const char DESC_BONE_EXPLOSION[] PROGMEM = "the opponents bones explode and shatter";
static const char DESC_BONE_BEAM[] PROGMEM = "you carve a figure out of bone for the opponent";
static const char DESC_BONE_BOLT[] PROGMEM = "you launch a sharpened femur at the opponent";
static const char DESC_BONE_PUNCH[] PROGMEM = "you precisely strike the opponent breaking their bones";
static const char DESC_BONE_WHIP[] PROGMEM = "you use a skeletal spine to lash the opponent";

// --- PLASTIC Element ---
static const char DESC_PLASTIC_NONE[] PROGMEM = "you and the opponent play with a toy dump truck nice";
static const char DESC_PLASTIC_MISSILE[] PROGMEM = "a plastic missile whizs forward";
static const char DESC_PLASTIC_BALL[] PROGMEM = "you and the opponent play with a super bouncy ball";
static const char DESC_PLASTIC_EXPLOSION[] PROGMEM = "legos explode on the floor making it hard to move";
static const char DESC_PLASTIC_BEAM[] PROGMEM = "nothing";
static const char DESC_PLASTIC_BOLT[] PROGMEM = "you and your opponent have a nerf battle";
static const char DESC_PLASTIC_PUNCH[] PROGMEM = "you and the opponent fight with toy hulk fists";
static const char DESC_PLASTIC_WHIP[] PROGMEM = "your opponent is restrained with zip ties";

// --- LIGHT Element ---
static const char DESC_LIGHT_NONE[] PROGMEM = "you temporarily blind yourself with a flashlight";
static const char DESC_LIGHT_MISSILE[] PROGMEM = "a light missile whizs forward";
static const char DESC_LIGHT_BALL[] PROGMEM = "you light the room with a calming glowing orb";
static const char DESC_LIGHT_EXPLOSION[] PROGMEM = "an explosion of light blinds the opponent";
static const char DESC_LIGHT_BEAM[] PROGMEM = "a beam of light streams down from the heavens";
static const char DESC_LIGHT_BOLT[] PROGMEM = "you and the opponent play with a high power laser pointer";
static const char DESC_LIGHT_PUNCH[] PROGMEM = "you poke the opponents eyes";
static const char DESC_LIGHT_WHIP[] PROGMEM = "nothing";

// --- DARK Element ---
static const char DESC_DARK_NONE[] PROGMEM = "you brood maliciously in the corner";
static const char DESC_DARK_MISSILE[] PROGMEM = "a dark missile whizs forward";
static const char DESC_DARK_BALL[] PROGMEM = "the opponent is cast into absolute darkness";
static const char DESC_DARK_EXPLOSION[] PROGMEM = "darkness engulfs the opponent draining them of life";
static const char DESC_DARK_BEAM[] PROGMEM = "a shadowy beam blasts the opponent";
static const char DESC_DARK_BOLT[] PROGMEM = "a black bolt of lightning strikes the opponent";
static const char DESC_DARK_PUNCH[] PROGMEM = "nothing";
static const char DESC_DARK_WHIP[] PROGMEM = "you nut tap the opponent";


// =============================================================================
// SPELL DATA MATRIX [Element][Type] = {damage, kindness, description}
// =============================================================================

static const SpellData SPELL_MATRIX[ELEM_COUNT][TYPE_COUNT] PROGMEM = {
    // ELEM_NONE
    {
        { 0,  0, DESC_NONE_NONE},
        { 0,  0, DESC_NONE_MISSILE},
        { 0, 20, DESC_NONE_BALL},
        { 0,  0, DESC_NONE_EXPLOSION},
        { 0,  0, DESC_NONE_BEAM},
        { 0,  0, DESC_NONE_BOLT},
        { 2,  0, DESC_NONE_PUNCH},
        { 0, 10, DESC_NONE_WHIP}
    },
    // ELEM_FIRE
    {
        { 0,  5, DESC_FIRE_NONE},
        {10,  0, DESC_FIRE_MISSILE},
        {20,  0, DESC_FIRE_BALL},
        {40,  0, DESC_FIRE_EXPLOSION},
        {15,  0, DESC_FIRE_BEAM},
        { 0,  0, DESC_FIRE_BOLT},
        {15,  0, DESC_FIRE_PUNCH},
        { 0,  0, DESC_FIRE_WHIP}
    },
    // ELEM_ICE
    {
        { 0,  0, DESC_ICE_NONE},
        {10,  0, DESC_ICE_MISSILE},
        { 0, 15, DESC_ICE_BALL},
        { 0,  0, DESC_ICE_EXPLOSION},
        {40,  0, DESC_ICE_BEAM},
        {15,  0, DESC_ICE_BOLT},
        {15,  0, DESC_ICE_PUNCH},
        {30,  0, DESC_ICE_WHIP}
    },
    // ELEM_WATER
    {
        { 0,  0, DESC_WATER_NONE},
        {10,  0, DESC_WATER_MISSILE},
        {35,  0, DESC_WATER_BALL},
        { 0, 15, DESC_WATER_EXPLOSION},
        {30,  0, DESC_WATER_BEAM},
        {10,  0, DESC_WATER_BOLT},
        { 1,  0, DESC_WATER_PUNCH},
        {40,  0, DESC_WATER_WHIP}
    },
    // ELEM_WOOD
    {
        { 0,  0, DESC_WOOD_NONE},
        {10,  0, DESC_WOOD_MISSILE},
        {25,  0, DESC_WOOD_BALL},
        { 0, 30, DESC_WOOD_EXPLOSION},
        { 0, 15, DESC_WOOD_BEAM},
        {30,  0, DESC_WOOD_BOLT},
        {15,  0, DESC_WOOD_PUNCH},
        {40,  0, DESC_WOOD_WHIP}
    },
    // ELEM_EARTH
    {
        { 0,  0, DESC_EARTH_NONE},
        {10,  0, DESC_EARTH_MISSILE},
        { 5,  0, DESC_EARTH_BALL},
        {40,  0, DESC_EARTH_EXPLOSION},
        { 0,  0, DESC_EARTH_BEAM},
        {20,  0, DESC_EARTH_BOLT},
        { 0, 10, DESC_EARTH_PUNCH},
        { 0,  0, DESC_EARTH_WHIP}
    },
    // ELEM_WIND
    {
        { 0,  0, DESC_WIND_NONE},
        {10,  0, DESC_WIND_MISSILE},
        {40,  0, DESC_WIND_BALL},
        {30,  0, DESC_WIND_EXPLOSION},
        {20,  0, DESC_WIND_BEAM},
        { 0,  0, DESC_WIND_BOLT},
        { 0, 20, DESC_WIND_PUNCH},
        {20,  0, DESC_WIND_WHIP}
    },
    // ELEM_LIGHTNING
    {
        { 0,  0, DESC_LIGHTNING_NONE},
        {10,  0, DESC_LIGHTNING_MISSILE},
        {30,  0, DESC_LIGHTNING_BALL},
        { 0,  0, DESC_LIGHTNING_EXPLOSION},
        { 0, 20, DESC_LIGHTNING_BEAM},
        {40,  0, DESC_LIGHTNING_BOLT},
        {20,  0, DESC_LIGHTNING_PUNCH},
        {25,  0, DESC_LIGHTNING_WHIP}
    },
    // ELEM_BUBBLE
    {
        { 0, 30, DESC_BUBBLE_NONE},
        {10,  0, DESC_BUBBLE_MISSILE},
        { 0, 20, DESC_BUBBLE_BALL},
        { 5,  0, DESC_BUBBLE_EXPLOSION},
        {30,  0, DESC_BUBBLE_BEAM},
        { 0,  0, DESC_BUBBLE_BOLT},
        { 0, 20, DESC_BUBBLE_PUNCH},
        { 0,  0, DESC_BUBBLE_WHIP}
    },
    // ELEM_HAIR
    {
        { 0,  0, DESC_HAIR_NONE},
        {10,  0, DESC_HAIR_MISSILE},
        {20,  0, DESC_HAIR_BALL},
        { 5,  0, DESC_HAIR_EXPLOSION},
        {15,  0, DESC_HAIR_BEAM},
        { 0,  0, DESC_HAIR_BOLT},
        { 0, 25, DESC_HAIR_PUNCH},
        { 0, 30, DESC_HAIR_WHIP}
    },
    // ELEM_METAL
    {
        { 0,  0, DESC_METAL_NONE},
        {10,  0, DESC_METAL_MISSILE},
        {30,  0, DESC_METAL_BALL},
        {20,  0, DESC_METAL_EXPLOSION},
        {25,  0, DESC_METAL_BEAM},
        { 0, 20, DESC_METAL_BOLT},
        {15,  0, DESC_METAL_PUNCH},
        { 0,  0, DESC_METAL_WHIP}
    },
    // ELEM_TICKLE
    {
        { 0, 10, DESC_TICKLE_NONE},
        {10,  0, DESC_TICKLE_MISSILE},
        { 0, 30, DESC_TICKLE_BALL},
        {15,  0, DESC_TICKLE_EXPLOSION},
        { 0, 10, DESC_TICKLE_BEAM},
        { 0,  0, DESC_TICKLE_BOLT},
        { 0,  5, DESC_TICKLE_PUNCH},
        {30,  0, DESC_TICKLE_WHIP}
    },
    // ELEM_BONE
    {
        { 0,  0, DESC_BONE_NONE},
        {10,  0, DESC_BONE_MISSILE},
        {20,  0, DESC_BONE_BALL},
        {40,  0, DESC_BONE_EXPLOSION},
        { 0, 15, DESC_BONE_BEAM},
        {20,  0, DESC_BONE_BOLT},
        {30,  0, DESC_BONE_PUNCH},
        {15,  0, DESC_BONE_WHIP}
    },
    // ELEM_PLASTIC
    {
        { 0, 10, DESC_PLASTIC_NONE},
        {10,  0, DESC_PLASTIC_MISSILE},
        { 0, 30, DESC_PLASTIC_BALL},
        { 5,  0, DESC_PLASTIC_EXPLOSION},
        { 0,  0, DESC_PLASTIC_BEAM},
        { 0, 30, DESC_PLASTIC_BOLT},
        { 0, 30, DESC_PLASTIC_PUNCH},
        {30,  0, DESC_PLASTIC_WHIP}
    },
    // ELEM_LIGHT
    {
        { 0,  0, DESC_LIGHT_NONE},
        {10,  0, DESC_LIGHT_MISSILE},
        { 0,  5, DESC_LIGHT_BALL},
        {10,  0, DESC_LIGHT_EXPLOSION},
        {35,  0, DESC_LIGHT_BEAM},
        { 0, 15, DESC_LIGHT_BOLT},
        {20,  0, DESC_LIGHT_PUNCH},
        { 0,  0, DESC_LIGHT_WHIP}
    },
    // ELEM_DARK
    {
        { 0,  0, DESC_DARK_NONE},
        {10,  0, DESC_DARK_MISSILE},
        {20,  0, DESC_DARK_BALL},
        {40,  0, DESC_DARK_EXPLOSION},
        {25,  0, DESC_DARK_BEAM},
        {35,  0, DESC_DARK_BOLT},
        { 0,  0, DESC_DARK_PUNCH},
        { 5,  0, DESC_DARK_WHIP}
    }
};


// =============================================================================
// IMPLEMENTATION
// =============================================================================

SpellData SpellMatrix::getSpell(Element elem, SpellType type) {
    SpellData result;
    
    if (elem >= ELEM_COUNT || type >= TYPE_COUNT) {
        // Invalid - return empty spell
        result.damage = 0;
        result.kindness = 0;
        result.description = "invalid spell";
        return result;
    }
    
    // Read from PROGMEM
    memcpy_P(&result, &SPELL_MATRIX[elem][type], sizeof(SpellData));
    return result;
}

uint8_t SpellMatrix::getDamage(Element elem, SpellType type) {
    if (elem >= ELEM_COUNT || type >= TYPE_COUNT) return 0;
    return pgm_read_byte(&SPELL_MATRIX[elem][type].damage);
}

uint8_t SpellMatrix::getKindness(Element elem, SpellType type) {
    if (elem >= ELEM_COUNT || type >= TYPE_COUNT) return 0;
    return pgm_read_byte(&SPELL_MATRIX[elem][type].kindness);
}

const char* SpellMatrix::getDescription(Element elem, SpellType type) {
    if (elem >= ELEM_COUNT || type >= TYPE_COUNT) return "invalid";
    
    // Read pointer from PROGMEM
    const char* ptr;
    memcpy_P(&ptr, &SPELL_MATRIX[elem][type].description, sizeof(ptr));
    return ptr;
}

String SpellMatrix::getSpellName(Element elem, SpellType type) {
    // Special case for None+None
    if (elem == ELEM_NONE && type == TYPE_NONE) {
        return "Nothing";
    }
    
    String name = "";
    
    // Add element name (skip if None)
    if (elem != ELEM_NONE) {
        name += getElementName(elem);
        name += " ";
    }
    
    // Add type name (skip if None)
    if (type != TYPE_NONE) {
        name += getTypeName(type);
    } else {
        // Remove trailing space if type is None
        name.trim();
    }
    
    return name;
}

void SpellMatrix::printSpell(Element elem, SpellType type) {
    SpellData spell = getSpell(elem, type);
    
    Serial.print("Spell: ");
    Serial.println(getSpellName(elem, type));
    Serial.print("  Damage: ");
    Serial.println(spell.damage);
    Serial.print("  Kindness: ");
    Serial.println(spell.kindness);
    Serial.print("  Description: ");
    
    // Read description from PROGMEM
    char buffer[128];
    strncpy_P(buffer, spell.description, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    Serial.println(buffer);
}