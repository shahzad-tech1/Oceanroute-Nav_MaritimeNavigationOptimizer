#pragma once
#include "Scene.h"
#include "../systems/AssetManager.h"
#include "../systems/SceneManager.h"
#include "../graph/PortManagementSystem.h"
#include "../user/User.h"
#include "../ui/Button.h"
#include "../rendering/MapRenderer.h"
#include "../rendering/Renderer.h"

class BookingScene : public Scene {
private:
    AssetManager& assets;
    SceneManager& sceneManager;
    PortManagementSystem& portSystem;
    User& user;
    Sprite mapSprite;
    Button bookingButtons[3];
    int sourcePort, destPort, selectedRoute, loadFlag;
    bool displayBooking;
    Vector<Vector<Vertex>> pathPoints;
    Vector<Vector<Routes>> pathRoutes;
    float animTime;
    void drawPathsThick(RenderWindow& window, Vector<Vector<Vertex>>& points, int rcolor, float thickness);
    void drawFullPathInfo(RenderWindow& window, Vector<Vector<Routes>>& routes, Vector<Vector<Vertex>>& points);
public:
    BookingScene(AssetManager& a, SceneManager& sm, PortManagementSystem& ps, User& u)
        : assets(a), sceneManager(sm), portSystem(ps), user(u),
          sourcePort(-1), destPort(-1), selectedRoute(-1), loadFlag(1),
          displayBooking(false), animTime(0) {}
    void onEnter() override;
    void onExit() override;
    void handleEvent(Event& event) override;
    void update(float dt) override;
    void render(RenderWindow& window) override;
    const char* getName() const override { return "Booking"; }
};
