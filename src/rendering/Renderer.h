#pragma once
/*=======================================================================
 * Renderer.h — Drawing Utilities (thick lines, arrows, glow effects)
 *=======================================================================*/
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;

class Renderer {
public:
    static void drawThickLine(RenderWindow& window, Vector2f a, Vector2f b,
                              float thickness, Color color);
    static void drawArrowhead(RenderWindow& window, Vector2f a, Vector2f b,
                              float size, Color color);
    static void drawArrow(RenderWindow& window, Vector2f a, Vector2f b,
                          float thickness, float arrowSize, Color color);
    static void drawGlowCircle(RenderWindow& window, Vector2f center,
                               float radius, Color color, int layers = 3);
    static void drawPulsingCircle(RenderWindow& window, Vector2f center,
                                  float baseRadius, float pulseAmount,
                                  float time, Color color);
    static void drawDashedLine(RenderWindow& window, Vector2f a, Vector2f b,
                               float dashLen, float gapLen, float thickness, Color color);
    static float radToDeg(float r);
    static float lerp(float a, float b, float t);
    static Color lerpColor(Color a, Color b, float t);
};
