#include "Bioma.h"

Bioma::Bioma(float x, float y, float width, float height) {
    forma.setPosition(x, y);
    forma.setSize(sf::Vector2f(width, height));
    forma.setFillColor(sf::Color::Transparent);
    forma.setOutlineThickness(2);
}

bool Bioma::contienePunto(float x, float y) const { // Verifica  si el punto (x, y) está dentro de la forma del bioma
    return forma.getGlobalBounds().contains(x, y);
}

sf::RectangleShape Bioma::getForma() const {
    return forma;
}