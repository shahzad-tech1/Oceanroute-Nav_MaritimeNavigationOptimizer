/*=======================================================================
 * SceneManager.cpp — Scene Transitions
 *=======================================================================*/
#include "SceneManager.h"
#include <iostream>
using namespace std;

SceneManager::SceneManager()
    : currentScene(SceneType::MAIN_MENU),
      previousScene(SceneType::MAIN_MENU),
      transitionAlpha(0), transitioning(false)
{
    for (int i = 0; i < 8; i++) scenes[i] = nullptr;
}

void SceneManager::registerScene(SceneType type, Scene* scene) {
    scenes[(int)type] = scene;
}

void SceneManager::switchTo(SceneType type) {
    if (scenes[(int)currentScene])
        scenes[(int)currentScene]->onExit();
    previousScene = currentScene;
    currentScene = type;
    if (scenes[(int)currentScene])
        scenes[(int)currentScene]->onEnter();
    transitioning = true;
    transitionAlpha = 255.f;
}

void SceneManager::goBack() {
    switchTo(previousScene);
}

SceneType SceneManager::getCurrentType() const { return currentScene; }

Scene* SceneManager::getCurrentScene() {
    return scenes[(int)currentScene];
}

void SceneManager::update(float dt) {
    if (transitioning) {
        transitionAlpha -= dt * 600.f;
        if (transitionAlpha <= 0) {
            transitionAlpha = 0;
            transitioning = false;
        }
    }
}

void SceneManager::render(RenderWindow& window) {
    Scene* scene = getCurrentScene();
    if (scene) scene->render(window);

    // Fade overlay during transition
    if (transitioning && transitionAlpha > 0) {
        RectangleShape overlay;
        overlay.setSize(Vector2f((float)window.getSize().x, (float)window.getSize().y));
        overlay.setFillColor(Color(10, 22, 40, (Uint8)transitionAlpha));
        window.draw(overlay);
    }
}

bool SceneManager::isTransitioning() const { return transitioning; }
