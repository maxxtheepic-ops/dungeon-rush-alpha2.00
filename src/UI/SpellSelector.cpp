#include "SpellSelector.h"

// =============================================================================
// SPELL SELECTOR - Implementation
// =============================================================================

SpellSelector::SpellSelector(Display* disp) : display(disp), player(nullptr) {
    startY = 0;
    phase = SelectorPhase::SELECT_ELEMENT;
    elementIndex = 0;
    typeIndex = 0;
    lastElementIndex = -1;
    lastTypeIndex = -1;
    lastPhase = SelectorPhase::SELECT_ELEMENT;
}

// =============================================================================
// LIFECYCLE
// =============================================================================

void SpellSelector::init(const Player* p, int yPosition) {
    player = p;
    startY = yPosition;
    reset();
}

void SpellSelector::reset() {
    phase = SelectorPhase::SELECT_ELEMENT;
    elementIndex = 0;
    typeIndex = 0;
    lastElementIndex = -1;
    lastTypeIndex = -1;
    lastPhase = SelectorPhase::SELECT_ELEMENT;
}

// =============================================================================
// INPUT HANDLING
// =============================================================================

void SpellSelector::moveUp() {
    if (phase == SelectorPhase::SELECT_ELEMENT) {
        elementIndex--;
        if (elementIndex < 0) {
            elementIndex = player->getElementCount() - 1;
        }
    } else if (phase == SelectorPhase::SELECT_TYPE) {
        typeIndex--;
        if (typeIndex < 0) {
            typeIndex = player->getTypeCount() - 1;
        }
    }
}

void SpellSelector::moveDown() {
    if (phase == SelectorPhase::SELECT_ELEMENT) {
        elementIndex++;
        if (elementIndex >= player->getElementCount()) {
            elementIndex = 0;
        }
    } else if (phase == SelectorPhase::SELECT_TYPE) {
        typeIndex++;
        if (typeIndex >= player->getTypeCount()) {
            typeIndex = 0;
        }
    }
}

bool SpellSelector::confirm() {
    if (phase == SelectorPhase::SELECT_ELEMENT) {
        phase = SelectorPhase::SELECT_TYPE;
        typeIndex = 0;
        return false;
    } else if (phase == SelectorPhase::SELECT_TYPE) {
        phase = SelectorPhase::READY_TO_CAST;
        return true;  // Ready to cast!
    }
    return false;
}

bool SpellSelector::cancel() {
    if (phase == SelectorPhase::SELECT_TYPE) {
        phase = SelectorPhase::SELECT_ELEMENT;
        return false;
    } else if (phase == SelectorPhase::SELECT_ELEMENT) {
        return true;  // Cancelled out completely
    }
    return false;
}

// =============================================================================
// DRAWING
// =============================================================================

void SpellSelector::drawFull() {
    if (!player) return;
    
    // Clear selection area
    display->clearArea(0, startY, SCREEN_WIDTH, getBottomY() - startY, COLOR_BG);
    
    // Labels
    display->drawText("Element:", 10, startY, COLOR_WHITE, 1);
    display->drawText("Type:", 10, startY + TYPE_ROW_Y_OFFSET - 15, COLOR_WHITE, 1);
    display->drawText("Spell:", 10, startY + PREVIEW_Y_OFFSET - 5, COLOR_WHITE, 1);
    
    // Draw rows
    drawElementRow();
    drawTypeRow();
    drawPhaseIndicator();
    drawSpellPreview();
    
    // Update cache
    lastElementIndex = elementIndex;
    lastTypeIndex = typeIndex;
    lastPhase = phase;
}

void SpellSelector::update() {
    if (!player) return;
    
    bool needsPreviewUpdate = false;
    
    // Update element cursor if changed
    if (lastElementIndex != elementIndex || lastPhase != phase) {
        // Clear old cursor
        if (lastElementIndex >= 0 && lastElementIndex < player->getElementCount()) {
            clearCursor(getElementX(lastElementIndex), startY + ELEMENT_ROW_Y_OFFSET);
        }
        // Draw new cursor if in element phase
        if (phase == SelectorPhase::SELECT_ELEMENT) {
            drawCursor(getElementX(elementIndex), startY + ELEMENT_ROW_Y_OFFSET);
        }
        lastElementIndex = elementIndex;
        needsPreviewUpdate = true;
    }
    
    // Update type cursor if changed
    if (lastTypeIndex != typeIndex || lastPhase != phase) {
        // Clear old cursor
        if (lastTypeIndex >= 0 && lastTypeIndex < player->getTypeCount()) {
            clearCursor(getTypeX(lastTypeIndex), startY + TYPE_ROW_Y_OFFSET);
        }
        // Draw new cursor if in type phase
        if (phase == SelectorPhase::SELECT_TYPE) {
            drawCursor(getTypeX(typeIndex), startY + TYPE_ROW_Y_OFFSET);
        }
        lastTypeIndex = typeIndex;
        needsPreviewUpdate = true;
    }
    
    // Update phase indicator if changed
    if (lastPhase != phase) {
        drawPhaseIndicator();
        lastPhase = phase;
        needsPreviewUpdate = true;
    }
    
    // Update spell preview if anything changed
    if (needsPreviewUpdate) {
        drawSpellPreview();
    }
}

