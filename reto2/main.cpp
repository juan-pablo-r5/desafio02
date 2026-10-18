#include <iostream>
#include <cstdlib>
#include <ctime>
#include "UdeABookSystem.h"
#include "Cadena.h"
#include "Fecha.h"

// Función auxiliar para pausar y esperar ENTER
void pausar() {
    std::cout << "\nPresione ENTER para continuar...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

// Función para mostrar las 3 fotos de perfil aleatorias ponderadas de los amigos
void mostrarFotosPerfilAmigos(Usuario* usuarioActivo, UdeABookSystem& sistema) {
    ListaEnlazada<Usuario*>& amigos = usuarioActivo->getAmigos();
    size_t totalAmigos = amigos.getTamano();

    std::cout << "\n--------------------------------------------------\n";
    std::cout << " FOTOS DE PERFIL DE AMIGOS (Destacadas):\n";
    std::cout << "--------------------------------------------------\n";

    if (totalAmigos == 0) {
        std::cout << "  (No tienes amigos agregados aún para mostrar fotos de perfil).\n";
        return;
    }

    // Seleccionar hasta 3 amigos (o los que tenga)
    size_t mostrados = 0;
    size_t intentos = 0;
    bool usados[100] = {false}; // Para evitar repetir en el mismo set de 3

    while (mostrados < 3 && mostrados < totalAmigos && intentos < 50) {
        int idx = std::rand() % totalAmigos;
        if (!usados[idx]) {
            usados[idx] = true;
            Usuario* amigo = amigos.obtener(idx);

            // Calcular amigos en común para mostrar la prioridad conceptual
            int comunes = sistema.amigosEnComun(sistema.buscarIndiceUsuario(usuarioActivo->getUsername()),
                                                sistema.buscarIndiceUsuario(amigo->getUsername())).getTamano();

            std::cout << "  - Amigo (" << amigo->getUsername() << "): " << amigo->getRutaFotoPerfil()
                      << " [Amigos en común: " << comunes << "]\n";
            mostrados++;
        }
        intentos++;
    }
}

// Menú secundario: Visitar perfil de un amigo (Carrusel de Fotos y Likes)
void menuVisitarPerfil(size_t usuarioActivoIndex, UdeABookSystem& sistema) {
    std::cout << "\n--- VISITAR PERFIL DE UN AMIGO ---\n";
    std::cout << "Ingrese el nombre de usuario (username) del amigo que desea visitar: ";
    char buffer[100];
    std::cin >> buffer;
    Cadena usernameAmigo(buffer);

    int amigoIndex = sistema.buscarIndiceUsuario(usernameAmigo);
    if (amigoIndex < 0) {
        std::cout << ">> Usuario no encontrado en la plataforma.\n";
        return;
    }

    Usuario* duenio = sistema.obtenerUsuario(static_cast<size_t>(amigoIndex));
    ArregloDinamico<Album> albumesVisibles = sistema.obtenerAlbumesVisibles(usuarioActivoIndex, static_cast<size_t>(amigoIndex));

    if (albumesVisibles.getTamano() == 0) {
        std::cout << ">> No tienes permisos para ver los álbumes de este usuario o no tiene álbumes.\n";
        return;
    }

    std::cout << "\nÁlbumes disponibles de " << duenio->getUsername() << ":\n";
    for (size_t i = 0; i < albumesVisibles.getTamano(); i++) {
        std::cout << "  [" << i << "] " << albumesVisibles.obtener(i).getNombre()
                  << " (Fotos: " << albumesVisibles.obtener(i).getCantidadFotos() << ")\n";
    }

    std::cout << "Seleccione el índice del álbum a recorrer: ";
    size_t albIdx;
    std::cin >> albIdx;

    if (albIdx >= albumesVisibles.getTamano()) {
        std::cout << ">> Índice de álbum inválido.\n";
        return;
    }

    Album& albumSeleccionado = albumesVisibles.obtener(albIdx);
    if (albumSeleccionado.getCantidadFotos() == 0) {
        std::cout << ">> Este álbum no contiene fotos.\n";
        return;
    }

    // Modo Carrusel Circular de Fotos
    size_t fotoActual = 0;
    char opcionCarrusel = ' ';
    while (opcionCarrusel != '0') {
        Foto& foto = albumSeleccionado.obtenerFoto(fotoActual);
        std::cout << "\n========================================\n";
        std::cout << " [CARRUSEL] Foto " << (fotoActual + 1) << " de " << albumSeleccionado.getCantidadFotos() << "\n";
        std::cout << " Ruta: " << foto.getRutaArchivo() << "\n";
        std::cout << " Likes actuales: " << foto.getContadorLikes() << "\n";
        std::cout << "========================================\n";
        std::cout << "Opciones:\n";
        std::cout << "  [1] Siguiente foto\n";
        std::cout << "  [2] Foto previa\n";
        std::cout << "  [3] Dar Like ❤️\n";
        std::cout << "  [4] Quitar Like \n";
        std::cout << "  [0] Salir del álbum\n";
        std::cout << "Su opción: ";
        std::cin >> opcionCarrusel;

        if (opcionCarrusel == '1') {
            fotoActual = (fotoActual + 1) % albumSeleccionado.getCantidadFotos(); // Circular
        } else if (opcionCarrusel == '2') {
            fotoActual = (fotoActual == 0) ? albumSeleccionado.getCantidadFotos() - 1 : fotoActual - 1; // Circular inverso
        } else if (opcionCarrusel == '3') {
            foto.darLike();
            std::cout << ">> ¡Has dado Like a la foto!\n";
        } else if (opcionCarrusel == '4') {
            foto.quitarLike();
            std::cout << ">> Has quitado el Like a la foto.\n";
        }
    }
}

// Menú de Administración de Amistades
void menuAdministrarAmistades(size_t usuarioIndex, UdeABookSystem& sistema) {
    Usuario* usuario = sistema.obtenerUsuario(usuarioIndex);
    char op = ' ';
    while (op != '3') {
        std::cout << "\n--- ADMINISTRAR AMISTADES ---\n";
        std::cout << "  [1] Enviar solicitud de amistad\n";
        std::cout << "  [2] Ver y responder solicitudes recibidas\n";
        std::cout << "  [3] Volver al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Ingrese el username del usuario a quien desea enviar solicitud: ";
            char buffer[100];
            std::cin >> buffer;
            Cadena receptor(buffer);
            if (sistema.enviarSolicitud(usuarioIndex, receptor)) {
                std::cout << ">> ¡Solicitud enviada con éxito!\n";
            } else {
                std::cout << ">> No se pudo enviar la solicitud (ya son amigos, usuario no existe o ya hay una pendiente).\n";
            }
            pausar();
        } else if (op == '2') {
            ListaEnlazada<SolicitudAmistad>& solicitudes = usuario->getSolicitudes();
            std::cout << "\nSolicitudes de amistad pendientes (" << solicitudes.getTamano() << "):\n";
            for (size_t i = 0; i < solicitudes.getTamano(); i++) {
                SolicitudAmistad& sol = solicitudes.obtener(i);
                if (sol.getEstado() == EstadoSolicitud::PENDIENTE) {
                    std::cout << "  [" << i << "] De: " << sol.getRemitente() << "\n";
                }
            }
            if (solicitudes.getTamano() > 0) {
                std::cout << "Ingrese el índice de la solicitud que desea gestionar (-1 para salir): ";
                int idx;
                std::cin >> idx;
                if (idx >= 0 && static_cast<size_t>(idx) < solicitudes.getTamano()) {
                    SolicitudAmistad& sol = solicitudes.obtener(idx);
                    int remitenteIdx = sistema.buscarIndiceUsuario(sol.getRemitente());
                    std::cout << "  [1] Aceptar\n  [2] Rechazar\n  Opción: ";
                    int accion;
                    std::cin >> accion;
                    if (accion == 1) {
                        sistema.aceptarSolicitud(usuarioIndex, static_cast<size_t>(remitenteIdx));
                        std::cout << ">> ¡Solicitud aceptada!\n";
                    } else if (accion == 2) {
                        sistema.rechazarSolicitud(usuarioIndex, static_cast<size_t>(remitenteIdx));
                        std::cout << ">> Solicitud rechazada.\n";
                    }
                }
            }
            pausar();
        }
    }
}

