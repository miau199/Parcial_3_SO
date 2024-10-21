#ifndef BIOMA_H
#define BIOMA_H

#include <SFML/Graphics.hpp>

class Bioma {
public:
    Bioma(float x, float y, float width, float height);
    bool contienePunto(float x, float y) const;
    sf::RectangleShape getForma() const;

private:
    sf::RectangleShape forma;
};

#endif // BIOMA_H