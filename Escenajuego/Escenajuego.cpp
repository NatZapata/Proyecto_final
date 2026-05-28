#include "EscenaJuego.h"
#include "Fisica.h"

#include <QBrush>
#include <QFont>
#include <QKeyEvent>
#include <QLinearGradient>
#include <QPen>
#include <QRandomGenerator>
#include <QGraphicsTextItem>
#include <algorithm>
#include <stdexcept>

EscenaJuego::EscenaJuego(const ConfigDificultad& dificultad, QObject* parent)
    : QGraphicsScene(parent),
    m_dificultad(dificultad),
    m_sueloY(438.0),
    m_segundosTranscurridos(0.0),
    m_temporizadorObstaculo(0.8),
    m_temporizadorRecolectable(4.0),
    m_puntaje(0),
    m_corriendo(false)
{
    setSceneRect(0, 0, 900, 560);
    connect(&m_temporizador, &QTimer::timeout, this, &EscenaJuego::buclePrincipal);
}

EscenaJuego::~EscenaJuego()
{
    m_temporizador.stop();
    limpiarMundo();
}

void EscenaJuego::iniciarNivelUno()
{
    limpiarMundo();
    m_sueloY = 438.0;
    m_segundosTranscurridos = 0.0;
    m_temporizadorObstaculo = 0.8;
    m_temporizadorRecolectable = 4.0;
    m_puntaje = 0;
    m_corriendo = true;

    dibujarFondo();

    m_jugador = std::make_unique<Jugador>(m_sueloY);
    if (!m_jugador) {
        throw std::runtime_error("No se pudo crear el jugador principal.");
    }
    addItem(m_jugador.get());

    auto* titulo = addText("Nivel 1: Habitacion de juguetes");
    titulo->setDefaultTextColor(Qt::white);
    titulo->setFont(QFont("Arial", 15, QFont::Bold));
    titulo->setPos(22, 18);

    m_temporizadorTranscurrido.start();
    m_temporizador.start(16);
    emit hudActualizado(m_jugador->energia(), m_dificultad.duracionNivelSegundos(), m_puntaje, m_dificultad.nombre());
}

void EscenaJuego::dibujarFondo()
{
    QLinearGradient degradado(0, 0, 0, 560);
    degradado.setColorAt(0, QColor(78, 130, 200));
    degradado.setColorAt(1, QColor(232, 183, 120));
    addRect(sceneRect(), Qt::NoPen, QBrush(degradado));

    addRect(0, 392, 900, 96, Qt::NoPen, QColor(186, 126, 72));
    addRect(0, 488, 900, 72, Qt::NoPen, QColor(112, 80, 54));

    for (int i = 0; i < 7; ++i) {
        addRect(35 + i * 130, 325, 74, 67, QPen(QColor(120, 85, 48)), QColor(235, 191, 119));
        addText(QString::number(i + 1))->setPos(64 + i * 130, 344);
    }

    auto* controles = addText("Controles: ESPACIO saltar | A/Flecha izquierda | D/Flecha derecha");
    controles->setDefaultTextColor(QColor(245, 245, 245));
    controles->setFont(QFont("Arial", 10));
    controles->setPos(24, 520);

    auto* nota = addText("Pendiente para entrega final: nivel 2, enemigo inteligente, sonidos y ejecutable desplegado.");
    nota->setDefaultTextColor(QColor(255, 250, 215));
    nota->setFont(QFont("Arial", 9));
    nota->setPos(24, 493);
}

void EscenaJuego::buclePrincipal()
{
    if (!m_corriendo || !m_jugador) {
        return;
    }

    double tiempoDelta = m_temporizadorTranscurrido.restart() / 1000.0;
    tiempoDelta = std::clamp(tiempoDelta, 0.001, 0.045);
    m_segundosTranscurridos += tiempoDelta;

    procesarEntrada();
    m_jugador->actualizar(tiempoDelta);

    m_temporizadorObstaculo -= tiempoDelta;
    if (m_temporizadorObstaculo <= 0.0) {
        generarObstaculo();
        m_temporizadorObstaculo = m_dificultad.intervaloObstaculo();
    }

    m_temporizadorRecolectable -= tiempoDelta;
    if (m_temporizadorRecolectable <= 0.0) {
        generarRecolectable();
        m_temporizadorRecolectable = 6.0;
    }

    actualizarEntidades(tiempoDelta);
    manejarColisiones();
    eliminarEntidadesMuertas();
    verificarEstado();

    const int restantes = std::max(0, m_dificultad.duracionNivelSegundos() - static_cast<int>(m_segundosTranscurridos));
    emit hudActualizado(m_jugador ? m_jugador->energia() : 0, restantes, m_puntaje, m_dificultad.nombre());
}

