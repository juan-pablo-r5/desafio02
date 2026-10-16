#include <iostream>
#include "UdeABookSystem.h"
#include "Cadena.h"

int main() {
    std::cout << "=== INICIANDO PRUEBAS PARCIALES DE UDEABOOK ===\n";

    UdeABookSystem sistema;

    // Define las rutas de tus archivos de texto plano
    Cadena archivoUsuarios("usuarios.txt");
    Cadena archivoRelaciones("relaciones.txt");

    std::cout << "\n[1] Intentando cargar datos desde archivos...\n";
    bool cargado = sistema.cargarDatos(archivoUsuarios, archivoRelaciones);

    if (cargado) {
        std::cout << ">> ¡Carga exitosa!\n";
        std::cout << ">> Total de usuarios en el sistema: " << sistema.getCantidadUsuarios() << "\n";

        // Prueba rápida de obtención de un usuario
        if (sistema.getCantidadUsuarios() > 0) {
            Usuario* primerUsuario = sistema.obtenerUsuario(0);
            if (primerUsuario != nullptr) {
                std::cout << ">> Primer usuario verificado: " << primerUsuario->getUsername() << "\n";
            }
        }
    } else {
        std::cout << ">> [Aviso] No se pudieron cargar los archivos.\n";
        std::cout << "   Asegurate de colocar 'usuarios.txt' y 'relaciones.txt' en la carpeta de compilacion/ejecucion de Qt Creator.\n";
    }

    // [2] Imprimir el reporte del medidor de recursos obligatorio
    std::cout << "\n[3] Obteniendo metricas de rendimiento...\n";
    sistema.getMedidor().mostrarReporte();

    std::cout << ">> ¡Carga exitosa!\n";
    std::cout << ">> Total de usuarios en el sistema: " << sistema.getCantidadUsuarios() << "\n";

    if (sistema.getCantidadUsuarios() < 2) {
        std::cout << ">> [Aviso] Se requieren al menos 2 usuarios en 'usuarios.txt' para probar la Fase 2.\n";
        return 0;
    }

    // Seleccionamos los dos primeros usuarios para las pruebas
    Usuario* u0 = sistema.obtenerUsuario(0);
    Usuario* u1 = sistema.obtenerUsuario(1);

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[2] PRUEBA DE GESTIÓN DE AMISTADES Y SOLICITUDES\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Usuario 0 (" << u0->getUsername() << ") envía solicitud a Usuario 1 (" << u1->getUsername() << ")...\n";

    bool enviado = sistema.enviarSolicitud(0, u1->getUsername());
    if (enviado) {
        std::cout << ">> ¡Solicitud enviada con éxito!\n";
    } else {
        std::cout << ">> No se pudo enviar (ya son amigos o ya existe solicitud pendiente).\n";
    }

    // Aceptar la solicitud (Usuario 1 acepta la solicitud de Usuario 0)
    std::cout << "Usuario 1 (" << u1->getUsername() << ") acepta la solicitud de Usuario 0...\n";
    bool aceptado = sistema.aceptarSolicitud(1, 0);
    if (aceptado) {
        std::cout << ">> ¡Solicitud aceptada! Conexión establecida en la Matriz de Adyacencia.\n";
    } else {
        std::cout << ">> No hay solicitudes pendientes de Usuario 0 para Usuario 1.\n";
    }

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[3] PRUEBA DE SUGERENCIAS DE AMISTAD (Matriz)\n";
    std::cout << "--------------------------------------------------\n";
    size_t K = 3;
    ArregloDinamico<size_t> sugerencias = sistema.obtenerSugerenciasAmistad(0, K);
    std::cout << "Top " << K << " sugerencias de amistad para " << u0->getUsername() << ":\n";

    for (size_t i = 0; i < sugerencias.getTamano(); i++) {
        size_t idxSug = sugerencias.obtener(i);
        Usuario* uSug = sistema.obtenerUsuario(idxSug);
        int comunes = sistema.amigosEnComun(0, idxSug).getTamano();
        std::cout << "  -> " << uSug->getUsername() << " (Amigos en común: " << comunes << ")\n";
    }

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[4] PRUEBA DE SISTEMA DE MENSAJERÍA\n";
    std::cout << "--------------------------------------------------\n";
    Cadena textoMsj("Hola! Este es un mensaje de prueba de la Fase 2.");
    std::cout << "Enviando mensaje de " << u0->getUsername() << " a " << u1->getUsername() << "...\n";

    bool msgEnviado = sistema.enviarMensaje(0, u1->getUsername(), textoMsj);
    if (msgEnviado) {
        std::cout << ">> Mensaje enviado correctamente.\n";
    }

    std::cout << "Consultando mensajes no leídos para " << u1->getUsername() << " (se marcarán como leídos):\n";
    ArregloDinamico<Mensaje> noLeidos = sistema.obtenerMensajesNoLeidos(1);

    for (size_t i = 0; i < noLeidos.getTamano(); i++) {
        const Mensaje& m = noLeidos.obtener(i);
        std::cout << "  [Emisor: " << m.getEmisor() << "] Texto: \"" << m.getContenido()
                  << "\" | Fecha: " << m.getFechaEnvio().toString() << "\n";
    }

    // Prueba de búsqueda por rango de fecha
    Fecha fechaInicio(1, 1, 2020, 0, 0);
    Fecha fechaFin(31, 12, 2030, 23, 59);
    ArregloDinamico<Mensaje> msjsRango = sistema.buscarMensajesPorRangoFecha(1, fechaInicio, fechaFin);
    std::cout << ">> Mensajes en el rango de fechas (2020 - 2030): " << msjsRango.getTamano() << "\n";

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[5] PRUEBA DE MULTIMEDIA Y PRIVACIDAD\n";
    std::cout << "--------------------------------------------------\n";
    ArregloDinamico<Album> albumesVisibles = sistema.obtenerAlbumesVisibles(0, 1);
    std::cout << "Álbumes visibles de " << u1->getUsername() << " para " << u0->getUsername() << ": "
              << albumesVisibles.getTamano() << "\n";

    for (size_t i = 0; i < albumesVisibles.getTamano(); i++) {
        std::cout << "  - Álbum: " << albumesVisibles.obtener(i).getNombre()
                  << " | Fotos contenidas: " << albumesVisibles.obtener(i).getCantidadFotos() << "\n";
    }

    // [6] Reporte final obligatorio del Medidor de Recursos
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "[6] MÉTRICAS DE RENDIMIENTO FINALES\n";
    std::cout << "--------------------------------------------------\n";
    sistema.getMedidor().mostrarReporte();

    std::cout << "\n=== FIN DE LA PRUEBA PARCIAL ===\n";
    return 0;
}
