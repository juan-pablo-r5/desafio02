#ifndef UDEABOOKSYSTEM_H
#define UDEABOOKSYSTEM_H

#include "ArregloDinamico.h"
#include "MatrizAdyacencia.h"
#include "GestorArchivos.h"
#include "MedidorRecursos.h"
#include "Usuario.h"
#include "Cadena.h"
#include "Fecha.h"
#include "Mensaje.h"
#include "Album.h"
#include "SolicitudAmistad.h"

class UdeABookSystem {
private:
    ArregloDinamico<Usuario*> usuarios;
    MatrizAdyacencia matrizAdyacencia;
    GestorArchivos gestorArchivos;
    MedidorRecursos medidor;

public:
    UdeABookSystem();
    ~UdeABookSystem();

    // Carga inicial y guardado
    bool cargarDatos(const Cadena& rutaUsuarios, const Cadena& rutaRelaciones);
    bool guardarDatos(const Cadena& rutaUsuarios, const Cadena& rutaRelaciones);

    // Métodos de control y acceso
    size_t getCantidadUsuarios() const;
    Usuario* obtenerUsuario(size_t indice);
    int buscarIndiceUsuario(const Cadena& username) const;
    const MedidorRecursos& getMedidor() const;

    // --- FASE 2: GESTIÓN DE AMISTADES ---
    bool enviarSolicitud(size_t emisorIndex, const Cadena& usernameReceptor);
    bool aceptarSolicitud(size_t receptorIndex, size_t emisorIndex);
    bool rechazarSolicitud(size_t receptorIndex, size_t emisorIndex);
    ArregloDinamico<size_t> amigosEnComun(size_t u1, size_t u2);

    // Sugerencias de amistad rankeadas por matriz de adyacencia (K sugerencias)
    ArregloDinamico<size_t> obtenerSugerenciasAmistad(size_t usuarioIndex, size_t K);

    // --- FASE 2: SISTEMA DE MENSAJERÍA ---
    bool enviarMensaje(size_t emisorIndex, const Cadena& usernameReceptor, const Cadena& contenido);
    ArregloDinamico<Mensaje> obtenerMensajesNoLeidos(size_t usuarioIndex);
    ArregloDinamico<Mensaje> buscarMensajesPorRangoFecha(size_t usuarioIndex, const Fecha& inicio, const Fecha& fin);

    // --- FASE 2: MULTIMEDIA Y PRIVACIDAD ---
    bool validarPermisoAcceso(size_t idConsultor, size_t idDuenio, Privacidad nivelPrivacidad);
    ArregloDinamico<Album> obtenerAlbumesVisibles(size_t idConsultor, size_t idDuenio);
};

#endif // UDEABOOKSYSTEM_H
