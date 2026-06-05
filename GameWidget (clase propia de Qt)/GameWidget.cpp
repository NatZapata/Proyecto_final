#include "GameWidget.h"
#include "GestorRecursos.h"
#include "GestorSonido.h"

#include "../logica/Nivel.h"
#include "../logica/Personaje.h"
#include "../logica/Obstaculo.h"
#include "../logica/ObjetoCayendo.h"
#include "../logica/Enemigo.h"

#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QLinearGradient>
#include <QMessageBox>
#include <cmath>
#include <algorithm>

static const int ANCHO = 960;
static const int ALTO  = 540;

GameWidget::GameWidget(GestorRecursos* recursos, GestorSonido* sonido, QWidget* parent)
    : QWidget(parent), recursos_(recursos), sonido_(sonido),
    timer_(new QTimer(this)), nivel_(nullptr),
    numeroNivel_(1), dificultad_(logica::NivelDificultad::Normal),
    pausado_(false), tiempoAnim_(0.0), frameJugador_(0), frameEnemigo_(0),
    energiaPrev_(0.0), coulombPrev_(false), estadoPrev_(0), finReproducido_(false),
    tiempoTransicionNivel_(0.0) {
    setFixedSize(ANCHO, ALTO);
    setFocusPolicy(Qt::StrongFocus);
    connect(timer_, &QTimer::timeout, this, &GameWidget::tick);
}

GameWidget::~GameWidget() = default;

void GameWidget::iniciarNivel(int numeroNivel, logica::NivelDificultad dificultad) {
    numeroNivel_ = numeroNivel;
    dificultad_  = dificultad;

    logica::ConfiguracionDificultad cfg =
        logica::ConfiguracionDificultad::crear(dificultad);

    nivel_ = std::make_unique<logica::Nivel>(numeroNivel, cfg, ANCHO, ALTO);
    nivel_->iniciar();

    pausado_ = false;
    tiempoAnim_ = 0.0;
    finReproducido_ = false;
    tiempoTransicionNivel_ = 0.0;
    energiaPrev_ = nivel_->jugador()->energia();
    coulombPrev_ = false;
    estadoPrev_  = 0;

    sonido_->reproducirFondo(numeroNivel == 1 ? "bg_level" : "bg_level");

    reloj_.restart();
    timer_->start(16);   // ~60 FPS
    setFocus();
    update();
}

// --------------------------- bucle de juego --------------------------------
void GameWidget::tick() {
    qint64 ms = reloj_.restart();
    double dt = ms / 1000.0;
    if (dt > 0.05) dt = 0.05;

    if (nivel_) {
        tiempoAnim_ += dt;
        frameJugador_ = static_cast<int>(tiempoAnim_ * 12.0);
        frameEnemigo_ = static_cast<int>(tiempoAnim_ * 6.0);

        if (!pausado_ && nivel_->estado() == logica::EstadoNivel::EnCurso) {
            nivel_->actualizar(dt);
            detectarEventosSonido();
        } else if (!pausado_ && nivel_->estado() == logica::EstadoNivel::Ganado && numeroNivel_ == 1) {
            // Transición automática: si gana el nivel 1, pasa al nivel 2.
            tiempoTransicionNivel_ += dt;
            if (tiempoTransicionNivel_ >= 2.0) {
                iniciarNivel(2, dificultad_);
                return;
            }
        }
    }
    update();
}

void GameWidget::detectarEventosSonido() {
    const logica::Personaje* j = nivel_->jugador();
    if (!j) return;

    double e = j->energia();
    if (e < energiaPrev_ - 0.5)      sonido_->reproducirEvento("hit");
    else if (e > energiaPrev_ + 0.5) sonido_->reproducirEvento("pickup");
    energiaPrev_ = e;

    bool coulomb = j->energizado();
    if (coulomb && !coulombPrev_) sonido_->reproducirEvento("power");
    coulombPrev_ = coulomb;
}

