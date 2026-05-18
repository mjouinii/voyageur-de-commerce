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

// Remappe une valeur de [in_min, in_max] vers [out_min, out_max]
static float remap(float value, float in_min, float in_max,
                   float out_min, float out_max) {
    if (in_max - in_min < 1e-9f) return (out_min + out_max) / 2.0f;
    return out_min + (value - in_min) / (in_max - in_min) * (out_max - out_min);
}

// Ouvre la fenêtre et affiche les villes avec leurs coordonnées réelles
void visualize(const TSPInstance *inst) {

    // Si pas de coordonnées disponibles, on ne peut rien afficher
    if (!inst->has_coords || inst->coords == nullptr) {
        std::printf("  [Visualizer] Pas de coordonnées disponibles,"
                    " aucune fenêtre ouverte.\n");
        return;
    }

    int n = inst->n;

    // Calcul des bornes pour le remapping des coordonnées à l'écran
    float x_min = static_cast<float>(inst->coords[0].x);
    float x_max = x_min;
    float y_min = static_cast<float>(inst->coords[0].y);
    float y_max = y_min;

    for (int i = 1; i < n; i++) {
        float cx = static_cast<float>(inst->coords[i].x);
        float cy = static_cast<float>(inst->coords[i].y);
        if (cx < x_min) x_min = cx;
        if (cx > x_max) x_max = cx;
        if (cy < y_min) y_min = cy;
        if (cy > y_max) y_max = cy;
    }

    // Création de la fenêtre SFML
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_W, WINDOW_H),
        inst->name
    );
    window.setFramerateLimit(60);

    // Chargement de la police pour les étiquettes
    sf::Font font;
    bool font_loaded = font.loadFromFile("/usr/share/fonts/truetype/"
                                         "dejavu/DejaVuSans.ttf");

    // Boucle principale d'affichage
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

        // Dessin de chaque ville
        for (int i = 0; i < n; i++) {

            // Remapping des coordonnées vers l'espace fenêtre
            float sx = remap(static_cast<float>(inst->coords[i].x),
                             x_min, x_max,
                             static_cast<float>(MARGIN),
                             static_cast<float>(WINDOW_W - MARGIN));

            // L'axe Y est inversé en SFML (0 en haut)
            float sy = remap(static_cast<float>(inst->coords[i].y),
                             y_min, y_max,
                             static_cast<float>(WINDOW_H - MARGIN),
                             static_cast<float>(MARGIN));

            // Cercle représentant la ville
            sf::CircleShape node(NODE_RADIUS);
            node.setFillColor(COLOR_NODE);
            node.setOrigin(NODE_RADIUS, NODE_RADIUS);
            node.setPosition(sx, sy);
            window.draw(node);

            // Étiquette numérique si la police est disponible
            if (font_loaded) {
                sf::Text label;
                label.setFont(font);
                label.setString(std::to_string(i + 1));
                label.setCharacterSize(11);
                label.setFillColor(COLOR_TEXT);
                label.setPosition(sx + NODE_RADIUS + 2.0f, sy - NODE_RADIUS);
                window.draw(label);
            }
        }

        window.display();
    }
}
