#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include "Cadena.h"
#include "ArregloDinamico.h"
#include "MatrizAdyacencia.h"
#include "Usuario.h"

class GestorArchivos {
private:
    int parseEntero(const char* texto);

public:
    GestorArchivos();

    bool cargarUsuarios(const Cadena& rutaArchivo, ArregloDinamico<Usuario*>& usuarios);
    bool cargarRelaciones(const Cadena& rutaArchivo, MatrizAdyacencia& matriz);
    bool guardarRelaciones(const Cadena& rutaArchivo, const MatrizAdyacencia& matriz);
    bool guardarUsuarios(const Cadena& rutaArchivo, const ArregloDinamico<Usuario*>& usuarios);
    bool cargarAlbumes(const Cadena& rutaArchivo, ArregloDinamico<Usuario*>& usuarios);
};

#endif // GESTORARCHIVOS_H
