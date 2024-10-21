#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "Criatura.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 900;

void moverCriatura(Criatura* criatura, std::vector<Criatura*>& criaturas1, std::vector<Criatura*>& criaturas2, std::vector<Criatura*>& criaturas3, std::mutex& mtx) {
    criatura->mover(criaturas1, criaturas2, criaturas3, mtx);
}

int main() {

    srand(static_cast<unsigned>(time(0)));  // Semilla aleatoria para el movimiento
    const int numCriaturasPorTipo = 3; // Cambiar según sea necesario
    std::vector<Criatura*> criaturas1; // Tipo 1
    std::vector<Criatura*> criaturas2; // Tipo 2
    std::vector<Criatura*> criaturas3; // Tipo 3
    std::mutex mtx;

// Definir un margen para que las criaturas no aparezcan justo en el borde
const int MARGIN = 20;

// Crear criaturas
for (int i = 0; i < numCriaturasPorTipo; ++i) {
    int x = MARGIN + rand() % (WINDOW_WIDTH - 2 * MARGIN);
    int y = MARGIN + rand() % (WINDOW_HEIGHT - 2 * MARGIN);
    criaturas1.push_back(new Criatura(i + 1, "Especie 1", 350, 350, 1));
    
    x = MARGIN + rand() % (WINDOW_WIDTH - 2 * MARGIN);
    y = MARGIN + rand() % (WINDOW_HEIGHT - 2 * MARGIN);
    criaturas2.push_back(new Criatura(i + 1, "Especie 2", 700, 100, 2));
    
    x = MARGIN + rand() % (WINDOW_WIDTH - 2 * MARGIN);
    y = MARGIN + rand() % (WINDOW_HEIGHT - 2 * MARGIN);
    criaturas3.push_back(new Criatura(i + 1, "Especie 3", 500, 800, 3));
}

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Simulador de Ecosistema");

    // Crear formas para los bordes
    sf::RectangleShape bordeAzul(sf::Vector2f(650, 650));
    bordeAzul.setFillColor(sf::Color::Transparent);
    bordeAzul.setOutlineThickness(2);
    bordeAzul.setOutlineColor(sf::Color::Blue);
    bordeAzul.setPosition(0, 0);

    sf::RectangleShape bordeVerde(sf::Vector2f(600, 800));
    bordeVerde.setFillColor(sf::Color::Transparent);
    bordeVerde.setOutlineThickness(2);
    bordeVerde.setOutlineColor(sf::Color::Green);
    bordeVerde.setPosition(WINDOW_WIDTH - 600, 0);

    sf::RectangleShape bordeAmarillo(sf::Vector2f(800, 350));
    bordeAmarillo.setFillColor(sf::Color::Transparent);
    bordeAmarillo.setOutlineThickness(2);
    bordeAmarillo.setOutlineColor(sf::Color::Yellow);
    bordeAmarillo.setPosition(0, WINDOW_HEIGHT - 350);

    // Crear biomas (y sus límites)
    Bioma biomaAzul(0, 0, 650, 650);
    Bioma biomaVerde(WINDOW_WIDTH - 600, 0, 600, 800);
    Bioma biomaAmarillo(0, WINDOW_HEIGHT - 350, 800, 350);


    // Asignar biomas a las criaturas
    for (auto& criatura : criaturas1) {
        criatura->setBioma(&biomaAzul, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    for (auto& criatura : criaturas2) {
        criatura->setBioma(&biomaVerde, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    for (auto& criatura : criaturas3) {
        criatura->setBioma(&biomaAmarillo, WINDOW_WIDTH, WINDOW_HEIGHT);
    }


    // Iniciar hilo para cada criatura
    std::vector<std::thread> hilos;
    for (auto& criatura : criaturas1) {
        hilos.emplace_back(moverCriatura, criatura, std::ref(criaturas1), std::ref(criaturas2), std::ref(criaturas3), std::ref(mtx));
    }
    for (auto& criatura : criaturas2) {
        hilos.emplace_back(moverCriatura, criatura, std::ref(criaturas1), std::ref(criaturas2), std::ref(criaturas3), std::ref(mtx));
    }
    for (auto& criatura : criaturas3) {
        hilos.emplace_back(moverCriatura, criatura, std::ref(criaturas1), std::ref(criaturas2), std::ref(criaturas3), std::ref(mtx));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Limpiar la ventana
        window.clear(sf::Color::White);

        // Dibujar los bordes
        window.draw(bordeAzul);
        window.draw(bordeVerde);
        window.draw(bordeAmarillo);

        // Dibujar criaturas
        for (const auto& criatura : criaturas1) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Blue); // Color de la criatura (azul para tipo 1)
            shape.setPosition(criatura->getX(), criatura->getY()); // Posición de la criatura
            window.draw(shape);
        }
        for (const auto& criatura : criaturas2) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Green); // Color de la criatura (verde para tipo 2)
            shape.setPosition(criatura->getX(), criatura->getY()); // Posición de la criatura
            window.draw(shape);
        }
        for (const auto& criatura : criaturas3) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Yellow); // Color de la criatura (amarillo para tipo 3)
            shape.setPosition(criatura->getX(), criatura->getY()); // Posición de la criatura
            window.draw(shape);
        }

        // Mostrar el contenido de la ventana
        window.display();
    }

    // Detener criaturas y liberar memoria
    for (auto& criatura : criaturas1) {
        criatura->detener();
        delete criatura; // Liberar memoria
    }
    for (auto& criatura : criaturas2) {
        criatura->detener();
        delete criatura; // Liberar memoria
    }
    for (auto& criatura : criaturas3) {
        criatura->detener();
        delete criatura; // Liberar memoria
    }

    // Unir hilos
    for (auto& hilo : hilos) {
        hilo.join();
    }

    return 0;
}


    

    

