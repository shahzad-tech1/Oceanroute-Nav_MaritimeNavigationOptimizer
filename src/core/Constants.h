#pragma once
/*=======================================================================
 * Constants.h — Design System, Colors, Coordinates & Configuration
 * OceanRoute Nav — Maritime Navigation Optimizer
 *
 * Central configuration for the entire application:
 * - Color palette (deep ocean theme)
 * - Typography settings
 * - Asset paths
 * - Normalized port coordinates (0.0-1.0 range)
 * - UI sizing constants
 * - Animation timing
 *=======================================================================*/

#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

namespace Config {

    // ═══════════════════════════════════════════════════════════════
    // APPLICATION INFO
    // ═══════════════════════════════════════════════════════════════
    const string APP_TITLE   = "OceanRoute Nav";
    const string APP_SUBTITLE = "Maritime Navigation Optimizer";
    const string APP_VERSION = "2.0.0";
    const string AUTHORS     = "Shahzad Ahmad & Syed Muhammad Rayyan Hassan";

    // ═══════════════════════════════════════════════════════════════
    // WINDOW DIMENSIONS
    // ═══════════════════════════════════════════════════════════════
    const unsigned int DEFAULT_WIDTH  = 1300;
    const unsigned int DEFAULT_HEIGHT = 740;
    const unsigned int FRAMERATE_LIMIT = 60;

    // Original reference dimensions (for coordinate normalization)
    const float REF_WIDTH  = 1300.0f;
    const float REF_HEIGHT = 740.0f;

    // ═══════════════════════════════════════════════════════════════
    // COLOR PALETTE — Deep Ocean Theme
    // ═══════════════════════════════════════════════════════════════
    namespace Colors {
        // Backgrounds
        const Color BG_DEEP       = Color(10, 22, 40);           // #0a1628
        const Color BG_SURFACE    = Color(22, 34, 53);           // #162235
        const Color BG_ELEVATED   = Color(30, 45, 68);           // #1e2d44
        const Color BG_OVERLAY    = Color(10, 22, 40, 180);      // Semi-transparent

        // Primary palette
        const Color PRIMARY       = Color(0, 212, 255);          // Cyan glow
        const Color SECONDARY     = Color(0, 136, 204);          // Ocean blue
        const Color ACCENT        = Color(255, 107, 53);         // Orange beacon
        const Color GOLD          = Color(255, 215, 0);          // Gold highlight

        // Semantic colors
        const Color SUCCESS       = Color(0, 200, 83);           // Green
        const Color DANGER        = Color(255, 82, 82);          // Red
        const Color WARNING       = Color(255, 193, 7);          // Amber
        const Color INFO          = Color(33, 150, 243);         // Blue

        // Text colors
        const Color TEXT_PRIMARY  = Color(230, 240, 255);        // Bright
        const Color TEXT_SECONDARY= Color(140, 160, 190);        // Muted
        const Color TEXT_DARK     = Color(10, 22, 40);           // On light backgrounds

        // UI element colors
        const Color BUTTON_FILL   = Color(0, 136, 204);
        const Color BUTTON_HOVER  = Color(0, 170, 230);
        const Color BUTTON_PRESS  = Color(0, 100, 170);
        const Color BUTTON_GLOW   = Color(0, 212, 255, 60);

        // Port node state colors
        const Color PORT_DEFAULT  = Color(0, 212, 255);          // Cyan
        const Color PORT_SELECTED = Color(255, 215, 0);          // Gold
        const Color PORT_START    = Color(0, 200, 83);           // Green
        const Color PORT_END      = Color(255, 107, 53);         // Orange
        const Color PORT_EXPLORED = Color(255, 193, 7);          // Yellow
        const Color PORT_PATH     = Color(0, 255, 136);          // Bright green
        const Color PORT_EXCLUDED = Color(100, 100, 120);        // Gray
        const Color PORT_GLOW     = Color(0, 212, 255, 40);      // Transparent cyan

        // Edge/route colors
        const Color EDGE_DEFAULT  = Color(60, 80, 120, 150);     // Subtle blue
        const Color EDGE_ACTIVE   = Color(0, 212, 255);          // Cyan
        const Color EDGE_PATH     = Color(0, 255, 136);          // Green path

