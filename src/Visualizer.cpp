#include "Visualizer.h"
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>

static const int   WINDOW_W    = 900;
static const int   WINDOW_H    = 900;
static const int   MARGIN      = 60;
static const float NODE_RADIUS = 6.0f;
static const float PI          = 3.14159265358979f;

static const sf::Color COLOR_BG   (30,  30,  30);
static const sf::Color COLOR_NODE (220, 80,  80);
static const sf::Color COLOR_TEXT (255, 255, 255);

static float remap(float value, float in_min, float in_max,
                   float out_min, float out_max) {
    if (in_max - in_min < 1e-9f) return (out_min + out_max) / 2.0f;
    return out_min + (value - in_min) / (in_max - in_min) * (out_max - out_min);
}

static sf::Vector2f *make_circle_positions(int n) {
    sf::Vector2f *positions = new sf::Vector2f[n];
    float cx = WINDOW_W / 2.0f;
    float cy = WINDOW_H / 2.0f;
    float r  = (WINDOW_W - 2 * MARGIN) / 2.0f;
    for (int i = 0; i < n; i++) {
        float angle  = 2.0f * PI * i / (float)n - PI / 2.0f;
        positions[i] = sf::Vector2f(cx + r * std::cos(angle),
                                    cy + r * std::sin(angle));
    }
    return positions;
}

static sf::Vector2f *make_coord_positions(const TSPInstance *inst) {
    int n = inst->n;
    sf::Vector2f *positions = new sf::Vector2f[n];
    float x_min = inst->coords[0].x, x_max = inst->coords[0].x;
    float y_min = inst->coords[0].y, y_max = inst->coords[0].y;
    for (int i = 1; i < n; i++) {
        if (inst->coords[i].x < x_min) x_min = inst->coords[i].x;
        if (inst->coords[i].x > x_max) x_max = inst->coords[i].x;
        if (inst->coords[i].y < y_min) y_min = inst->coords[i].y;
        if (inst->coords[i].y > y_max) y_max = inst->coords[i].y;
    }
    float draw_w = (float)(WINDOW_W - 2 * MARGIN);
    float draw_h = (float)(WINDOW_H - 2 * MARGIN);
    for (int i = 0; i < n; i++) {
        float px = remap(inst->coords[i].x, x_min, x_max,
                         (float)MARGIN, (float)MARGIN + draw_w);
        float py = remap(inst->coords[i].y, y_min, y_max,
                         (float)MARGIN + draw_h, (float)MARGIN);
        positions[i] = sf::Vector2f(px, py);
    }
    return positions;
}

void visualize(const TSPInstance *inst) {
    int n = inst->n;
    bool has_coords = (inst->coords != nullptr);
    sf::Vector2f *positions = has_coords
        ? make_coord_positions(inst)
        : make_circle_positions(n);

    sf::Font font;
    bool has_font = font.loadFromFile(
    "C:/Windows/Fonts/arial.ttf");
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_W, WINDOW_H),
        std::string("TSP - ") + inst->name,
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    sf::CircleShape node_shape(NODE_RADIUS);
    node_shape.setFillColor(COLOR_NODE);
    node_shape.setOrigin(NODE_RADIUS, NODE_RADIUS);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }
        window.clear(COLOR_BG);
        for (int i = 0; i < n; i++) {
            node_shape.setPosition(positions[i]);
            window.draw(node_shape);
            if (has_font) {
                sf::Text label;
                label.setFont(font);
                label.setString(std::to_string(i));
                label.setCharacterSize(11);
                label.setFillColor(COLOR_TEXT);
                label.setPosition(positions[i].x + NODE_RADIUS + 2,
                                  positions[i].y - NODE_RADIUS - 2);
                window.draw(label);
            }
        }
        window.display();
    }
    delete[] positions;
}