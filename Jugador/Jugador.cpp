#include "Jugador.h"
#include "Fisica.h"

#include <QPainter>
#include <algorithm>

Jugador::Jugador(double sueloY, QGraphicsItem* parent)
    : Entidad(TipoEntidad::Jugador, QSizeF(54, 78), parent),
    m_sueloY(sueloY),
    m_enSuelo(true),
    m_izquierda(false),
    m_derecha(false),
    m_energia(100)
{
    setMasa(62.0);
    setPos(90, m_sueloY - m_tamanio.height());
}

void Jugador::actualizar(double tiempoDelta)
{
    double vx = 0.0;
    if (m_izquierda) vx -= 155.0;
    if (m_derecha)   vx += 155.0;

    QPointF siguienteVelocidad(vx, velocidad().y());
    if (!m_enSuelo) {
        siguienteVelocidad.setY(siguienteVelocidad.y() + MotorFisica::Gravedad * tiempoDelta);
    }

    QPointF siguientePosicion = MotorFisica::pasoParabolico(pos(), siguienteVelocidad, tiempoDelta);

    if (siguientePosicion.y() >= m_sueloY - m_tamanio.height()) {
        siguientePosicion.setY(m_sueloY - m_tamanio.height());
        siguienteVelocidad.setY(0.0);
        m_enSuelo = true;
    }

    siguientePosicion.setX(std::clamp(siguientePosicion.x(), 20.0, 430.0));
    setVelocidad(siguienteVelocidad);
    setPos(siguientePosicion);
}

void Jugador::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(QPen(QColor(45, 45, 45), 2));
    painter->setBrush(QColor(255, 220, 155));
    painter->drawEllipse(15, 0, 26, 26);

    painter->setBrush(QColor(255, 210, 75));
    painter->drawRoundedRect(12, 26, 30, 32, 7, 7);

    painter->setPen(QPen(QColor(30, 80, 170), 5, Qt::RoundCap));
    painter->drawLine(18, 58, 11, 76);
    painter->drawLine(36, 58, 45, 76);

    painter->setPen(QPen(QColor(255, 240, 130), 4, Qt::RoundCap));
    painter->drawLine(14, 34, 2, 47);
    painter->drawLine(40, 34, 52, 47);

    painter->setPen(QPen(Qt::black, 2));
    painter->drawPoint(23, 11);
    painter->drawPoint(34, 11);
}

void Jugador::saltar()
{
    if (!m_enSuelo || m_energia <= 0) {
        return;
    }
    setVelocidad(QPointF(velocidad().x(), -520.0));
    m_enSuelo = false;
    perderEnergia(2);
}

void Jugador::moverIzquierda(bool activo) { m_izquierda = activo; }
void Jugador::moverDerecha(bool activo)   { m_derecha = activo; }

void Jugador::perderEnergia(int valor)
{
    m_energia = std::max(0, m_energia - valor);
}

void Jugador::agregarEnergia(int valor)
{
    m_energia = std::min(100, m_energia + valor);
}

int Jugador::energia() const { return m_energia; }
bool Jugador::estaEnSuelo() const { return m_enSuelo; }
