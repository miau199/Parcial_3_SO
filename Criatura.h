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
    void aumentarHambre();
    void actualizarVelocidad();
    int getHambre() const { return hambre; }
    int getVelocidadActual() const { return velocidadActual; }
    void huir();
    bool estaSiendoPerseguida(const std::vector<Criatura*>& depredadores);


private:
    int id;
    std::string especie;
    int x, y;
    bool vivo;
    int hambre;
    int tipo; // Tipo de criatura
    std::thread hilo; // Hilo para el movimiento
    Bioma* bioma;
    int screenWidth;
    int screenHeight;
    int direccionX; //Direccion actual
    int direccionY;
    int tiempoDireccion;    //Tiempo que seguira la dirección actual
    const static int TIEMPO_CAMBIO_DIRECCION = 50; //5 segundos
    int velocidadBase;
    int velocidadActual;
    std::chrono::steady_clock::time_point ultimaActualizacionHambre;
    static const int HAMBRE_MAXIMA = 100;
    static const int TIEMPO_HAMBRE_MAXIMA = 60; // 60 segundos
    float centroBiomaX;
    float centroBiomaY;
    bool estaEnBordeBioma(int x, int y) const;
};

#endif // CRIATURA_H