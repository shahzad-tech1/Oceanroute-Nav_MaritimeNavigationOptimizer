#include "CustomShipScene.h"
#include "../core/Constants.h"
#include <iostream>
using namespace std;

void CustomShipScene::onEnter() {
    startPort = endPort = -1; companyValidated = false; selectingExcluded = false;
    showInvalid = false; inputCompany = ""; preferredCompany = ""; animTime = 0;
    excludedPorts = Vector<int>(); customPath = Vector<int>(); customExplored = Vector<int>();
    mapSprite.setTexture(assets.getMapTexture());
    mapSprite.setScale(Config::REF_WIDTH / (float)assets.getMapTexture().getSize().x,
                       Config::REF_HEIGHT / (float)assets.getMapTexture().getSize().y);
    okButton.setup(Vector2f(150, 50), Vector2f(Config::REF_WIDTH - 180, Config::REF_HEIGHT - 80),
                   "Find Path", 18, assets.getFont(), Config::Colors::SUCCESS, Config::Colors::PRIMARY);
    companyInput.setup(Vector2f(400, 50), Vector2f(Config::REF_WIDTH/2 - 200, 300), "Enter company name...", assets.getFont());
    companyInput.setFocused(true);
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void CustomShipScene::onExit() {
    Vector<GraphNode*>& ports = portSystem.getPorts();
    for (int i = 0; i < ports.getSize(); i++) ports[i]->circle.setFillColor(Config::Colors::PORT_DEFAULT);
}

void CustomShipScene::handleEvent(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) { sceneManager.switchTo(SceneType::MAIN_MENU); return; }

    if (!companyValidated) {
        companyInput.handleEvent(event);
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Return) {
            string val = companyInput.getValue();
            if (portSystem.isValidShippingCompany(val)) {
                preferredCompany = val; companyValidated = true; selectingExcluded = true;
                showInvalid = false; assets.playClickSound();
            } else { showInvalid = true; }
        }
    } else if (selectingExcluded) {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
            if (okButton.isClicked(mp) && startPort != -1 && endPort != -1) {
                Vector<GraphNode*>& ports = portSystem.getPorts();
                portSystem.customShortestPath(ports[startPort]->portName, ports[endPort]->portName,
                                              preferredCompany, excludedPorts, customPath, customExplored);
                selectingExcluded = false; assets.playClickSound(); return;
            }
            Vector<GraphNode*>& ports = portSystem.getPorts();
            for (int i = 0; i < ports.getSize(); i++) {
                if (ports[i]->circle.getGlobalBounds().contains(mp)) {
                    assets.playClickSound();
                    if (startPort == -1) { startPort = i; }
                    else if (endPort == -1 && i != startPort) { endPort = i; }
                    else {
                        bool found = false;
                        for (int j = 0; j < excludedPorts.getSize(); j++) {
                            if (excludedPorts[j] == i) {
                                Vector<int> ne; for (int k = 0; k < excludedPorts.getSize(); k++) if (k != j) ne.push_back(excludedPorts[k]);
                                excludedPorts = ne; found = true; break;
                            }
                        }
                        if (!found && i != startPort && i != endPort) excludedPorts.push_back(i);
                    }
                    break;
                }
            }
        }
    }
}

void CustomShipScene::update(float dt) {
    animTime += dt;
    companyInput.update(dt);
    Vector2f mp = (Vector2f)Mouse::getPosition();
    okButton.update(dt, mp);
}

