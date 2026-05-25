/*=======================================================================
 * StatusBar.cpp — Top & Bottom Bar Implementation
 *=======================================================================*/
#include "StatusBar.h"
#include "../core/Constants.h"

StatusBar::StatusBar() : fontPtr(nullptr), fpsTimer(0), frameCount(0), currentFps(0) {}

void StatusBar::setup(float windowWidth, float windowHeight, Font& font) {
    fontPtr = &font;
    topBar.setSize(Vector2f(windowWidth, Config::UI::STATUSBAR_HEIGHT));
    topBar.setFillColor(Config::Colors::STATUSBAR_BG);
    topBar.setPosition(0, 0);

    bottomBar.setSize(Vector2f(windowWidth, Config::UI::BOTTOM_BAR_HEIGHT));
    bottomBar.setFillColor(Config::Colors::STATUSBAR_BG);
    bottomBar.setPosition(0, windowHeight - Config::UI::BOTTOM_BAR_HEIGHT);

    titleText.setFont(font);
    titleText.setString(Config::APP_TITLE);
    titleText.setCharacterSize(Config::Font::SIZE_BODY);
    titleText.setFillColor(Config::Colors::PRIMARY);
    titleText.setStyle(Text::Bold);
    titleText.setPosition(15, 9);

    breadcrumbText.setFont(font);
    breadcrumbText.setCharacterSize(Config::Font::SIZE_SMALL);
    breadcrumbText.setFillColor(Config::Colors::TEXT_SECONDARY);
    breadcrumbText.setPosition(windowWidth / 2 - 100, 12);

    statusText.setFont(font);
    statusText.setCharacterSize(Config::Font::SIZE_TINY);
    statusText.setFillColor(Config::Colors::STATUSBAR_TEXT);
    statusText.setPosition(15, windowHeight - Config::UI::BOTTOM_BAR_HEIGHT + 7);

    fpsText.setFont(font);
    fpsText.setCharacterSize(Config::Font::SIZE_TINY);
    fpsText.setFillColor(Config::Colors::TEXT_SECONDARY);
    fpsText.setPosition(windowWidth - 80, windowHeight - Config::UI::BOTTOM_BAR_HEIGHT + 7);
}

void StatusBar::update(float dt, const string& scene) {
    breadcrumbText.setString("Home > " + scene);
    frameCount++;
    fpsTimer += dt;
    if (fpsTimer >= 1.0f) {
        currentFps = frameCount;
        frameCount = 0;
        fpsTimer = 0;
        fpsText.setString(to_string(currentFps) + " FPS");
    }
}

void StatusBar::render(RenderWindow& window) {
    window.draw(topBar);
    window.draw(bottomBar);
    window.draw(titleText);
    window.draw(breadcrumbText);
    window.draw(statusText);
    window.draw(fpsText);
}

void StatusBar::setBreadcrumb(const string& path) { breadcrumbText.setString(path); }
void StatusBar::setStatus(const string& status) { statusText.setString(status); }
float StatusBar::getTopHeight() const { return Config::UI::STATUSBAR_HEIGHT; }
float StatusBar::getBottomY() const { return bottomBar.getPosition().y; }
