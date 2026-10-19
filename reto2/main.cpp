#include <iostream>
#include <cstdlib>
#include <ctime>
#include "UdeABookSystem.h"
#include "Cadena.h"
#include "Fecha.h"

// --- FUNCIONES AUXILIARES (Para simplificar visualmente) ---
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

// --- MODULOS DEL SISTEMA ---
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
        std::cout << "\n--- AMISTADES ---\n";
        std::cout << " [1] Enviar solicitud de amistad\n";
        std::cout << " [2] Aceptar solicitudes recibidas\n";
        std::cout << " [3] Volver al menu principal\nSeleccione: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Username del destinatario: ";
            char buffer[100]; std::cin >> buffer;
            if (sistema.enviarSolicitud(usuarioIndex, Cadena(buffer))) {
                std::cout << ">> Solicitud enviada exitosamente.\n";
            } else {
                std::cout << ">> Error: Ya son amigos, no existe el usuario o hay solicitud pendiente.\n";
            }
            imprimirMetricas(sistema, "Envio de Solicitud");

        } else if (op == '2') {
            ListaEnlazada<SolicitudAmistad>& solicitudes = usuario->getSolicitudes();
            std::cout << "\n- SOLICITUDES PENDIENTES -\n";
            bool hayPendientes = false;
            for (size_t i = 0; i < solicitudes.getTamano(); i++) {
                if (solicitudes.obtener(i).getEstado() == EstadoSolicitud::PENDIENTE) {
                    std::cout << " [" << i << "] De: " << solicitudes.obtener(i).getRemitente() << "\n";
                    hayPendientes = true;
                }
            }
            if (hayPendientes) {
                std::cout << "Ingrese el indice a aceptar (-1 para cancelar): ";
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
            imprimirMetricas(sistema, "Aceptacion de Amistad");
        }
    }
}

void menuGestionarMisAlbumes(size_t usuarioIndex, UdeABookSystem& sistema) {
    Usuario* miUsuario = sistema.obtenerUsuario(usuarioIndex);
    char op = ' ';

    while (op != '3') {
        limpiarConsola();
        std::cout << "\n--- MIS ALBUMES Y PRIVACIDAD ---\n";
        ListaEnlazada<Album>& misAlbumes = miUsuario->getAlbumes();

        if (misAlbumes.getTamano() == 0) {
            std::cout << " (No tienes albumes creados actualmente).\n";
        } else {
            for (size_t i = 0; i < misAlbumes.getTamano(); i++) {
                Album& alb = misAlbumes.obtener(i);
                std::cout << " [" << i << "] " << alb.getNombre() << " | Fotos: " << alb.getCantidadFotos() << "\n";
            }
        }

        std::cout << "--------------------------------\n";
        std::cout << " [1] Crear nuevo album\n [2] Agregar foto a un album existente\n [3] Volver al menu principal\nOpcion: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "\nNombre del nuevo album: ";
            char bufferNombre[100]; std::cin >> bufferNombre;

            std::cout << "Nivel de Privacidad:\n [1] PUBLICO (Todos lo ven)\n [2] AMIGOS (Solo conexiones mutuas)\n [3] PRIVADO (Solo tu lo ves)\nSeleccione: ";
            int privOp; std::cin >> privOp;

            Privacidad nivelPrivacidad;
            if (privOp == 1) nivelPrivacidad = Privacidad::PUBLICO;
            else if (privOp == 2) nivelPrivacidad = Privacidad::AMIGOS;
            else nivelPrivacidad = Privacidad::PRIVADO;

            Album nuevoAlbum(Cadena(bufferNombre), nivelPrivacidad);
            miUsuario->agregarAlbum(nuevoAlbum);

            std::cout << ">> Album creado.\n";
            imprimirMetricas(sistema, "Creacion de Album Propio");

        } else if (op == '2') {
            if (misAlbumes.getTamano() == 0) {
                std::cout << ">> Debes crear un album primero.\n";
                pausar(); continue;
            }
            std::cout << "Ingrese el indice del album al que agregara la foto: ";
            size_t idxAlb; std::cin >> idxAlb;

            if (idxAlb < misAlbumes.getTamano()) {
                std::cout << "Escriba la ruta de la foto (ej. fotos/vacaciones.jpg): ";
                char bufferRuta[150]; std::cin >> bufferRuta;

                std::time_t t = std::time(nullptr);
                std::tm* now = std::localtime(&t);
                Fecha fechaActual(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min);

                Foto nuevaFoto(Cadena(bufferRuta), fechaActual);
                misAlbumes.obtener(idxAlb).agregarFoto(nuevaFoto);

                std::cout << ">> Foto agregada al album exitosamente.\n";
            } else {
                std::cout << ">> Indice de album no valido.\n";
            }
            imprimirMetricas(sistema, "Carga de Foto Propia");
        }
    }
}

