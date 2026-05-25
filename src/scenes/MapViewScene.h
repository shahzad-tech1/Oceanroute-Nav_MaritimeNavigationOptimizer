#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../graph/PortManagementSystem.h"
#include "../rendering/MapRenderer.h"

class MapViewScene : public Scene {
private:
    AssetManager& assets;
    SceneManager& sceneManager;
    PortManagementSystem& portSystem;
    Sprite mapSprite;
    int currentPort;
    float animTime;
public:
    MapViewScene(AssetManager& a, SceneManager& sm, PortManagementSystem& ps)
        : assets(a), sceneManager(sm), portSystem(ps), currentPort(-1), animTime(0) {}
    void onEnter() override;
    void onExit() override;
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Route Data"; }
};
