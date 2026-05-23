#include "Entidad.h"

Entidad::Entidad(TipoEntidad tipo, const QSizeF& tamanio, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    m_tamanio(tamanio),
    m_velocidad(0.0, 0.0),
    m_masa(1.0),
    m_vivo(true),
    m_tipo(tipo)
{
}

QRectF Entidad::boundingRect() const
{
    return QRectF(0, 0, m_tamanio.width(), m_tamanio.height());
}

void Entidad::actualizar(double tiempoDelta)
{
    setPos(pos() + m_velocidad * tiempoDelta);
}

TipoEntidad Entidad::tipoEntidad() const { return m_tipo; }
QPointF Entidad::velocidad() const { return m_velocidad; }
void Entidad::setVelocidad(const QPointF& velocidad) { m_velocidad = velocidad; }
double Entidad::masa() const { return m_masa; }
void Entidad::setMasa(double masa) { m_masa = masa; }
bool Entidad::estaVivo() const { return m_vivo; }
void Entidad::destruir() { m_vivo = false; }
