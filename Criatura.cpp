#include "Criatura.h"

const float DISTANCIA_DETECCION = 100.0f; //100 pixeles o menos de distancia

Criatura::Criatura(int id, std::string especie, int x, int y, int tipo)
    : id(id), especie(especie), x(x), y(y), vivo(true), hambre(0), 
      velocidadBase(25), velocidadActual(velocidadBase), tipo(tipo),
      direccionX(0), direccionY(0), tiempoDireccion(0) {
    actualizarDireccionAleatoria();
    ultimaActualizacionHambre = std::chrono::steady_clock::now();
}


void Criatura::mover(std::vector<Criatura*>& criaturas1, std::vector<Criatura*>& criaturas2, std::vector<Criatura*>& criaturas3, std::mutex& mtx) {
  
    while (vivo) {

        aumentarHambre(); // Aumenta el hambre y actualiza la velocidad
        std::this_thread::sleep_for(std::chrono::milliseconds(velocidadActual));

        // Determina qué vector de criaturas seguir basado en el tipo
        std::vector<Criatura*>& criaturasObjetivo = 
            (tipo == 1) ? criaturas2 : 
            (tipo == 2) ? criaturas3 : criaturas1;

        // Asigna depredadores a cada tipo de criatura
        std::vector<Criatura*>& depredadores = 
            (tipo == 1) ? criaturas3 : 
            (tipo == 2) ? criaturas1 : criaturas2;


        std::lock_guard<std::mutex> lock(mtx);

        int newX, newY;
        if (estaSiendoPerseguida(depredadores)) {
            huir();
        } else {
            Criatura* objetivoMasCercano = buscarObjetivoMasCercano(criaturasObjetivo);
            if (objetivoMasCercano && !estaEnBordeBioma(x, y)) {
                newX = x + (objetivoMasCercano->x > x ? 1 : (objetivoMasCercano->x < x ? -1 : 0));
                newY = y + (objetivoMasCercano->y > y ? 1 : (objetivoMasCercano->y < y ? -1 : 0));
                moverDentroDelBioma(newX, newY);
            } else {
                // No hay objetivos cercanos o está en el borde, se mueve aleatoriamente
                moverAleatorio();
            }
      
        
        }
        std::cout << "Criatura " << id << " (Tipo " << tipo << ") en posición (" << x << ", " << y << ")\n";
    }
}



void Criatura::detener() {
    vivo = false;   // Detiene a todas las criaturas (cambiar por matar)
}

void Criatura::matar() {
    vivo = false; // Cambiar el estado a muerto
}


// Busca la criatura más cercana del tipo apropiado (calcula la hipotenusa de la distancia x, distancia y)
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

// Incrementa el nivel de hambre de la criatura con el tiempo
void Criatura::aumentarHambre() {
    auto ahora = std::chrono::steady_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::seconds>(ahora - ultimaActualizacionHambre).count();
    
    if (duracion >= 1) { // Actualiza cada segundo
        hambre += static_cast<int>((HAMBRE_MAXIMA * duracion) / TIEMPO_HAMBRE_MAXIMA);
        if (hambre > HAMBRE_MAXIMA) {
            hambre = HAMBRE_MAXIMA;
        }
        ultimaActualizacionHambre = ahora;
        actualizarVelocidad();
    }
}

// Ajusta la velocidad de la criatura basándose en su nivel de hambre (mayor hambre, más letna la criatura, el doble de lento)
void Criatura::actualizarVelocidad() {
    float factor = 1.0f + (static_cast<float>(hambre) / HAMBRE_MAXIMA);
    velocidadActual = static_cast<int>(velocidadBase * factor);
    if (velocidadActual > velocidadBase * 2) {
        velocidadActual = velocidadBase * 2;
    }
}

// - Calcula un factor de aumento basado en el hambre actual
// - Aumenta la velocidad (tiempo de espera) proporcionalmente al hambre
// - La velocidad máxima es el doble de la velocidad base
// - A mayor hambre, mayor velocidad (tiempo de espera), por lo tanto, movimiento más lento

//Velocidad base: velocidad inicial

/*Ejemplo: Criatura con hambre = 50 (hambre media)

factor = 1.0f + (50 / 100) = 1.5
velocidadActual = 25 * 1.5 = 37.5 (se redondea a 37 al convertirse a int)
No se aplica el límite máximo, ya que 37 no es mayor que 50*/


// Huye a un punto especifico
void Criatura::huir() {
    int margen = 30;
    int newX, newY;

    switch(tipo) {
        case 1: // Esquina superior izquierda
            newX = margen;
            newY = margen;
            break;
        case 2: // Esquina superior derecha
            newX = screenWidth - margen;
            newY = margen;
            break;
        case 3: // Esquina inferior derecha
            newX = margen;
            newY = screenHeight - margen;
            break;
        default:
            return; 
    }

    // Calcula la dirección hacia la esquina
    int dx = newX - x;
    int dy = newY - y;
    float distancia = std::sqrt(dx*dx + dy*dy);
    
    if (distancia > 0) {
        int moveX = x + (dx / distancia);
        int moveY = y + (dy / distancia);
        moverDentroDelBioma(moveX, moveY);
    }
}





// Verifica si la criatura está siendo perseguida por algún depredador
bool Criatura::estaSiendoPerseguida(const std::vector<Criatura*>& depredadores) {
    for (const auto& depredador : depredadores) {
        float dx = x - depredador->x;
        float dy = y - depredador->y;
        float distancia = std::sqrt(dx*dx + dy*dy);
        if (distancia <= DISTANCIA_DETECCION) {
            return true;
        }
    }
    return false;
}

/*
Recorre la lista de depredadores potenciales.
Para cada depredador, calcula la distancia entre la criatura y el depredador.
Si algún depredador está dentro de la DISTANCIA_DETECCION, retorna true.
Si no hay depredadores cercanos, retorna false.*/


bool Criatura::estaEnBordeBioma(int x, int y) const {
    // Margen de 5 píxeles hasta el borde del bioma
    const int margen = 5;
    return x <= bioma->getForma().getPosition().x + margen ||
           x >= bioma->getForma().getPosition().x + bioma->getForma().getSize().x - margen ||
           y <= bioma->getForma().getPosition().y + margen ||
           y >= bioma->getForma().getPosition().y + bioma->getForma().getSize().y - margen;
}