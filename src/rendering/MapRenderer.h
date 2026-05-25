#pragma once
/*=======================================================================
 * MapRenderer.h — Port/Edge/Path Drawing on Map
 *=======================================================================*/
#include <SFML/Graphics.hpp>
#include "../graph/GraphNode.h"
#include "../core/DataStructures.h"
#include "Renderer.h"
using namespace sf;

class MapRenderer {
public:
    static void drawPorts(RenderWindow& window, Vector<GraphNode*>& ports,
                          Font& font, float time);
    static void drawPortLabels(RenderWindow& window, Vector<GraphNode*>& ports,
                               Font& font);
    static void drawEdges(RenderWindow& window, Vector<GraphNode*>& ports,
                          int selectedPort = -1);
    static void drawPath(RenderWindow& window, Vector<GraphNode*>& ports,
                         Vector<int>& path, Color color, float thickness = 4.f);
    static void drawNodeTooltip(RenderWindow& window, Vector<GraphNode*>& ports,
                                Vector2f mousePos, Font& font);
    static void drawEdgeTooltip(RenderWindow& window, Vector<GraphNode*>& ports,
                                Vector2f mousePos, Font& font, int selectedPort = -1);
    static bool isMouseNearLine(Vector2f mouse, Vector2f A, Vector2f B, float threshold = 5.f);
};
