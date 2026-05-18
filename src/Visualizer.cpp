#include "Visualizer.h"
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>
#include <string>

static const int   WINDOW_W    = 900;
static const int   WINDOW_H    = 900;
static const int   MARGIN      = 60;
static const float NODE_RADIUS = 6.0f;
static const float PI          = 3.14159265358979f;

static const sf::Color COLOR_BG   (30,  30,  30);
static const sf::Color COLOR_NODE (220, 80,  80);
static const sf::Color COLOR_TEXT (255, 255, 255);
static const sf::Color COLOR_EDGE (100, 180, 255);
static const sf::Color COLOR_NUM  (255, 220, 50);

static float remap(float value, float in_min, float in_max,
                   float out_min, float out_max) {
    if (in_max - in_min < 1e-9f) return (out_min + out_max) / 2.0f;
    return out_min + (value - in_min) / (in_max - in_min) * (out_max - out_min);
}

static sf::Vector2f node_position(const TSPInstance *inst, int i,
                                   float x_min, float x_max,
                                   float y_min, float y_max) {
    float px = remap(inst->coords[i].x, x_min, x_max,
                     (float)MARGIN, (float)(WINDOW_W - MARGIN));
    float py = remap(inst->coords[i].y, y_min, y_max,
                     (float)(WINDOW_H - MARGIN), (float)MARGIN);
    return sf::Vector2f(px, py);
}

void visualize(const TSPInstance *inst, int* chemin, int taille, int distance) {

    if (!inst->has_coords || inst->coords == nullptr) {
        std::printf("  [Visualizer] Pas de coordonnees disponibles.\n");
        return;
    }

    int n = inst->n;

    // Calcul des bornes
    float x_min = inst->coords[0].x, x_max = inst->coords[0].x;
    float y_min = inst->coords[0].y, y_max = inst->coords[0].y;
    for (int i = 1; i < n; i++) {
        if (inst->coords[i].x < x_min) x_min = inst->coords[i].x;
        if (inst->coords[i].x > x_max) x_max = inst->coords[i].x;
        if (inst->coords[i].y < y_min) y_min = inst->coords[i].y;
        if (inst->coords[i].y > y_max) y_max = inst->coords[i].y;
    }

    sf::Font font;
    bool has_font = font.loadFromFile("C:/Windows/Fonts/arial.ttf");

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

        // ── Traits du chemin avec numéros ──────────────────────
        if (chemin != nullptr && has_font) {
            for (int i = 0; i < taille - 1; i++) {
                sf::Vector2f p1 = node_position(inst, chemin[i],
                                                x_min, x_max, y_min, y_max);
                sf::Vector2f p2 = node_position(inst, chemin[i + 1],
                                                x_min, x_max, y_min, y_max);

                // Trait
                sf::Vertex line[2];
                line[0] = sf::Vertex(p1, COLOR_EDGE);
                line[1] = sf::Vertex(p2, COLOR_EDGE);
                window.draw(line, 2, sf::Lines);

                // Numéro au milieu du trait
                sf::Text num;
                num.setFont(font);
                num.setString(std::to_string(i + 1));
                num.setCharacterSize(9);
                num.setFillColor(COLOR_NUM);
                num.setPosition((p1.x + p2.x) / 2.0f,
                                (p1.y + p2.y) / 2.0f);
                window.draw(num);
            }
        }

        // ── Villes ─────────────────────────────────────────────
        for (int i = 0; i < n; i++) {
            sf::Vector2f pos = node_position(inst, i,
                                             x_min, x_max, y_min, y_max);
            node_shape.setPosition(pos);
            window.draw(node_shape);

            if (has_font) {
                sf::Text label;
                label.setFont(font);
                label.setString(std::to_string(i));
                label.setCharacterSize(11);
                label.setFillColor(COLOR_TEXT);
                label.setPosition(pos.x + NODE_RADIUS + 2,
                                  pos.y - NODE_RADIUS - 2);
                window.draw(label);
            }
        }

        // ── Distance en haut à gauche ──────────────────────────
        if (has_font) {
            sf::Text dist_text;
            dist_text.setFont(font);
            dist_text.setString("Distance : " + std::to_string(distance));
            dist_text.setCharacterSize(18);
            dist_text.setFillColor(sf::Color(100, 255, 100));
            dist_text.setPosition(10, 10);
            window.draw(dist_text);
        }

        window.display();
    }
}