// Menú de Mensajería
void menuMensajeria(size_t usuarioIndex, UdeABookSystem& sistema) {
    char op = ' ';
    while (op != '4') {
        std::cout << "\n--- ADMINISTRAR MENSAJES ---\n";
        std::cout << "  [1] Enviar mensaje a un amigo\n";
        std::cout << "  [2] Leer nuevos mensajes (bandeja de entrada)\n";
        std::cout << "  [3] Buscar mensajes por rango de fecha\n";
        std::cout << "  [4] Volver al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Ingrese el username del destinatario: ";
            char buffer[100];
            std::cin >> buffer;
            Cadena dest(buffer);
            std::cout << "Ingrese el texto del mensaje (máx 512 caracteres):\n> ";
            std::cin.ignore();
            char texto[512];
            std::cin.getline(texto, 512);
            Cadena contenido(texto);

            if (sistema.enviarMensaje(usuarioIndex, dest, contenido)) {
                std::cout << ">> ¡Mensaje enviado con éxito!\n";
            } else {
                std::cout << ">> Error al enviar el mensaje (verifique longitud o usuario).\n";
            }
            pausar();
        } else if (op == '2') {
            ArregloDinamico<Mensaje> noLeidos = sistema.obtenerMensajesNoLeidos(usuarioIndex);
            std::cout << "\n--- BANDEJA DE MENSAJES NUEVOS (" << noLeidos.getTamano() << ") ---\n";
            for (size_t i = 0; i < noLeidos.getTamano(); i++) {
                const Mensaje& m = noLeidos.obtener(i);
                std::cout << "  [De: " << m.getEmisor() << "] \"" << m.getContenido()
                          << "\" (Fecha: " << m.getFechaEnvio().toString() << ")\n";
            }
            pausar();
        } else if (op == '3') {
            int d1, m1, a1, d2, m2, a2;
            std::cout << "Fecha de inicio (DD MM AAAA): ";
            std::cin >> d1 >> m1 >> a1;
            std::cout << "Fecha de fin (DD MM AAAA): ";
            std::cin >> d2 >> m2 >> a2;
            Fecha inicio(d1, m1, a1, 0, 0);
            Fecha fin(d2, m2, a2, 23, 59);

            ArregloDinamico<Mensaje> rango = sistema.buscarMensajesPorRangoFecha(usuarioIndex, inicio, fin);
            std::cout << "\n--- MENSAJES EN RANGO (" << rango.getTamano() << ") ---\n";
            for (size_t i = 0; i < rango.getTamano(); i++) {
                const Mensaje& m = rango.obtener(i);
                std::cout << "  [De: " << m.getEmisor() << "] \"" << m.getContenido()
                          << "\" (Fecha: " << m.getFechaEnvio().toString() << ")\n";
            }
            pausar();
        }
    }
}

