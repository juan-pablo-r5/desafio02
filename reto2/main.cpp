#include <iostream>
#include "UdeABookSystem.h"
#include "Cadena.h"

void mostrarMenu() {
    std::cout << "\n=== RED SOCIAL UDEABOOK ===\n";
    std::cout << "1. Cargar datos del sistema\n";
    std::cout << "2. Ver cantidad de usuarios\n";
    std::cout << "3. Ver detalles del primer usuario\n";
    std::cout << "4. Mostrar metricas de rendimiento\n";
    std::cout << "5. Guardar y Salir\n";
    std::cout << "Ingrese una opcion: ";
}

int main() {
    UdeABookSystem sistema;
    Cadena archivoUsuarios("usuarios.txt");
    Cadena archivoRelaciones("relaciones.txt");

    bool ejecutando = true;
    int opcion = 0;

    while (ejecutando) {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            if (sistema.cargarDatos(archivoUsuarios, archivoRelaciones)) {
                std::cout << "Datos cargados correctamente en memoria.\n";
            } else {
                std::cout << "Fallo al cargar los archivos.\n";
            }
            break;
        case 2:
            std::cout << "Total de usuarios activos: " << sistema.getCantidadUsuarios() << "\n";
            break;
        case 3:
            if (sistema.getCantidadUsuarios() > 0) {
                Usuario* primer = sistema.obtenerUsuario(0);
                if (primer != nullptr) {
                    std::cout << "Username: " << primer->getUsername() << "\n";
                }
            } else {
                std::cout << "No hay usuarios cargados.\n";
            }
            break;
        case 4:
            sistema.getMedidor().mostrarReporte();
            break;
        case 5:
            sistema.guardarDatos(archivoUsuarios, archivoRelaciones);
            std::cout << "Datos guardados. Saliendo...\n";
            ejecutando = false;
            break;
        default:
            std::cout << "Opcion no valida.\n";
            break;
        }
    }

    return 0;
}
