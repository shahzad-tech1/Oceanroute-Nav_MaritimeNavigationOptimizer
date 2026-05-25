#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../graph/PortManagementSystem.h"
#include "../rendering/MapRenderer.h"
#include "../rendering/Renderer.h"
#include "../ui/Panel.h"

class DijkstraScene : public Scene {
private:
    AssetManager& assets; SceneManager& sceneManager; PortManagementSystem& portSystem;
    Sprite mapSprite;
    int startPort, endPort;
    Vector<int> exploredOrder, finalPath;
    int exploreStep;
    float animTime, stepTimer, stepInterval;
    bool autoPlay, pathFound;
    Panel infoPanel;
public:
    DijkstraScene(AssetManager& a, SceneManager& sm, PortManagementSystem& ps)
        : assets(a), sceneManager(sm), portSystem(ps), startPort(-1), endPort(-1),
          exploreStep(0), animTime(0), stepTimer(0), stepInterval(0.15f), autoPlay(true), pathFound(false) {}
    void onEnter() override;
    void onExit() override;
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Dijkstra"; }
};
