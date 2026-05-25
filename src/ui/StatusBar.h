#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Button.h"
using namespace sf;
using namespace std;

class StatusBar {
private:
    RectangleShape topBar, bottomBar;
    Text titleText, breadcrumbText, statusText, fpsText;
    Font* fontPtr;
    float fpsTimer;
    int frameCount, currentFps;
public:
    StatusBar();
    void setup(float windowWidth, float windowHeight, Font& font);
    void update(float dt, const string& scene);
    void render(RenderWindow& window);
    void setBreadcrumb(const string& path);
    void setStatus(const string& status);
    float getTopHeight() const;
    float getBottomY() const;
};
