#ifndef UDEABOOKSYSTEM_H
#define UDEABOOKSYSTEM_H
#include "ArregloDinamico.h"
#include "MatrizAdyacencia.h"
#include "GestorArchivos.h"
#include "MedidorRecursos.h"
#include "Usuario.h"
#include "Cadena.h"


class UdeABookSystem
{
private:
    ArregloDinamico<Usuario*> usuarios;
    MatrizAdyacencia matrizAdyacencia;
    GestorArchivos gestorArchivos;
    MedidorRecursos medidor;

    // Metodos internos de ayuda
    int buscarIndiceUsuario(const Cadena& username) const;
public:
    UdeABookSystem();
    ~UdeABookSystem();

    // Carga inicial y guardado al salir
    bool cargarDatos(const Cadena& rutaUsuarios, const Cadena& rutaRelaciones);
    bool guardarDatos(const Cadena& rutaUsuarios, const Cadena& rutaRelaciones);

    // Metodos de control y acceso
    size_t getCantidadUsuarios() const;
    Usuario* obtenerUsuario(size_t indice);

    const MedidorRecursos& getMedidor() const;
};

#endif // UDEABOOKSYSTEM_H
