/*=======================================================================
 * MainMenuScene.cpp — Animated Main Menu
 *=======================================================================*/
#include "MainMenuScene.h"
#include <iostream>
using namespace std;

void MainMenuScene::onEnter() {
    animTime = 0;
    Font& font = assets.getFontBold();
    Font& fontReg = assets.getFont();

    backgroundSprite.setTexture(assets.getMenuBgTexture());
    // Scale background to fill window
    float scaleX = Config::REF_WIDTH / (float)assets.getMenuBgTexture().getSize().x;
    float scaleY = Config::REF_HEIGHT / (float)assets.getMenuBgTexture().getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    overlay.setSize(Vector2f(Config::REF_WIDTH, Config::REF_HEIGHT));
    overlay.setFillColor(Config::Colors::BG_OVERLAY);

    // Title
    titleText.setFont(font);
    titleText.setString(Config::APP_TITLE);
    titleText.setCharacterSize(Config::Font::SIZE_TITLE);
    titleText.setFillColor(Config::Colors::PRIMARY);
    titleText.setStyle(Text::Bold);
    FloatRect tr = titleText.getLocalBounds();
    titleText.setOrigin(tr.left + tr.width / 2.f, tr.top + tr.height / 2.f);
    titleText.setPosition(Config::REF_WIDTH / 2.f, 55);

    subtitleText.setFont(fontReg);
    subtitleText.setString(Config::APP_SUBTITLE);
    subtitleText.setCharacterSize(Config::Font::SIZE_SUBTITLE);
    subtitleText.setFillColor(Config::Colors::TEXT_SECONDARY);
    FloatRect sr = subtitleText.getLocalBounds();
    subtitleText.setOrigin(sr.left + sr.width / 2.f, sr.top + sr.height / 2.f);
    subtitleText.setPosition(Config::REF_WIDTH / 2.f, 95);

    authorText.setFont(fontReg);
    authorText.setString("By " + Config::AUTHORS);
    authorText.setCharacterSize(Config::Font::SIZE_SMALL);
    authorText.setFillColor(Color(140, 160, 190, 150));
    FloatRect ar = authorText.getLocalBounds();
    authorText.setOrigin(ar.left + ar.width / 2.f, ar.top + ar.height / 2.f);
    authorText.setPosition(Config::REF_WIDTH / 2.f, Config::REF_HEIGHT - 20);

    // Button labels and layout
    const char* labels[] = {
        "Route Data", "Booking", "Dijkstra (Cost)",
        "A* (Time)", "Custom Ship", "Docking Queue", "Multi-Leg Route"
    };
    Color btnColors[] = {
        Config::Colors::SECONDARY, Config::Colors::SECONDARY, Config::Colors::SECONDARY,
        Config::Colors::SECONDARY, Config::Colors::SECONDARY, Config::Colors::SECONDARY,
        Config::Colors::ACCENT
    };

    float btnW = Config::UI::BUTTON_WIDTH;
    float btnH = Config::UI::BUTTON_HEIGHT;
    float spacing = Config::UI::BUTTON_SPACING;
    float leftX = 80;
    float rightX = Config::REF_WIDTH - btnW - 80;
    float startY = 160;

    // Left column: 0, 1, 2
    for (int i = 0; i < 3; i++) {
        menuButtons[i].setup(Vector2f(btnW, btnH),
                             Vector2f(leftX, startY + i * (btnH + spacing)),
                             labels[i], Config::Font::SIZE_BUTTON, fontReg,
                             btnColors[i], Config::Colors::PRIMARY);
    }
    // Right column: 3, 4, 5
    for (int i = 3; i < 6; i++) {
        menuButtons[i].setup(Vector2f(btnW, btnH),
                             Vector2f(rightX, startY + (i - 3) * (btnH + spacing)),
                             labels[i], Config::Font::SIZE_BUTTON, fontReg,
                             btnColors[i], Config::Colors::PRIMARY);
    }
    // Center bottom: 6
    menuButtons[6].setup(Vector2f(btnW, btnH),
                         Vector2f((Config::REF_WIDTH - btnW) / 2.f, startY + 3 * (btnH + spacing) + 40),
                         labels[6], Config::Font::SIZE_BUTTON, fontReg,
                         btnColors[6], Config::Colors::GOLD);
}

void MainMenuScene::handleEvent(Event& event) {
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
        SceneType targets[] = {
            SceneType::MAP_VIEW, SceneType::BOOKING, SceneType::DIJKSTRA,
            SceneType::ASTAR, SceneType::CUSTOM_SHIP, SceneType::DOCKING,
            SceneType::MULTI_LEG
        };
        for (int i = 0; i < 7; i++) {
            if (menuButtons[i].isClicked(mp)) {
                assets.playClickSound();
                sceneManager.switchTo(targets[i]);
                return;
            }
        }
    }
}

void MainMenuScene::update(float dt) {
    animTime += dt;
    Vector2f mp = Vector2f(Mouse::getPosition().x, Mouse::getPosition().y);
    for (int i = 0; i < 7; i++)
        menuButtons[i].update(dt, mp);
}

void MainMenuScene::render(RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(overlay);

    // Animated wave decoration at bottom
    for (int i = 0; i < 40; i++) {
        float x = (float)i * (Config::REF_WIDTH / 40.f);
        float waveH = 8.f + sin(animTime * Config::Anim::WAVE_SPEED + i * 0.4f) * 6.f;
        RectangleShape wave(Vector2f(Config::REF_WIDTH / 40.f + 1, waveH));
        wave.setPosition(x, Config::REF_HEIGHT - 40 - waveH);
        wave.setFillColor(Color(0, 212, 255, 25));
        window.draw(wave);
    }

    // Title with subtle glow
    Text glowTitle = titleText;
    glowTitle.setFillColor(Color(0, 212, 255, (Uint8)(30 + sin(animTime * 2.f) * 15)));
    glowTitle.setPosition(titleText.getPosition().x + 1, titleText.getPosition().y + 1);
    window.draw(glowTitle);
    window.draw(titleText);
    window.draw(subtitleText);

    for (int i = 0; i < 7; i++)
        menuButtons[i].render(window);

    window.draw(authorText);
}
