#ifndef MATRIZADYACENCIA_H
#define MATRIZADYACENCIA_H

#include <cstddef>

class MatrizAdyacencia
{
private:
    bool** matriz;
    size_t dimension;
public:
    MatrizAdyacencia(size_t dim);

    ~MatrizAdyacencia();

    // Constructor de copia (Rule of Three)
    MatrizAdyacencia(const MatrizAdyacencia& otro);

    // Operador de asignación
    MatrizAdyacencia& operator=(const MatrizAdyacencia& otro);

    void conectar(size_t u1, size_t u2);

    void desconectar(size_t u1, size_t u2);

    bool estanConectados(size_t u1, size_t u2) const;
    int obtenerAmigosEnComun(size_t u1, size_t u2) const;

    void redimensionar(size_t nuevaDimension);
    size_t getDimension() const;
};

#endif // MATRIZADYACENCIA_H
