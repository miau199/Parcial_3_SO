#include "Criatura.h"

const float DISTANCIA_DETECCION = 100.0f; 

Criatura::Criatura(int id, std::string especie, int x, int y, int tipo)
    : id(id), especie(especie), x(x), y(y), vivo(true), hambre(0), velocidad(100), tipo(tipo),
      direccionX(0), direccionY(0), tiempoDireccion(0) {
    actualizarDireccionAleatoria();
}

void Criatura::mover(std::vector<Criatura*>& criaturas1, std::vector<Criatura*>& criaturas2, std::vector<Criatura*>& criaturas3, std::mutex& mtx) {
  
    while (vivo) {

        std::this_thread::sleep_for(std::chrono::milliseconds(velocidad));

        // Determina qué vector de criaturas seguir basado en el tipo
        std::vector<Criatura*>& criaturasObjetivo = 
            (tipo == 1) ? criaturas2 : 
            (tipo == 2) ? criaturas3 : criaturas1;

        Criatura* objetivoMasCercano = buscarObjetivoMasCercano(criaturasObjetivo);

        std::lock_guard<std::mutex> lock(mtx);

        int newX, newY;
        if (objetivoMasCercano) {
            newX = x + (objetivoMasCercano->x > x ? 1 : (objetivoMasCercano->x < x ? -1 : 0));
            newY = y + (objetivoMasCercano->y > y ? 1 : (objetivoMasCercano->y < y ? -1 : 0));
        } else {
             moverAleatorio();
        }

        moverDentroDelBioma(newX, newY);

        std::cout << "Criatura " << id << " (Tipo " << tipo << ") en posición (" << x << ", " << y << ")\n";
    }
}

// Calcula una nueva posición (newX y newY) basada en la posición actual de la criatura y la posición del objetivo
void Criatura::seguir(Criatura* objetivo) {
    if (objetivo) {
        int newX = x + (objetivo->x > x ? 1 : (objetivo->x < x ? -1 : 0));
        int newY = y + (objetivo->y > y ? 1 : (objetivo->y < y ? -1 : 0));
        moverDentroDelBioma(newX, newY); // Verifica si está dentro del bioma
    }
}

void Criatura::detener() {
    vivo = false;   // Detiene a todas las criaturas (cambiar por matar)
}

void Criatura::matar() {
    vivo = false; // Cambiar el estado a muerto
}


// Busca la criatura más cercana del tipo apropiado (usa la formula de la distancia euclidiana)
Criatura* Criatura::buscarObjetivoMasCercano(const std::vector<Criatura*>& criaturas) {
    Criatura* objetivoMasCercano = nullptr;
    float distanciaMasCorta = std::numeric_limits<float>::max();

    for (Criatura* criatura : criaturas) {
        float dx = criatura->x - x;
        float dy = criatura->y - y;
        float distancia = std::sqrt(dx*dx + dy*dy);

        if (distancia < distanciaMasCorta && distancia <= DISTANCIA_DETECCION) {
            distanciaMasCorta = distancia;
            objetivoMasCercano = criatura;
        }
    }

    return objetivoMasCercano;
}

// Asigna un bioma especifico a cada criatura
void Criatura::setBioma(Bioma* b, int sWidth, int sHeight) {
    bioma = b;
    screenWidth = sWidth;
    screenHeight = sHeight;
}

// Verifica si la nueva posición está dentro del bioma asignado
void Criatura::moverDentroDelBioma(int newX, int newY) {
    if (bioma && bioma->contienePunto(newX, newY)) {
        x = newX;
        y = newY;
    }
}

// Movimiento cuando no persigue a una ciratura
void Criatura::moverAleatorio() {
    if (tiempoDireccion <= 0) {
        actualizarDireccionAleatoria();
    }

    int newX = x + direccionX;
    int newY = y + direccionY;
    
    moverDentroDelBioma(newX, newY);
    
    tiempoDireccion--;
}

// Cambia a una dirección aleatoria cada 5 segundos
void Criatura::actualizarDireccionAleatoria() {
    direccionX = (rand() % 3) - 1;  // -1, 0, or 1
    direccionY = (rand() % 3) - 1;  // -1, 0, or 1
    tiempoDireccion = TIEMPO_CAMBIO_DIRECCION;
}