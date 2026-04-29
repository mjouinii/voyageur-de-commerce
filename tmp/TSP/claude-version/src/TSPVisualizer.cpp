#include "../include/TSPVisualizer.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

void TSPVisualizer::display(const TSPInstance& instance) {
    if (instance.coords.empty()) {
        throw std::runtime_error("Visualiseur : Aucune coordonnée disponible pour l'affichage.");
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Visualiseur TSP - " + instance.name);
    window.setFramerateLimit(60);

    // Calcul des bornes pour le cadrage automatique
    double minX = instance.coords[0].x, maxX = minX;
    double minY = instance.coords[0].y, maxY = minY;
    for (const auto& c : instance.coords) {
        minX = std::min(minX, c.x); maxX = std::max(maxX, c.x);
        minY = std::min(minY, c.y); maxY = std::max(maxY, c.y);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color(30, 30, 30)); // Fond gris foncé

        for (const auto& c : instance.coords) {
            sf::CircleShape city(5);
            city.setFillColor(sf::Color::Cyan);
            city.setOrigin(5, 5); // Centre le cercle

            // Normalisation avec marges (80px)
            float px = (c.x - minX) / (maxX - minX + 1) * 640 + 80;
            float py = (c.y - minY) / (maxY - minY + 1) * 440 + 80;
            
            city.setPosition(px, py);
            window.draw(city);
        }

        window.display();
    }
}