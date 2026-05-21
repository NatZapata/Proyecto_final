#ifndef OBSTACULO_H
#define OBSTACULO_H

#pragma once
#include "Entidad.h"

class Obstaculo : public Entidad
{
public:
    enum class TipoMovimiento { Estatico, Vertical };

    explicit Obstaculo(TipoMovimiento tipoMovimiento, QGraphicsItem* parent = nullptr);

    void actualizar(double tiempoDelta) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    TipoMovimiento m_tipoMovimiento;
    double m_tiempo;
    double m_baseY;
};

#endif // OBSTACULO_H