// --------------------------- entrada ---------------------------------------
void GameWidget::keyPressEvent(QKeyEvent* e) {
    if (!nivel_ || e->isAutoRepeat()) return;

    switch (e->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        if (nivel_->estado() == logica::EstadoNivel::EnCurso && !pausado_)
            nivel_->moverJugadorIzquierda(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        if (nivel_->estado() == logica::EstadoNivel::EnCurso && !pausado_)
            nivel_->moverJugadorDerecha(true);
        break;
    case Qt::Key_Space:
    case Qt::Key_Up:
    case Qt::Key_W:
        if (nivel_->estado() == logica::EstadoNivel::EnCurso && !pausado_) {
            bool enSuelo = nivel_->jugador()->enElSuelo();
            nivel_->saltarJugador();
            if (enSuelo) sonido_->reproducirEvento("jump");
        }
        break;
    case Qt::Key_P:
        if (nivel_->estado() == logica::EstadoNivel::EnCurso) {
            pausado_ = !pausado_;
            if (!pausado_) reloj_.restart();
        }
        break;
    case Qt::Key_R:
        iniciarNivel(numeroNivel_, dificultad_);
        break;
    case Qt::Key_Escape:
        timer_->stop();
        sonido_->detenerFondo();
        emit volverAlMenu();
        break;
    default:
        QWidget::keyPressEvent(e);
    }
    update();
}

void GameWidget::keyReleaseEvent(QKeyEvent* e) {
    if (!nivel_ || e->isAutoRepeat()) return;

    switch (e->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        nivel_->moverJugadorIzquierda(false);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        nivel_->moverJugadorDerecha(false);
        break;
    default:
        QWidget::keyReleaseEvent(e);
    }
}

// --------------------------- vista ----------------------------------------
void GameWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    dibujarFondo(p);
    if (nivel_) {
        dibujarEntidades(p);
        dibujarHUD(p);
        if (nivel_->estado() != logica::EstadoNivel::EnCurso || pausado_) {
            dibujarMensajeFin(p);
        }
    }
}

void GameWidget::dibujarFondo(QPainter& p) {
    const QString fondoClave = (numeroNivel_ == 1) ? "background_room" : "background_garden";
    QPixmap fondo = recursos_->frame(fondoClave);
    if (!fondo.isNull()) {
        p.drawPixmap(QRectF(rect()), fondo, QRectF(fondo.rect()));
    } else {
        QLinearGradient g(0, 0, 0, ALTO);
        if (numeroNivel_ == 1) {
            g.setColorAt(0.0, QColor(120, 170, 220));
            g.setColorAt(0.6, QColor(200, 215, 235));
            g.setColorAt(1.0, QColor(225, 205, 175));
        } else {
            g.setColorAt(0.0, QColor(135, 206, 235));
            g.setColorAt(0.65, QColor(180, 225, 200));
            g.setColorAt(1.0, QColor(120, 180, 110));
        }
        p.fillRect(rect(), g);
    }

    double sueloY = nivel_ ? nivel_->sueloY() : ALTO - 70.0;
    QColor linea = (numeroNivel_ == 1) ? QColor(120, 85, 45) : QColor(65, 120, 55);
    p.setPen(QPen(linea, 4));
    p.drawLine(QPointF(0, sueloY), QPointF(ANCHO, sueloY));
}

static void dibujarSprite(QPainter& p, const QPixmap& pm,
                          double x, double y, double w, double h,
                          const QColor& fallback) {
    if (!pm.isNull()) {
        p.drawPixmap(QRectF(x, y, w, h), pm, QRectF(pm.rect()));
    } else {
        p.setBrush(fallback);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QRectF(x, y, w, h));
    }
}

