#ifndef RECOLECTABLE_H
#define RECOLECTABLE_H

#pragma once
#include "Entidad.h"

class Recolectable : public Entidad
{
public:
    explicit Recolectable(QGraphicsItem* parent = nullptr);

    void actualizar(double tiempoDelta) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
#endif // RECOLECTABLE_H
