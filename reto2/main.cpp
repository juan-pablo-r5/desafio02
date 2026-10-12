#include <iostream>
#include "ListaEnlazada.h"
#include "ArregloDinamico.h"
#include "MatrizAdyacencia.h"

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << " PRUEBAS DE FUNCIONALIDAD: LISTA ENLAZADA " << std::endl;
    std::cout << "===========================================" << std::endl;

    ListaEnlazada<int> listaAmigosIDs;

    // Agregando elementos
    listaAmigosIDs.agregar(101);
    listaAmigosIDs.agregar(102);
    listaAmigosIDs.agregar(103);
    listaAmigosIDs.agregar(104);

    std::cout << "Elementos iniciales en la Lista Enlazada (Tamano: " << listaAmigosIDs.getTamano() << "):" << std::endl;
    for (size_t i = 0; i < listaAmigosIDs.getTamano(); i++) {
        std::cout << " -> ID Amigo en indice " << i << ": " << listaAmigosIDs.obtener(i) << std::endl;
    }

    // Probando eliminación de un elemento intermedio (índice 1 -> ID 102)
    listaAmigosIDs.eliminar(1);
    std::cout << "\nDespues de eliminar el elemento en el indice 1 (Tamano: " << listaAmigosIDs.getTamano() << "):" << std::endl;
    for (size_t i = 0; i < listaAmigosIDs.getTamano(); i++) {
        std::cout << " -> ID Amigo en indice " << i << ": " << listaAmigosIDs.obtener(i) << std::endl;
    }

    std::cout << "\n===========================================" << std::endl;
    std::cout << " PRUEBAS DE FUNCIONALIDAD: ARREGLO DINAMICO " << std::endl;
    std::cout << "===========================================" << std::endl;

    ArregloDinamico<int> arregloUsuarios;

    // Agregando elementos para forzar el redimensionamiento dinámico
    for (int i = 1; i <= 15; i++) {
        arregloUsuarios.agregar(i * 1000);
    }

    std::cout << "Elementos en el Arreglo Dinamico (Tamano: " << arregloUsuarios.getTamano()
              << ", Capacidad reservada: " << arregloUsuarios.getCapacidad() << "):" << std::endl;

    for (size_t i = 0; i < arregloUsuarios.getTamano(); i++) {
        std::cout << " -> Usuario ID en indice " << i << ": " << arregloUsuarios.obtener(i) << std::endl;
    }

    // Probando eliminación en arreglo dinámico
    arregloUsuarios.eliminar(0); // Elimina el primer elemento
    std::cout << "\nDespues de eliminar el primer elemento del arreglo (Nuevo tamano: " << arregloUsuarios.getTamano() << "):" << std::endl;
    std::cout << " -> Nuevo elemento en el indice 0: " << arregloUsuarios.obtener(0) << std::endl;

    // Creamos una red para 5 usuarios (índices 0 a 4)
    MatrizAdyacencia red(5);

    // Establecemos conexiones de amistad
    red.conectar(0, 1); // Usuario 0 es amigo de Usuario 1
    red.conectar(0, 2); // Usuario 0 es amigo de Usuario 2
    red.conectar(1, 2); // Usuario 1 es amigo de Usuario 2
    red.conectar(1, 3); // Usuario 1 es amigo de Usuario 3

    std::cout << "¿Son amigos 0 y 1? " << (red.estanConectados(0, 1) ? "SI" : "NO") << std::endl;
    std::cout << "¿Son amigos 0 y 3? " << (red.estanConectados(0, 3) ? "SI" : "NO") << std::endl;

    // Ambos son amigos del Usuario 1, por lo que deberían tener 1 amigo en común.
    int comunes = red.obtenerAmigosEnComun(0, 3);
    std::cout << "Amigos en comun entre Usuario 0 y Usuario 3: " << comunes << std::endl;

    return 0;
}
