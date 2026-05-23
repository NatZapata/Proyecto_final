#include "Obstaculo.h"

#include <QPainter>
#include <QtMath>

Obstaculo::Obstaculo(TipoMovimiento tipoMovimiento, QGraphicsItem* parent)
    : Entidad(TipoEntidad::Obstaculo, QSizeF(50, 50), parent),
    m_tipoMovimiento(tipoMovimiento),
    m_tiempo(0.0),
    m_baseY(0.0)
{
    setMasa(tipoMovimiento == TipoMovimiento::Estatico ? 6.0 : 9.0);
}

void Obstaculo::actualizar(double tiempoDelta)
{
    if (m_baseY == 0.0) {
        m_baseY = pos().y();
    }

    m_tiempo += tiempoDelta;
    Entidad::actualizar(tiempoDelta);

    if (m_tipoMovimiento == TipoMovimiento::Vertical) {
        setY(m_baseY + qSin(m_tiempo * 4.0) * 22.0);
    }

    if (pos().x() < -80.0) {
        destruir();
    }
}

void Obstaculo::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(QColor(65, 65, 65), 2));

    if (m_tipoMovimiento == TipoMovimiento::Estatico) {
        painter->setBrush(QColor(210, 90, 90));
        painter->drawRoundedRect(4, 8, 42, 34, 7, 7);
        painter->setBrush(QColor(250, 230, 120));
        painter->drawEllipse(9, 12, 10, 10);
        painter->drawEllipse(30, 12, 10, 10);
    } else {
        painter->setBrush(QColor(115, 155, 230));
        painter->drawEllipse(5, 5, 40, 40);
        painter->setPen(QPen(Qt::white, 3));
        painter->drawLine(16, 16, 34, 34);
        painter->drawLine(34, 16, 16, 34);
    }
}
