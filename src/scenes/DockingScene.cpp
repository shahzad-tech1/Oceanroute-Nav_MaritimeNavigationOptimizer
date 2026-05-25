#include "DockingScene.h"
#include "../core/Constants.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

void DockingScene::onEnter() {
    selectedPort = -1; animationTime = processingTimer = animTime = 0;
    mapSprite.setTexture(assets.getMapTexture());
    mapSprite.setScale(Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x,
                       Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y);
    Font& f = assets.getFont();
    dockingButtons[0].setup(Vector2f(170, 45), Vector2f(Config::REF_WIDTH - 190, 500), "Add Ship", 18, f, Config::Colors::SECONDARY, Config::Colors::PRIMARY);
    dockingButtons[1].setup(Vector2f(170, 45), Vector2f(Config::REF_WIDTH - 190, 555), "Process Ship", 17, f, Config::Colors::SUCCESS, Config::Colors::PRIMARY);
    dockingButtons[2].setup(Vector2f(170, 45), Vector2f(Config::REF_WIDTH - 190, 610), "Complete", 18, f, Config::Colors::DANGER, Config::Colors::ACCENT);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void DockingScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
    selectedPort = -1;
}

void DockingScene::handleEvent(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) { sceneManager.switchTo(SceneType::MAIN_MENU); return; }
    if (event.type != Event::MouseButtonPressed || event.mouseButton.button != Mouse::Left) return;
    Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
    Vector<GraphNode*>& ports = portSystem.getPorts();

    if (dockingButtons[0].isClicked(mp) && selectedPort != -1) {
        Ship s; s.shipName = "Ship_" + to_string(rand() % 1000);
        s.arrivalTime = to_string(rand() % 24) + ":00";
        s.processingTime = 2 + rand() % 6; s.isProcessing = false;
        ports[selectedPort]->dockingQueue.enqueue(s);
        assets.playClickSound(); return;
    }
    if (dockingButtons[1].isClicked(mp) && selectedPort != -1) {
        GraphNode* port = ports[selectedPort];
        if (!port->dockingQueue.isEmpty() && port->processingShips.getSize() < port->availableDocks) {
            Ship s = port->dockingQueue.dequeue(); s.isProcessing = true;
            port->processingShips.push_back(s); assets.playClickSound();
        }
        return;
    }
    if (dockingButtons[2].isClicked(mp) && selectedPort != -1) {
        GraphNode* port = ports[selectedPort];
        if (port->processingShips.getSize() > 0) {
            Vector<Ship> np; for (int i = 1; i < port->processingShips.getSize(); i++) np.push_back(port->processingShips[i]);
            port->processingShips = np; assets.playClickSound();
        }
        return;
    }
    for (int i = 0; i < ports.getSize(); i++) {
        if (ports[i]->circle.getGlobalBounds().contains(mp)) {
            selectedPort = i; assets.playClickSound(); break;
        }
    }
}

void DockingScene::update(float dt) {
    animTime += dt;
    animationTime += dockingClock.restart().asSeconds();
    processingTimer += processingClock.restart().asSeconds();
    Vector2f mp = (Vector2f)Mouse::getPosition();
    for (int i = 0; i < 3; i++) dockingButtons[i].update(dt, mp);

    if (processingTimer >= 1.0f && selectedPort != -1) {
        processingTimer = 0;
        Vector<GraphNode*>& ports = portSystem.getPorts();
        GraphNode* port = ports[selectedPort];
        for (int i = 0; i < port->processingShips.getSize(); i++) {
            port->processingShips[i].processingProgress += 1;
            if (port->processingShips[i].processingProgress >= port->processingShips[i].processingTime) {
                Vector<Ship> np; for (int j = 0; j < port->processingShips.getSize(); j++) if (j != i) np.push_back(port->processingShips[j]);
                port->processingShips = np;
                if (!port->dockingQueue.isEmpty() && port->processingShips.getSize() < port->availableDocks) {
                    Ship ns = port->dockingQueue.dequeue(); ns.isProcessing = true; ns.processingProgress = 0;
                    port->processingShips.push_back(ns);
                }
                break;
            }
        }
    }
    if (animationTime > 1.0f) animationTime = 0;
}

