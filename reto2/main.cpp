#include <iostream>
#include <cstdlib>
#include <ctime>
#include "UdeABookSystem.h"
#include "Cadena.h"
#include "Fecha.h"

void pausar() {
    std::cout << "\nPresione ENTER para continuar...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void imprimirMetricas(UdeABookSystem& sistema, const char* operacion) {
    std::cout << "\n[ Metricas: " << operacion << " ]";
    sistema.getMedidor().mostrarReporte();
    pausar();
}

void limpiarConsola() {
    std::system("cls || clear");
}

void mostrarFotosPerfilAmigos(Usuario* usuarioActivo, UdeABookSystem& sistema) {
    ListaEnlazada<Usuario*>& amigos = usuarioActivo->getAmigos();
    size_t totalAmigos = amigos.getTamano();

    std::cout << "\n--- FOTOS DESTACADAS ---\n";
    if (totalAmigos == 0) {
        std::cout << " (No tienes amigos agregados aun para mostrar fotos).\n";
        return;
    }

    ArregloDinamico<int> poolSorteo(totalAmigos * 7);
    int idxActivo = sistema.buscarIndiceUsuario(usuarioActivo->getUsername());

    for (size_t i = 0; i < totalAmigos; i++) {
        Usuario* amigo = amigos.obtener(i);
        int idxAmigo = sistema.buscarIndiceUsuario(amigo->getUsername());
        int comunes = sistema.amigosEnComun(idxActivo, idxAmigo).getTamano();

        int peso = (comunes >= 21) ? 7 : (comunes >= 11 ? 5 : 2);
        for (int p = 0; p < peso; p++) poolSorteo.agregar(static_cast<int>(i));
    }

    size_t mostrados = 0, intentos = 0;
    bool usados[1000] = {false};

    while (mostrados < 3 && mostrados < totalAmigos && intentos < 100) {
        int randomIdx = std::rand() % poolSorteo.getTamano();
        int idxElegido = poolSorteo.obtener(randomIdx);

        if (!usados[idxElegido]) {
            usados[idxElegido] = true;
            Usuario* amigo = amigos.obtener(idxElegido);
            std::cout << " > [" << amigo->getUsername() << "]: " << amigo->getRutaFotoPerfil() << "\n";
            mostrados++;
        }
        intentos++;
    }
}

void menuAdministrarAmistades(size_t usuarioIndex, UdeABookSystem& sistema) {
    Usuario* usuario = sistema.obtenerUsuario(usuarioIndex);
    char op = ' ';
    while (op != '3') {
        std::cout << "\n--- AMISTADES ---\n [1] Enviar solicitud\n [2] Aceptar recibidas\n [3] Volver\nSeleccione: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Username del destinatario: ";
            char buffer[100]; std::cin >> buffer;
            if (sistema.enviarSolicitud(usuarioIndex, Cadena(buffer))) std::cout << ">> Solicitud enviada.\n";
            else std::cout << ">> Error al enviar.\n";
            imprimirMetricas(sistema, "Envio de Solicitud");

        } else if (op == '2') {
            ListaEnlazada<SolicitudAmistad>& solicitudes = usuario->getSolicitudes();
            std::cout << "\n- PENDIENTES -\n";
            bool hay = false;
            for (size_t i = 0; i < solicitudes.getTamano(); i++) {
                if (solicitudes.obtener(i).getEstado() == EstadoSolicitud::PENDIENTE) {
                    std::cout << " [" << i << "] De: " << solicitudes.obtener(i).getRemitente() << "\n";
                    hay = true;
                }
            }
            if (hay) {
                std::cout << "Indice a aceptar (-1 cancelar): ";
                int idx; std::cin >> idx;
                if (idx >= 0 && static_cast<size_t>(idx) < solicitudes.getTamano()) {
                    int remitenteIdx = sistema.buscarIndiceUsuario(solicitudes.obtener(idx).getRemitente());
                    if(sistema.aceptarSolicitud(usuarioIndex, remitenteIdx)) std::cout << ">> ¡Son amigos!\n";
                }
            } else std::cout << ">> Sin solicitudes.\n";
            imprimirMetricas(sistema, "Aceptacion Amistad");
        }
    }
}

