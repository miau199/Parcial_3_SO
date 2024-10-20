#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include "Criatura.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void moverCriatura(Criatura* criatura, std::vector<Criatura*>& criaturas1, std::vector<Criatura*>& criaturas2, std::vector<Criatura*>& criaturas3, std::mutex& mtx) {
    criatura->mover(criaturas1, criaturas2, criaturas3, mtx);
}

int main() {
    const int numCriaturasPorTipo = 3; // Cambiar este número según sea necesario
    std::vector<Criatura*> criaturas1; // Tipo 1
    std::vector<Criatura*> criaturas2; // Tipo 2
    std::vector<Criatura*> criaturas3; // Tipo 3
    std::mutex mtx;

    // Crear criaturas
    for (int i = 0; i < numCriaturasPorTipo; ++i) {
        criaturas1.push_back(new Criatura(i + 1, "Especie 1", rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, 1));
        criaturas2.push_back(new Criatura(i + 1, "Especie 2", rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, 2));
        criaturas3.push_back(new Criatura(i + 1, "Especie 3", rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, 3));
    }

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Simulador de Ecosistema");

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

        // Dibujar criaturas
        for (const auto& criatura : criaturas1) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Red); // Color de la criatura (rojo para tipo 1)
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
            shape.setFillColor(sf::Color::Blue); // Color de la criatura (azul para tipo 3)
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