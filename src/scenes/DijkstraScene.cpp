#include "DijkstraScene.h"
#include "../core/Constants.h"
#include <iostream>
using namespace std;

void DijkstraScene::onEnter() {
    startPort = endPort = -1; exploreStep = 0; animTime = stepTimer = 0;
    autoPlay = true; pathFound = false;
    exploredOrder = Vector<int>(); finalPath = Vector<int>();
    mapSprite.setTexture(assets.getMapTexture());
    mapSprite.setScale(Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x,
                       Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y);
    infoPanel.setup(Vector2f(300, 200), Vector2f(Config::REF_WIDTH - 320, 20), "Dijkstra Info", assets.getFont());
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void DijkstraScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void DijkstraScene::handleEvent(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
        sceneManager.switchTo(SceneType::MAIN_MENU); return;
    }
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
        autoPlay = !autoPlay; return;
    }
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
        Vector<GraphNode*>& ports = portSystem.getPorts();
        for (int i = 0; i < ports.getSize(); i++) {
            if (ports[i]->circle.getGlobalBounds().contains(mp)) {
                assets.playClickSound();
                if (startPort == -1) { startPort = i; ports[i]->circle.setFillColor(Config::Colors::PORT_START); }
                else if (endPort == -1 && i != startPort) {
                    endPort = i; ports[i]->circle.setFillColor(Config::Colors::PORT_END);
                    portSystem.shortestPathDijkstra(ports[startPort]->portName, ports[endPort]->portName, finalPath, exploredOrder);
                    exploreStep = 0; pathFound = true; stepTimer = 0;
                }
                return;
            }
        }
    }
}

void DijkstraScene::update(float dt) {
    animTime += dt;
    infoPanel.update(dt);
    if (pathFound && autoPlay && exploreStep < exploredOrder.getSize()) {
        stepTimer += dt;
        if (stepTimer >= stepInterval) { stepTimer = 0; exploreStep++; }
    }
}

void DijkstraScene::render(RenderWindow& window) {
    window.draw(mapSprite);
    Vector<GraphNode*>& ports = portSystem.getPorts();

    // Color explored nodes progressively
    for (int i = 0; i < exploreStep && i < exploredOrder.getSize(); i++) {
        int idx = exploredOrder[i];
        if (idx != startPort && idx != endPort)
            ports[idx]->circle.setFillColor(Config::Colors::PORT_EXPLORED);
        // Expanding ring effect for latest explored
        if (i == exploreStep - 1) {
            float ringSize = (stepTimer / stepInterval) * 20.f;
            Renderer::drawGlowCircle(window, ports[idx]->circle.getPosition(), ringSize, Config::Colors::PORT_EXPLORED, 2);
        }
    }

    // Draw final path if exploration complete
    if (exploreStep >= exploredOrder.getSize() && finalPath.getSize() > 0) {
        for (int i = 0; i < finalPath.getSize(); i++)
            ports[finalPath[i]]->circle.setFillColor(Config::Colors::PORT_PATH);
        if (startPort > -1) ports[startPort]->circle.setFillColor(Config::Colors::PORT_START);
        if (endPort > -1) ports[endPort]->circle.setFillColor(Config::Colors::PORT_END);
        MapRenderer::drawPath(window, ports, finalPath, Config::Colors::PORT_PATH, 4.f);
    }

    MapRenderer::drawPorts(window, ports, assets.getFont(), animTime);
    MapRenderer::drawPortLabels(window, ports, assets.getFont());
    MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());

    // Info panel
    infoPanel.render(window);
    Vector2f cp = infoPanel.getContentPosition();
    Text info; info.setFont(assets.getFont());
    info.setCharacterSize(Config::Font::SIZE_SMALL);
    info.setFillColor(Config::Colors::TEXT_PRIMARY);
    string s = "Algorithm: Dijkstra (Cost)\n";
    s += "Status: " + string(pathFound ? (exploreStep >= exploredOrder.getSize() ? "Complete" : "Exploring...") : "Select 2 ports") + "\n";
    s += "Explored: " + to_string(exploreStep) + "/" + to_string(exploredOrder.getSize()) + "\n";
    s += "Path Length: " + to_string(finalPath.getSize()) + " ports\n";
    s += "\nSPACE: " + string(autoPlay ? "Pause" : "Play");
    s += "\nESC: Return to menu";
    info.setString(s); info.setPosition(cp);
    window.draw(info);
}
