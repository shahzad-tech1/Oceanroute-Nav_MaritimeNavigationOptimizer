#pragma once
/*=======================================================================
 * Scene.h — Abstract Scene Interface
 *=======================================================================*/
#include <SFML/Graphics.hpp>
using namespace sf;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void handleEvent(Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(RenderWindow& window) = 0;
    virtual const char* getName() const = 0;
};
