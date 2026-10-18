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

void mostrarFotosPerfilAmigos(Usuario* usuarioActivo, UdeABookSystem& sistema) {
    ListaEnlazada<Usuario*>& amigos = usuarioActivo->getAmigos();
    size_t totalAmigos = amigos.getTamano();

    std::cout << "\n--------------------------------------------------\n";
    std::cout << " FOTOS DE PERFIL DE AMIGOS (Destacadas):\n";
    std::cout << "--------------------------------------------------\n";

    if (totalAmigos == 0) {
        std::cout << "  (No tienes amigos agregados aun para mostrar fotos).\n";
        return;
    }

    ArregloDinamico<int> poolSorteo(totalAmigos * 7);
    int idxActivo = sistema.buscarIndiceUsuario(usuarioActivo->getUsername());

    for (size_t i = 0; i < totalAmigos; i++) {
        Usuario* amigo = amigos.obtener(i);
        int idxAmigo = sistema.buscarIndiceUsuario(amigo->getUsername());
        int comunes = sistema.amigosEnComun(idxActivo, idxAmigo).getTamano();

        int peso = 2;
        if (comunes >= 11 && comunes <= 20) {
            peso = 5;
        } else if (comunes >= 21) {
            peso = 7;
        }

        for (int p = 0; p < peso; p++) {
            poolSorteo.agregar(static_cast<int>(i));
        }
    }

    size_t mostrados = 0, intentos = 0;
    bool usados[1000] = {false};

    while (mostrados < 3 && mostrados < totalAmigos && intentos < 100) {
        int randomIdx = std::rand() % poolSorteo.getTamano();
        int idxElegido = poolSorteo.obtener(randomIdx);

        if (!usados[idxElegido]) {
            usados[idxElegido] = true;
            Usuario* amigo = amigos.obtener(idxElegido);
            std::cout << "  - [" << amigo->getUsername() << "]: " << amigo->getRutaFotoPerfil() << "\n";
            mostrados++;
        }
        intentos++;
    }
}

void menuAdministrarAmistades(size_t usuarioIndex, UdeABookSystem& sistema) {
    Usuario* usuario = sistema.obtenerUsuario(usuarioIndex);
    char op = ' ';
    while (op != '3') {
        std::cout << "\n--- ADMINISTRAR AMISTADES ---\n";
        std::cout << "  [1] a. Enviar solicitud de amistad\n";
        std::cout << "  [2] b. Aceptar solicitudes de amistad recibidas\n";
        std::cout << "  [3] Volver al menu principal\nSeleccione: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Username del destinatario: ";
            char buffer[100]; std::cin >> buffer;
            if (sistema.enviarSolicitud(usuarioIndex, Cadena(buffer))) {
                std::cout << ">> Solicitud enviada exitosamente.\n";
            } else {
                std::cout << ">> Error: Ya son amigos, no existe el usuario, o ya hay solicitud pendiente.\n";
            }
            sistema.getMedidor().mostrarReporte(); pausar();
        } else if (op == '2') {
            ListaEnlazada<SolicitudAmistad>& solicitudes = usuario->getSolicitudes();
            std::cout << "\n--- SOLICITUDES PENDIENTES ---\n";
            bool hayPendientes = false;
            for (size_t i = 0; i < solicitudes.getTamano(); i++) {
                if (solicitudes.obtener(i).getEstado() == EstadoSolicitud::PENDIENTE) {
                    std::cout << "  [" << i << "] De: " << solicitudes.obtener(i).getRemitente() << "\n";
                    hayPendientes = true;
                }
            }
            if (hayPendientes) {
                std::cout << "Ingrese el indice de la solicitud a aceptar (-1 para cancelar): ";
                int idx; std::cin >> idx;
                if (idx >= 0 && static_cast<size_t>(idx) < solicitudes.getTamano()) {
                    int remitenteIdx = sistema.buscarIndiceUsuario(solicitudes.obtener(idx).getRemitente());
                    if(sistema.aceptarSolicitud(usuarioIndex, remitenteIdx)) {
                        std::cout << ">> ¡Solicitud aceptada! Ahora son amigos.\n";
                    }
                }
            } else {
                std::cout << ">> No tienes solicitudes pendientes.\n";
            }
            sistema.getMedidor().mostrarReporte(); pausar();
        }
    }
}

