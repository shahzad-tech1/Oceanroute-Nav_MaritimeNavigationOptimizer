/*=======================================================================
 * Button.cpp — Modern Animated Button Implementation
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include "Button.h"
#include "../core/Constants.h"

// ── Helper: Linear interpolation ────────────────────────────────────
static float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

static Color lerpColor(Color a, Color b, float t) {
    return Color(
        (Uint8)lerp(a.r, b.r, t),
        (Uint8)lerp(a.g, b.g, t),
        (Uint8)lerp(a.b, b.b, t),
        (Uint8)lerp(a.a, b.a, t)
    );
}

Button::Button()
    : hovered(false), pressed(false), enabled(true), hoverAnim(0.0f),
      baseColor(Config::Colors::BUTTON_FILL),
      hoverColor(Config::Colors::BUTTON_HOVER),
      glowColor(Config::Colors::BUTTON_GLOW)
{}

void Button::setup(Vector2f size, Vector2f position, const string& text,
                   unsigned int textSize, Font& font,
                   Color fillColor, Color outlineColor) {
    baseColor = fillColor;
    hoverColor = Color(
        min(255, fillColor.r + 40),
        min(255, fillColor.g + 40),
        min(255, fillColor.b + 40)
    );
    glowColor = Color(outlineColor.r, outlineColor.g, outlineColor.b, 50);

    box.setSize(size);
    box.setFillColor(baseColor);
    box.setOutlineColor(outlineColor);
    box.setOutlineThickness(2.f);
    box.setPosition(position);

    // Glow effect — slightly larger box behind
    glowBox.setSize(Vector2f(size.x + 8, size.y + 8));
    glowBox.setFillColor(Color::Transparent);
    glowBox.setOutlineColor(glowColor);
    glowBox.setOutlineThickness(3.f);
    glowBox.setPosition(position.x - 4, position.y - 4);

    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(textSize);
    label.setFillColor(Config::Colors::TEXT_PRIMARY);
    label.setStyle(Text::Bold);

    // Center text
    FloatRect textRect = label.getLocalBounds();
    label.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
    label.setPosition(position.x + size.x / 2.0f,
                      position.y + size.y / 2.0f);
}

void Button::update(float dt, Vector2f mousePos) {
    if (!enabled) {
        hoverAnim = 0;
        return;
    }

    bool isOver = box.getGlobalBounds().contains(mousePos);

    if (isOver && !hovered) {
        hovered = true;
    } else if (!isOver && hovered) {
        hovered = false;
    }

    // Smooth hover transition
    float target = hovered ? 1.0f : 0.0f;
    hoverAnim += (target - hoverAnim) * dt * Config::Anim::HOVER_SPEED;
    hoverAnim = max(0.0f, min(1.0f, hoverAnim));

    // Animate colors
    Color currentFill = lerpColor(baseColor, hoverColor, hoverAnim);
    box.setFillColor(currentFill);

    // Glow intensity
    Color glow = glowColor;
    glow.a = (Uint8)(hoverAnim * 120);
    glowBox.setOutlineColor(glow);

    // Subtle scale effect
    Vector2f baseSize = box.getSize();
    float scaleOffset = hoverAnim * 2.0f;
    glowBox.setSize(Vector2f(baseSize.x + 8 + scaleOffset,
                              baseSize.y + 8 + scaleOffset));
    glowBox.setPosition(box.getPosition().x - 4 - scaleOffset / 2,
                        box.getPosition().y - 4 - scaleOffset / 2);
}

void Button::render(RenderWindow& window) {
    if (hoverAnim > 0.01f)
        window.draw(glowBox);
    window.draw(box);
    window.draw(label);
}

bool Button::contains(Vector2f point) const {
    return box.getGlobalBounds().contains(point);
}

bool Button::isClicked(Vector2f mousePos) const {
    return enabled && box.getGlobalBounds().contains(mousePos);
}

void Button::setLabel(const string& text) {
    label.setString(text);
    // Re-center
    FloatRect textRect = label.getLocalBounds();
    label.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
    label.setPosition(box.getPosition().x + box.getSize().x / 2.0f,
                      box.getPosition().y + box.getSize().y / 2.0f);
}

void Button::setPosition(Vector2f pos) {
    box.setPosition(pos);
    glowBox.setPosition(pos.x - 4, pos.y - 4);
    FloatRect textRect = label.getLocalBounds();
    label.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
    label.setPosition(pos.x + box.getSize().x / 2.0f,
                      pos.y + box.getSize().y / 2.0f);
}

void Button::setEnabled(bool en) {
    enabled = en;
    if (!enabled) {
        box.setFillColor(Color(80, 80, 100));
        label.setFillColor(Color(120, 120, 140));
    } else {
        box.setFillColor(baseColor);
        label.setFillColor(Config::Colors::TEXT_PRIMARY);
    }
}

void Button::setColors(Color base, Color hover, Color glow) {
    baseColor = base;
    hoverColor = hover;
    glowColor = glow;
    box.setFillColor(base);
}
