/*=======================================================================
 * MapRenderer.cpp — Map Drawing Implementation
 *=======================================================================*/
#include "MapRenderer.h"
#include "../core/Constants.h"
#include <cmath>
#include <string>
using namespace std;

void MapRenderer::drawPorts(RenderWindow& window, Vector<GraphNode*>& ports,
                            Font& font, float time) {
    for (int i = 0; i < ports.getSize(); i++) {
        GraphNode* node = ports[i];
        Vector2f pos = node->circle.getPosition();

        // Outer glow ring
        float pulse = sin(time * Config::Anim::GLOW_SPEED + i * 0.5f) * 2.0f;
        CircleShape glow(Config::UI::PORT_GLOW_RADIUS + pulse);
        glow.setOrigin(Config::UI::PORT_GLOW_RADIUS + pulse, Config::UI::PORT_GLOW_RADIUS + pulse);
        glow.setPosition(pos);
        Color glowCol = node->circle.getFillColor();
        glowCol.a = 30;
        glow.setFillColor(glowCol);
        window.draw(glow);

        // Main port circle
        window.draw(node->circle);

        // Inner highlight dot
        CircleShape inner(3);
        inner.setOrigin(3, 3);
        inner.setPosition(pos);
        inner.setFillColor(Color(255, 255, 255, 120));
        window.draw(inner);
    }
}

void MapRenderer::drawPortLabels(RenderWindow& window, Vector<GraphNode*>& ports,
                                  Font& font) {
    for (int i = 0; i < ports.getSize(); i++) {
        Vector2f pos = ports[i]->circle.getPosition();
        Text nameLabel;
        nameLabel.setFont(font);
        nameLabel.setString(ports[i]->portName);
        nameLabel.setCharacterSize(Config::Font::SIZE_TINY);
        nameLabel.setFillColor(Config::Colors::TEXT_SECONDARY);
        FloatRect bounds = nameLabel.getLocalBounds();
        nameLabel.setOrigin(bounds.width / 2.f, 0);
        nameLabel.setPosition(pos.x, pos.y + Config::UI::PORT_RADIUS + 3);
        window.draw(nameLabel);
    }
}

void MapRenderer::drawEdges(RenderWindow& window, Vector<GraphNode*>& ports,
                             int selectedPort) {
    if (selectedPort > -1) {
        // Draw only selected port's edges
        for (int j = 0; j < ports[selectedPort]->edges.getSize(); j++) {
            Vector<Vertex>& line = ports[selectedPort]->edges[j];
            if (line.getSize() >= 2) {
                Renderer::drawThickLine(window, line[0].position, line[1].position,
                                        Config::UI::EDGE_THICKNESS, Config::Colors::EDGE_ACTIVE);
            }
        }
    } else {
        // Draw all edges
        for (int i = 0; i < ports.getSize(); i++) {
            for (int j = 0; j < ports[i]->edges.getSize(); j++) {
                Vector<Vertex>& line = ports[i]->edges[j];
                if (line.getSize() >= 2) {
                    Renderer::drawThickLine(window, line[0].position, line[1].position,
                                            1.0f, Config::Colors::EDGE_DEFAULT);
                }
            }
        }
    }
}

void MapRenderer::drawPath(RenderWindow& window, Vector<GraphNode*>& ports,
                            Vector<int>& path, Color color, float thickness) {
    for (int i = 0; i + 1 < path.getSize(); i++) {
        Vector2f a = ports[path[i]]->circle.getPosition();
        Vector2f b = ports[path[i + 1]]->circle.getPosition();
        Renderer::drawArrow(window, a, b, thickness, 12.f, color);
    }
}

bool MapRenderer::isMouseNearLine(Vector2f mouse, Vector2f A, Vector2f B, float threshold) {
    float dx = B.x - A.x;
    float dy = B.y - A.y;
    float lengthSquared = dx * dx + dy * dy;
    if (lengthSquared == 0) return false;
    float t = ((mouse.x - A.x) * dx + (mouse.y - A.y) * dy) / lengthSquared;
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    float closestX = A.x + t * dx;
    float closestY = A.y + t * dy;
    float dist = sqrt((mouse.x - closestX) * (mouse.x - closestX) +
                       (mouse.y - closestY) * (mouse.y - closestY));
    return dist <= threshold;
}

