/*=======================================================================
 * Panel.cpp — Glassmorphism Panel Implementation
 *=======================================================================*/
#include "Panel.h"
#include "../core/Constants.h"

Panel::Panel() : visible(true), slideProgress(1.0f) {}

void Panel::setup(Vector2f size, Vector2f position, const string& title, Font& font) {
    targetPos = position;
    hiddenPos = Vector2f(position.x + size.x + 50, position.y);

    background.setSize(size);
    background.setFillColor(Config::Colors::PANEL_BG);
    background.setOutlineColor(Config::Colors::PANEL_BORDER);
    background.setOutlineThickness(2.f);
    background.setPosition(position);

    headerBar.setSize(Vector2f(size.x, 36));
    headerBar.setFillColor(Config::Colors::PANEL_HEADER);
    headerBar.setPosition(position);

    titleText.setFont(font);
    titleText.setString(title);
    titleText.setCharacterSize(Config::Font::SIZE_BODY);
    titleText.setFillColor(Config::Colors::TEXT_PRIMARY);
    titleText.setStyle(Text::Bold);
    titleText.setPosition(position.x + 12, position.y + 8);
}

void Panel::update(float dt) {
    float target = visible ? 1.0f : 0.0f;
    slideProgress += (target - slideProgress) * dt * 8.0f;
    slideProgress = max(0.0f, min(1.0f, slideProgress));

    float x = hiddenPos.x + (targetPos.x - hiddenPos.x) * slideProgress;
    float y = targetPos.y;
    background.setPosition(x, y);
    headerBar.setPosition(x, y);
    titleText.setPosition(x + 12, y + 8);
}

void Panel::render(RenderWindow& window) {
    if (slideProgress < 0.01f) return;
    window.draw(background);
    window.draw(headerBar);
    window.draw(titleText);
}

void Panel::show()  { visible = true; }
void Panel::hide()  { visible = false; }
bool Panel::isVisible() const { return visible; }

Vector2f Panel::getContentPosition() const {
    return Vector2f(background.getPosition().x + 12,
                    background.getPosition().y + 44);
}

Vector2f Panel::getContentSize() const {
    return Vector2f(background.getSize().x - 24,
                    background.getSize().y - 56);
}
