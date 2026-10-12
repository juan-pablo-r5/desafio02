#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H
#include "Nodo.h"

template <typename T>
class ListaEnlazada {
private:
    Nodo<T>* cabeza;
    size_t tamano;

public:
    // Constructor por defecto
    ListaEnlazada() : cabeza(nullptr), tamano(0) {}

    // Destructor
    ~ListaEnlazada() {
        limpiar();
    }

    // Constructor de copia
    ListaEnlazada(const ListaEnlazada& otro) : cabeza(nullptr), tamano(0) {
        Nodo<T>* actual = otro.cabeza;
        while (actual != nullptr) {
            agregar(actual->getDato());
            actual = actual->getSiguiente();
        }
    }

    // Operador de asignación
    ListaEnlazada& operator=(const ListaEnlazada& otro) {
        if (this != &otro) {
            limpiar();
            Nodo<T>* actual = otro.cabeza;
            while (actual != nullptr) {
                agregar(actual->getDato());
                actual = actual->getSiguiente();
            }
        }
        return *this;
    }

    // Agregar elemento al final
    void agregar(const T& valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->getSiguiente() != nullptr) {
                actual = actual->getSiguiente();
            }
            actual->setSiguiente(nuevo);
        }
        tamano++;
    }

    // Obtener elemento por índice
    T& obtener(size_t indice) {
        if (indice >= tamano) {
            throw "Error: Indice fuera de rango en ListaEnlazada.";
        }
        Nodo<T>* actual = cabeza;
        for (size_t i = 0; i < indice; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getDato();
    }

    // Eliminar elemento por índice
    void eliminar(size_t indice) {
        if (indice >= tamano) return;
        Nodo<T>* aEliminar = nullptr;
        if (indice == 0) {
            aEliminar = cabeza;
            cabeza = cabeza->getSiguiente();
        } else {
            Nodo<T>* actual = cabeza;
            for (size_t i = 0; i < indice - 1; i++) {
                actual = actual->getSiguiente();
            }
            aEliminar = actual->getSiguiente();
            actual->setSiguiente(aEliminar->getSiguiente());
        }
        delete aEliminar;
        tamano--;
    }

    // Consultar tamaño
    size_t getTamano() const {
        return tamano;
    }

    // Vaciar lista
    void limpiar() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->getSiguiente();
            delete actual;
            actual = siguiente;
        }
        cabeza = nullptr;
        tamano = 0;
    }
};

#endif // LISTAENLAZADA_H
