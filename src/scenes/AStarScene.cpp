#include "AStarScene.h"
#include "../core/Constants.h"

void AStarScene::onEnter() {
    startPort = endPort = -1; exploreStep = 0; animTime = stepTimer = 0;
    autoPlay = true; pathFound = false;
    exploredOrder = Vector<int>(); finalPath = Vector<int>();
    mapSprite.setTexture(assets.getMapTexture());
    mapSprite.setScale(Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x,
                       Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y);
    infoPanel.setup(Vector2f(300, 200), Vector2f(Config::REF_WIDTH - 320, 20), "A* Info", assets.getFont());
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void AStarScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void AStarScene::handleEvent(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) { sceneManager.switchTo(SceneType::MAIN_MENU); return; }
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) { autoPlay = !autoPlay; return; }
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
        Vector<GraphNode*>& ports = portSystem.getPorts();
        for (int i = 0; i < ports.getSize(); i++) {
            if (ports[i]->circle.getGlobalBounds().contains(mp)) {
                assets.playClickSound();
                if (startPort == -1) { startPort = i; ports[i]->circle.setFillColor(Config::Colors::PORT_START); }
                else if (endPort == -1 && i != startPort) {
                    endPort = i; ports[i]->circle.setFillColor(Config::Colors::PORT_END);
                    portSystem.shortestPathAStar(ports[startPort]->portName, ports[endPort]->portName, finalPath, exploredOrder);
                    exploreStep = 0; pathFound = true; stepTimer = 0;
                }
                return;
            }
        }
    }
}

void AStarScene::update(float dt) {
    animTime += dt; infoPanel.update(dt);
    if (pathFound && autoPlay && exploreStep < exploredOrder.getSize()) {
        stepTimer += dt;
        if (stepTimer >= stepInterval) { stepTimer = 0; exploreStep++; }
    }
}

void AStarScene::render(RenderWindow& window) {
    window.draw(mapSprite);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < exploreStep && i < exploredOrder.getSize(); i++) {
        int idx = exploredOrder[i];
        if (idx != startPort && idx != endPort) ports[idx]->circle.setFillColor(Config::Colors::PORT_EXPLORED);
        if (i == exploreStep - 1) Renderer::drawGlowCircle(window, ports[idx]->circle.getPosition(), 15, Config::Colors::PORT_EXPLORED, 2);
    }
    // Heuristic line from current to goal
    if (pathFound && endPort > -1 && exploreStep > 0 && exploreStep <= exploredOrder.getSize()) {
        int cur = exploredOrder[min(exploreStep - 1, exploredOrder.getSize() - 1)];
        Renderer::drawDashedLine(window, ports[cur]->circle.getPosition(), ports[endPort]->circle.getPosition(), 8, 6, 1.5f, Color(255, 107, 53, 80));
    }
    if (exploreStep >= exploredOrder.getSize() && finalPath.getSize() > 0) {
        for (int i = 0; i < finalPath.getSize(); i++) ports[finalPath[i]]->circle.setFillColor(Config::Colors::PORT_PATH);
        if (startPort > -1) ports[startPort]->circle.setFillColor(Config::Colors::PORT_START);
        if (endPort > -1) ports[endPort]->circle.setFillColor(Config::Colors::PORT_END);
        MapRenderer::drawPath(window, ports, finalPath, Color(200, 0, 255), 4.f);
    }
    MapRenderer::drawPorts(window, ports, assets.getFont(), animTime);
    MapRenderer::drawPortLabels(window, ports, assets.getFont());
    MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());
    infoPanel.render(window);
    Vector2f cp = infoPanel.getContentPosition();
    Text info; info.setFont(assets.getFont());
    info.setCharacterSize(Config::Font::SIZE_SMALL); info.setFillColor(Config::Colors::TEXT_PRIMARY);
    string s = "Algorithm: A* (Time)\nStatus: " + string(pathFound ? (exploreStep >= exploredOrder.getSize() ? "Complete" : "Exploring...") : "Select 2 ports");
    s += "\nExplored: " + to_string(exploreStep) + "/" + to_string(exploredOrder.getSize());
    s += "\nPath: " + to_string(finalPath.getSize()) + " ports";
    s += "\n\nSPACE: " + string(autoPlay ? "Pause" : "Play") + "\nESC: Return";
    info.setString(s); info.setPosition(cp); window.draw(info);
}
