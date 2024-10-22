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

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./imagenes/mapaSO.png")) {
        std::cout << "Error cargando la imagen de fondo" << std::endl;
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);

    // Obtener el tamaño original de la textura
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // Calcula la escala necesaria para ajustar la imagen a la ventana
    float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;

    // Aplica la escala al sprite
    backgroundSprite.setScale(scaleX, scaleY);

// Crear criaturas
for (int i = 0; i < numCriaturasPorTipo; ++i) {
    criaturas1.push_back(new Criatura(i + 1, "Especie 1", 350, 350, 1));
    criaturas2.push_back(new Criatura(i + 1, "Especie 2", 700, 300, 2));
    criaturas3.push_back(new Criatura(i + 1, "Especie 3", 500, 800, 3));
}

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Simulador de Ecosistema");

    // Crear formas para los bordes
    sf::RectangleShape bordeVerde(sf::Vector2f(600, 600));
    bordeVerde.setFillColor(sf::Color::Transparent);
    bordeVerde.setOutlineThickness(2);
    bordeVerde.setOutlineColor(sf::Color::Green);
    bordeVerde.setPosition(0, 0);

    sf::RectangleShape bordeAmarillo(sf::Vector2f(600, 600));
    bordeAmarillo.setFillColor(sf::Color::Transparent);
    bordeAmarillo.setOutlineThickness(2);
    bordeAmarillo.setOutlineColor(sf::Color::Yellow);
    bordeAmarillo.setPosition(WINDOW_WIDTH - 600, 0);

    sf::RectangleShape bordeAzul(sf::Vector2f(WINDOW_WIDTH, 500));
    bordeAzul.setFillColor(sf::Color::Transparent);
    bordeAzul.setOutlineThickness(2);
    bordeAzul.setOutlineColor(sf::Color::Blue);
    bordeAzul.setPosition(0, WINDOW_HEIGHT - 500);

    // Crear biomas (y sus límites)
    Bioma biomaVerde(0, 0, 600, 600, 100, 100);
    Bioma biomaAmarillo(WINDOW_WIDTH - 600, 0, 600, 600, 900, 120);
    Bioma biomaAzul(0, WINDOW_HEIGHT - 500, WINDOW_WIDTH, 500, 500, 800);


    // Asignar biomas a las criaturas
    for (auto& criatura : criaturas1) {
        criatura->setBioma(&biomaVerde, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    for (auto& criatura : criaturas2) {
        criatura->setBioma(&biomaAmarillo, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    for (auto& criatura : criaturas3) {
        criatura->setBioma(&biomaAzul, WINDOW_WIDTH, WINDOW_HEIGHT);
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

        // Dibuja el fondo primero
        window.draw(backgroundSprite);

        // Dibujar los bordes
        window.draw(bordeAzul);
        window.draw(bordeVerde);
        window.draw(bordeAmarillo);

        // Dibujar criaturas
        for (const auto& criatura : criaturas1) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Green); // Color de la criatura (azul para tipo 1)
            shape.setPosition(criatura->getX(), criatura->getY()); // Posición de la criatura
            window.draw(shape);
        }
        for (const auto& criatura : criaturas2) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Red); // Color de la criatura (verde para tipo 2)
            shape.setPosition(criatura->getX(), criatura->getY()); // Posición de la criatura
            window.draw(shape);
        }
        for (const auto& criatura : criaturas3) {
            sf::CircleShape shape(10); // Tamaño del círculo
            shape.setFillColor(sf::Color::Blue); // Color de la criatura (amarillo para tipo 3)
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


    

    

