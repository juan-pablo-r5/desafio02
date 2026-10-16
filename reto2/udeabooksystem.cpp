#include "UdeABookSystem.h"
#include <iostream>

UdeABookSystem::UdeABookSystem()
    : usuarios(100), matrizAdyacencia(1000) {
}

UdeABookSystem::~UdeABookSystem() {
    size_t tam = usuarios.getTamano();
    for (size_t i = 0; i < tam; i++) {
        delete usuarios.obtener(i); // Liberamos cada Usuario* creado con new

    }
}

int UdeABookSystem::buscarIndiceUsuario(const Cadena& username) const {
    size_t tam = usuarios.getTamano();
    for (size_t i = 0; i < tam; i++) {
        if (usuarios.obtener(i)->getUsername() == username) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool UdeABookSystem::cargarDatos(const Cadena& rutaUsuarios, const Cadena& rutaRelaciones) {
    medidor.reiniciar();

    bool exitoUsuarios = gestorArchivos.cargarUsuarios(rutaUsuarios, usuarios);
    if (!exitoUsuarios) {
        std::cout << "Error al cargar el archivo de usuarios.\n";
        return false;
    }
    // Redimensionar cantidad usuario
    size_t totalUsuarios = usuarios.getTamano();
    matrizAdyacencia = MatrizAdyacencia(totalUsuarios > 0 ? totalUsuarios : 100);

    //  Cargar relaciones de amistad
    bool exitoRelaciones = gestorArchivos.cargarRelaciones(rutaRelaciones, matrizAdyacencia);
    if (!exitoRelaciones) {
        std::cout << "Aviso: No se pudo cargar el archivo de relaciones o está vacío.\n";
    }

    medidor.registrarMemoria(totalUsuarios * sizeof(Usuario*));
    return true;
}


bool UdeABookSystem::guardarDatos(const Cadena& rutaUsuarios, const Cadena& rutaRelaciones) {
    bool guardadoU = gestorArchivos.guardarUsuarios(rutaUsuarios, usuarios);
    bool guardadoR = gestorArchivos.guardarRelaciones(rutaRelaciones, matrizAdyacencia);
    return guardadoU && guardadoR;
}

size_t UdeABookSystem::getCantidadUsuarios() const {
    return usuarios.getTamano();
}

Usuario* UdeABookSystem::obtenerUsuario(size_t indice) {
    if (indice < usuarios.getTamano()) {
        return usuarios.obtener(indice);
    }
    return nullptr;
}

const MedidorRecursos& UdeABookSystem::getMedidor() const {
    return medidor;
}


bool UdeABookSystem::enviarSolicitud(size_t emisorIndex, const Cadena& usernameReceptor) {
    int receptorIndex = buscarIndiceUsuario(usernameReceptor);
    if (receptorIndex < 0 || static_cast<size_t>(receptorIndex) == emisorIndex) {
        return false;
    }

    size_t recIdx = static_cast<size_t>(receptorIndex);

    medidor.registrarIteracion(1);
    if (matrizAdyacencia.estanConectados(emisorIndex, recIdx)) {
        return false;
    }

    Usuario* uEmisor = usuarios.obtener(emisorIndex);
    Usuario* uReceptor = usuarios.obtener(recIdx);

    ListaEnlazada<SolicitudAmistad>& listaSol = uReceptor->getSolicitudes();
    size_t totalSol = listaSol.getTamano();
    for (size_t i = 0; i < totalSol; i++) {
        medidor.registrarIteracion(1);
        if (listaSol.obtener(i).getRemitente() == uEmisor->getUsername() &&
            listaSol.obtener(i).getEstado() == EstadoSolicitud::PENDIENTE) {
            return false;
        }
    }

    Fecha fechaActual;
    SolicitudAmistad nuevaSolicitud(uEmisor->getUsername(), uReceptor->getUsername(), fechaActual);
    uReceptor->agregarSolicitud(nuevaSolicitud);
    medidor.registrarMemoria(sizeof(SolicitudAmistad));
    return true;
}

bool UdeABookSystem::aceptarSolicitud(size_t receptorIndex, size_t emisorIndex) {
    Usuario* uReceptor = usuarios.obtener(receptorIndex);
    Usuario* uEmisor = usuarios.obtener(emisorIndex);

    if (!uReceptor || !uEmisor) return false;

    ListaEnlazada<SolicitudAmistad>& listaSol = uReceptor->getSolicitudes();
    size_t totalSol = listaSol.getTamano();

    for (size_t i = 0; i < totalSol; i++) {
        medidor.registrarIteracion(1);
        SolicitudAmistad& sol = listaSol.obtener(i);
        if (sol.getRemitente() == uEmisor->getUsername() && sol.getEstado() == EstadoSolicitud::PENDIENTE) {
            sol.aceptar();
            matrizAdyacencia.conectar(receptorIndex, emisorIndex);
            uReceptor->agregarAmigo(uEmisor);
            uEmisor->agregarAmigo(uReceptor);
            listaSol.eliminar(i);
            return true;
        }
    }
    return false;
}

bool UdeABookSystem::rechazarSolicitud(size_t receptorIndex, size_t emisorIndex) {
    Usuario* uReceptor = usuarios.obtener(receptorIndex);
    Usuario* uEmisor = usuarios.obtener(emisorIndex);

    if (!uReceptor || !uEmisor) return false;

    ListaEnlazada<SolicitudAmistad>& listaSol = uReceptor->getSolicitudes();
    size_t totalSol = listaSol.getTamano();

    for (size_t i = 0; i < totalSol; i++) {
        medidor.registrarIteracion(1);
        SolicitudAmistad& sol = listaSol.obtener(i);
        if (sol.getRemitente() == uEmisor->getUsername() && sol.getEstado() == EstadoSolicitud::PENDIENTE) {
            sol.rechazar();
            listaSol.eliminar(i);
            return true;
        }
    }
    return false;
}

ArregloDinamico<size_t> UdeABookSystem::amigosEnComun(size_t u1, size_t u2) {
    ArregloDinamico<size_t> comunes;
    size_t dim = matrizAdyacencia.getDimension();
    for (size_t j = 0; j < dim; j++) {
        medidor.registrarIteracion(1);
        if (matrizAdyacencia.estanConectados(u1, j) && matrizAdyacencia.estanConectados(u2, j)) {
            comunes.agregar(j);
        }
    }
    return comunes;
}


ArregloDinamico<size_t> UdeABookSystem::obtenerSugerenciasAmistad(size_t usuarioIndex, size_t K) {
    size_t total = usuarios.getTamano();
    ArregloDinamico<size_t> candidatos;
    ArregloDinamico<int> puntajes;

    for (size_t i = 0; i < total; i++) {
        medidor.registrarIteracion(1);
        // No sugerirse a si mismo, ni a quienes ya son amigos directos
        if (i != usuarioIndex && !matrizAdyacencia.estanConectados(usuarioIndex, i)) {
            int comunes = matrizAdyacencia.obtenerAmigosEnComun(usuarioIndex, i);
            if (comunes > 0) {
                candidatos.agregar(i);
                puntajes.agregar(comunes);
            }
        }
    }

    size_t n = candidatos.getTamano();
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            medidor.registrarIteracion(1);
            if (puntajes.obtener(j) > puntajes.obtener(i)) {

                int tempP = puntajes.obtener(i);
                puntajes.obtener(i) = puntajes.obtener(j);
                puntajes.obtener(j) = tempP;

                size_t tempC = candidatos.obtener(i);
                candidatos.obtener(i) = candidatos.obtener(j);
                candidatos.obtener(j) = tempC;
            }
        }
    }
    ArregloDinamico<size_t> resultado;
    size_t limite = (n < K) ? n : K;
    for (size_t i = 0; i < limite; i++) {
        resultado.agregar(candidatos.obtener(i));
    }
    return resultado;
}