void menuVisitarPerfil(size_t usuarioIndex, UdeABookSystem& sistema) {
    std::cout << "\n--- VISITAR PERFIL DE UN AMIGO ---\n";
    std::cout << "Ingrese el username del amigo: ";
    char buffer[100]; std::cin >> buffer;

    int amigoIndex = sistema.buscarIndiceUsuario(Cadena(buffer));
    if (amigoIndex < 0) {
        std::cout << ">> Usuario no encontrado.\n";
        sistema.getMedidor().mostrarReporte(); pausar();
        return;
    }

    ArregloDinamico<Album> albumesVisibles = sistema.obtenerAlbumesVisibles(usuarioIndex, static_cast<size_t>(amigoIndex));
    if (albumesVisibles.getTamano() == 0) {
        std::cout << ">> No tiene albumes (Publicos/Amigos) disponibles para ti.\n";
        sistema.getMedidor().mostrarReporte(); pausar();
        return;
    }

    std::cout << "\nAlbumes disponibles:\n";
    for (size_t i = 0; i < albumesVisibles.getTamano(); i++) {
        std::cout << "  [" << i << "] " << albumesVisibles.obtener(i).getNombre() << " (Fotos: " << albumesVisibles.obtener(i).getCantidadFotos() << ")\n";
    }

    std::cout << "Seleccione el indice del album a abrir en modo carrusel: ";
    size_t albIdx; std::cin >> albIdx;
    if (albIdx >= albumesVisibles.getTamano()) return;

    Album& albumSeleccionado = albumesVisibles.obtener(albIdx);
    if (albumSeleccionado.getCantidadFotos() == 0) {
        std::cout << ">> El album esta vacio.\n";
        sistema.getMedidor().mostrarReporte(); pausar();
        return;
    }

    size_t fotoActual = 0;
    char opCarrusel = ' ';
    while (opCarrusel != '4') {
        Foto& foto = albumSeleccionado.obtenerFoto(fotoActual);
        std::system("cls || clear");
        std::cout << "\n========================================\n";
        std::cout << " [CARRUSEL] Foto " << (fotoActual + 1) << " de " << albumSeleccionado.getCantidadFotos() << "\n";
        std::cout << " Ruta Visualizada: " << foto.getRutaArchivo() << "\n";
        std::cout << " Likes actuales: " << foto.getContadorLikes() << "\n";
        std::cout << "========================================\n";
        std::cout << " [1] Siguiente foto\n [2] Foto previa\n [3] Dar/Quitar Like\n [4] Salir del album\nSu opcion: ";
        std::cin >> opCarrusel;

        if (opCarrusel == '1') fotoActual = (fotoActual + 1) % albumSeleccionado.getCantidadFotos();
        else if (opCarrusel == '2') fotoActual = (fotoActual == 0) ? albumSeleccionado.getCantidadFotos() - 1 : fotoActual - 1;
        else if (opCarrusel == '3') {
            std::cout << "¿Desea [1] Dar Like o [2] Quitar Like?: ";
            int opLike; std::cin >> opLike;
            if (opLike == 1) foto.darLike();
            else if (opLike == 2) foto.quitarLike();
        }
        sistema.getMedidor().mostrarReporte();
    }
}

void menuMensajeria(size_t usuarioIndex, UdeABookSystem& sistema) {
    char op = ' ';
    while (op != '4') {
        std::cout << "\n--- ADMINISTRAR MENSAJES ---\n";
        std::cout << "  [1] a. Enviar mensaje\n  [2] b. Leer nuevos mensajes\n  [3] c. Buscar mensajes por rango de fecha\n  [4] Volver\nSeleccione: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Username del amigo destinatario: ";
            char buffer[100]; std::cin >> buffer;
            std::cout << "Escriba su mensaje (Max 512 caracteres):\n> ";
            std::cin.ignore();
            char texto[512]; std::cin.getline(texto, 512);
            if(sistema.enviarMensaje(usuarioIndex, Cadena(buffer), Cadena(texto))) {
                std::cout << ">> Mensaje enviado correctamente.\n";
            } else {
                std::cout << ">> Error al enviar: Verifique que el usuario existe y es su amigo.\n";
            }
            sistema.getMedidor().mostrarReporte(); pausar();
        } else if (op == '2') {
            ArregloDinamico<Mensaje> noLeidos = sistema.obtenerMensajesNoLeidos(usuarioIndex);
            if (noLeidos.getTamano() == 0) {
                std::cout << ">> No tienes mensajes nuevos no leidos.\n";
            } else {
                std::cout << "\n--- MENSAJES NUEVOS ---\n";
                for (size_t i = 0; i < noLeidos.getTamano(); i++) {
                    std::cout << " [De: " << noLeidos.obtener(i).getEmisor() << " | Fecha: " << noLeidos.obtener(i).getFechaEnvio().toString() << "]\n";
                    std::cout << " Mensaje: " << noLeidos.obtener(i).getContenido() << "\n-----------------------\n";
                }
                Usuario* usr = sistema.obtenerUsuario(usuarioIndex);
                ListaEnlazada<Mensaje>& recibidos = usr->getMensajesRecibidos();
                for (size_t j = 0; j < recibidos.getTamano(); j++) {
                    recibidos.obtener(j).marcarComoLeido();
                }
                std::cout << ">> (Todos los mensajes han sido marcados como leidos automaticamente).\n";
            }
            sistema.getMedidor().mostrarReporte(); pausar();
        } else if (op == '3') {
            int d1, m1, a1, d2, m2, a2;
            std::cout << "Fecha de INICIO (DD MM AAAA): "; std::cin >> d1 >> m1 >> a1;
            std::cout << "Fecha de FIN (DD MM AAAA): "; std::cin >> d2 >> m2 >> a2;
            ArregloDinamico<Mensaje> rango = sistema.buscarMensajesPorRangoFecha(usuarioIndex, Fecha(d1,m1,a1,0,0), Fecha(d2,m2,a2,23,59));
            std::cout << "\n--- MENSAJES EN EL RANGO (" << rango.getTamano() << " encontrados) ---\n";
            for (size_t i = 0; i < rango.getTamano(); i++) {
                std::cout << " [De: " << rango.obtener(i).getEmisor() << " | Fecha: " << rango.obtener(i).getFechaEnvio().toString() << "]\n";
                std::cout << " Mensaje: " << rango.obtener(i).getContenido() << "\n-----------------------\n";
            }
            sistema.getMedidor().mostrarReporte(); pausar();
        }
    }
}

