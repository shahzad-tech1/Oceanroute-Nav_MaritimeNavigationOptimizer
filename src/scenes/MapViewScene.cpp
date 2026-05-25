#include "MapViewScene.h"
#include "../core/Constants.h"

void MapViewScene::onEnter() {
    currentPort = -1;
    animTime = 0;
    mapSprite.setTexture(assets.getMapTexture());
    float sx = Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x;
    float sy = Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y;
    mapSprite.setScale(sx, sy);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++)
        ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void MapViewScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++)
        ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
    currentPort = -1;
}

void MapViewScene::handleEvent(Event& event) {
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
        Vector<GraphNode*>& ports = portSystem.getPorts();
        for (int i = 0; i < ports.getSize(); i++) {
            if (ports[i]->circle.getGlobalBounds().contains(mp)) {
                assets.playClickSound();
                if (currentPort == i) { currentPort = -1; }
                else {
                    if (currentPort > -1) {
                        ports[currentPort]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
                        for (int j = 0; j < ports[currentPort]->destinations.getSize(); j++)
                            ports[currentPort]->destinations[j]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
                    }
                    currentPort = i;
                    ports[i]->circle.setFillColor(Config::Colors::PORT_SELECTED);
                    for (int j = 0; j < ports[i]->destinations.getSize(); j++)
                        ports[i]->destinations[j]->circle.setFillColor(Config::Colors::PORT_START);
                }
                return;
            }
        }
    }
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
        sceneManager.switchTo(SceneType::MAIN_MENU);
    }
}

void MapViewScene::update(float dt) { animTime += dt; }

void MapViewScene::render(RenderWindow& window) {
    window.draw(mapSprite);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    MapRenderer::drawEdges(window, ports, currentPort);
    MapRenderer::drawPorts(window, ports, assets.getFont(), animTime);
    MapRenderer::drawPortLabels(window, ports, assets.getFont());
    Vector2f mp = (Vector2f)Mouse::getPosition(window);
    MapRenderer::drawNodeTooltip(window, ports, mp, assets.getFont());
    MapRenderer::drawEdgeTooltip(window, ports, mp, assets.getFont(), currentPort);

    // Instructions
    Text inst;
    inst.setFont(assets.getFont());
    inst.setString("Click ports to explore | ESC to return");
    inst.setCharacterSize(Config::Font::SIZE_SMALL);
    inst.setFillColor(Config::Colors::TEXT_SECONDARY);
    inst.setPosition(Config::REF_WIDTH / 2.f - 150, Config::REF_HEIGHT - 25);
    window.draw(inst);
}
