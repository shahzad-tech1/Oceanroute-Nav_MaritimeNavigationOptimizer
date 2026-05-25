/*=======================================================================
 * AssetManager.cpp — Asset Loading with Fallbacks
 *=======================================================================*/
#include "AssetManager.h"
#include "../core/Constants.h"

AssetManager::AssetManager() : loaded(false) {}

bool AssetManager::loadAll() {
    bool success = true;

    // Load fonts — try Raleway first, fall back to Arial
    if (!fontRegular.loadFromFile(Config::Assets::FONT_REGULAR)) {
        cout << "[Assets] Raleway Regular not found, trying fallback...\n";
        if (!fontRegular.loadFromFile(Config::Assets::FONT_FALLBACK)) {
            cout << "[Assets] ERROR: Could not load any font!\n";
            success = false;
        }
    }
    if (!fontBold.loadFromFile(Config::Assets::FONT_BOLD)) {
        fontBold = fontRegular;  // Fallback to regular
    }
    if (!fontLight.loadFromFile(Config::Assets::FONT_LIGHT)) {
        fontLight = fontRegular;
    }

    // Load textures
    if (!mapTexture.loadFromFile(Config::Assets::TEX_MAP)) {
        cout << "[Assets] ERROR: Could not load map texture!\n";
        success = false;
    }
    mapTexture.setSmooth(true);

    if (!menuBgTexture.loadFromFile(Config::Assets::TEX_MENU_BG)) {
        cout << "[Assets] ERROR: Could not load menu background!\n";
        success = false;
    }
    menuBgTexture.setSmooth(true);

    // Load sounds
    if (clickBuffer.loadFromFile(Config::Assets::SND_CLICK)) {
        clickSound.setBuffer(clickBuffer);
    } else {
        cout << "[Assets] Warning: Click sound not found\n";
    }

    loaded = success;
    if (success) cout << "[Assets] All assets loaded successfully.\n";
    return success;
}

Font& AssetManager::getFont()        { return fontRegular; }
Font& AssetManager::getFontBold()    { return fontBold; }
Font& AssetManager::getFontLight()   { return fontLight; }
Texture& AssetManager::getMapTexture()    { return mapTexture; }
Texture& AssetManager::getMenuBgTexture() { return menuBgTexture; }

void AssetManager::playClickSound() {
    if (clickBuffer.getSampleCount() > 0)
        clickSound.play();
}

bool AssetManager::isLoaded() const { return loaded; }
