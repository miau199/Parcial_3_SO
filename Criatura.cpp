#include "Criatura.h"

Criatura::Criatura(int id, std::string especie, int x, int y, int tipo)
    : id(id), especie(especie), x(x), y(y), vivo(true), hambre(0), velocidad(100), tipo(tipo) {}

void Criatura::mover(std::vector<Criatura*>& criaturas1, std::vector<Criatura*>& criaturas2, std::vector<Criatura*>& criaturas3, std::mutex& mtx) {
    while (vivo) {
        std::this_thread::sleep_for(std::chrono::milliseconds(velocidad));
        std::lock_guard<std::mutex> lock(mtx);

        // Movimiento aleatorio
        x += (rand() % 3) - 1;
        y += (rand() % 3) - 1;

        // Lógica de seguimiento
        if (tipo == 1 && !criaturas2.empty()) {
            seguir(criaturas2[rand() % criaturas2.size()]);
        } else if (tipo == 2 && !criaturas3.empty()) {
            seguir(criaturas3[rand() % criaturas3.size()]);
        } else if (tipo == 3 && !criaturas1.empty()) {
            seguir(criaturas1[rand() % criaturas1.size()]);
        }

        std::cout << "Criatura " << id << " (Tipo " << tipo << ") en posición (" << x << ", " << y << ")\n";
    }
}

void Criatura::seguir(Criatura* objetivo) {
    if (objetivo) {
        if (x < objetivo->x) x++;
        else if (x > objetivo->x) x--;

        if (y < objetivo->y) y++;
        else if (y > objetivo->y) y--;
    }
}

void Criatura::detener() {
    vivo = false;
}

void Criatura::matar() {
    vivo = false; // Cambiar el estado a muerto
}