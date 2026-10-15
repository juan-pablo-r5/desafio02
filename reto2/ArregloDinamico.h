#ifndef ARREGLODINAMICO_H
#define ARREGLODINAMICO_H
#include <cstddef>
template <typename T>

class ArregloDinamico {
private:
    T* datos;
    size_t capacidad;
    size_t tamano;

    void redimensionar() {
        capacidad *= 2;
        T* nuevosDatos = new T[capacidad];
        for (size_t i = 0; i < tamano; i++) {
            nuevosDatos[i] = datos[i];
        }
        delete[] datos;
        datos = nuevosDatos;
    }

public:
    ArregloDinamico(size_t capacidadInicial = 10) : capacidad(capacidadInicial), tamano(0) {
        datos = new T[capacidad];
    }
    ~ArregloDinamico() {
        delete[] datos;
    }

    // Constructor de copia
    ArregloDinamico(const ArregloDinamico& otro) : capacidad(otro.capacidad), tamano(otro.tamano) {
        datos = new T[capacidad];
        for (size_t i = 0; i < tamano; i++) {
            datos[i] = otro.datos[i];
        }
    }

    // Operador de asignación
    ArregloDinamico& operator=(const ArregloDinamico& otro) {
        if (this != &otro) {
            delete[] datos;
            capacidad = otro.capacidad;
            tamano = otro.tamano;
            datos = new T[capacidad];
            for (size_t i = 0; i < tamano; i++) {
                datos[i] = otro.datos[i];
            }
        }
        return *this;
    }

    void agregar(const T& valor) {
        if (tamano >= capacidad) {
            redimensionar();
        }
        datos[tamano++] = valor;
    }

    // Obtener elemento (modificable)
    T& obtener(size_t indice) {
        if (indice >= tamano) {
            throw "Error: Indice fuera de rango en ArregloDinamico.";
        }
        return datos[indice];
    }

    // Obtener elemento
    const T& obtener(size_t indice) const {
        if (indice >= tamano) {
            throw "Error: Indice fuera de rango en ArregloDinamico.";
        }
        return datos[indice];
    }

    void eliminar(size_t indice) {
        if (indice >= tamano) return;
        for (size_t i = indice; i < tamano - 1; i++) {
            datos[i] = datos[i + 1];
        }
        tamano--;
    }

    size_t getTamano() const {
        return tamano;
    }

    size_t getCapacidad() const {
        return capacidad;
    }
};
#endif // ARREGLODINAMICO_H
