#include "UdeABookSystem.h"
#include <iostream>

UdeABookSystem::UdeABookSystem()
    : usuarios(100), matrizAdyacencia(1000) {
}

UdeABookSystem::~UdeABookSystem() {
    size_t tam = usuarios.getTamano();
    for (size_t i = 0; i < tam; i++) {
        delete usuarios.obtener(i);
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

    size_t totalUsuarios = usuarios.getTamano();
    matrizAdyacencia = MatrizAdyacencia(totalUsuarios > 0 ? totalUsuarios : 100);

    bool exitoRelaciones = gestorArchivos.cargarRelaciones(rutaRelaciones, matrizAdyacencia);
    if (!exitoRelaciones) {
        std::cout << "No se pudo cargar el archivo de relaciones o esta vacio.\n";
    }

    // Registro "en bloque" desde el sistema central para no tocar el GestorArchivos
    medidor.registrarIteracion(totalUsuarios);
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