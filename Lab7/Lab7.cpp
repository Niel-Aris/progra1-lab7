#include "Lab7.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidgetItem>

Lab7::Lab7(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.btnGuardar, &QPushButton::clicked,
            this, &Lab7::guardarContacto);

    // Parte 3: menu de navegacion
    connect(ui.actionRegistrar, &QAction::triggered,
            this, &Lab7::mostrarRegistrar);
    connect(ui.actionHistorial, &QAction::triggered,
            this, &Lab7::mostrarHistorial);
    connect(ui.actionEditar, &QAction::triggered,
            this, &Lab7::mostrarEditar);
    connect(ui.actionEliminar, &QAction::triggered,
            this, &Lab7::mostrarEliminar);

    // Botones de la pagina de la tabla
    connect(ui.btnGuardarCambios, &QPushButton::clicked,
            this, &Lab7::guardarCambios);
    connect(ui.btnEliminar, &QPushButton::clicked,
            this, &Lab7::eliminarContacto);

    // Al seleccionar una fila en modo Editar se llenan los campos
    connect(ui.tblContactos, &QTableWidget::itemSelectionChanged,
            this, &Lab7::cargarSeleccionado);

    mostrarRegistrar();
}

Lab7::~Lab7()
{}

void Lab7::guardarContacto()
{
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo",
        "El contacto se guardó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    ui.txtNombre->setFocus();
}

// Las mismas validaciones de la Parte 2, para usarlas al guardar y al editar
bool Lab7::validarDatos(const QString& nombre,
                        const QString& telefono,
                        const QString& correo)
{
    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
                             "Completá los tres campos.");
        return false;
    }

    for (const QString& dato : QStringList{nombre, telefono, correo}) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return false;
        }
    }

    return true;
}

// ---------------- Parte 3: navegacion ----------------

void Lab7::mostrarRegistrar()
{
    modo = "";
    ui.stackedWidget->setCurrentWidget(ui.pageRegistrar);
    ui.txtNombre->setFocus();
}

void Lab7::mostrarHistorial()
{
    abrirPaginaTabla("Historial");
}

void Lab7::mostrarEditar()
{
    abrirPaginaTabla("Editar");
}

void Lab7::mostrarEliminar()
{
    abrirPaginaTabla("Eliminar");
}

// Historial, Editar y Eliminar usan la misma pagina con la tabla.
// Solo se muestran los controles que sirven en cada modo.
void Lab7::abrirPaginaTabla(const QString& nuevoModo)
{
    modo = nuevoModo;

    if (modo == "Historial") {
        ui.lblTitulo->setText("Historial de contactos");
        ui.panelEditar->setVisible(false);
        ui.btnEliminar->setVisible(false);
    }
    else if (modo == "Editar") {
        ui.lblTitulo->setText("Editar contacto: seleccioná una fila y cambiá los datos");
        ui.panelEditar->setVisible(true);
        ui.btnEliminar->setVisible(false);
    }
    else {
        ui.lblTitulo->setText("Eliminar contacto: seleccioná una fila");
        ui.panelEditar->setVisible(false);
        ui.btnEliminar->setVisible(true);
    }

    ui.stackedWidget->setCurrentWidget(ui.pageTabla);
    cargarTabla();
}

// Lee el archivo completo y vuelve a llenar la tabla desde cero,
// asi no se duplican filas cada vez que se abre.
void Lab7::cargarTabla()
{
    QVector<Contacto> contactos = archivoContactos.leerContactos();

    ui.tblContactos->clearSelection();
    ui.tblContactos->setRowCount(0);
    ui.tblContactos->setRowCount(contactos.size());

    // La fila i de la tabla es la linea i del archivo (no hay ordenamiento)
    for (int i = 0; i < contactos.size(); i++) {
        ui.tblContactos->setItem(i, 0, new QTableWidgetItem(contactos[i].nombre));
        ui.tblContactos->setItem(i, 1, new QTableWidgetItem(contactos[i].telefono));
        ui.tblContactos->setItem(i, 2, new QTableWidgetItem(contactos[i].correo));
    }

    ui.lblVacio->setVisible(contactos.size() == 0);
    limpiarCamposEditar();
}

void Lab7::limpiarCamposEditar()
{
    ui.txtEditarNombre->clear();
    ui.txtEditarTelefono->clear();
    ui.txtEditarCorreo->clear();
}

void Lab7::cargarSeleccionado()
{
    if (modo != "Editar")
        return;

    QList<QTableWidgetItem*> seleccion = ui.tblContactos->selectedItems();
    if (seleccion.isEmpty()) {
        limpiarCamposEditar();
        return;
    }

    int fila = seleccion[0]->row();
    ui.txtEditarNombre->setText(ui.tblContactos->item(fila, 0)->text());
    ui.txtEditarTelefono->setText(ui.tblContactos->item(fila, 1)->text());
    ui.txtEditarCorreo->setText(ui.tblContactos->item(fila, 2)->text());
}

void Lab7::guardarCambios()
{
    QList<QTableWidgetItem*> seleccion = ui.tblContactos->selectedItems();
    if (seleccion.isEmpty()) {
        QMessageBox::information(this, "Sin selección",
            "Primero seleccioná en la tabla el contacto que querés editar.");
        return;
    }

    int fila = seleccion[0]->row();

    QString nombre = ui.txtEditarNombre->text().trimmed();
    QString telefono = ui.txtEditarTelefono->text().trimmed();
    QString correo = ui.txtEditarCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    if (!archivoContactos.editar(fila, nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo editar",
            "No se pudo actualizar el archivo.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    cargarTabla();
    ui.tblContactos->selectRow(fila);
    QMessageBox::information(this, "Contacto editado",
        "Los cambios se guardaron correctamente.");
}

void Lab7::eliminarContacto()
{
    QList<QTableWidgetItem*> seleccion = ui.tblContactos->selectedItems();
    if (seleccion.isEmpty()) {
        QMessageBox::information(this, "Sin selección",
            "Primero seleccioná en la tabla el contacto que querés eliminar.");
        return;
    }

    int fila = seleccion[0]->row();
    QString nombre = ui.tblContactos->item(fila, 0)->text();

    QMessageBox::StandardButton respuesta = QMessageBox::question(this,
        "Confirmar eliminación",
        "¿Seguro que querés eliminar a " + nombre + "?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (respuesta != QMessageBox::Yes)
        return; // se cancelo, no se toca nada

    if (!archivoContactos.eliminar(fila)) {
        QMessageBox::critical(this, "No se pudo eliminar",
            "No se pudo actualizar el archivo.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    cargarTabla();
    QMessageBox::information(this, "Contacto eliminado",
        "El contacto se eliminó correctamente.");
}
