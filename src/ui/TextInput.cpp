/*=======================================================================
 * TextInput.cpp — Text Input Widget
 *=======================================================================*/
#include "TextInput.h"
#include "../core/Constants.h"

TextInput::TextInput() : focused(false), active(true), cursorBlink(0) {}

void TextInput::setup(Vector2f size, Vector2f position, const string& placeholder, Font& font) {
    box.setSize(size);
    box.setFillColor(Config::Colors::BG_ELEVATED);
    box.setOutlineColor(Config::Colors::TEXT_SECONDARY);
    box.setOutlineThickness(2.f);
    box.setPosition(position);

    cursor.setSize(Vector2f(2, size.y - 16));
    cursor.setFillColor(Config::Colors::PRIMARY);
    cursor.setPosition(position.x + 12, position.y + 8);

    displayText.setFont(font);
    displayText.setCharacterSize(Config::Font::SIZE_BUTTON);
    displayText.setFillColor(Config::Colors::TEXT_PRIMARY);
    displayText.setPosition(position.x + 12, position.y + (size.y - 24) / 2);

    placeholderText.setFont(font);
    placeholderText.setString(placeholder);
    placeholderText.setCharacterSize(Config::Font::SIZE_BUTTON);
    placeholderText.setFillColor(Config::Colors::TEXT_SECONDARY);
    placeholderText.setPosition(position.x + 12, position.y + (size.y - 24) / 2);
}

void TextInput::handleEvent(Event& event) {
    if (!focused || !active) return;
    if (event.type == Event::TextEntered) {
        if (event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b' && value.size() > 0) {
                value.pop_back();
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                value += c;
            }
            displayText.setString(value);
        }
    }
}

void TextInput::update(float dt) {
    cursorBlink += dt;
    if (cursorBlink > 1.0f) cursorBlink = 0;

    box.setOutlineColor(focused ? Config::Colors::PRIMARY : Config::Colors::TEXT_SECONDARY);

    // Position cursor after text
    float textWidth = displayText.getGlobalBounds().width;
    cursor.setPosition(box.getPosition().x + 12 + textWidth + 2,
                       box.getPosition().y + 8);
}

void TextInput::render(RenderWindow& window) {
    window.draw(box);
    if (value.empty() && !focused)
        window.draw(placeholderText);
    else
        window.draw(displayText);
    if (focused && cursorBlink < 0.5f)
        window.draw(cursor);
}

string TextInput::getValue() const { return value; }
void TextInput::setValue(const string& val) { value = val; displayText.setString(val); }
void TextInput::clear() { value = ""; displayText.setString(""); }
void TextInput::setFocused(bool f) { focused = f; cursorBlink = 0; }
bool TextInput::isFocused() const { return focused; }
