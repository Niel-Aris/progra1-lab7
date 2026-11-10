#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QSaveFile>
#include <QStringList>

QString ArchivoContactos::ruta() const
{
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
                              const QString& telefono,
                              const QString& correo) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

// Lee todas las lineas del archivo y las devuelve como contactos.
// La posicion de cada contacto en el vector es su posicion en el archivo.
QVector<Contacto> ArchivoContactos::leerContactos() const
{
    QVector<Contacto> contactos;

    QFile archivo(ruta());
    if (!archivo.exists())
        return contactos; // todavia no hay contactos

    if (!archivo.open(QIODevice::ReadOnly))
        return contactos;

    QString texto = QString::fromUtf8(archivo.readAll());
    archivo.close();

    QStringList lineas = texto.split('\n');
    for (int i = 0; i < lineas.size(); i++) {
        QString linea = lineas[i];
        linea.remove('\r'); // por si el archivo se edito en Windows

        if (linea.trimmed().isEmpty())
            continue; // se saltan las lineas vacias

        QStringList partes = linea.split(';');
        Contacto c;
        if (partes.size() > 0)
            c.nombre = partes[0];
        if (partes.size() > 1)
            c.telefono = partes[1];
        if (partes.size() > 2)
            c.correo = partes[2];
        contactos.append(c);
    }

    return contactos;
}

bool ArchivoContactos::editar(int posicion,
                             const QString& nombre,
                             const QString& telefono,
                             const QString& correo) const
{
    QVector<Contacto> contactos = leerContactos();
    if (posicion < 0 || posicion >= contactos.size())
        return false;

    // Solo se cambia el contacto de esa posicion
    contactos[posicion].nombre = nombre;
    contactos[posicion].telefono = telefono;
    contactos[posicion].correo = correo;

    return reescribir(contactos);
}

bool ArchivoContactos::eliminar(int posicion) const
{
    QVector<Contacto> contactos = leerContactos();
    if (posicion < 0 || posicion >= contactos.size())
        return false;

    contactos.removeAt(posicion);

    return reescribir(contactos);
}

// Escribe el archivo completo otra vez. Se usa QSaveFile para que el
// archivo original solo se reemplace si toda la escritura salio bien.
bool ArchivoContactos::reescribir(const QVector<Contacto>& contactos) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QSaveFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly))
        return false;

    QString texto = "";
    for (int i = 0; i < contactos.size(); i++) {
        texto = texto + contactos[i].nombre + ";" + contactos[i].telefono
                + ";" + contactos[i].correo + "\n";
    }

    QByteArray datos = texto.toUtf8();
    if (archivo.write(datos) != datos.size()) {
        archivo.cancelWriting();
        return false;
    }

    return archivo.commit();
}
