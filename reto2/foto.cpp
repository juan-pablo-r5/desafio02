#include "Foto.h"

Foto::Foto() : rutaArchivo(""), fechaCaptura(), contadorLikes(0) {}

Foto::Foto(const Cadena& ruta, const Fecha& fecha)
    : rutaArchivo(ruta), fechaCaptura(fecha), contadorLikes(0) {}

const Cadena& Foto::getRutaArchivo() const {
    return rutaArchivo;
}

const Fecha& Foto::getFechaCaptura() const {
    return fechaCaptura;
}

int Foto::getContadorLikes() const {
    return contadorLikes;
}

void Foto::darLike() {
    contadorLikes++;
}

void Foto::quitarLike() {
    if (contadorLikes > 0) {
        contadorLikes--;
    }
}
