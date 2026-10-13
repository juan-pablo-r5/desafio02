#include "Usuario.h"

Usuario::Usuario() {}

Usuario::Usuario(const Cadena& usr, const Cadena& correo, const Fecha& nac,
                 const Cadena& cd, const Cadena& p, const Fecha& insc, const Cadena& rutaFoto)
    : username(usr), email(correo), fechaNacimiento(nac), ciudad(cd),
    pais(p), fechaInscripcion(insc), rutaFotoPerfil(rutaFoto) {}

void Usuario::agregarAmigo(Usuario* amigo) {
    amigos.agregar(amigo);
}

void Usuario::agregarAlbum(const Album& album) {
    albumes.agregar(album);
}

void Usuario::enviarMensaje(Usuario* destinatario, const Cadena& contenido) {
    Fecha fechaActual;
    Mensaje nuevoMsj(username, destinatario->getUsername(), contenido, fechaActual);
    mensajesEnviados.agregar(nuevoMsj);
    destinatario->recibirMensaje(nuevoMsj);
}

void Usuario::recibirMensaje(const Mensaje& msj) {
    mensajesRecibidos.agregar(msj);
}

void Usuario::agregarSolicitud(const SolicitudAmistad& solicitud) {
    solicitudes.agregar(solicitud);
}

// Getters implementados
const Cadena& Usuario::getUsername() const {
    return username;
}
const Cadena& Usuario::getEmail() const {
    return email;
}
const Cadena& Usuario::getCiudad() const {
    return ciudad;
}
const Cadena& Usuario::getPais() const {
    return pais;
}
const Fecha& Usuario::getFechaNacimiento() const {
    return fechaNacimiento;
}
const Fecha& Usuario::getFechaInscripcion() const {
    return fechaInscripcion;
}
const Cadena& Usuario::getRutaFotoPerfil() const {
    return rutaFotoPerfil;
}

ListaEnlazada<Usuario*>& Usuario::getAmigos() {
    return amigos;
}
ListaEnlazada<Album>& Usuario::getAlbumes() {
    return albumes;
}
ListaEnlazada<Mensaje>& Usuario::getMensajesEnviados() {
    return mensajesEnviados;
}
ListaEnlazada<Mensaje>& Usuario::getMensajesRecibidos() {
    return mensajesRecibidos;
}
ListaEnlazada<SolicitudAmistad>& Usuario::getSolicitudes() {
    return solicitudes;
}
