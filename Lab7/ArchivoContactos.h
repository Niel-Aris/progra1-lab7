#pragma once
#include <QString>
#include <QVector>

// Un contacto es una linea del archivo: nombre;telefono;correo
struct Contacto {
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos {
public:
    QString ruta() const;
    bool guardar(const QString& nombre,
                 const QString& telefono,
                 const QString& correo) const;

    // Parte 3: leer, editar y eliminar
    QVector<Contacto> leerContactos() const;
    bool editar(int posicion,
                const QString& nombre,
                const QString& telefono,
                const QString& correo) const;
    bool eliminar(int posicion) const;

private:
    bool reescribir(const QVector<Contacto>& contactos) const;
};
