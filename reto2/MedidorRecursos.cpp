#include "MedidorRecursos.h"
#include <iostream>

MedidorRecursos::MedidorRecursos() {
    iteracionesActuales = 0;
    memoriaConsumidaBytes = 0;
}

void MedidorRecursos::registrarIteracion(size_t n) {
    iteracionesActuales += n;
}

void MedidorRecursos::registrarMemoria(size_t bytes) {
    memoriaConsumidaBytes += bytes;
}

void MedidorRecursos::liberarMemoria(size_t bytes) {
    if (memoriaConsumidaBytes >= bytes) {
        memoriaConsumidaBytes -= bytes;
    } else {
        memoriaConsumidaBytes = 0;
    }
}

void MedidorRecursos::reiniciar() {
    iteracionesActuales = 0;
}

void MedidorRecursos::mostrarReporte() const {
    std::cout << "\n=== METRICAS DE RENDIMIENTO ===\n";
    std::cout << "Iteraciones de la funcionalidad: " << iteracionesActuales << "\n";
    std::cout << "Memoria total actual en uso: " << memoriaConsumidaBytes << " bytes\n";
    std::cout << "===============================\n";
}
