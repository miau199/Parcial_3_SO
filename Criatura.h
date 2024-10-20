#ifndef CRIATURA_H
#define CRIATURA_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <cstdlib>
#include <chrono>

class Criatura {
public:
    Criatura(int id, std::string especie, int x, int y, int tipo);
    void mover(std::vector<Criatura*>& criaturas1, std::vector<Criatura*>& criaturas2, std::vector<Criatura*>& criaturas3, std::mutex& mtx);
    void seguir(Criatura* objetivo);
    void detener();
    void matar();
    int getX() const { return x; }
    int getY() const { return y; }
    int getTipo() const { return tipo; }

private:
    int id;
    std::string especie;
    int x, y;
    bool vivo;
    int hambre;
    int velocidad;
    int tipo; // Tipo de criatura
    std::thread hilo; // Hilo para el movimiento
};

#endif // CRIATURA_H