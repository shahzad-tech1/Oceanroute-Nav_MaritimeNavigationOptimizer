#include "MultiLegScene.h"
#include "../core/Constants.h"
#include <iostream>
#include <cmath>
using namespace std;

void MultiLegScene::onEnter() {
    selectedSegment = -1; isInsertMode = isDeleteMode = multiRouteBuilt = false;
    scrollOffset = 0; animTime = 0;
    multiLegSeq = Vector<int>();
    routeList.clear();
    mapSprite.setTexture(assets.getMapTexture());
    mapSprite.setScale(Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x,
                       Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y);
    Font& f = assets.getFont();
    multiButtons[0].setup(Vector2f(170, 42), Vector2f(Config::REF_WIDTH - 190, 500), "Clear All", 17, f, Config::Colors::DANGER, Config::Colors::ACCENT);
    multiButtons[1].setup(Vector2f(170, 42), Vector2f(Config::REF_WIDTH - 190, 550), "Remove Last", 17, f, Color(200, 150, 100), Config::Colors::ACCENT);
    multiButtons[2].setup(Vector2f(170, 42), Vector2f(Config::REF_WIDTH - 190, 600), "Toggle Mode", 17, f, Config::Colors::SUCCESS, Config::Colors::PRIMARY);
    multiButtons[3].setup(Vector2f(170, 42), Vector2f(Config::REF_WIDTH - 190, 650), "Build Route", 17, f, Config::Colors::SECONDARY, Config::Colors::PRIMARY);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void MultiLegScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
    routeList.clear();
}

bool MultiLegScene::buildMultiLegRoute(Vector<string>& portSequence) {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    if (portSequence.getSize() == 0) return false;
    GraphNode* start = ports[searchNode(ports, portSequence[0])];
    routeList.addStart(start);
    Routes* last = nullptr;
    for (int i = 1; i < portSequence.getSize(); i++) {
        GraphNode* curr = ports[searchNode(ports, portSequence[i - 1])];
        GraphNode* next = ports[searchNode(ports, portSequence[i])];
        bool foundValid = false; Routes chosen;
        for (int f = 0; f < curr->trips.getSize(); f++) {
            Routes& fl = curr->trips[f];
            if (fl.destination != next->portName) continue;
            if (last != nullptr && !isConnectionValid(*last, fl)) continue;
            foundValid = true; chosen = fl; break;
        }
        if (!foundValid) { multiRouteBuilt = false; return false; }
        routeList.addStop(next, chosen);
        last = &routeList.getLast()->flight;
    }
    multiRouteBuilt = true;
    return true;
}

void MultiLegScene::handleEvent(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) { sceneManager.switchTo(SceneType::MAIN_MENU); return; }
    if (event.type == Event::MouseWheelScrolled) {
        scrollOffset += (int)(event.mouseWheelScroll.delta * 20);
        int maxS = (multiLegSeq.getSize() * 25) - 250; if (maxS < 0) maxS = 0;
        if (scrollOffset > 0) scrollOffset = 0;
        if (scrollOffset < -maxS) scrollOffset = -maxS;
        return;
    }
    if (event.type != Event::MouseButtonPressed || event.mouseButton.button != Mouse::Left) return;
    Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
    Vector<GraphNode*>& ports = portSystem.getPorts();

    // Button handling
    if (multiButtons[0].isClicked(mp)) {
        while (multiLegSeq.getSize() > 0) multiLegSeq.pop_back();
        routeList.clear(); selectedSegment = -1; isInsertMode = isDeleteMode = multiRouteBuilt = false;
        assets.playClickSound(); return;
    }
    if (multiButtons[1].isClicked(mp)) {
        if (multiLegSeq.getSize() > 0) { multiLegSeq.pop_back(); routeList.clear(); multiRouteBuilt = false; assets.playClickSound(); } return;
    }
    if (multiButtons[2].isClicked(mp) && multiLegSeq.getSize() >= 2) {
        if (!isInsertMode && !isDeleteMode) { isInsertMode = true; isDeleteMode = false; multiRouteBuilt = false; }
        else if (isInsertMode) { isInsertMode = false; isDeleteMode = true; multiRouteBuilt = false; }
        else { isInsertMode = isDeleteMode = false; }
        selectedSegment = -1; assets.playClickSound(); return;
    }
    if (multiButtons[3].isClicked(mp) && multiLegSeq.getSize() >= 2) {
        routeList.clear();
        Vector<string> names;
        for (int i = 0; i < multiLegSeq.getSize(); i++) names.push_back(ports[multiLegSeq[i]]->portName);
        bool ok = buildMultiLegRoute(names);
        cout << (ok ? "Route built successfully!" : "Failed - no valid connections!") << endl;
        assets.playClickSound(); return;
    }

    // Delete mode: click port to remove
    if (isDeleteMode) {
        for (int i = 0; i < multiLegSeq.getSize(); i++) {
            Vector2f pc = ports[multiLegSeq[i]]->circle.getPosition();
            float dx = mp.x - pc.x, dy = mp.y - pc.y;
            if (sqrt(dx*dx + dy*dy) <= ports[multiLegSeq[i]]->circle.getRadius() + 5) {
                if (multiLegSeq.getSize() <= 1) return;
                Vector<int> ns; for (int j = 0; j < multiLegSeq.getSize(); j++) if (j != i) ns.push_back(multiLegSeq[j]);
                multiLegSeq = ns; routeList.clear(); multiRouteBuilt = false; assets.playClickSound(); return;
            }
        }
        return;
    }

    // Port clicks
    for (int i = 0; i < ports.getSize(); i++) {
        if (ports[i]->circle.getGlobalBounds().contains(mp)) {
            assets.playClickSound();
            if (!isInsertMode) { multiLegSeq.push_back(i); routeList.clear(); multiRouteBuilt = false; }
            else if (selectedSegment >= 0 && selectedSegment < multiLegSeq.getSize() - 1) {
                Vector<int> ns;
                for (int j = 0; j <= selectedSegment; j++) ns.push_back(multiLegSeq[j]);
                ns.push_back(i);
                for (int j = selectedSegment + 1; j < multiLegSeq.getSize(); j++) ns.push_back(multiLegSeq[j]);
                multiLegSeq = ns; selectedSegment = -1; isInsertMode = false;
                routeList.clear(); multiRouteBuilt = false;
            }
            return;
        }
    }

    // Segment click for insert mode
    if (isInsertMode && multiLegSeq.getSize() >= 2) {
        for (int i = 0; i < multiLegSeq.getSize() - 1; i++) {
            Vector2f pa = ports[multiLegSeq[i]]->circle.getPosition();
            Vector2f pb = ports[multiLegSeq[i + 1]]->circle.getPosition();
            if (MapRenderer::isMouseNearLine(mp, pa, pb, 10)) {
                selectedSegment = i; assets.playClickSound(); return;
            }
        }
    }
}

