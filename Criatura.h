#ifndef CRIATURA_H
#define CRIATURA_H

#include "Bioma.h"

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <ctime>

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
    Criatura* buscarObjetivoMasCercano(const std::vector<Criatura*>& criaturas);
    void setBioma(Bioma* bioma, int screenWidth, int screenHeight);
    void moverDentroDelBioma(int newX, int newY);
    void moverAleatorio();
    void actualizarDireccionAleatoria();


private:
    int id;
    std::string especie;
    int x, y;
    bool vivo;
    int hambre;
    int velocidad;
    int tipo; // Tipo de criatura
    std::thread hilo; // Hilo para el movimiento
    Bioma* bioma;
    int screenWidth;
    int screenHeight;
    int direccionX; //Direccion actual
    int direccionY;
    int tiempoDireccion;    //Tiempo que seguira la dirección actual
    const static int TIEMPO_CAMBIO_DIRECCION = 50; //5 segundos
};

#endif // CRIATURA_H