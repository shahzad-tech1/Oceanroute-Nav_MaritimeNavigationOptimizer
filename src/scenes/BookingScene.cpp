#include "BookingScene.h"
#include "../core/Constants.h"
#include <iostream>
#include <cmath>
using namespace std;

void BookingScene::onEnter() {
    sourcePort = destPort = selectedRoute = -1;
    loadFlag = 1; displayBooking = false; animTime = 0;
    pathPoints = Vector<Vector<Vertex>>();
    pathRoutes = Vector<Vector<Routes>>();
    mapSprite.setTexture(assets.getMapTexture());
    float sx = Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x;
    float sy = Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y;
    mapSprite.setScale(sx, sy);
    Font& f = assets.getFont();
    bookingButtons[0].setup(Vector2f(180, 45), Vector2f(20, 550), "Book", 18, f, Config::Colors::SUCCESS, Config::Colors::PRIMARY);
    bookingButtons[1].setup(Vector2f(180, 45), Vector2f(20, 605), "Display Bookings", 16, f, Config::Colors::SECONDARY, Config::Colors::PRIMARY);
    bookingButtons[2].setup(Vector2f(180, 45), Vector2f(20, 660), "Back", 18, f, Config::Colors::DANGER, Config::Colors::ACCENT);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++)
        ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void BookingScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++)
        ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void BookingScene::handleEvent(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
        sceneManager.switchTo(SceneType::MAIN_MENU);
        return;
    }
    if (event.type != Event::MouseButtonPressed || event.mouseButton.button != Mouse::Left) return;
    Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
    Vector<GraphNode*>& ports = portSystem.getPorts();

    // Button clicks (only when a route is selected)
    if (selectedRoute > -1) {
        if (bookingButtons[0].isClicked(mp)) { // Book
            assets.playClickSound();
            if (selectedRoute >= 0 && selectedRoute < pathRoutes.getSize()) {
                for (int i = 0; i < pathRoutes[selectedRoute].getSize(); i++)
                    user.setVoyages(pathRoutes[selectedRoute][i]);
                user.setUserEdges(pathPoints[selectedRoute]);
                cout << "Booking successful for path " << selectedRoute << endl;
            }
            return;
        }
        if (bookingButtons[1].isClicked(mp)) { // Display bookings
            assets.playClickSound();
            displayBooking = true;
            return;
        }
    }
    if (bookingButtons[2].isClicked(mp)) { // Back
        assets.playClickSound();
        sceneManager.switchTo(SceneType::MAIN_MENU);
        return;
    }

    // Port selection
    for (int i = 0; i < ports.getSize(); i++) {
        if (ports[i]->circle.getGlobalBounds().contains(mp)) {
            assets.playClickSound();
            loadFlag = 1;
            while (pathPoints.getSize() > 0) pathPoints.pop_back();
            while (pathRoutes.getSize() > 0) pathRoutes.pop_back();
            selectedRoute = -1;
            if (sourcePort == i) { ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT); sourcePort = -1; return; }
            if (destPort == i) { ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT); destPort = -1; return; }
            if (sourcePort > -1 && destPort > -1) ports[destPort]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
            else if (sourcePort > -1) ports[sourcePort]->circle.setFillColor(Config::Colors::PORT_START);
            if (sourcePort < 0) { ports[i]->circle.setFillColor(Config::Colors::PORT_START); sourcePort = i; }
            else { ports[i]->circle.setFillColor(Config::Colors::PORT_END); destPort = i; }
            return;
        }
    }

    // Path selection (click near a path line)
    if (pathPoints.getSize() > 0) {
        for (int i = 0; i < pathPoints.getSize(); i++) {
            for (int j = 0; j + 1 < pathPoints[i].getSize(); j++) {
                if (MapRenderer::isMouseNearLine(mp, pathPoints[i][j].position, pathPoints[i][j+1].position, 6)) {
                    assets.playClickSound();
                    selectedRoute = (selectedRoute == i) ? -1 : i;
                    return;
                }
            }
        }
    }
}

