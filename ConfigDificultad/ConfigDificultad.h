#ifndef CONFIGDIFICULTAD_H
#define CONFIGDIFICULTAD_H

#pragma once
#include <QString>

class ConfigDificultad
{
public:
    enum class Modo { Facil, Normal, Dificil };

    explicit ConfigDificultad(Modo modo = Modo::Normal);

    static QString modoATexto(Modo modo);

    Modo modo() const;
    QString nombre() const;
    double velocidadMundo() const;
    double intervaloObstaculo() const;
    int danoObstaculo() const;
    int energiaRecolectable() const;
    int duracionNivelSegundos() const;

private:
    Modo m_modo;
    QString m_nombre;
    double m_velocidadMundo;
    double m_intervaloObstaculo;
    int m_danoObstaculo;
    int m_energiaRecolectable;
    int m_duracionNivelSegundos;
};

#endif // CONFIGDIFICULTAD_H
