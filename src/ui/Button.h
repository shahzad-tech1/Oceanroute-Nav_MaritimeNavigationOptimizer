#pragma once
/*=======================================================================
 * Button.h — Modern Animated Button Widget
 * OceanRoute Nav — Maritime Navigation Optimizer
 *=======================================================================*/

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
using namespace sf;
using namespace std;

class Button {
private:
    RectangleShape box;
    RectangleShape glowBox;
    Text label;
    bool hovered;
    bool pressed;
    bool enabled;
    float hoverAnim;          // 0.0 → 1.0 transition
    Color baseColor;
    Color hoverColor;
    Color glowColor;

public:
    Button();

    void setup(Vector2f size, Vector2f position, const string& text,
               unsigned int textSize, Font& font,
               Color fillColor = Color(0, 136, 204),
               Color outlineColor = Color(0, 212, 255));

    void update(float dt, Vector2f mousePos);
    void render(RenderWindow& window);
    bool contains(Vector2f point) const;
    bool isClicked(Vector2f mousePos) const;

    void setLabel(const string& text);
    void setPosition(Vector2f pos);
    void setEnabled(bool en);
    void setColors(Color base, Color hover, Color glow);

    RectangleShape& getBox() { return box; }
    Text& getLabel() { return label; }
    bool isHovered() const { return hovered; }
};
