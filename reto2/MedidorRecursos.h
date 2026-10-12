#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <cstddef>

class MedidorRecursos {
private:
    unsigned long long iteracionesActuales;
    size_t memoriaConsumidaBytes;

public:
    MedidorRecursos();

    void registrarIteracion(size_t n = 1);
    void registrarMemoria(size_t bytes);
    void liberarMemoria(size_t bytes);
    void reiniciar();
    void mostrarReporte() const;
};

#endif // MEDIDORRECURSOS_H
