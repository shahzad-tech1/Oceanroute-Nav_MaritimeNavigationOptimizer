#pragma once
/*=======================================================================
 * SceneManager.h — Scene Transition Manager
 *=======================================================================*/
#include "../scenes/Scene.h"

enum class SceneType {
    MAIN_MENU, MAP_VIEW, BOOKING, DIJKSTRA, ASTAR,
    CUSTOM_SHIP, DOCKING, MULTI_LEG, COUNT
};

class SceneManager {
private:
    Scene* scenes[8];
    SceneType currentScene;
    SceneType previousScene;
    float transitionAlpha;
    bool transitioning;
public:
    SceneManager();
    void registerScene(SceneType type, Scene* scene);
    void switchTo(SceneType type);
    void goBack();
    SceneType getCurrentType() const;
    Scene* getCurrentScene();
    void update(float dt);
    void render(RenderWindow& window);
    bool isTransitioning() const;
};