bool UdeABookSystem::enviarMensaje(size_t emisorIndex, const Cadena& usernameReceptor, const Cadena& contenido) {
    if (contenido.getLongitud() > 512) return false; // Límite de 512 caracteres

    int receptorIndex = buscarIndiceUsuario(usernameReceptor);
    if (receptorIndex < 0) return false;

    Usuario* uEmisor = usuarios.obtener(emisorIndex);
    Usuario* uReceptor = usuarios.obtener(static_cast<size_t>(receptorIndex));

    if (!uEmisor || !uReceptor) return false;

    uEmisor->enviarMensaje(uReceptor, contenido);
    medidor.registrarMemoria(sizeof(Mensaje));
    return true;
}

ArregloDinamico<Mensaje> UdeABookSystem::obtenerMensajesNoLeidos(size_t usuarioIndex) {
    Usuario* u = usuarios.obtener(usuarioIndex);
    ArregloDinamico<Mensaje> noLeidos;
    if (!u) return noLeidos;

    ListaEnlazada<Mensaje>& recibidos = u->getMensajesRecibidos();
    size_t n = recibidos.getTamano();

    for (size_t i = 0; i < n; i++) {
        medidor.registrarIteracion(1);
        Mensaje& m = recibidos.obtener(i);
        if (!m.esLeido()) {
            noLeidos.agregar(m);
            m.marcarComoLeido(); // Se marcan como leidos
        }
    }
    // Ordenamiento cronologico ascendente (mas antiguo al mas reciente) usando sobrecarga de Fecha
    size_t totalNL = noLeidos.getTamano();
    for (size_t i = 0; i < totalNL; i++) {
        for (size_t j = i + 1; j < totalNL; j++) {
            medidor.registrarIteracion(1);
            if (noLeidos.obtener(i).getFechaEnvio() > noLeidos.obtener(j).getFechaEnvio()) {
                Mensaje temp = noLeidos.obtener(i);
                noLeidos.obtener(i) = noLeidos.obtener(j);
                noLeidos.obtener(j) = temp;
            }
        }
    }

    return noLeidos;
}