// Menú Principal del Usuario Autenticado
void sesionIniciada(size_t usuarioIndex, UdeABookSystem& sistema) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Usuario* usuario = sistema.obtenerUsuario(usuarioIndex);
    char opcion = ' ';

    while (opcion != 's' && opcion != 'S') {
        std::system("cls || clear"); // Limpiar consola
        std::cout << "\n==================================================\n";
        std::cout << "   Bienvenido a UdeABook // " << usuario->getUsername() << "\n";
        std::cout << "==================================================\n";

        // Mostrar fotos de perfil aleatorias de amigos al entrar o refrescar
        mostrarFotosPerfilAmigos(usuario, sistema);

        std::cout << "\nMenú de opciones:\n";
        std::cout << "  [1] Administrar amistades\n";
        std::cout << "  [2] Visitar perfil de un amigo\n";
        std::cout << "  [3] Ver sugerencias de amistad\n";
        std::cout << "  [4] Administrar mensajes\n";
        std::cout << "  [5] Ver métricas de rendimiento (Medidor de recursos)\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "  - Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil.\n";
        std::cout << "  - Ingrese 'S' o 's' para cerrar su sesión.\n";
        std::cout << "Su opción: ";
        std::cin >> opcion;

        if (opcion == '1') {
            menuAdministrarAmistades(usuarioIndex, sistema);
        } else if (opcion == '2') {
            menuVisitarPerfil(usuarioIndex, sistema);
            pausar();
        } else if (opcion == '3') {
            std::cout << "Ingrese el valor de K (cantidad de sugerencias entre 1 y 20): ";
            size_t K;
            std::cin >> K;
            ArregloDinamico<size_t> sugerencias = sistema.obtenerSugerenciasAmistad(usuarioIndex, K);
            std::cout << "\nTop " << K << " sugerencias de amistad:\n";
            for (size_t i = 0; i < sugerencias.getTamano(); i++) {
                Usuario* sugUser = sistema.obtenerUsuario(sugerencias.obtener(i));
                int comunes = sistema.amigosEnComun(usuarioIndex, sugerencias.obtener(i)).getTamano();
                std::cout << "  -> [" << i << "] " << sugUser->getUsername() << " (Amigos en común: " << comunes << ")\n";
            }
            pausar();
        } else if (opcion == '4') {
            menuMensajeria(usuarioIndex, sistema);
        } else if (opcion == '5') {
            sistema.getMedidor().mostrarReporte();
            pausar();
        } else if (opcion == 'N' || opcion == 'n') {
            // Simplemente vuelve al bucle para refrescar las fotos aleatorias
            continue;
        }
    }
}

