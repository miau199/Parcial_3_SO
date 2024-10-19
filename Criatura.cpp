#include "Criatura.h"
#include <cstdlib> // Para rand() y RAND_MAX
#include <ctime>   // Para time()

// Constructor
Criatura::Criatura(int id, float x, float y, sf::Color color) 
    : id(id), x(x), y(y), vivo(true), velocidad(2.0f), areaDeProximidad(50.0f) {
    forma.setRadius(10);
    forma.setFillColor(color);
    forma.setPosition(x, y);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Semilla para la generación de números aleatorios
}

void Criatura::iniciar() {
    std::thread(&Criatura::movimiento, this).detach(); // Inicia el hilo de movimiento
}

void Criatura::movimiento() {
    while (vivo) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Espera 1 segundo entre movimientos
        movimientoAleatorio(); // Moverse aleatoriamente por defecto
    }
}

void Criatura::follow(Criatura* objetivo) {
    if (objetivo) {
        std::lock_guard<std::mutex> lock(mtx);
        
        // Calcular la distancia al objetivo
        float distanciaX = objetivo->getX() - x;
        float distanciaY = objetivo->getY() - y;
        float distancia = sqrt(distanciaX * distanciaX + distanciaY * distanciaY);

        // Solo seguir si está dentro del umbral de proximidad
        if (distancia < areaDeProximidad) {
            x += (distanciaX / distancia) * velocidad; // Moverse hacia el objetivo
            y += (distanciaY / distancia) * velocidad;
            forma.setPosition(x, y);
            std::cout << "Criatura " << id << " sigue a criatura " << objetivo->id << " hasta (" << x << ", " << y << ")\n";
        }
    }
}

void Criatura::movimientoAleatorio() {
    std::lock_guard<std::mutex> lock(mtx);
    if (std::rand() % 5 == 0) {
        // Generar coordenadas aleatorias dentro de los límites de la pantalla
        float randomX = static_cast<float>(std::rand() % 800); // Asumiendo que el ancho de la pantalla es 800
        float randomY = static_cast<float>(std::rand() % 600); // Asumiendo que la altura de la pantalla es 600

        // Calcular la dirección hacia el punto aleatorio
        float distanciaX = randomX - x;
        float distanciaY = randomY - y;
        float distancia = sqrt(distanciaX * distanciaX + distanciaY * distanciaY);

        // Moverse hacia el punto aleatorio
        if (distancia > 0) {
            x += (distanciaX / distancia) * velocidad; // Normalizar y mover
            y += (distanciaY / distancia) * velocidad;
            forma.setPosition(x, y);
            std::cout << "Criatura " << id << " se está moviendo hacia (" << x << ", " << y << ")\n";
        }
    }
}

float Criatura::getX() const {
    return x;
}

float Criatura::getY() const {
    return y;
}

sf::CircleShape Criatura::getForma() const {
    return forma; // Devolver la forma para renderizar
}
