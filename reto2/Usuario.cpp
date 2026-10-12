#include "Usuario.h"

Usuario::Usuario() {
}

Usuario::Usuario(Cadena usr, Cadena correo, Fecha nac, Cadena cd, Cadena p, Fecha insc, Cadena rutaFoto) {
    username = usr;
    email = correo;
    fechaNacimiento = nac;
    ciudad = cd;
    pais = p;
    fechaInscripcion = insc;
    rutaFotoPerfil = rutaFoto;
}

void Usuario::agregarAmigo(Usuario* amigo) {
    amigos.agregar(amigo);
}

void Usuario::agregarAlbum(const Album& album) {
    albumes.agregar(album);
}

void Usuario::enviarMensaje(Usuario* destinatario, Cadena contenido) {
    Fecha fechaActual;
    Mensaje nuevoMsj(username, destinatario->getUsername(), contenido, fechaActual);
    mensajesEnviados.agregar(nuevoMsj);
    destinatario->recibirMensaje(nuevoMsj);
}

void Usuario::recibirMensaje(const Mensaje& msj) {
    mensajesRecibidos.agregar(msj);
}

Cadena Usuario::getUsername() const {
    return username;
}

ListaEnlazada<Usuario*>& Usuario::getAmigos() {
    return amigos;
}

ListaEnlazada<Album>& Usuario::getAlbumes() {
    return albumes;
}

ListaEnlazada<Mensaje>& Usuario::getMensajesRecibidos() {
    return mensajesRecibidos;
}