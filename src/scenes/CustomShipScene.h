#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../graph/PortManagementSystem.h"
#include "../ui/Button.h"
#include "../ui/TextInput.h"
#include "../rendering/MapRenderer.h"

class CustomShipScene : public Scene {
private:
    AssetManager& assets; SceneManager& sceneManager; PortManagementSystem& portSystem;
    Sprite mapSprite;
    int startPort, endPort;
    string preferredCompany, inputCompany;
    bool companyValidated, selectingExcluded, showInvalid;
    Vector<int> excludedPorts, customPath, customExplored;
    Button okButton;
    TextInput companyInput;
    float animTime;
public:
    CustomShipScene(AssetManager& a, SceneManager& sm, PortManagementSystem& ps)
        : assets(a), sceneManager(sm), portSystem(ps), startPort(-1), endPort(-1),
          companyValidated(false), selectingExcluded(false), showInvalid(false), animTime(0) {}
    void onEnter() override;
    void onExit() override;
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Custom Ship"; }
};