void BookingScene::update(float dt) {
    animTime += dt;
    Vector2f mp = (Vector2f)Mouse::getPosition();
    for (int i = 0; i < 3; i++) bookingButtons[i].update(dt, mp);
}

void BookingScene::drawPathsThick(RenderWindow& window, Vector<Vector<Vertex>>& points, int rcolor, float thickness) {
    for (int pi = 0; pi < points.getSize(); pi++) {
        if (points[pi].getSize() < 2) continue;
        Color routeColor = Config::Colors::PATH_PALETTE[pi % Config::Colors::PATH_PALETTE_SIZE];
        if (rcolor > -1) routeColor = Config::Colors::GOLD;
        for (int s = 0; s + 1 < points[pi].getSize(); s++)
            Renderer::drawThickLine(window, points[pi][s].position, points[pi][s+1].position, thickness, routeColor);
    }
}

void BookingScene::drawFullPathInfo(RenderWindow& window, Vector<Vector<Routes>>& routes, Vector<Vector<Vertex>>& points) {
    if (points.getSize() == 0 || routes.getSize() == 0) return;
    Vector2f mp = (Vector2f)Mouse::getPosition(window);
    for (int i = 0; i < points.getSize(); i++) {
        for (int j = 0; j + 1 < points[i].getSize(); j++) {
            if (MapRenderer::isMouseNearLine(mp, points[i][j].position, points[i][j+1].position, 6)) {
                Vector<Routes>& cp = routes[i];
                string route = ""; int totalCost = 0;
                for (int k = 0; k < cp.getSize(); k++) {
                    if (route.empty()) route = cp[k].currentPort;
                    route += " -> " + cp[k].destination;
                    totalCost += cp[k].cost;
                }
                float fx = mp.x + 15, fy = mp.y - 60;
                if (mp.x > 700) fx = mp.x - 450;
                RectangleShape box(Vector2f(440, 80));
                box.setFillColor(Config::Colors::PANEL_BG);
                box.setOutlineColor(Config::Colors::PRIMARY);
                box.setOutlineThickness(2);
                box.setPosition(fx, fy);
                Text t; t.setFont(assets.getFont());
                t.setCharacterSize(Config::Font::SIZE_SMALL);
                t.setFillColor(Config::Colors::TEXT_PRIMARY);
                string s = route + "\nCost: $" + to_string(totalCost);
                if (cp.getSize() > 0) s += " | " + cp[0].date + " " + cp[0].departureTime;
                t.setString(s); t.setPosition(fx + 8, fy + 8);
                window.draw(box); window.draw(t);
                return;
            }
        }
    }
}

void BookingScene::render(RenderWindow& window) {
    window.draw(mapSprite);
    Vector<GraphNode*>& ports = portSystem.getPorts();

    if (displayBooking) {
        Vector<Vector<Vertex>>& ue = user.getUserEdges();
        drawPathsThick(window, ue, -1, 4);
    } else {
        if (sourcePort > -1 && destPort > -1) {
            if (loadFlag == 1) {
                portSystem.printAllValidFlightPaths(sourcePort, destPort, ports, pathPoints, pathRoutes);
                loadFlag = 0;
            }
        }
        if (pathPoints.getSize() > 0) {
            drawPathsThick(window, pathPoints, -1, 2);
            if (selectedRoute > -1 && selectedRoute < pathPoints.getSize()) {
                Vector<Vector<Vertex>> hl; hl.push_back(pathPoints[selectedRoute]);
                drawPathsThick(window, hl, selectedRoute, 5);
            }
            drawFullPathInfo(window, pathRoutes, pathPoints);
        }
        MapRenderer::drawEdges(window, ports, sourcePort);
    }

    MapRenderer::drawPorts(window, ports, assets.getFont(), animTime);
    MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());
    for (int i = 0; i < 3; i++) bookingButtons[i].render(window);

    Text inst; inst.setFont(assets.getFont());
    inst.setString("Select source & destination ports | ESC to return");
    inst.setCharacterSize(Config::Font::SIZE_SMALL);
    inst.setFillColor(Config::Colors::TEXT_SECONDARY);
    inst.setPosition(10, 10);
    window.draw(inst);
}
