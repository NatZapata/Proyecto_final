#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QElapsedTimer>
#include <memory>
#include "../logica/Dificultad.h"


class QTimer;
class GestorRecursos;
class GestorSonido;

namespace logica { class Nivel; }

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(GestorRecursos* recursos, GestorSonido* sonido,
                        QWidget* parent = nullptr);
    ~GameWidget() override;

    void iniciarNivel(int numeroNivel, logica::NivelDificultad dificultad);

signals:
    void volverAlMenu();

protected:
    void paintEvent(QPaintEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

private slots:
    void tick();

private:
    void dibujarFondo(class QPainter& p);
    void dibujarEntidades(QPainter& p);
    void dibujarHUD(QPainter& p);
    void dibujarMensajeFin(QPainter& p);
    void detectarEventosSonido();

    GestorRecursos* recursos_;
    GestorSonido*   sonido_;
    QTimer*         timer_;
    QElapsedTimer   reloj_;
    std::unique_ptr<logica::Nivel> nivel_;

    int    numeroNivel_;
    logica::NivelDificultad dificultad_;
    bool   pausado_;

    double tiempoAnim_;
    int    frameJugador_;
    int    frameEnemigo_;

    double energiaPrev_;
    bool   coulombPrev_;
    int    estadoPrev_;   // 0 en curso, 1 ganado, 2 perdido
    bool   finReproducido_;
    double tiempoTransicionNivel_;
};

#endif // GAMEWIDGET_H

