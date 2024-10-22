#ifndef BIOMA_H
#define BIOMA_H

#include <SFML/Graphics.hpp>

class Bioma {
public:
     Bioma(float x, float y, float width, float height, float safeX, float safeY);
    bool contienePunto(float x, float y) const;
    sf::RectangleShape getForma() const;
    sf::Vector2f getPuntoSeguro() const;

private:
    sf::RectangleShape forma;
    sf::Vector2f puntoSeguro;
};

#endif // BIOMA_H