ArregloDinamico<Mensaje> UdeABookSystem::buscarMensajesPorRangoFecha(size_t usuarioIndex, const Fecha& inicio, const Fecha& fin) {
    Usuario* u = usuarios.obtener(usuarioIndex);
    ArregloDinamico<Mensaje> resultado;
    if (!u) return resultado;

    ListaEnlazada<Mensaje>& recibidos = u->getMensajesRecibidos();
    size_t n = recibidos.getTamano();

    for (size_t i = 0; i < n; i++) {
        medidor.registrarIteracion(1);
        Mensaje& m = recibidos.obtener(i);
        const Fecha& f = m.getFechaEnvio();
        if (!(f < inicio) && !(fin < f)) {
            resultado.agregar(m);
        }
    }
    return resultado;
}


bool UdeABookSystem::validarPermisoAcceso(size_t idConsultor, size_t idDuenio, Privacidad nivelPrivacidad) {
    medidor.registrarIteracion(1);
    if (idConsultor == idDuenio) {
        return true;
    }
    if (nivelPrivacidad == Privacidad::PUBLICO) {
        return true;
    }
    if (nivelPrivacidad == Privacidad::AMIGOS) {
        return matrizAdyacencia.estanConectados(idConsultor, idDuenio); // Amigos directos
    }
    return false;
}

ArregloDinamico<Album> UdeABookSystem::obtenerAlbumesVisibles(size_t idConsultor, size_t idDuenio) {
    ArregloDinamico<Album> albumesVisibles;
    Usuario* duenio = usuarios.obtener(idDuenio);
    if (!duenio) return albumesVisibles;

    ListaEnlazada<Album>& listaAlbumes = duenio->getAlbumes();
    size_t total = listaAlbumes.getTamano();

    for (size_t i = 0; i < total; i++) {
        medidor.registrarIteracion(1);
        Album& alb = listaAlbumes.obtener(i);
        if (validarPermisoAcceso(idConsultor, idDuenio, alb.getPrivacidad())) {
            albumesVisibles.agregar(alb);
        }
    }
    return albumesVisibles;
}

