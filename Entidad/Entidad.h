#ifndef ENTIDAD_H
#define ENTIDAD_H

#pragma once
#include <QGraphicsObject>
#include <QPointF>
#include <QSizeF>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

enum class TipoEntidad {
    Jugador,
    Obstaculo,
    Recolectable
};

class Entidad : public QGraphicsObject
{
public:
    Entidad(TipoEntidad tipo, const QSizeF& tamanio, QGraphicsItem* parent = nullptr);
    ~Entidad() override = default;

    QRectF boundingRect() const override;
    virtual void actualizar(double tiempoDelta);

    TipoEntidad tipoEntidad() const;
    QPointF velocidad() const;
    void setVelocidad(const QPointF& velocidad);
    double masa() const;
    void setMasa(double masa);
    bool estaVivo() const;
    void destruir();

protected:
    QSizeF m_tamanio;
    QPointF m_velocidad;
    double m_masa;
    bool m_vivo;
    TipoEntidad m_tipo;
};

#endif // ENTIDAD_H