void MultiLegScene::update(float dt) {
    animTime += dt;
    Vector2f mp = (Vector2f)Mouse::getPosition();
    for (int i = 0; i < 4; i++) multiButtons[i].update(dt, mp);
}

void MultiLegScene::render(RenderWindow& window) {
    window.draw(mapSprite);
    Vector<GraphNode*>& ports = portSystem.getPorts();

    // Color ports based on role in route
    for (int i = 0; i < ports.getSize(); i++) {
        Color c = Config::Colors::PORT_DEFAULT;
        for (int j = 0; j < multiLegSeq.getSize(); j++) {
            if (multiLegSeq[j] == i) {
                if (j == 0) c = Config::Colors::PORT_START;
                else if (j == multiLegSeq.getSize() - 1) c = Config::Colors::PORT_END;
                else c = Config::Colors::PORT_EXPLORED;
                break;
            }
        }
        ports[i]->circle.setFillColor(c);
        window.draw(ports[i]->circle);
    }

    // Draw route segments with arrows and numbering
    for (int i = 0; i + 1 < multiLegSeq.getSize(); i++) {
        Vector2f pa = ports[multiLegSeq[i]]->circle.getPosition();
        Vector2f pb = ports[multiLegSeq[i + 1]]->circle.getPosition();
        Color lc = Config::Colors::PORT_START;
        if (isInsertMode && selectedSegment == i) lc = Color(200, 0, 255);
        else if (isDeleteMode) lc = Config::Colors::DANGER;
        else if (multiRouteBuilt) lc = Config::Colors::SECONDARY;
        Renderer::drawArrow(window, pa, pb, 4.f, 14.f, lc);
        // Segment number
        Vector2f mid((pa.x + pb.x) / 2, (pa.y + pb.y) / 2);
        CircleShape nc(12); nc.setFillColor(Color(0, 0, 0, 200));
        nc.setOutlineColor(lc); nc.setOutlineThickness(2); nc.setOrigin(12, 12);
        nc.setPosition(mid); window.draw(nc);
        Text sn; sn.setFont(assets.getFont()); sn.setString(to_string(i + 1));
        sn.setCharacterSize(13); sn.setFillColor(Config::Colors::TEXT_PRIMARY); sn.setStyle(Text::Bold);
        FloatRect sb = sn.getLocalBounds(); sn.setOrigin(sb.left + sb.width / 2, sb.top + sb.height / 2);
        sn.setPosition(mid); window.draw(sn);
    }

    // Info panel
    RectangleShape ib(Vector2f(350, 300)); ib.setFillColor(Config::Colors::PANEL_BG);
    ib.setOutlineColor(Config::Colors::PORT_START); ib.setOutlineThickness(2); ib.setPosition(15, 380);
    window.draw(ib);
    string modeStr = (!isInsertMode && !isDeleteMode) ? "ADD" : (isInsertMode ? "INSERT" : "DELETE");
    string infoStr = "Multi-Leg Route Builder\n====================\nMode: " + modeStr +
                     "\nPorts: " + to_string(multiLegSeq.getSize()) +
                     "\nSegments: " + to_string(multiLegSeq.getSize() > 0 ? multiLegSeq.getSize() - 1 : 0) + "\n";
    if (multiLegSeq.getSize() > 0) {
        infoStr += "\nRoute:\n";
        for (int i = 0; i < multiLegSeq.getSize(); i++) {
            infoStr += to_string(i + 1) + ". " + ports[multiLegSeq[i]]->portName;
            if (i < multiLegSeq.getSize() - 1) infoStr += " ->\n";
        }
    } else { infoStr += "\nClick ports to build route"; }
    infoStr += "\n\nGreen=Start Yellow=Waypoint\nOrange=End";
    Text info; info.setFont(assets.getFont()); info.setCharacterSize(13);
    info.setFillColor(Config::Colors::TEXT_PRIMARY);
    info.setString(infoStr); info.setPosition(25, 388 + scrollOffset);
    window.draw(info);

    for (int i = 0; i < 4; i++) multiButtons[i].render(window);
    MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());
}