void menuVisitarPerfil(size_t usuarioIndex, UdeABookSystem& sistema) {
    std::cout << "\n--- VISITAR PERFIL ---\n";
    std::cout << "Ingrese el username del amigo: ";
    char buffer[100]; std::cin >> buffer;

    int amigoIndex = sistema.buscarIndiceUsuario(Cadena(buffer));
    if (amigoIndex < 0) {
        std::cout << ">> Usuario no encontrado.\n";
        imprimirMetricas(sistema, "Busqueda Perfil Fallida");
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
        std::cout << " [" << i << "] " << albumesVisibles.obtener(i).getNombre() << " (Fotos: " << albumesVisibles.obtener(i).getCantidadFotos() << ")\n";
    }

    std::cout << "Seleccione el indice del album a abrir: ";
    size_t albIdx; std::cin >> albIdx;
    if (albIdx >= albumesVisibles.getTamano()) return;

    Album& albumSeleccionado = albumesVisibles.obtener(albIdx);
    if (albumSeleccionado.getCantidadFotos() == 0) {
        std::cout << ">> El album esta vacio.\n";
        imprimirMetricas(sistema, "Apertura Album");
        return;
    }

    size_t fotoActual = 0;
    char opCarrusel = ' ';
    while (opCarrusel != '4') {
        Foto& foto = albumSeleccionado.obtenerFoto(fotoActual);
        limpiarConsola();
        std::cout << "\n[ Foto " << (fotoActual + 1) << " de " << albumSeleccionado.getCantidadFotos() << " ]\n";
        std::cout << "Ruta: " << foto.getRutaArchivo() << "\nLikes: " << foto.getContadorLikes() << "\n";
        std::cout << "--------------------------------\n";
        std::cout << " [1] Siguiente | [2] Previa | [3] Dar/Quitar Like | [4] Salir\nOpcion: ";
        std::cin >> opCarrusel;

        if (opCarrusel == '1') fotoActual = (fotoActual + 1) % albumSeleccionado.getCantidadFotos();
        else if (opCarrusel == '2') fotoActual = (fotoActual == 0) ? albumSeleccionado.getCantidadFotos() - 1 : fotoActual - 1;
        else if (opCarrusel == '3') {
            std::cout << "¿Desea [1] Dar Like o [2] Quitar Like?: ";
            int opLike; std::cin >> opLike;
            if (opLike == 1) foto.darLike();
            else if (opLike == 2) foto.quitarLike();
        }

        if (opCarrusel != '4') {
            imprimirMetricas(sistema, "Carga Foto / Like");
        }
    }
}

