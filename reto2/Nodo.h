#ifndef NODO_H
#define NODO_H

template <typename T>
class Nodo {
private:
    T dato;
    Nodo<T>* siguiente;

public:
    // Constructor
    Nodo(const T& valor) : dato(valor), siguiente(nullptr) {}

    // Getters y Setters
    T& getDato() {
        return dato;
    }

    const T& getDato() const {
        return dato;
    }

    Nodo<T>* getSiguiente() const {
        return siguiente;
    }

    void setSiguiente(Nodo<T>* sig) {
        siguiente = sig;
    }
};

#endif