void DockingScene::render(RenderWindow& window) {
    window.draw(mapSprite);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) {
        ports[i]->circle.setFillColor(i == selectedPort ? Config::Colors::PORT_SELECTED : Config::Colors::PORT_DEFAULT);
        window.draw(ports[i]->circle);
    }

    if (selectedPort != -1) {
        GraphNode* port = ports[selectedPort];
        Vector2f pp = port->circle.getPosition();
        // Processing ships
        for (int i = 0; i < port->processingShips.getSize(); i++) {
            Ship& s = port->processingShips[i];
            CircleShape sc(8); sc.setFillColor(Config::Colors::SUCCESS); sc.setOrigin(8, 8);
            sc.setPosition(pp.x + 30 + i * 28, pp.y); window.draw(sc);
            Text sl; sl.setFont(assets.getFont()); sl.setString("D" + to_string(i+1));
            sl.setCharacterSize(10); sl.setFillColor(Config::Colors::TEXT_PRIMARY);
            sl.setPosition(pp.x + 24 + i * 28, pp.y - 6); window.draw(sl);
            RectangleShape pbg(Vector2f(22, 4)); pbg.setPosition(pp.x + 28 + i * 28, pp.y + 12);
            pbg.setFillColor(Color(60, 60, 80)); window.draw(pbg);
            float prog = s.processingProgress / (float)s.processingTime;
            RectangleShape pb(Vector2f(22 * prog, 4)); pb.setPosition(pp.x + 28 + i * 28, pp.y + 12);
            pb.setFillColor(Config::Colors::SUCCESS); window.draw(pb);
        }
        // Queue with wave animation
        int qs = port->dockingQueue.getSize();
        for (int q = 0; q < qs; q++) {
            float ty = 40 + q * 22;
            float wave = sin(animationTime * 6.28f) * 5;
            Color dc = (q == 0 && port->processingShips.getSize() < port->availableDocks) ? Config::Colors::GOLD : Config::Colors::PRIMARY;
            for (float t = 0; t < 1.0f; t += 0.08f) {
                int di = (int)(t * 12.5f);
                if ((di + (int)(animationTime * 10)) % 2 == 0) {
                    Renderer::drawThickLine(window, Vector2f(pp.x + t * 50, pp.y + ty), Vector2f(pp.x + (t+0.04f)*50, pp.y + ty), 1.5f, dc);
                }
            }
            CircleShape qc(6); qc.setFillColor(dc); qc.setOrigin(6, 6);
            qc.setPosition(pp.x + 50 + wave, pp.y + ty); window.draw(qc);
            Text ql; ql.setFont(assets.getFont()); ql.setString("Q" + to_string(q+1));
            ql.setCharacterSize(10); ql.setFillColor(Config::Colors::TEXT_PRIMARY);
            ql.setPosition(pp.x + 44, pp.y + ty - 5); window.draw(ql);
        }
        // Info panel
        RectangleShape ib(Vector2f(350, 220)); ib.setFillColor(Config::Colors::PANEL_BG);
        ib.setOutlineColor(Config::Colors::GOLD); ib.setOutlineThickness(2); ib.setPosition(15, 460); window.draw(ib);
        Text info; info.setFont(assets.getFont()); info.setCharacterSize(14);
        info.setFillColor(Config::Colors::TEXT_PRIMARY);
        string is = "Port: " + port->portName + "\nDocks: " + to_string(port->availableDocks) +
                     "\nProcessing: " + to_string(port->processingShips.getSize()) +
                     "\nQueue: " + to_string(qs);
        if (port->processingShips.getSize() > 0) {
            is += "\n\nProcessing:";
            for (int i = 0; i < port->processingShips.getSize(); i++) {
                int rem = port->processingShips[i].processingTime - (int)port->processingShips[i].processingProgress;
                is += "\n  D" + to_string(i+1) + ": " + to_string(rem) + "h left";
            }
        }
        is += "\n\n1 sec = 1 hour simulation";
        info.setString(is); info.setPosition(25, 468); window.draw(info);
    }

    for (int i = 0; i < 3; i++) dockingButtons[i].render(window);
    MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());
    Text inst; inst.setFont(assets.getFont());
    inst.setString("Click port to select | ESC to return");
    inst.setCharacterSize(Config::Font::SIZE_SMALL); inst.setFillColor(Config::Colors::TEXT_SECONDARY);
    inst.setPosition(Config::REF_WIDTH - 300, 15); window.draw(inst);
}
