#ifndef Criatura_H
#define Criatura_H

#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <random>

class Criatura {
public:
    Criatura(int id, float x, float y, sf::Color color);
    void iniciar();
    void movimiento();
    void follow(Criatura* objetivo);
    void movimientoAleatorio();
    float getX() const;
    float getY() const;
    sf::CircleShape getForma() const;

private:
    int id;
    float x, y; // Posición
    bool vivo;
    std::mutex mtx; // Mutex para la seguridad en los hilos
    sf::CircleShape forma;
private:
    float velocidad; // Velocidad de la criatura
    float areaDeProximidad; // Distancia dentro de la cual la criatura seguirá a otra
};

#endif // Criatura_H
