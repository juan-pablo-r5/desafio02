#ifndef USUARIO_H
#define USUARIO_H

#include "Cadena.h"
#include "Fecha.h"
#include "ListaEnlazada.h"
#include "Album.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"

class Usuario {
private:
    Cadena username;
    Cadena email;
    Fecha fechaNacimiento;
    Cadena ciudad;
    Cadena pais;
    Fecha fechaInscripcion;
    Cadena rutaFotoPerfil;

    ListaEnlazada<Usuario*> amigos;
    ListaEnlazada<Album> albumes;
    ListaEnlazada<Mensaje> mensajesEnviados;
    ListaEnlazada<Mensaje> mensajesRecibidos;
    ListaEnlazada<SolicitudAmistad> solicitudes;

public:
    Usuario();
    Usuario(const Cadena& usr, const Cadena& correo, const Fecha& nac,
            const Cadena& cd, const Cadena& p, const Fecha& insc, const Cadena& rutaFoto);

    void agregarAmigo(Usuario* amigo);
    void agregarAlbum(const Album& album);
    void enviarMensaje(Usuario* destinatario, const Cadena& contenido);
    void recibirMensaje(const Mensaje& msj);
    void agregarSolicitud(const SolicitudAmistad& solicitud); // F

    // (Faltaban para ver perfiles)
    const Cadena& getUsername() const;
    const Cadena& getEmail() const;
    const Cadena& getCiudad() const;
    const Cadena& getPais() const;
    const Fecha& getFechaNacimiento() const;
    const Fecha& getFechaInscripcion() const;
    const Cadena& getRutaFotoPerfil() const;

    ListaEnlazada<Usuario*>& getAmigos();
    ListaEnlazada<Album>& getAlbumes();
    ListaEnlazada<Mensaje>& getMensajesEnviados();     // F
    ListaEnlazada<Mensaje>& getMensajesRecibidos();
    ListaEnlazada<SolicitudAmistad>& getSolicitudes(); // F
};

#endif // USUARIO_H
