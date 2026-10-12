#include "Mensaje.h"

// Constructor por defecto
Mensaje::Mensaje()
    : emisorUsername(""), receptorUsername(""), contenido(""), fechaEnvio(), leido(false) {}

// Constructor parametrizado
Mensaje::Mensaje(const Cadena& emisor, const Cadena& receptor, const Cadena& texto, const Fecha& fecha)
    : emisorUsername(emisor), receptorUsername(receptor), contenido(texto), fechaEnvio(fecha), leido(false) {}

// Getters
const Cadena& Mensaje::getEmisor() const {
    return emisorUsername;
}

const Cadena& Mensaje::getReceptor() const {
    return receptorUsername;
}

const Cadena& Mensaje::getContenido() const {
    return contenido;
}

const Fecha& Mensaje::getFechaEnvio() const {
    return fechaEnvio;
}

bool Mensaje::esLeido() const {
    return leido;
}

// Cambiar estado del mensaje
void Mensaje::marcarComoLeido() {
    leido = true;
}

// Operadores basados en la clase Fecha para facilitar el ordenamiento cronológico
bool Mensaje::operator<(const Mensaje& otro) const {
    return fechaEnvio < otro.fechaEnvio;
}

bool Mensaje::operator>(const Mensaje& otro) const {
    return fechaEnvio > otro.fechaEnvio;
}
