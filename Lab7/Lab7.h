#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab7.h"
#include "ArchivoContactos.h"

class Lab7 : public QMainWindow
{
    Q_OBJECT

public:
    Lab7(QWidget* parent = nullptr);
    ~Lab7();

private:
    Ui::Lab7Class ui;
    ArchivoContactos archivoContactos;
    void guardarContacto();

    // Parte 3
    QString modo; // "Historial", "Editar" o "Eliminar"
    bool validarDatos(const QString& nombre,
                      const QString& telefono,
                      const QString& correo);
    void mostrarRegistrar();
    void mostrarHistorial();
    void mostrarEditar();
    void mostrarEliminar();
    void abrirPaginaTabla(const QString& nuevoModo);
    void cargarTabla();
    void limpiarCamposEditar();
    void cargarSeleccionado();
    void guardarCambios();
    void eliminarContacto();
};
