#include "ConfigDificultad.h"

ConfigDificultad::ConfigDificultad(Modo modo)
    : m_modo(modo)
{
    switch (modo) {
    case Modo::Facil:
        m_nombre = "Facil";
        m_velocidadMundo = 180.0;
        m_intervaloObstaculo = 1.65;
        m_danoObstaculo = 7;
        m_energiaRecolectable = 14;
        m_duracionNivelSegundos = 45;
        break;
    case Modo::Normal:
        m_nombre = "Normal";
        m_velocidadMundo = 215.0;
        m_intervaloObstaculo = 1.25;
        m_danoObstaculo = 10;
        m_energiaRecolectable = 11;
        m_duracionNivelSegundos = 50;
        break;
    case Modo::Dificil:
        m_nombre = "Dificil";
        m_velocidadMundo = 250.0;
        m_intervaloObstaculo = 0.95;
        m_danoObstaculo = 14;
        m_energiaRecolectable = 8;
        m_duracionNivelSegundos = 55;
        break;
    }
}

QString ConfigDificultad::modoATexto(Modo modo)
{
    return ConfigDificultad(modo).nombre();
}

ConfigDificultad::Modo ConfigDificultad::modo() const { return m_modo; }
QString ConfigDificultad::nombre() const { return m_nombre; }
double ConfigDificultad::velocidadMundo() const { return m_velocidadMundo; }
double ConfigDificultad::intervaloObstaculo() const { return m_intervaloObstaculo; }
int ConfigDificultad::danoObstaculo() const { return m_danoObstaculo; }
int ConfigDificultad::energiaRecolectable() const { return m_energiaRecolectable; }
int ConfigDificultad::duracionNivelSegundos() const { return m_duracionNivelSegundos; }
