#pragma once
/*=======================================================================
 * Panel.h — Glassmorphism Info Panel
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

class Panel {
private:
    RectangleShape background;
    RectangleShape headerBar;
    Text titleText;
    bool visible;
    float slideProgress;
    Vector2f targetPos;
    Vector2f hiddenPos;

public:
    Panel();
    void setup(Vector2f size, Vector2f position, const string& title, Font& font);
    void update(float dt);
    void render(RenderWindow& window);
    void show();
    void hide();
    bool isVisible() const;
    Vector2f getContentPosition() const;
    Vector2f getContentSize() const;
    RectangleShape& getBackground() { return background; }
};
