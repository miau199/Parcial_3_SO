#ifndef GAME_H
#define GAME_H

#include "Criatura.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void iniciar();
    void actualizar();
    void render();

private:
    std::vector<Criatura*> criaturas;
    sf::RenderWindow window;
    static const int MAX_CRIATURAS = 4; // Número máximo de criaturas
    Criatura* primeraCriatura[MAX_CRIATURAS]; // Arreglo para criaturas 1
    Criatura* segundaCriatura[MAX_CRIATURAS]; // Arreglo para criaturas 2
    Criatura* terceraCriatura[MAX_CRIATURAS]; // Arreglo para criaturas 3
    int contadorC1; // Conteo de criaturas 1
    int contadorC2; // Conteo de criaturas 2
    int contadorC3; // Conteo de criaturas 3
};

#endif // GAME_H
