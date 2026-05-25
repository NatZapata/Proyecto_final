#ifndef JUGADOR_H
#define JUGADOR_H

#pragma once
#include "Entidad.h"

class Jugador : public Entidad
{
public:
    explicit Jugador(double sueloY, QGraphicsItem* parent = nullptr);

    void actualizar(double tiempoDelta) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void saltar();
    void moverIzquierda(bool activo);
    void moverDerecha(bool activo);
    void perderEnergia(int valor);
    void agregarEnergia(int valor);

    int energia() const;
    bool estaEnSuelo() const;

private:
    double m_sueloY;
    bool m_enSuelo;
    bool m_izquierda;
    bool m_derecha;
    int m_energia;
};

#endif // JUGADOR_H