        // Panel colors
        const Color PANEL_BG      = Color(22, 34, 53, 230);
        const Color PANEL_BORDER  = Color(0, 212, 255, 120);
        const Color PANEL_HEADER  = Color(0, 136, 204, 200);

        // Status bar
        const Color STATUSBAR_BG  = Color(8, 16, 32, 240);
        const Color STATUSBAR_TEXT= Color(140, 160, 190);

        // Route path palette for multi-path visualization
        const Color PATH_PALETTE[] = {
            Color(220, 20, 60),     // Crimson
            Color(30, 144, 255),    // Dodger blue
            Color(60, 179, 113),    // Medium sea green
            Color(255, 165, 0),     // Orange
            Color(148, 0, 211),     // Purple
            Color(255, 105, 180),   // Hot pink
            Color(0, 206, 209),     // Turquoise
            Color(218, 165, 32),    // Golden rod
            Color(34, 139, 34),     // Forest green
            Color(70, 130, 180)     // Steel blue
        };
        const int PATH_PALETTE_SIZE = 10;
    }

    // ═══════════════════════════════════════════════════════════════
    // ASSET PATHS
    // ═══════════════════════════════════════════════════════════════
    namespace Assets {
        const string FONT_REGULAR  = "assets/fonts/Raleway-Regular.ttf";
        const string FONT_BOLD     = "assets/fonts/Raleway-Bold.ttf";
        const string FONT_LIGHT    = "assets/fonts/Raleway-Light.ttf";
        const string FONT_SEMIBOLD = "assets/fonts/Raleway-SemiBold.ttf";
        const string FONT_FALLBACK = "C:/Windows/Fonts/arial.ttf";

        const string TEX_MAP       = "assets/map.jpg";
        const string TEX_MENU_BG   = "assets/backGroundMenu.jpg";

        const string SND_CLICK     = "assets/mouseClick.wav";

        const string DATA_ROUTES   = "assets/routes.txt";
        const string DATA_CHARGES  = "assets/portCharges.txt";
    }

    // ═══════════════════════════════════════════════════════════════
    // PORT COORDINATES (Normalized 0.0 - 1.0)
    // Original pixel positions / reference dimensions
    // ═══════════════════════════════════════════════════════════════
    struct PortPosition {
        const char* name;
        float nx;   ///< Normalized X (0.0 - 1.0)
        float ny;   ///< Normalized Y (0.0 - 1.0)
    };

