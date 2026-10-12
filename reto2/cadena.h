#ifndef CADENA_H
#define CADENA_H

#include <cstddef>

class Cadena {
private:
    char* buffer;
    size_t longitud;

public:
    Cadena();
    Cadena(const char* str);
    Cadena(const Cadena& otra);
    ~Cadena();

    Cadena& operator=(const Cadena& otra);
    bool operator==(const Cadena& otra) const;
    bool operator!=(const Cadena& otra) const;
    Cadena operator+(const Cadena& otra) const;

    const char* getBuffer() const;
    size_t getLongitud() const;
};

#endif // CADENA_H