void menuVisitarPerfil(size_t usuarioIndex, UdeABookSystem& sistema) {
    std::cout << "\n--- VISITAR PERFIL ---\nUsername del amigo: ";
    char buffer[100]; std::cin >> buffer;

    int amigoIndex = sistema.buscarIndiceUsuario(Cadena(buffer));
    if (amigoIndex < 0) {
        std::cout << ">> Usuario no encontrado.\n";
        imprimirMetricas(sistema, "Busqueda Fallida");
        return;
    }

    ArregloDinamico<Album> albumesVisibles = sistema.obtenerAlbumesVisibles(usuarioIndex, static_cast<size_t>(amigoIndex));
    if (albumesVisibles.getTamano() == 0) {
        std::cout << ">> No tiene albumes disponibles para ti.\n";
        imprimirMetricas(sistema, "Filtro Privacidad Albumes");
        return;
    }

    std::cout << "\nAlbumes disponibles:\n";
    for (size_t i = 0; i < albumesVisibles.getTamano(); i++) {
        std::cout << " [" << i << "] " << albumesVisibles.obtener(i).getNombre() << " (" << albumesVisibles.obtener(i).getCantidadFotos() << " fotos)\n";
    }

    std::cout << "Indice del album a abrir: ";
    size_t albIdx; std::cin >> albIdx;
    if (albIdx >= albumesVisibles.getTamano()) return;

    Album& album = albumesVisibles.obtener(albIdx);
    if (album.getCantidadFotos() == 0) {
        std::cout << ">> Album vacio.\n";
        return;
    }

    size_t fotoActual = 0;
    char opCarrusel = ' ';
    while (opCarrusel != '4') {
        Foto& foto = album.obtenerFoto(fotoActual);
        limpiarConsola();
        std::cout << "\n[ Foto " << (fotoActual + 1) << " de " << album.getCantidadFotos() << " ]\n";
        std::cout << "Ruta: " << foto.getRutaArchivo() << "\nLikes: " << foto.getContadorLikes() << "\n";
        std::cout << "--------------------------------\n [1] Siguiente | [2] Previa | [3] Like | [4] Salir\nOpcion: ";
        std::cin >> opCarrusel;

        if (opCarrusel == '1') fotoActual = (fotoActual + 1) % album.getCantidadFotos();
        else if (opCarrusel == '2') fotoActual = (fotoActual == 0) ? album.getCantidadFotos() - 1 : fotoActual - 1;
        else if (opCarrusel == '3') {
            std::cout << " [1] Dar | [2] Quitar: ";
            int op; std::cin >> op;
            if (op == 1) foto.darLike(); else if (op == 2) foto.quitarLike();
        }
        if (opCarrusel != '4') imprimirMetricas(sistema, "Interaccion Foto");
    }
}

void menuMensajeria(size_t usuarioIndex, UdeABookSystem& sistema) {
    char op = ' ';
    while (op != '4') {
        std::cout << "\n--- MENSAJES ---\n [1] Enviar\n [2] Leer nuevos\n [3] Buscar por fecha\n [4] Volver\nOpcion: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Para (Username): ";
            char buffer[100]; std::cin >> buffer;
            std::cout << "Mensaje: "; std::cin.ignore();
            char texto[512]; std::cin.getline(texto, 512);
            if(sistema.enviarMensaje(usuarioIndex, Cadena(buffer), Cadena(texto))) std::cout << ">> Enviado.\n";
            else std::cout << ">> Error.\n";
            imprimirMetricas(sistema, "Envio Mensaje");

        } else if (op == '2') {
            ArregloDinamico<Mensaje> noLeidos = sistema.obtenerMensajesNoLeidos(usuarioIndex);
            if (noLeidos.getTamano() == 0) std::cout << ">> Sin mensajes nuevos.\n";
            else {
                for (size_t i = 0; i < noLeidos.getTamano(); i++) {
                    std::cout << "[De: " << noLeidos.obtener(i).getEmisor() << "] " << noLeidos.obtener(i).getContenido() << "\n";
                }
                ListaEnlazada<Mensaje>& recibidos = sistema.obtenerUsuario(usuarioIndex)->getMensajesRecibidos();
                for (size_t j = 0; j < recibidos.getTamano(); j++) recibidos.obtener(j).marcarComoLeido();
                std::cout << ">> Marcados como leidos.\n";
            }
            imprimirMetricas(sistema, "Lectura Mensajes");

        } else if (op == '3') {
            int d1, m1, a1, d2, m2, a2;
            std::cout << "Inicio (DD MM AAAA): "; std::cin >> d1 >> m1 >> a1;
            std::cout << "Fin (DD MM AAAA): "; std::cin >> d2 >> m2 >> a2;
            ArregloDinamico<Mensaje> r = sistema.buscarMensajesPorRangoFecha(usuarioIndex, Fecha(d1,m1,a1,0,0), Fecha(d2,m2,a2,23,59));
            std::cout << "\nEncontrados: " << r.getTamano() << "\n";
            for (size_t i = 0; i < r.getTamano(); i++) {
                std::cout << "[" << r.obtener(i).getFechaEnvio().toString() << " | De: " << r.obtener(i).getEmisor() << "] " << r.obtener(i).getContenido() << "\n";
            }
            imprimirMetricas(sistema, "Busqueda Fecha");
        }
    }
}