    const PortPosition PORT_POSITIONS[] = {
        {"HongKong",    1085.0f / REF_WIDTH, 315.0f / REF_HEIGHT},
        {"Jeddah",       765.0f / REF_WIDTH, 315.0f / REF_HEIGHT},
        {"Durban",       730.0f / REF_WIDTH, 580.0f / REF_HEIGHT},
        {"Marseille",    620.0f / REF_WIDTH, 215.0f / REF_HEIGHT},
        {"Oslo",         645.0f / REF_WIDTH, 130.0f / REF_HEIGHT},
        {"Dubai",        845.0f / REF_WIDTH, 300.0f / REF_HEIGHT},
        {"Genoa",        640.0f / REF_WIDTH, 200.0f / REF_HEIGHT},
        {"Karachi",      885.0f / REF_WIDTH, 305.0f / REF_HEIGHT},
        {"Osaka",       1150.0f / REF_WIDTH, 260.0f / REF_HEIGHT},
        {"Lisbon",       560.0f / REF_WIDTH, 235.0f / REF_HEIGHT},
        {"Hamburg",      645.0f / REF_WIDTH, 165.0f / REF_HEIGHT},
        {"Rotterdam",    630.0f / REF_WIDTH, 150.0f / REF_HEIGHT},
        {"Sydney",      1220.0f / REF_WIDTH, 600.0f / REF_HEIGHT},
        {"Melbourne",   1185.0f / REF_WIDTH, 615.0f / REF_HEIGHT},
        {"Colombo",      950.0f / REF_WIDTH, 400.0f / REF_HEIGHT},
        {"Helsinki",     685.0f / REF_WIDTH, 120.0f / REF_HEIGHT},
        {"Montreal",     320.0f / REF_WIDTH, 200.0f / REF_HEIGHT},
        {"Mumbai",       910.0f / REF_WIDTH, 340.0f / REF_HEIGHT},
        {"Manila",      1125.0f / REF_WIDTH, 350.0f / REF_HEIGHT},
        {"Istanbul",     725.0f / REF_WIDTH, 215.0f / REF_HEIGHT},
        {"Jakarta",     1070.0f / REF_WIDTH, 455.0f / REF_HEIGHT},
        {"Tokyo",       1170.0f / REF_WIDTH, 240.0f / REF_HEIGHT},
        {"Athens",       695.0f / REF_WIDTH, 245.0f / REF_HEIGHT},
        {"Dublin",       570.0f / REF_WIDTH, 160.0f / REF_HEIGHT},
        {"CapeTown",     670.0f / REF_WIDTH, 590.0f / REF_HEIGHT},
        {"Singapore",   1045.0f / REF_WIDTH, 410.0f / REF_HEIGHT},
        {"Stockholm",    670.0f / REF_WIDTH, 140.0f / REF_HEIGHT},
        {"PortLouis",    590.0f / REF_WIDTH, 185.0f / REF_HEIGHT},
        {"Busan",       1120.0f / REF_WIDTH, 250.0f / REF_HEIGHT},
        {"Shanghai",    1100.0f / REF_WIDTH, 280.0f / REF_HEIGHT},
        {"Alexandria",   730.0f / REF_WIDTH, 270.0f / REF_HEIGHT},
        {"NewYork",      310.0f / REF_WIDTH, 230.0f / REF_HEIGHT},
        {"AbuDhabi",     825.0f / REF_WIDTH, 305.0f / REF_HEIGHT},
        {"Doha",         810.0f / REF_WIDTH, 290.0f / REF_HEIGHT},
        {"Copenhagen",   560.0f / REF_WIDTH, 100.0f / REF_HEIGHT},
        {"Vancouver",    140.0f / REF_WIDTH, 170.0f / REF_HEIGHT},
        {"LosAngeles",   110.0f / REF_WIDTH, 250.0f / REF_HEIGHT},
        {"Antwerp",      625.0f / REF_WIDTH, 170.0f / REF_HEIGHT},
        {"London",       605.0f / REF_WIDTH, 165.0f / REF_HEIGHT},
        {"Chittagong",   985.0f / REF_WIDTH, 310.0f / REF_HEIGHT}
    };
    const int PORT_COUNT = 40;

    // ═══════════════════════════════════════════════════════════════
    // UI SIZING
    // ═══════════════════════════════════════════════════════════════
    namespace UI {
        const float STATUSBAR_HEIGHT   = 40.0f;
        const float BOTTOM_BAR_HEIGHT  = 30.0f;
        const float PANEL_WIDTH        = 380.0f;
        const float BUTTON_WIDTH       = 280.0f;
        const float BUTTON_HEIGHT      = 60.0f;
        const float BUTTON_SPACING     = 18.0f;
        const float PORT_RADIUS        = 8.0f;
        const float PORT_GLOW_RADIUS   = 16.0f;
        const float EDGE_THICKNESS     = 2.0f;
        const float PATH_THICKNESS     = 4.0f;
        const float TOOLTIP_PADDING    = 10.0f;
        const float MARGIN             = 20.0f;
    }

    // ═══════════════════════════════════════════════════════════════
    // TYPOGRAPHY
    // ═══════════════════════════════════════════════════════════════
    namespace Font {
        const unsigned int SIZE_TITLE     = 42;
        const unsigned int SIZE_SUBTITLE  = 22;
        const unsigned int SIZE_HEADING   = 28;
        const unsigned int SIZE_BUTTON    = 20;
        const unsigned int SIZE_BODY      = 16;
        const unsigned int SIZE_SMALL     = 14;
        const unsigned int SIZE_TINY      = 11;
    }

    // ═══════════════════════════════════════════════════════════════
    // ANIMATION TIMING
    // ═══════════════════════════════════════════════════════════════
    namespace Anim {
        const float HOVER_SPEED       = 6.0f;     // seconds⁻¹
        const float TRANSITION_SPEED  = 3.0f;
        const float PULSE_SPEED       = 2.5f;
        const float GLOW_SPEED        = 1.8f;
        const float WAVE_SPEED        = 1.2f;
        const float STEP_INTERVAL     = 0.4f;     // Algorithm step delay
        const float SLIDE_DURATION    = 0.3f;
    }

} // namespace Config