int main() {
    // Forzar codificación UTF-8 en consola para tildes
    system("chcp 65001 > nul");

    UdeABookSystem sistema;
    Cadena archivoUsuarios("usuarios.txt");
    Cadena archivoRelaciones("relaciones.txt");

    std::cout << "=== INICIANDO SISTEMA UDEABOOK ===\n";
    if (!sistema.cargarDatos(archivoUsuarios, archivoRelaciones)) {
        std::cout << ">> [Error] No se pudieron cargar los archivos de datos.\n";
        return 1;
    }

    char opcionPrincipal = ' ';
    while (opcionPrincipal != '0') {
        std::cout << "\n========================================\n";
        std::cout << "          INICIO DE SESIÓN             \n";
        std::cout << "========================================\n";
        std::cout << "Total de usuarios registrados: " << sistema.getCantidadUsuarios() << "\n";
        std::cout << "  [1] Iniciar sesión con Username\n";
        std::cout << "  [0] Salir del programa\n";
        std::cout << "Opción: ";
        std::cin >> opcionPrincipal;

        if (opcionPrincipal == '1') {
            std::cout << "Ingrese su nombre de usuario (username): ";
            char buffer[100];
            std::cin >> buffer;
            Cadena usernameInput(buffer);

            int userIdx = sistema.buscarIndiceUsuario(usernameInput);
            if (userIdx >= 0) {
                std::cout << ">> ¡Inicio de sesión exitoso!\n";
                pausar();
                sesionIniciada(static_cast<size_t>(userIdx), sistema);
            } else {
                std::cout << ">> [Error] Usuario no encontrado.\n";
                pausar();
            }
        }
    }

    std::cout << "\n=== Saliendo de UdeABook. ¡Hasta pronto! ===\n";
    return 0;
}
