#include "GestorArchivos.h"
#include <fstream>

GestorArchivos::GestorArchivos() {
}

int GestorArchivos::parseEntero(const char* texto) {
    int valor = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] >= '0' && texto[i] <= '9') {
            valor = valor * 10 + (texto[i] - '0');
        }
    }
    return valor;
}

bool GestorArchivos::cargarUsuarios(const Cadena& rutaArchivo, ArregloDinamico<Usuario*>& usuarios) {
    std::ifstream archivo(rutaArchivo.getBuffer());
    if (!archivo.is_open()) return false;

    char linea[512];
    while (archivo.getline(linea, 512)) {
        char campos[11][100];
        int col = 0, pos = 0;

        for (int i = 0; linea[i] != '\0'; i++) {
            if (linea[i] == ';') {
                campos[col][pos] = '\0';
                col++;
                pos = 0;
            } else {
                campos[col][pos++] = linea[i];
            }
        }
        campos[col][pos] = '\0';

        if (col == 10) {
            Cadena username(campos[0]);
            Cadena email(campos[1]);
            Fecha fNac(parseEntero(campos[2]), parseEntero(campos[3]), parseEntero(campos[4]), 0, 0);
            Cadena ciudad(campos[5]);
            Cadena pais(campos[6]);
            Fecha fInsc(parseEntero(campos[7]), parseEntero(campos[8]), parseEntero(campos[9]), 0, 0);
            Cadena ruta(campos[10]);

            Usuario* nuevo = new Usuario(username, email, fNac, ciudad, pais, fInsc, ruta);
            usuarios.agregar(nuevo);
        }
    }
    archivo.close();
    return true;
}

bool GestorArchivos::guardarUsuarios(const Cadena& rutaArchivo, const ArregloDinamico<Usuario*>& usuarios) {
    std::ofstream archivo(rutaArchivo.getBuffer());
    if (!archivo.is_open()) return false;

    for (size_t i = 0; i < usuarios.getTamano(); i++) {
        Usuario* u = usuarios.obtener(i);

        archivo << u->getUsername() << ";"
                << u->getEmail() << ";"
                << u->getFechaNacimiento().getDia() << ";"
                << u->getFechaNacimiento().getMes() << ";"
                << u->getFechaNacimiento().getAnio() << ";"
                << u->getCiudad() << ";"
                << u->getPais() << ";"
                << u->getFechaInscripcion().getDia() << ";"
                << u->getFechaInscripcion().getMes() << ";"
                << u->getFechaInscripcion().getAnio() << ";"
                << u->getRutaFotoPerfil() << "\n";
    }

    archivo.close();
    return true;
}

bool GestorArchivos::cargarRelaciones(const Cadena& rutaArchivo, MatrizAdyacencia& matriz) {
    std::ifstream archivo(rutaArchivo.getBuffer());
    if (!archivo.is_open()) return false;

    char linea[256];
    while (archivo.getline(linea, 256)) {
        char campos[2][100];
        int col = 0, pos = 0;

        for (int i = 0; linea[i] != '\0'; i++) {
            if (linea[i] == ';') {
                campos[col][pos] = '\0';
                col++;
                pos = 0;
                if (col > 1) break;
            } else {
                campos[col][pos++] = linea[i];
            }
        }
        campos[col][pos] = '\0';

        if (col == 1) {
            size_t u1 = parseEntero(campos[0]);
            size_t u2 = parseEntero(campos[1]);
            matriz.conectar(u1, u2);
        }
    }
    archivo.close();
    return true;
}

bool GestorArchivos::guardarRelaciones(const Cadena& rutaArchivo, const MatrizAdyacencia& matriz) {
    std::ofstream archivo(rutaArchivo.getBuffer());
    if (!archivo.is_open()) return false;

    size_t dim = matriz.getDimension();
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i + 1; j < dim; j++) {
            if (matriz.estanConectados(i, j)) {
                archivo << i << ";" << j << "\n";
            }
        }
    }

    archivo.close();
    return true;
}