void SpellSelector::drawElementRow() {
    int y = startY + ELEMENT_ROW_Y_OFFSET;
    
    for (int i = 0; i < player->getElementCount(); i++) {
        Element e = player->getElement(i);
        int x = getElementX(i);
        
        // Draw element abbreviation (3 chars)
        String abbrev = String(getElementName(e)).substring(0, 3);
        display->drawText(abbrev, x, y, getElementColor(e), 1);
        
        // Draw cursor if selected and in element phase
        if (phase == SelectorPhase::SELECT_ELEMENT && i == elementIndex) {
            drawCursor(x, y);
        }
    }
}

void SpellSelector::drawTypeRow() {
    int y = startY + TYPE_ROW_Y_OFFSET;
    
    for (int i = 0; i < player->getTypeCount(); i++) {
        SpellType t = player->getType(i);
        int x = getTypeX(i);
        
        // Draw type abbreviation (3 chars)
        String abbrev = String(getTypeName(t)).substring(0, 3);
        display->drawText(abbrev, x, y, COLOR_WHITE, 1);
        
        // Draw cursor if selected and in type phase
        if (phase == SelectorPhase::SELECT_TYPE && i == typeIndex) {
            drawCursor(x, y);
        }
    }
}

void SpellSelector::drawPhaseIndicator() {
    // Clear indicator areas
    display->clearArea(90, startY, 30, 12, COLOR_BG);
    display->clearArea(90, startY + TYPE_ROW_Y_OFFSET - 15, 30, 12, COLOR_BG);
    
    // Draw arrow next to current phase
    if (phase == SelectorPhase::SELECT_ELEMENT) {
        display->drawText("<", 90, startY, COLOR_CURSOR, 1);
    } else {
        display->drawText("<", 90, startY + TYPE_ROW_Y_OFFSET - 15, COLOR_CURSOR, 1);
    }
}

void SpellSelector::drawCursor(int x, int y) {
    display->drawRect(x - 3, y - 2, ITEM_WIDTH, ITEM_HEIGHT, COLOR_CURSOR);
}

void SpellSelector::clearCursor(int x, int y) {
    display->drawRect(x - 3, y - 2, ITEM_WIDTH, ITEM_HEIGHT, COLOR_BG);
}

int SpellSelector::getElementX(int index) const {
    return 15 + (index * ITEM_WIDTH);
}

int SpellSelector::getTypeX(int index) const {
    return 15 + (index * ITEM_WIDTH);
}

void SpellSelector::drawSpellPreview() {
    int previewY = startY + PREVIEW_Y_OFFSET;
    
    // Clear preview area
    display->clearArea(10, previewY + 10, SCREEN_WIDTH - 20, 60, COLOR_BG);
    
    Element elem = getSelectedElement();
    SpellType type = getSelectedType();
    SpellData spell = SpellMatrix::getSpell(elem, type);
    String spellName = SpellMatrix::getSpellName(elem, type);
    
    // Spell name in element color
    display->drawText(spellName, 10, previewY + 12, getElementColor(elem), 1);
    
    // Stats line
    String stats = "DMG:" + String(spell.damage) + " KIND:" + String(spell.kindness);
    display->drawText(stats, 10, previewY + 26, COLOR_WHITE, 1);
    
    // Description (with word wrap)
    char descBuffer[80];
    strncpy_P(descBuffer, spell.description, 79);
    descBuffer[79] = '\0';
    
    String desc = String(descBuffer);
    
    // Simple word wrap at ~28 chars for small font
    if (desc.length() > 28) {
        int splitPos = desc.lastIndexOf(' ', 28);
        if (splitPos < 10) splitPos = 28;
        
        display->drawText(desc.substring(0, splitPos), 10, previewY + 42, COLOR_LIGHT_GRAY, 1);
        
        String remainder = desc.substring(splitPos + 1);
        if (remainder.length() > 28) {
            remainder = remainder.substring(0, 25) + "...";
        }
        display->drawText(remainder, 10, previewY + 54, COLOR_LIGHT_GRAY, 1);
    } else {
        display->drawText(desc, 10, previewY + 42, COLOR_LIGHT_GRAY, 1);
    }
}

// =============================================================================
// SELECTION RESULTS
// =============================================================================

Element SpellSelector::getSelectedElement() const {
    if (!player || elementIndex >= player->getElementCount()) {
        return ELEM_NONE;
    }
    return player->getElement(elementIndex);
}

SpellType SpellSelector::getSelectedType() const {
    if (!player || typeIndex >= player->getTypeCount()) {
        return TYPE_NONE;
    }
    return player->getType(typeIndex);
}

SpellData SpellSelector::getSelectedSpell() const {
    return SpellMatrix::getSpell(getSelectedElement(), getSelectedType());
}

String SpellSelector::getSelectedSpellName() const {
    return SpellMatrix::getSpellName(getSelectedElement(), getSelectedType());
}