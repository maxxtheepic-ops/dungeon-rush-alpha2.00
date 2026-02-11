#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// DUNGEON RUSH ALPHA - CONFIGURATION
// =============================================================================
// All hardware pins, display settings, and game constants in one place
// =============================================================================

// === HARDWARE PINS - ESP32-S3 ===

// TFT Display Pins (ST7789 SPI) - Configured in platformio.ini build_flags
// These are here for reference only
// TFT_DC   = 11  (Data/Command)
// TFT_CS   = 12  (Chip Select)
// TFT_SCK  = 10  (SPI Clock)
// TFT_MOSI = 13  (SPI Data Out)
// TFT_RST  = 9   (Reset)
// TFT_BL   = 14  (Backlight - INVERTED: LOW = ON)

// Input Buttons
#define PIN_BTN_UP      1
#define PIN_BTN_DOWN    5
#define PIN_BTN_LEFT    3
#define PIN_BTN_RIGHT   2
#define PIN_BTN_A       40
#define PIN_BTN_B       39
#define PIN_BTN_X       41
#define PIN_BTN_Y       42
#define PIN_BTN_OPTIONS 46

// SD Card pins (unused)
#define PIN_SD_CS       -1
#define PIN_SD_MOSI     -1
#define PIN_SD_MISO     -1
#define PIN_SD_SCK      -1

// Audio (unused)
#define PIN_BUZZER      -1

// === DISPLAY CONFIGURATION ===

#define SCREEN_WIDTH    170
#define SCREEN_HEIGHT   320
#define SCREEN_ROTATION 2

// === COLORS (16-bit RGB565) ===

// Basic colors
#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0xFFFF
#define COLOR_RED           0xF800
#define COLOR_GREEN         0x07E0
#define COLOR_BLUE          0x001F
#define COLOR_YELLOW        0xFFE0
#define COLOR_CYAN          0x07FF
#define COLOR_MAGENTA       0xF81F
#define COLOR_ORANGE        0xFD20
#define COLOR_PURPLE        0x8010

// Grayscale
#define COLOR_GRAY          0x8410
#define COLOR_DARK_GRAY     0x4208
#define COLOR_LIGHT_GRAY    0xC618

// UI-specific colors
#define COLOR_BG            COLOR_BLACK
#define COLOR_TEXT          COLOR_WHITE
#define COLOR_CURSOR        COLOR_YELLOW
#define COLOR_HP            COLOR_RED
#define COLOR_KINDNESS      COLOR_YELLOW
#define COLOR_MENU_BG       COLOR_DARK_GRAY

// === INPUT CONFIGURATION ===

#define INPUT_DEBOUNCE_MS   50
#define BUTTON_COUNT        9

// === GAME BALANCE ===

// Player starting stats
#define PLAYER_START_HP         100
#define PLAYER_START_MAX_HP     100
#define PLAYER_START_ATTACK     10
#define PLAYER_START_DEFENSE    5

// Dungeon progression
#define ROOMS_PER_FLOOR     5
#define BOSS_EVERY_FLOORS   3

// === UI LAYOUT ===

// Combat HUD region (top of screen)
#define HUD_Y               0
#define HUD_HEIGHT          60

// Text box region (bottom of screen)
#define TEXTBOX_HEIGHT      50
#define TEXTBOX_Y           (SCREEN_HEIGHT - TEXTBOX_HEIGHT)

// Menu area (middle)
#define MENU_Y              HUD_HEIGHT
#define MENU_HEIGHT         (SCREEN_HEIGHT - HUD_HEIGHT - TEXTBOX_HEIGHT)

// Bar dimensions
#define BAR_WIDTH           100
#define BAR_HEIGHT          12

// === SERIAL DEBUG ===

#define DEBUG_ENABLED       1

#if DEBUG_ENABLED
    #define DEBUG_PRINT(x)      Serial.print(x)
    #define DEBUG_PRINTLN(x)    Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

#endif // CONFIG_H