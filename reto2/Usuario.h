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
    Usuario(Cadena usr, Cadena correo, Fecha nac, Cadena cd, Cadena p, Fecha insc, Cadena rutaFoto);

    void agregarAmigo(Usuario* amigo);
    void agregarAlbum(const Album& album);
    void enviarMensaje(Usuario* destinatario, Cadena contenido);
    void recibirMensaje(const Mensaje& msj);

    Cadena getUsername() const;
    ListaEnlazada<Usuario*>& getAmigos();
    ListaEnlazada<Album>& getAlbumes();
    ListaEnlazada<Mensaje>& getMensajesRecibidos();
};

#endif // USUARIO_H
