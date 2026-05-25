#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../ui/Button.h"
#include "../core/Constants.h"
#include <cmath>

class MainMenuScene : public Scene {
private:
    AssetManager& assets;
    SceneManager& sceneManager;
    Sprite backgroundSprite;
    Button menuButtons[7];
    Text titleText, subtitleText, authorText;
    float animTime;
    RectangleShape overlay;
public:
    MainMenuScene(AssetManager& a, SceneManager& sm) : assets(a), sceneManager(sm), animTime(0) {}
    void onEnter() override;
    void onExit() override {}
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Main Menu"; }
};
