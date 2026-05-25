#pragma once
/*=======================================================================
 * AssetManager.h — Centralized Asset Loading
 *=======================================================================*/
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
using namespace sf;
using namespace std;

class AssetManager {
private:
    Font fontRegular, fontBold, fontLight;
    Texture mapTexture, menuBgTexture;
    SoundBuffer clickBuffer;
    Sound clickSound;
    bool loaded;
public:
    AssetManager();
    bool loadAll();
    Font& getFont();
    Font& getFontBold();
    Font& getFontLight();
    Texture& getMapTexture();
    Texture& getMenuBgTexture();
    void playClickSound();
    bool isLoaded() const;
};
