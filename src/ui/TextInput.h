#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

class TextInput {
private:
    RectangleShape box, cursor;
    Text displayText, placeholderText;
    string value;
    bool focused, active;
    float cursorBlink;
public:
    TextInput();
    void setup(Vector2f size, Vector2f position, const string& placeholder, Font& font);
    void handleEvent(Event& event);
    void update(float dt);
    void render(RenderWindow& window);
    string getValue() const;
    void setValue(const string& val);
    void clear();
    void setFocused(bool f);
    bool isFocused() const;
};
