#ifndef FOTO_H
#define FOTO_H

#include "Cadena.h"
#include "Fecha.h"
class Foto
{
private:
    Cadena rutaArchivo;
    Fecha fechaCaptura;
    int contadorLikes;
public:
    Foto();
    Foto(const Cadena& ruta, const Fecha& fecha);

    const Cadena& getRutaArchivo() const;
    const Fecha& getFechaCaptura() const;
    int getContadorLikes() const;

    void darLike();
    void quitarLike();
};
#endif // FOTO_H


