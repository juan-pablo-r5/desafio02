#ifndef MATRIZADYACENCIA_H
#define MATRIZADYACENCIA_H

#include <cstddef>

class MatrizAdyacencia
{
private:
    bool** matriz;
    size_t dimension;
public:
    // Constructor que recibe la dimensión inicial (ej. número de usuarios)
    MatrizAdyacencia(size_t dim);

    // Destructor para liberar la memoria dinámica del heap
    ~MatrizAdyacencia();

    // Constructor de copia (Rule of Three)
    MatrizAdyacencia(const MatrizAdyacencia& otro);

    // Operador de asignación
    MatrizAdyacencia& operator=(const MatrizAdyacencia& otro);

    // Conectar dos usuarios (establecer amistad bidireccional)
    void conectar(size_t u1, size_t u2);

    // Desconectar dos usuarios
    void desconectar(size_t u1, size_t u2);

    // Verificar si dos usuarios son amigos directamente
    bool estanConectados(size_t u1, size_t u2) const;

    // Calcular amigos en común entre u1 y u2 (vital para sugerencias y prioridades)
    int obtenerAmigosEnComun(size_t u1, size_t u2) const;

    // Redimensionar la matriz si la red crece superando la capacidad inicial
    void redimensionar(size_t nuevaDimension);

    // Consultar la dimensión actual de la matriz
    size_t getDimension() const;
};

#endif // MATRIZADYACENCIA_H