void CustomShipScene::render(RenderWindow& window) {
    if (!companyValidated) {
        // Phase 1: Company input
        Sprite bg; bg.setTexture(assets.getMenuBgTexture());
        bg.setScale(Config::REF_WIDTH / (float)assets.getMenuBgTexture().getSize().x,
                     Config::REF_HEIGHT / (float)assets.getMenuBgTexture().getSize().y);
        window.draw(bg);
        RectangleShape ov(Vector2f(Config::REF_WIDTH, Config::REF_HEIGHT));
        ov.setFillColor(Config::Colors::BG_OVERLAY); window.draw(ov);
        Text title; title.setFont(assets.getFontBold());
        title.setString("Enter Preferred Shipping Company"); title.setCharacterSize(32);
        title.setFillColor(Config::Colors::PRIMARY);
        FloatRect tr = title.getLocalBounds(); title.setOrigin(tr.width/2, tr.height/2);
        title.setPosition(Config::REF_WIDTH/2.f, 200); window.draw(title);
        companyInput.render(window);
        Text inst; inst.setFont(assets.getFont()); inst.setString("Press ENTER to validate | ESC to go back");
        inst.setCharacterSize(18); inst.setFillColor(Config::Colors::TEXT_SECONDARY);
        FloatRect ir = inst.getLocalBounds(); inst.setOrigin(ir.width/2, ir.height/2);
        inst.setPosition(Config::REF_WIDTH/2.f, 420); window.draw(inst);
        if (showInvalid) {
            Text err; err.setFont(assets.getFont()); err.setString("Invalid company! Try again.");
            err.setCharacterSize(22); err.setFillColor(Config::Colors::DANGER);
            FloatRect er = err.getLocalBounds(); err.setOrigin(er.width/2, er.height/2);
            err.setPosition(Config::REF_WIDTH/2.f, 480); window.draw(err);
        }
    } else if (selectingExcluded) {
        // Phase 2: Port selection
        window.draw(mapSprite);
        Vector<GraphNode*>& ports = portSystem.getPorts();
        for (int i = 0; i < ports.getSize(); i++) {
            Color c = Config::Colors::PORT_DEFAULT;
            if (i == startPort) c = Config::Colors::PORT_START;
            else if (i == endPort) c = Config::Colors::PORT_END;
            else { for (int j = 0; j < excludedPorts.getSize(); j++) { if (excludedPorts[j] == i) { c = Config::Colors::PORT_EXCLUDED; break; } } }
            ports[i]->circle.setFillColor(c); window.draw(ports[i]->circle);
        }
        okButton.render(window);
        MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());
        RectangleShape ib(Vector2f(320, 110)); ib.setFillColor(Config::Colors::PANEL_BG);
        ib.setOutlineColor(Config::Colors::PRIMARY); ib.setOutlineThickness(2); ib.setPosition(15, Config::REF_HEIGHT - 130);
        window.draw(ib);
        Text info; info.setFont(assets.getFont()); info.setCharacterSize(14); info.setFillColor(Config::Colors::TEXT_PRIMARY);
        info.setString("Company: " + preferredCompany + "\n1. Click START port (green)\n2. Click END port (orange)\n3. Click ports to EXCLUDE (gray)\n4. Click Find Path");
        info.setPosition(25, Config::REF_HEIGHT - 122); window.draw(info);
    } else {
        // Phase 3: Result
        window.draw(mapSprite);
        Vector<GraphNode*>& ports = portSystem.getPorts();
        for (int i = 0; i < customExplored.getSize(); i++) ports[customExplored[i]]->circle.setFillColor(Config::Colors::PORT_EXPLORED);
        for (int i = 0; i < customPath.getSize(); i++) ports[customPath[i]]->circle.setFillColor(Config::Colors::PRIMARY);
        for (int i = 0; i < ports.getSize(); i++) window.draw(ports[i]->circle);
        MapRenderer::drawPath(window, ports, customPath, Config::Colors::PRIMARY, 4.f);
        RectangleShape ib(Vector2f(300, 100)); ib.setFillColor(Config::Colors::PANEL_BG);
        ib.setOutlineColor(Config::Colors::PRIMARY); ib.setOutlineThickness(2); ib.setPosition(20, Config::REF_HEIGHT - 120);
        window.draw(ib);
        Text info; info.setFont(assets.getFont()); info.setCharacterSize(16); info.setFillColor(Config::Colors::TEXT_PRIMARY);
        info.setString("Optimized via " + preferredCompany + "\nPorts: " + to_string(customPath.getSize()) + "\nExcluded: " + to_string(excludedPorts.getSize()));
        info.setPosition(30, Config::REF_HEIGHT - 112); window.draw(info);
        MapRenderer::drawNodeTooltip(window, ports, (Vector2f)Mouse::getPosition(window), assets.getFont());
    }
}