void menuMensajeria(size_t usuarioIndex, UdeABookSystem& sistema) {
    char op = ' ';
    while (op != '4') {
        std::cout << "\n--- MENSAJERIA ---\n";
        std::cout << " [1] Enviar mensaje\n [2] Leer nuevos mensajes\n [3] Buscar mensajes por fecha\n [4] Volver\nSeleccione: ";
        std::cin >> op;

        if (op == '1') {
            std::cout << "Username del destinatario: ";
            char buffer[100]; std::cin >> buffer;
            std::cout << "Mensaje: ";
            std::cin.ignore();
            char texto[512]; std::cin.getline(texto, 512);
            if(sistema.enviarMensaje(usuarioIndex, Cadena(buffer), Cadena(texto))) {
                std::cout << ">> Mensaje enviado correctamente.\n";
            } else {
                std::cout << ">> Error al enviar: Verifique usuario o amistad.\n";
            }
            imprimirMetricas(sistema, "Envio de Mensaje");

        } else if (op == '2') {
            ArregloDinamico<Mensaje> noLeidos = sistema.obtenerMensajesNoLeidos(usuarioIndex);
            if (noLeidos.getTamano() == 0) {
                std::cout << ">> No tienes mensajes nuevos.\n";
            } else {
                std::cout << "\n- NUEVOS MENSAJES -\n";
                for (size_t i = 0; i < noLeidos.getTamano(); i++) {
                    std::cout << "[De: " << noLeidos.obtener(i).getEmisor() << " | Fecha: " << noLeidos.obtener(i).getFechaEnvio().toString() << "]\n";
                    std::cout << ">> " << noLeidos.obtener(i).getContenido() << "\n\n";
                }
                Usuario* usr = sistema.obtenerUsuario(usuarioIndex);
                ListaEnlazada<Mensaje>& recibidos = usr->getMensajesRecibidos();
                for (size_t j = 0; j < recibidos.getTamano(); j++) {
                    recibidos.obtener(j).marcarComoLeido();
                }
                std::cout << ">> (Mensajes marcados como leidos automáticamente).\n";
            }
            imprimirMetricas(sistema, "Lectura de Mensajes");

        } else if (op == '3') {
            int d1, m1, a1, d2, m2, a2;
            std::cout << "Fecha Inicio (DD MM AAAA): "; std::cin >> d1 >> m1 >> a1;
            std::cout << "Fecha Fin (DD MM AAAA): "; std::cin >> d2 >> m2 >> a2;
            ArregloDinamico<Mensaje> rango = sistema.buscarMensajesPorRangoFecha(usuarioIndex, Fecha(d1,m1,a1,0,0), Fecha(d2,m2,a2,23,59));
            std::cout << "\nEncontrados: " << rango.getTamano() << " mensajes\n";
            for (size_t i = 0; i < rango.getTamano(); i++) {
                std::cout << "[De: " << rango.obtener(i).getEmisor() << " | Fecha: " << rango.obtener(i).getFechaEnvio().toString() << "]\n";
                std::cout << ">> " << rango.obtener(i).getContenido() << "\n\n";
            }
            imprimirMetricas(sistema, "Busqueda por Fecha");
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
        std::cout << "\n [ Medidor: Calculo de Fotos Priorizadas ]";
        sistema.getMedidor().mostrarReporte();

        std::cout << "\nMENU PRINCIPAL:\n";
        std::cout << " [1] Administrar amistades\n";
        std::cout << " [2] Visitar perfil de un amigo\n";
        std::cout << " [3] Ver sugerencias de amistad\n";
        std::cout << " [4] Administrar mensajes\n";
        std::cout << " [5] Gestionar mis albumes\n";
        std::cout << " [N] Refrescar fotos | [S] Cerrar sesion\nOpcion: ";
        std::cin >> opcion;

        if (opcion == '1') {
            menuAdministrarAmistades(usuarioIndex, sistema);
        } else if (opcion == '2') {
            menuVisitarPerfil(usuarioIndex, sistema);
        } else if (opcion == '3') {
            std::cout << "\n--- SUGERENCIAS DE AMISTAD ---\n";
            std::cout << "Cantidad (K) de sugerencias a ver (1-20): ";
            size_t K; std::cin >> K;
            if (K >= 1 && K <= 20) {
                ArregloDinamico<size_t> sugerencias = sistema.obtenerSugerenciasAmistad(usuarioIndex, K);
                for (size_t i = 0; i < sugerencias.getTamano(); i++) {
                    Usuario* uSug = sistema.obtenerUsuario(sugerencias.obtener(i));
                    int comunes = sistema.amigosEnComun(usuarioIndex, sugerencias.obtener(i)).getTamano();
                    std::cout << " [" << i << "] " << uSug->getUsername() << " (Amigos en comun: " << comunes << ")\n";
                }
                std::cout << "\n¿Enviar solicitud a alguno? Ingrese Username (o '0' para cancelar): ";
                char sugBuffer[100]; std::cin >> sugBuffer;
                if (sugBuffer[0] != '0') {
                    if(sistema.enviarSolicitud(usuarioIndex, Cadena(sugBuffer))) std::cout << ">> Solicitud enviada!\n";
                    else std::cout << ">> Error al enviar solicitud.\n";
                }
            } else {
                std::cout << ">> El valor debe estar entre 1 y 20.\n";
            }
            imprimirMetricas(sistema, "Busqueda Sugerencias");
        } else if (opcion == '4') {
            menuMensajeria(usuarioIndex, sistema);
        } else if (opcion == '5') {
            menuGestionarMisAlbumes(usuarioIndex, sistema);
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

    std::cout << "\n*** UDE@BOOK SYSTEM ***\n[+] Cargando datos a memoria...\n";

    if (!sistema.cargarDatos(archivoUsuarios, archivoRelaciones)) {
        std::cout << "Error critico al cargar los archivos. Deteniendo el sistema.\n";
        return 1;
    }

    imprimirMetricas(sistema, "Carga Masiva Inicial (Heap)");

    char opcionPrincipal = ' ';
    while (opcionPrincipal != '0') {
        limpiarConsola();
        std::cout << "\n*** UDE@BOOK LOGIN ***\n";
        std::cout << " [1] Iniciar sesion con Username\n [0] Salir\nOpcion: ";
        std::cin >> opcionPrincipal;

        if (opcionPrincipal == '1') {
            std::cout << "Username: ";
            char buffer[100]; std::cin >> buffer;
            int userIdx = sistema.buscarIndiceUsuario(Cadena(buffer));

            imprimirMetricas(sistema, "Busqueda Login");

            if (userIdx >= 0) {
                sesionIniciada(static_cast<size_t>(userIdx), sistema);
            } else {
                std::cout << ">> Identidad no verificada. Usuario no encontrado.\n";
                pausar();
            }
        }
    }

    std::cout << "\n[+] Guardando datos permanentes...\n";
    sistema.guardarDatos(archivoUsuarios, archivoRelaciones);
    imprimirMetricas(sistema, "Escritura en Disco");

    std::cout << "\n=== Sistema apagado exitosamente ===\n";
    return 0;
}