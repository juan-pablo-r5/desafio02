#ifndef FECHA_H
#define FECHA_H

#include "Cadena.h"

class Fecha {
private:
    int dia;
    int mes;
    int anio;
    int hora;
    int minuto;

public:
    Fecha();
    Fecha(int d, int m, int a, int h, int min);

    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;

    Cadena toString() const;
};

#endif // FECHA_H
