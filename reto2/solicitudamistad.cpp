#include "SolicitudAmistad.h"

// Constructor por defecto
SolicitudAmistad::SolicitudAmistad()
    : remitenteUsername(""), destinatarioUsername(""), fechaEnvio(), estado(EstadoSolicitud::PENDIENTE) {}

// Constructor parametrizado
SolicitudAmistad::SolicitudAmistad(const Cadena& remitente, const Cadena& destinatario, const Fecha& fecha)
    : remitenteUsername(remitente), destinatarioUsername(destinatario), fechaEnvio(fecha), estado(EstadoSolicitud::PENDIENTE) {}

const Cadena& SolicitudAmistad::getRemitente() const {
    return remitenteUsername;
}

const Cadena& SolicitudAmistad::getDestinatario() const {
    return destinatarioUsername;
}

const Fecha& SolicitudAmistad::getFechaEnvio() const {
    return fechaEnvio;
}

EstadoSolicitud SolicitudAmistad::getEstado() const {
    return estado;
}

void SolicitudAmistad::aceptar() {
    estado = EstadoSolicitud::ACEPTADA;
}

void SolicitudAmistad::rechazar() {
    estado = EstadoSolicitud::RECHAZADA;
}