void sesionIniciada(size_t usuarioIndex, UdeABookSystem& sistema) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Usuario* usuario = sistema.obtenerUsuario(usuarioIndex);
    char opcion = ' ';

    while (opcion != 's' && opcion != 'S') {
        std::system("cls || clear");
        std::cout << "\n==================================================\n";
        std::cout << "   UdeABook - Sesion Iniciada: " << usuario->getUsername() << "\n";
        std::cout << "==================================================\n";

        mostrarFotosPerfilAmigos(usuario, sistema);

        std::cout << "\nMENU PRINCIPAL:\n";
        std::cout << "  [1] Administrar amistades\n";
        std::cout << "  [2] Visitar perfil de un amigo\n";
        std::cout << "  [3] Ver sugerencias de amistad\n";
        std::cout << "  [4] Administrar mensajes\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "  - Tecla 'N' o 'n': Refrescar fotos de perfil.\n";
        std::cout << "  - Tecla 'S' o 's': Cerrar sesion.\nSu opcion: ";
        std::cin >> opcion;

        if (opcion == '1') {
            menuAdministrarAmistades(usuarioIndex, sistema);
        } else if (opcion == '2') {
            menuVisitarPerfil(usuarioIndex, sistema);
        } else if (opcion == '3') {
            std::cout << "\n--- SUGERENCIAS DE AMISTAD ---\n";
            std::cout << "Ingrese la cantidad (K) de sugerencias a ver (1-20): ";
            size_t K; std::cin >> K;
            if (K >= 1 && K <= 20) {
                ArregloDinamico<size_t> sugerencias = sistema.obtenerSugerenciasAmistad(usuarioIndex, K);
                for (size_t i = 0; i < sugerencias.getTamano(); i++) {
                    Usuario* uSug = sistema.obtenerUsuario(sugerencias.obtener(i));
                    int comunes = sistema.amigosEnComun(usuarioIndex, sugerencias.obtener(i)).getTamano();
                    std::cout << "  [" << i << "] " << uSug->getUsername() << " (Amigos en comun: " << comunes << ")\n";
                }
                std::cout << "\n¿Desea enviar solicitud a alguno? Ingrese su Username (o escriba '0' para cancelar): ";
                char sugBuffer[100]; std::cin >> sugBuffer;
                if (sugBuffer[0] != '0') {
                    if(sistema.enviarSolicitud(usuarioIndex, Cadena(sugBuffer))) std::cout << ">> Solicitud enviada!\n";
                    else std::cout << ">> Error al enviar solicitud.\n";
                }
            } else {
                std::cout << ">> El valor de K debe estar entre 1 y 20.\n";
            }
            sistema.getMedidor().mostrarReporte(); pausar();
        } else if (opcion == '4') {
            menuMensajeria(usuarioIndex, sistema);
        } else if (opcion == 'N' || opcion == 'n') {
            continue;
        }
    }
}

int main() {
    system("chcp 65001 > nul");
    UdeABookSystem sistema;
    Cadena archivoUsuarios("usuarios.txt");
    Cadena archivoRelaciones("relaciones.txt");

    if (!sistema.cargarDatos(archivoUsuarios, archivoRelaciones)) {
        std::cout << "Error critico al cargar los archivos. Deteniendo el sistema.\n";
        return 1;
    }

    char opcionPrincipal = ' ';
    while (opcionPrincipal != '0') {
        std::cout << "\n========================================\n";
        std::cout << "           PORTAL UDEABOOK              \n";
        std::cout << "========================================\n";
        std::cout << "  [1] Iniciar sesion con Username\n  [0] Salir del sistema\nOpción: ";
        std::cin >> opcionPrincipal;

        if (opcionPrincipal == '1') {
            std::cout << "Ingrese sus credenciales (Username): ";
            char buffer[100]; std::cin >> buffer;
            int userIdx = sistema.buscarIndiceUsuario(Cadena(buffer));

            if (userIdx >= 0) {
                sesionIniciada(static_cast<size_t>(userIdx), sistema);
            } else {
                std::cout << ">> Identidad no verificada. Usuario no encontrado.\n";
            }
        }
    }

    std::cout << "\n=== Guardando datos en almacenamiento permanente... ===\n";
    sistema.guardarDatos(archivoUsuarios, archivoRelaciones);
    std::cout << "=== Sistema apagado exitosamente ===\n";
    return 0;
}