void GameWidget::dibujarEntidades(QPainter& p) {
    using namespace logica;

    // obstaculos
    for (const Obstaculo* o : nivel_->obstaculos()) {
        QString clave = QString::fromStdString(o->sprite());
        bool animado = (clave == "potato" || clave == "traveller");
        QPixmap pm = recursos_->frame(clave, animado ? frameJugador_ : 0);
        if (clave == "traveller") {
            // cambio de sprite para mirar al otro lado
            p.save();
            p.translate(o->x() + o->ancho(), 0);
            p.scale(-1.0, 1.0);
            dibujarSprite(p, pm, 0, o->y(), o->ancho(), o->alto(), QColor(200,80,80));
            p.restore();
        } else {
            dibujarSprite(p, pm, o->x(), o->y(), o->ancho(), o->alto(), QColor(200,80,80));
        }
    }

    // objetos que caen
    for (const ObjetoCayendo* obj : nivel_->objetos()) {
        QPixmap pm = recursos_->frame(QString::fromStdString(obj->sprite()));
        dibujarSprite(p, pm, obj->x(), obj->y(), obj->ancho(), obj->alto(),
                      obj->premiado() ? QColor(90,200,120) : QColor(230,200,60));
        if (obj->premiado()) {
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(QColor(255,235,120,180), 2));
            p.drawEllipse(QRectF(obj->x()-4, obj->y()-4, obj->ancho()+8, obj->alto()+8));
        }
    }

    // piedras del enemigo
    if (const logica::Enemigo* en = nivel_->enemigo()) {
        for (const logica::PiedraData& pd : en->piedras()) {
            QRadialGradient rg(pd.centroX(), pd.centroY(), pd.ancho);
            rg.setColorAt(0.0, QColor(160,160,165));
            rg.setColorAt(1.0, QColor(90,90,95));
            p.setBrush(rg);
            p.setPen(QPen(QColor(60,60,65), 1));
            p.drawEllipse(QRectF(pd.x, pd.y, pd.ancho, pd.ancho));
        }
    }

    // enemigo
    if (const Enemigo* en = nivel_->enemigo()) {
        QPixmap pm = recursos_->frame("enemy", frameEnemigo_);
        p.save();
        p.translate(en->x() + en->ancho(), 0);
        p.scale(-1.0, 1.0);
        dibujarSprite(p, pm, 0, en->y(), en->ancho(), en->alto(), QColor(120,80,160));
        p.restore();
    }

    // personaje
    const Personaje* j = nivel_->jugador();
    if (j) {
        if (j->energizado()) {   // coulomb
            QRadialGradient rg(j->centroX(), j->centroY(), j->ancho());
            rg.setColorAt(0.0, QColor(120, 200, 255, 150));
            rg.setColorAt(1.0, QColor(120, 200, 255, 0));
            p.setBrush(rg);
            p.setPen(Qt::NoPen);
            p.drawEllipse(QRectF(j->x()-18, j->y()-18, j->ancho()+36, j->alto()+36));
        }
        QString clave = j->enElSuelo() ? "woody_run" : "woody_jump";
        QPixmap pm = recursos_->frame(clave, frameJugador_);
        dibujarSprite(p, pm, j->x(), j->y(), j->ancho(), j->alto(), QColor(200,150,60));
    }
}