void MapRenderer::drawNodeTooltip(RenderWindow& window, Vector<GraphNode*>& ports,
                                   Vector2f mousePos, Font& font) {
    for (int i = 0; i < ports.getSize(); i++) {
        GraphNode* node = ports[i];
        if (node->circle.getGlobalBounds().contains(mousePos)) {
            float finalX = mousePos.x + 15;
            float finalY = mousePos.y + 15;
            if (mousePos.x > 1000) finalX = mousePos.x - 260;
            if (mousePos.y > 550)  finalY = mousePos.y - 130;

            RectangleShape infoBox;
            infoBox.setSize(Vector2f(250, 120));
            infoBox.setFillColor(Config::Colors::PANEL_BG);
            infoBox.setOutlineColor(Config::Colors::PRIMARY);
            infoBox.setOutlineThickness(2);
            infoBox.setPosition(finalX, finalY);

            Text infoText;
            infoText.setFont(font);
            infoText.setCharacterSize(Config::Font::SIZE_BODY);
            infoText.setFillColor(Config::Colors::TEXT_PRIMARY);
            string s = "Port: " + node->portName +
                       "\nCharge: $" + to_string(node->portCharge) +
                       "\nConnections: " + to_string(node->destinations.getSize()) +
                       "\nTrips: " + to_string(node->trips.getSize());
            infoText.setString(s);
            infoText.setPosition(finalX + 10, finalY + 10);

            window.draw(infoBox);
            window.draw(infoText);
            return;
        }
    }
}

void MapRenderer::drawEdgeTooltip(RenderWindow& window, Vector<GraphNode*>& ports,
                                   Vector2f mousePos, Font& font, int selectedPort) {
    int start = (selectedPort > -1) ? selectedPort : 0;
    int end   = (selectedPort > -1) ? selectedPort + 1 : ports.getSize();

    float finalX = mousePos.x + 15;
    float finalY = mousePos.y + 15;
    if (mousePos.x > 1000) finalX = mousePos.x - 260;
    if (mousePos.y > 550)  finalY = mousePos.y - 180;

    for (int j = start; j < end; j++) {
        for (int i = 0; i < ports[j]->edges.getSize(); i++) {
            if (ports[j]->edges[i].getSize() < 2) continue;
            Vertex a = ports[j]->edges[i][0];
            Vertex b = ports[j]->edges[i][1];
            if (isMouseNearLine(mousePos, a.position, b.position, 5)) {
                if (i < ports[j]->trips.getSize()) {
                    Routes r = ports[j]->trips[i];
                    RectangleShape infoBox;
                    infoBox.setSize(Vector2f(280, 150));
                    infoBox.setFillColor(Config::Colors::PANEL_BG);
                    infoBox.setOutlineColor(Config::Colors::ACCENT);
                    infoBox.setOutlineThickness(2);
                    infoBox.setPosition(finalX, finalY);

                    Text infoText;
                    infoText.setFont(font);
                    infoText.setCharacterSize(Config::Font::SIZE_SMALL);
                    infoText.setFillColor(Config::Colors::TEXT_PRIMARY);
                    string s = "Route #" + to_string(i + 1) +
                               "\n" + r.currentPort + " -> " + r.destination +
                               "\nDate: " + r.date +
                               "\nDepart: " + r.departureTime + " -> " + r.arrivalTime +
                               "\nCompany: " + r.shippingCompany +
                               "\nCost: $" + to_string(r.cost);
                    infoText.setString(s);
                    infoText.setPosition(finalX + 10, finalY + 10);

                    window.draw(infoBox);
                    window.draw(infoText);
                }
                return;
            }
        }
    }
}
