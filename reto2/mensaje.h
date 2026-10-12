#ifndef MENSAJE_H
#define MENSAJE_H

#include "Cadena.h"
#include "Fecha.h"
class Mensaje
{
private:
    Cadena emisorUsername;
    Cadena receptorUsername;
    Cadena contenido; // Texto del mensaje (hasta 512 caracteres)
    Fecha fechaEnvio;
    bool leido;
public:
    Mensaje();
    Mensaje(const Cadena& emisor, const Cadena& receptor, const Cadena& texto, const Fecha& fecha);

    // Métodos de consulta (Getters)
    const Cadena& getEmisor() const;
    const Cadena& getReceptor() const;
    const Cadena& getContenido() const;
    const Fecha& getFechaEnvio() const;
    bool esLeido() const;

    // Métodos de acción
    void marcarComoLeido();

    // Sobrecarga de operadores para ordenar mensajes cronológicamente
    bool operator<(const Mensaje& otro) const;
    bool operator>(const Mensaje& otro) const;
};

#endif // MENSAJE_H


