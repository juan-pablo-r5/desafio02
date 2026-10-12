#include "Fecha.h"

Fecha::Fecha() {
    dia = 1;
    mes = 1;
    anio = 2000;
    hora = 0;
    minuto = 0;
}

Fecha::Fecha(int d, int m, int a, int h, int min) {
    dia = d;
    mes = m;
    anio = a;
    hora = h;
    minuto = min;
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    if (dia != otra.dia) return dia < otra.dia;
    if (hora != otra.hora) return hora < otra.hora;
    return minuto < otra.minuto;
}

bool Fecha::operator>(const Fecha& otra) const {
    if (anio != otra.anio) return anio > otra.anio;
    if (mes != otra.mes) return mes > otra.mes;
    if (dia != otra.dia) return dia > otra.dia;
    if (hora != otra.hora) return hora > otra.hora;
    return minuto > otra.minuto;
}

bool Fecha::operator==(const Fecha& otra) const {
    return (anio == otra.anio && mes == otra.mes && dia == otra.dia &&
            hora == otra.hora && minuto == otra.minuto);
}

Cadena Fecha::toString() const {
    char temp[17];
    temp[0] = (dia / 10) + '0';
    temp[1] = (dia % 10) + '0';
    temp[2] = '/';
    temp[3] = (mes / 10) + '0';
    temp[4] = (mes % 10) + '0';
    temp[5] = '/';
    temp[6] = (anio / 1000) + '0';
    temp[7] = ((anio / 100) % 10) + '0';
    temp[8] = ((anio / 10) % 10) + '0';
    temp[9] = (anio % 10) + '0';
    temp[10] = ' ';
    temp[11] = (hora / 10) + '0';
    temp[12] = (hora % 10) + '0';
    temp[13] = ':';
    temp[14] = (minuto / 10) + '0';
    temp[15] = (minuto % 10) + '0';
    temp[16] = '\0';

    return Cadena(temp);
}