void GameWidget::dibujarHUD(QPainter& p) {
    const logica::Personaje* j = nivel_->jugador();
    if (!j) return;

    // barra de energia
    double frac = j->energia() / j->energiaMax();
    QRectF barra(20, 18, 240, 22);
    p.setPen(QPen(Qt::black, 2));
    p.setBrush(QColor(40, 40, 40, 160));
    p.drawRoundedRect(barra, 6, 6);
    QColor c = frac > 0.5 ? QColor(70, 200, 90)
               : frac > 0.25 ? QColor(230, 190, 50) : QColor(220, 70, 60);
    p.setBrush(c);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(barra.x()+2, barra.y()+2,
                             (barra.width()-4) * std::max(0.0, frac), barra.height()-4), 5, 5);
    p.setPen(Qt::white);
    QFont f = p.font(); f.setBold(true); f.setPointSize(10); p.setFont(f);
    p.drawText(barra, Qt::AlignCenter, QString("Energia: %1").arg(static_cast<int>(j->energia())));

    // tiempo y puntaje
    f.setPointSize(13); p.setFont(f);
    p.setPen(Qt::black);
    double restante = std::max(0.0, nivel_->tiempoRestante());
    p.drawText(QRectF(ANCHO/2.0-120, 14, 240, 28), Qt::AlignCenter,
               QString("Tiempo: %1 s").arg(static_cast<int>(restante)));
    p.drawText(QRectF(ANCHO-220, 14, 200, 28), Qt::AlignRight,
               QString("Puntaje: %1").arg(nivel_->puntaje()));

    // nivel  y dificultad
    f.setPointSize(10); f.setBold(false); p.setFont(f);
    p.drawText(QRectF(20, 46, 420, 22), Qt::AlignLeft,
               QString("Nivel %1 - %2  |  Dificultad: %3")
                   .arg(nivel_->numero())
                   .arg(QString::fromStdString(nivel_->nombre()))
                   .arg(QString::fromStdString(nivel_->config().nombre())));

    // indicador de coulomb
    if (j->energizado()) {
        f.setBold(true); p.setFont(f);
        p.setPen(QColor(30, 90, 200));
        p.drawText(QRectF(ANCHO/2.0-140, 44, 280, 22), Qt::AlignCenter,
                   "¡ATRACCION ELECTRICA ACTIVA! (Coulomb)");
    }

    // teclas
    f.setBold(false); f.setPointSize(9); p.setFont(f);
    p.setPen(QColor(20,20,20,180));
    p.drawText(QRectF(20, ALTO-26, ANCHO-40, 20), Qt::AlignLeft,
               "A/D o ←/→ : mover    ESPACIO/↑ : saltar    P : pausa    R : reiniciar    ESC : menu");
}

void GameWidget::dibujarMensajeFin(QPainter& p) {
    p.fillRect(rect(), QColor(0, 0, 0, 130));
    QFont f = p.font(); f.setBold(true); f.setPointSize(30); p.setFont(f);

    QString titulo, sub;
    auto estado = nivel_->estado();
    if (pausado_ && estado == logica::EstadoNivel::EnCurso) {
        titulo = "PAUSA";
        sub = "Pulsa P para continuar";
        p.setPen(Qt::white);
    } else if (estado == logica::EstadoNivel::Ganado) {
        titulo = "¡NIVEL SUPERADO!";
        sub = (numeroNivel_ == 1)
                  ? QString("Puntaje: %1   -   Pasando automaticamente al Nivel 2...").arg(nivel_->puntaje())
                  : QString("Puntaje: %1   -   Proyecto completado. R: reiniciar    ESC: menu").arg(nivel_->puntaje());
        p.setPen(QColor(120, 230, 130));
        if (!finReproducido_) { sonido_->reproducirEvento("win"); finReproducido_ = true; sonido_->detenerFondo(); }
    } else if (estado == logica::EstadoNivel::Perdido) {
        titulo = "GAME OVER";
        sub = QString("Te quedaste sin energia.   R: reiniciar    ESC: menu");
        p.setPen(QColor(235, 110, 100));
        if (!finReproducido_) { sonido_->reproducirEvento("gameover"); finReproducido_ = true; sonido_->detenerFondo(); }
    }

    p.drawText(QRectF(0, ALTO/2.0-60, ANCHO, 60), Qt::AlignCenter, titulo);
    f.setPointSize(13); f.setBold(false); p.setFont(f);
    p.setPen(Qt::white);
    p.drawText(QRectF(0, ALTO/2.0+10, ANCHO, 30), Qt::AlignCenter, sub);
}
