#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../graph/PortManagementSystem.h"
#include "../graph/UserRouteList.h"
#include "../ui/Button.h"
#include "../rendering/Renderer.h"
#include "../rendering/MapRenderer.h"

class MultiLegScene : public Scene {
private:
    AssetManager& assets; SceneManager& sceneManager; PortManagementSystem& portSystem;
    UserRouteList routeList;
    Sprite mapSprite;
    Vector<int> multiLegSeq;
    int selectedSegment;
    bool isInsertMode, isDeleteMode, multiRouteBuilt;
    int scrollOffset;
    Button multiButtons[4];
    float animTime;
    bool buildMultiLegRoute(Vector<string>& portSequence);
public:
    MultiLegScene(AssetManager& a, SceneManager& sm, PortManagementSystem& ps)
        : assets(a), sceneManager(sm), portSystem(ps), selectedSegment(-1),
          isInsertMode(false), isDeleteMode(false), multiRouteBuilt(false),
          scrollOffset(0), animTime(0) {}
    void onEnter() override;
    void onExit() override;
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Multi-Leg Route"; }
};
