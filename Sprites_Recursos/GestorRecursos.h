#ifndef GESTORRECURSOS_H
#define GESTORRECURSOS_H

#include <stdexcept>
#include <string>
#include <QPixmap>
#include <QVector>
#include <QString>
#include <QHash>

// carga los sprites (QPixmap).

class RecursoNoDisponibleException : public std::runtime_error {
public:
    explicit RecursoNoDisponibleException(const std::string& recurso)
        : std::runtime_error("Recurso no disponible: " + recurso) {}
};

class GestorRecursos {
    QHash<QString, QVector<QPixmap>> animaciones_;

    void cargar(const QString& clave, const QStringList& rutas);

public:
    GestorRecursos();   // carga todos los sprites

    QPixmap frame(const QString& clave, int indice = 0) const;

    int  numFrames(const QString& clave) const;
    bool tiene(const QString& clave) const { return animaciones_.contains(clave); }
};

#endif // GESTORRECURSOS_H
