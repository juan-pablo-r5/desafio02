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

    std::cout << "\n=== FIN DE LA PRUEBA PARCIAL ===\n";
    return 0;
}
