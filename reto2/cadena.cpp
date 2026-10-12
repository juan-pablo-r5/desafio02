#include "Cadena.h"

Cadena::Cadena() {
    longitud = 0;
    buffer = new char[1];
    buffer[0] = '\0';
}

Cadena::Cadena(const char* str) {
    longitud = 0;
    if (str != nullptr) {
        while (str[longitud] != '\0') longitud++;
    }

    buffer = new char[longitud + 1];
    for (size_t i = 0; i < longitud; ++i) {
        buffer[i] = str[i];
    }
    buffer[longitud] = '\0';
}

Cadena::Cadena(const Cadena& otra) {
    longitud = otra.longitud;
    buffer = new char[longitud + 1];
    for (size_t i = 0; i < longitud; ++i) {
        buffer[i] = otra.buffer[i];
    }
    buffer[longitud] = '\0';
}

Cadena::~Cadena() {
    delete[] buffer;
}

Cadena& Cadena::operator=(const Cadena& otra) {
    if (this != &otra) {
        delete[] buffer;

        longitud = otra.longitud;
        buffer = new char[longitud + 1];
        for (size_t i = 0; i < longitud; ++i) {
            buffer[i] = otra.buffer[i];
        }
        buffer[longitud] = '\0';
    }
    return *this;
}

bool Cadena::operator==(const Cadena& otra) const {
    if (longitud != otra.longitud) return false;

    for (size_t i = 0; i < longitud; ++i) {
        if (buffer[i] != otra.buffer[i]) return false;
    }
    return true;
}

bool Cadena::operator!=(const Cadena& otra) const {
    return !(*this == otra);
}

Cadena Cadena::operator+(const Cadena& otra) const {
    Cadena nueva;
    delete[] nueva.buffer;

    nueva.longitud = longitud + otra.longitud;
    nueva.buffer = new char[nueva.longitud + 1];

    size_t i = 0;
    for (size_t j = 0; j < longitud; ++j) {
        nueva.buffer[i++] = buffer[j];
    }
    for (size_t j = 0; j < otra.longitud; ++j) {
        nueva.buffer[i++] = otra.buffer[j];
    }
    nueva.buffer[nueva.longitud] = '\0';

    return nueva;
}

const char* Cadena::getBuffer() const { return buffer; }
size_t Cadena::getLongitud() const { return longitud; }