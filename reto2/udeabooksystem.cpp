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

// Guardado seguro al salir
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
