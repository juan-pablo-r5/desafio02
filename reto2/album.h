#ifndef ALBUM_H
#define ALBUM_H

#include "Cadena.h"
#include "Foto.h"
#include "ArregloDinamico.h"

enum class Privacidad {
    PUBLICO,
    PRIVADO,
    AMIGOS
};


class Album{
private:
    Cadena nombreAlbum;
    Privacidad nivelPrivacidad;
    ArregloDinamico<Foto> fotos; // Colección interna de fotos sin usar la STL

public:
    Album();
    Album(const Cadena& nombre, Privacidad privacidad);

    const Cadena& getNombre() const;
    Privacidad getPrivacidad() const;
    void setPrivacidad(Privacidad privacidad);

    void agregarFoto(const Foto& foto);
    Foto& obtenerFoto(size_t indice);
    const Foto& obtenerFoto(size_t indice) const;
    size_t getCantidadFotos() const;
};

#endif // ALBUM_H
