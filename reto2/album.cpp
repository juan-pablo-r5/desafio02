#include "Album.h"

Album::Album() : nombreAlbum(""), nivelPrivacidad(Privacidad::PUBLICO), fotos(10) {}


Album::Album(const Cadena& nombre, Privacidad privacidad)
    : nombreAlbum(nombre), nivelPrivacidad(privacidad), fotos(10) {}

const Cadena& Album::getNombre() const {
    return nombreAlbum;
}

Privacidad Album::getPrivacidad() const {
    return nivelPrivacidad;
}

void Album::setPrivacidad(Privacidad privacidad) {
    nivelPrivacidad = privacidad;
}

void Album::agregarFoto(const Foto& foto) {
    fotos.agregar(foto);
}

Foto& Album::obtenerFoto(size_t indice) {
    return fotos.obtener(indice);
}

const Foto& Album::obtenerFoto(size_t indice) const {
    return fotos.obtener(indice);
}

size_t Album::getCantidadFotos() const {
    return fotos.getTamano();
}
