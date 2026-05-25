/*=======================================================================
 * Renderer.cpp — Drawing Utility Implementations
 *=======================================================================*/
#include "Renderer.h"
#include <algorithm>
using namespace std;

float Renderer::radToDeg(float r) {
    return r * 180.0f / 3.14159265358979323846f;
}

float Renderer::lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

Color Renderer::lerpColor(Color a, Color b, float t) {
    return Color(
        (Uint8)lerp((float)a.r, (float)b.r, t),
        (Uint8)lerp((float)a.g, (float)b.g, t),
        (Uint8)lerp((float)a.b, (float)b.b, t),
        (Uint8)lerp((float)a.a, (float)b.a, t)
    );
}

void Renderer::drawThickLine(RenderWindow& window, Vector2f a, Vector2f b,
                              float thickness, Color color) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float length = sqrt(dx * dx + dy * dy);
    if (length <= 0.0001f) return;

    RectangleShape seg(Vector2f(length, thickness));
    seg.setFillColor(color);
    seg.setOrigin(0.f, thickness * 0.5f);
    seg.setPosition(a);
    seg.setRotation(radToDeg(atan2(dy, dx)));
    window.draw(seg);
}

void Renderer::drawArrowhead(RenderWindow& window, Vector2f a, Vector2f b,
                              float size, Color color) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float length = sqrt(dx * dx + dy * dy);
    if (length <= 0) return;

    Vector2f dir(dx / length, dy / length);
    Vector2f perpL(-dir.y, dir.x);
    Vector2f perpR(dir.y, -dir.x);

    ConvexShape arrow(3);
    arrow.setPoint(0, b);
    arrow.setPoint(1, b - dir * size + perpL * (size * 0.4f));
    arrow.setPoint(2, b - dir * size + perpR * (size * 0.4f));
    arrow.setFillColor(color);
    window.draw(arrow);
}

void Renderer::drawArrow(RenderWindow& window, Vector2f a, Vector2f b,
                          float thickness, float arrowSize, Color color) {
    drawThickLine(window, a, b, thickness, color);
    drawArrowhead(window, a, b, arrowSize, color);
}

void Renderer::drawGlowCircle(RenderWindow& window, Vector2f center,
                               float radius, Color color, int layers) {
    for (int i = layers; i >= 0; i--) {
        float r = radius + i * 4.0f;
        float alpha = (float)(color.a) / (float)(i + 1);
        CircleShape glow(r);
        glow.setOrigin(r, r);
        glow.setPosition(center);
        glow.setFillColor(Color(color.r, color.g, color.b, (Uint8)alpha));
        window.draw(glow);
    }
}

void Renderer::drawPulsingCircle(RenderWindow& window, Vector2f center,
                                  float baseRadius, float pulseAmount,
                                  float time, Color color) {
    float pulse = sin(time * 3.14159f * 2.0f) * pulseAmount;
    float r = baseRadius + pulse;
    CircleShape circle(r);
    circle.setOrigin(r, r);
    circle.setPosition(center);
    circle.setFillColor(color);
    window.draw(circle);
}

void Renderer::drawDashedLine(RenderWindow& window, Vector2f a, Vector2f b,
                               float dashLen, float gapLen, float thickness,
                               Color color) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float totalLen = sqrt(dx * dx + dy * dy);
    if (totalLen <= 0) return;

    Vector2f dir(dx / totalLen, dy / totalLen);
    float pos = 0;
    bool drawing = true;

    while (pos < totalLen) {
        float segLen = drawing ? dashLen : gapLen;
        segLen = min(segLen, totalLen - pos);

        if (drawing) {
            Vector2f start = Vector2f(a.x + dir.x * pos, a.y + dir.y * pos);
            Vector2f end = Vector2f(a.x + dir.x * (pos + segLen), a.y + dir.y * (pos + segLen));
            drawThickLine(window, start, end, thickness, color);
        }
        pos += segLen;
        drawing = !drawing;
    }
}
