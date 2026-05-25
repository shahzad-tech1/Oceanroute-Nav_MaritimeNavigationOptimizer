#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../graph/PortManagementSystem.h"
#include "../ui/Button.h"
#include "../rendering/MapRenderer.h"
#include "../rendering/Renderer.h"

class DockingScene : public Scene {
private:
    AssetManager& assets; SceneManager& sceneManager; PortManagementSystem& portSystem;
    Sprite mapSprite;
    int selectedPort;
    Button dockingButtons[3];
    Clock dockingClock, processingClock;
    float animationTime, processingTimer, animTime;
public:
    DockingScene(AssetManager& a, SceneManager& sm, PortManagementSystem& ps)
        : assets(a), sceneManager(sm), portSystem(ps), selectedPort(-1),
          animationTime(0), processingTimer(0), animTime(0) {}
    void onEnter() override;
    void onExit() override;
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Docking Queue"; }
};
