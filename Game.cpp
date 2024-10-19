#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "EcosSim"), contadorC1(0), contadorC2(0), contadorC3(0) {
    // Crear criaturas y asignarlas a los arreglos correspondientes
    primeraCriatura[contadorC1++] = new Criatura(1, 100, 100, sf::Color::Red);   
    segundaCriatura[contadorC2++] = new Criatura(2, 100, 200, sf::Color::Blue);  
    terceraCriatura[contadorC3++] = new Criatura(3, 500, 500, sf::Color::Green); 
}
void Game::iniciar() {
    for (auto& criatura : criaturas) {
        criatura->iniciar();
    }
}

void Game::actualizar() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Verificar proximidad y hacer que las criaturas se sigan entre sí
        // C1 sigue a C2, C2 sigue a C3, C3 sigue a C1

        // Iterar a través de las criaturas 1
        for (int i = 0; i < contadorC1; ++i) {
            if (primeraCriatura[i]) {
                for (int j = 0; j < contadorC2; ++j) {
                    if (segundaCriatura[j]) {
                        if (abs(primeraCriatura[i]->getX() - segundaCriatura[j]->getX()) < 50) {
                            primeraCriatura[i]->follow(segundaCriatura[j]);
                        } else {
                            primeraCriatura[i]->movimientoAleatorio();
                        }
                    }
                }
            }
        }

        // Iterar a través de las criaturas 2
        for (int i = 0; i < contadorC2; ++i) {
            if (segundaCriatura[i]) {
                for (int j = 0; j < contadorC3; ++j) {
                    if (terceraCriatura[j]) {
                        if (abs(segundaCriatura[i]->getX() - terceraCriatura[j]->getX()) < 50) {
                            segundaCriatura[i]->follow(terceraCriatura[j]);
                        } else {
                            segundaCriatura[i]->movimientoAleatorio();
                        }
                    }
                }
            }
        }

        // Iterar a través de las criaturas 3
        for (int i = 0; i < contadorC3; ++i) {
            if (terceraCriatura[i]) {
                for (int j = 0; j < contadorC1; ++j) {
                    if(primeraCriatura[j]){
                        if (abs(terceraCriatura[i]->getX() - primeraCriatura[j]->getX()) < 50) {
                            terceraCriatura[i]->follow(primeraCriatura[j]);
                        } else {
                            terceraCriatura[i]->movimientoAleatorio();
                        }
                    }
                }
            }
        }

        render();
    }
}

void Game::render() {
    window.clear();
    
    // Dibujar criaturas 1
    for (int i = 0; i < contadorC1; ++i) {
        if (primeraCriatura[i]) {
            window.draw(primeraCriatura[i]->getForma());
        }
    }

    // Dibujar criaturas 2
    for (int i = 0; i < contadorC2; ++i) {
        if (segundaCriatura[i]) {
            window.draw(segundaCriatura[i]->getForma());
        }
    }

    // Dibujar criaturas 3
    for (int i = 0; i < contadorC3; ++i) {
        if (terceraCriatura[i]) {
            window.draw(terceraCriatura[i]->getForma());
        }
    }

    window.display();
}