void EscenaJuego::procesarEntrada()
{
    if (!m_jugador) {
        return;
    }
    m_jugador->moverIzquierda(m_teclasPresionadas.contains(Qt::Key_A) || m_teclasPresionadas.contains(Qt::Key_Left));
    m_jugador->moverDerecha(m_teclasPresionadas.contains(Qt::Key_D) || m_teclasPresionadas.contains(Qt::Key_Right));
}

void EscenaJuego::generarObstaculo()
{
    const bool vertical = QRandomGenerator::global()->bounded(100) > 72;
    auto obstaculo = std::make_unique<Obstaculo>(vertical ? Obstaculo::TipoMovimiento::Vertical : Obstaculo::TipoMovimiento::Estatico);
    obstaculo->setPos(920, vertical ? m_sueloY - 68 : m_sueloY - 50);
    obstaculo->setVelocidad(QPointF(-m_dificultad.velocidadMundo(), 0));
    addItem(obstaculo.get());
    m_entidades.push_back(std::move(obstaculo));
}
void EscenaJuego::generarRecolectable()
{
    auto recolectable = std::make_unique<Recolectable>();
    recolectable->setPos(910, QRandomGenerator::global()->bounded(245, 335));
    recolectable->setVelocidad(QPointF(-m_dificultad.velocidadMundo() * 0.75, 0));
    addItem(recolectable.get());
    m_entidades.push_back(std::move(recolectable));
}

void EscenaJuego::actualizarEntidades(double tiempoDelta)
{
    for (auto& entidad : m_entidades) {
        entidad->actualizar(tiempoDelta);
    }
}

void EscenaJuego::manejarColisiones()
{
    if (!m_jugador) {
        return;
    }

    for (auto& entidad : m_entidades) {
        if (!entidad->estaVivo() || !m_jugador->collidesWithItem(entidad.get())) {
            continue;
        }

        if (entidad->tipoEntidad() == TipoEntidad::Obstaculo) {
            const double fuerza = MotorFisica::fuerzaNewton(entidad->masa(), 180.0);
            const int danoCalculado = MotorFisica::danoPorFuerza(fuerza);
            m_jugador->perderEnergia(std::max(danoCalculado, m_dificultad.danoObstaculo()));
            entidad->destruir();
        } else if (entidad->tipoEntidad() == TipoEntidad::Recolectable) {
            m_jugador->agregarEnergia(m_dificultad.energiaRecolectable());
            m_puntaje += 10;
            entidad->destruir();
        }
    }
}

void EscenaJuego::eliminarEntidadesMuertas()
{
    auto it = m_entidades.begin();
    while (it != m_entidades.end()) {
        if (!(*it)->estaVivo()) {
            removeItem(it->get());
            it = m_entidades.erase(it);
        } else {
            ++it;
        }
    }
}

void EscenaJuego::verificarEstado()
{
    if (!m_jugador) {
        return;
    }

    if (m_jugador->energia() <= 0) {
        finalizar(false, "La energia llego a cero.");
        return;
    }

    if (m_segundosTranscurridos >= m_dificultad.duracionNivelSegundos()) {
        finalizar(true, "Nivel 1 completado. Este avance corresponde aproximadamente al 50% del proyecto final.");
    }
}

void EscenaJuego::finalizar(bool completado, const QString& mensaje)
{
    if (!m_corriendo) {
        return;
    }
    m_corriendo = false;
    m_temporizador.stop();
    emit juegoTerminado(completado, mensaje);
}

void EscenaJuego::limpiarMundo()
{
    for (auto& entidad : m_entidades) {
        removeItem(entidad.get());
    }
    m_entidades.clear();

    if (m_jugador) {
        removeItem(m_jugador.get());
        m_jugador.reset();
    }

    clear();
}

void EscenaJuego::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat()) {
        m_teclasPresionadas.insert(event->key());
    }
    if (event->key() == Qt::Key_Space && m_jugador) {
        m_jugador->saltar();
    }
    QGraphicsScene::keyPressEvent(event);
}

void EscenaJuego::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat()) {
        m_teclasPresionadas.remove(event->key());
    }
    QGraphicsScene::keyReleaseEvent(event);
}
