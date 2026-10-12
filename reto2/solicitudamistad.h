#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H
#include "Cadena.h"
#include "Fecha.h"

enum class EstadoSolicitud {
    PENDIENTE,
    ACEPTADA,
    RECHAZADA
};

class SolicitudAmistad
{
private:
    Cadena remitenteUsername;
    Cadena destinatarioUsername;
    Fecha fechaEnvio;
    EstadoSolicitud estado;

public:
    SolicitudAmistad();
    SolicitudAmistad(const Cadena& remitente, const Cadena& destinatario, const Fecha& fecha);

    // Getters
    const Cadena& getRemitente() const;
    const Cadena& getDestinatario() const;
    const Fecha& getFechaEnvio() const;
    EstadoSolicitud getEstado() const;

    void aceptar();
    void rechazar();
};

#endif // SOLICITUDAMISTAD_H