void sesionIniciada(size_t usuarioIndex, UdeABookSystem& sistema) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Usuario* usuario = sistema.obtenerUsuario(usuarioIndex);
    char opcion = ' ';

    while (opcion != 's' && opcion != 'S') {
        limpiarConsola();
        std::cout << "\n*** UdeABook | Hola, " << usuario->getUsername() << " ***\n";

        mostrarFotosPerfilAmigos(usuario, sistema);
        std::cout << "\n [ Medidor: Calculo Fotos Priorizadas ]";
        sistema.getMedidor().mostrarReporte();

        std::cout << "\nMENU PRINCIPAL:\n [1] Amistades\n [2] Perfiles\n [3] Sugerencias\n [4] Mensajes\n [N] Refrescar | [S] Salir\nOpcion: ";
        std::cin >> opcion;

        if (opcion == '1') menuAdministrarAmistades(usuarioIndex, sistema);
        else if (opcion == '2') menuVisitarPerfil(usuarioIndex, sistema);
        else if (opcion == '3') {
            std::cout << "\nCant. sugerencias (1-20): "; size_t K; std::cin >> K;
            if (K >= 1 && K <= 20) {
                ArregloDinamico<size_t> sug = sistema.obtenerSugerenciasAmistad(usuarioIndex, K);
                for (size_t i = 0; i < sug.getTamano(); i++) {
                    Usuario* uSug = sistema.obtenerUsuario(sug.obtener(i));
                    std::cout << " [" << i << "] " << uSug->getUsername() << " (Mutuos: " << sistema.amigosEnComun(usuarioIndex, sug.obtener(i)).getTamano() << ")\n";
                }
                std::cout << "\nUsername para enviar solicitud (o '0' cancelar): ";
                char buf[100]; std::cin >> buf;
                if (buf[0] != '0') {
                    if(sistema.enviarSolicitud(usuarioIndex, Cadena(buf))) std::cout << ">> Enviada!\n";
                    else std::cout << ">> Error.\n";
                }
            }
            imprimirMetricas(sistema, "Sugerencias");
        } else if (opcion == '4') menuMensajeria(usuarioIndex, sistema);
        else if (opcion == 'N' || opcion == 'n') continue;
    }
}

int main() {
    system("chcp 65001 > nul");
    UdeABookSystem sistema;

    std::cout << "\n*** UDEABOOK SYSTEM ***\n[+] Cargando datos...\n";
    if (!sistema.cargarDatos(Cadena("usuarios.txt"), Cadena("relaciones.txt"))) return 1;
    imprimirMetricas(sistema, "Carga Inicial");

    char op = ' ';
    while (op != '0') {
        limpiarConsola();
        std::cout << "\n*** UDEABOOK LOGIN ***\n [1] Iniciar sesion\n [0] Salir\nOpcion: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Username: ";
            char buffer[100]; std::cin >> buffer;
            int idx = sistema.buscarIndiceUsuario(Cadena(buffer));
            imprimirMetricas(sistema, "Login");

            if (idx >= 0) sesionIniciada(static_cast<size_t>(idx), sistema);
            else { std::cout << ">> No encontrado.\n"; pausar(); }
        }
    }

    std::cout << "\n[+] Guardando datos...\n";
    sistema.guardarDatos(Cadena("usuarios.txt"), Cadena("relaciones.txt"));
    imprimirMetricas(sistema, "Escritura");
    return 0;
}
