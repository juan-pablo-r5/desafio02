#include "MatrizAdyacencia.h"

MatrizAdyacencia::MatrizAdyacencia(size_t dim) : dimension(dim) {
    matriz = new bool*[dimension];
    for (size_t i = 0; i < dimension; i++) {
        matriz[i] = new bool[dimension];
        for (size_t j = 0; j < dimension; j++) {
            matriz[i][j] = false; // Inicialmente sin conexiones
        }
    }
}

// Destructor: Libera toda la memoria reservada
MatrizAdyacencia::~MatrizAdyacencia() {
    for (size_t i = 0; i < dimension; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

// Constructor de copia
MatrizAdyacencia::MatrizAdyacencia(const MatrizAdyacencia& otro) : dimension(otro.dimension) {
    matriz = new bool*[dimension];
    for (size_t i = 0; i < dimension; i++) {
        matriz[i] = new bool[dimension];
        for (size_t j = 0; j < dimension; j++) {
            matriz[i][j] = otro.matriz[i][j];
        }
    }
}

MatrizAdyacencia& MatrizAdyacencia::operator=(const MatrizAdyacencia& otro) {
    if (this != &otro) {
        for (size_t i = 0; i < dimension; i++) {
            delete[] matriz[i];
        }
        delete[] matriz;

        dimension = otro.dimension;
        matriz = new bool*[dimension];
        for (size_t i = 0; i < dimension; i++) {
            matriz[i] = new bool[dimension];
            for (size_t j = 0; j < dimension; j++) {
                matriz[i][j] = otro.matriz[i][j];
            }
        }
    }
    return *this;
}

void MatrizAdyacencia::conectar(size_t u1, size_t u2) {
    if (u1 < dimension && u2 < dimension) {
        matriz[u1][u2] = true;
        matriz[u2][u1] = true;
    }
}

void MatrizAdyacencia::desconectar(size_t u1, size_t u2) {
    if (u1 < dimension && u2 < dimension) {
        matriz[u1][u2] = false;
        matriz[u2][u1] = false;
    }
}

bool MatrizAdyacencia::estanConectados(size_t u1, size_t u2) const {
    if (u1 < dimension && u2 < dimension) {
        return matriz[u1][u2];
    }
    return false;
}

// Algoritmo eficiente para calcular amigos en común (crucial para sugerencias de amistad)
int MatrizAdyacencia::obtenerAmigosEnComun(size_t u1, size_t u2) const {
    if (u1 >= dimension || u2 >= dimension) return 0;

    int contadorAmigosEnComun = 0;
    for (size_t i = 0; i < dimension; i++) {
        // Si 'i' es amigo de u1 y también de u2 (y no se compara a sí mismo ni con los dos consultados)
        if (i != u1 && i != u2 && matriz[u1][i] && matriz[u2][i]) {
            contadorAmigosEnComun++;
        }
    }
    return contadorAmigosEnComun;
}

// Redimensionar la matriz dinamicamente si se agregan mas usuarios de los previstos
void MatrizAdyacencia::redimensionar(size_t nuevaDimension) {
    if (nuevaDimension <= dimension) return;

    bool** nuevaMatriz = new bool*[nuevaDimension];
    for (size_t i = 0; i < nuevaDimension; i++) {
        nuevaMatriz[i] = new bool[nuevaDimension];
        for (size_t j = 0; j < nuevaDimension; j++) {
            if (i < dimension && j < dimension) {
                nuevaMatriz[i][j] = matriz[i][j];
            } else {
                nuevaMatriz[i][j] = false;
            }
        }
    }

    // Liberar matriz vieja
    for (size_t i = 0; i < dimension; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;

    matriz = nuevaMatriz;
    dimension = nuevaDimension;
}

size_t MatrizAdyacencia::getDimension() const {
    return dimension;
}
