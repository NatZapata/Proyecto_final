#include "Recolectable.h"

#include <QPainter>

Recolectable::Recolectable(QGraphicsItem* parent)
    : Entidad(TipoEntidad::Recolectable, QSizeF(34, 34), parent)
{
    setMasa(0.6);
}

void Recolectable::actualizar(double tiempoDelta)
{
    Entidad::actualizar(tiempoDelta);
    if (pos().x() < -60.0) {
        destruir();
    }
}

void Recolectable::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(QColor(40, 120, 70), 2));
    painter->setBrush(QColor(80, 210, 120));
    painter->drawEllipse(3, 3, 28, 28);
    painter->setPen(QPen(Qt::white, 4, Qt::RoundCap));
    painter->drawLine(17, 9, 17, 25);
    painter->drawLine(9, 17, 25, 17);
}
