#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H
#pragma once

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QTimer>
#include <QSet>
#include <memory>
#include <vector>

#include "ConfigDificultad.h"
#include "Jugador.h"
#include "Obstaculo.h"
#include "Recolectable.h"

class EscenaJuego : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EscenaJuego(const ConfigDificultad& dificultad, QObject* parent = nullptr);
    ~EscenaJuego() override;

    void iniciarNivelUno();

signals:
    void hudActualizado(int energia, int segundosRestantes, int puntaje, const QString& dificultad);
    void juegoTerminado(bool completado, const QString& mensaje);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void buclePrincipal();

private:
    void dibujarFondo();
    void generarObstaculo();
    void generarRecolectable();
    void actualizarEntidades(double tiempoDelta);
    void manejarColisiones();
    void eliminarEntidadesMuertas();
    void verificarEstado();
    void procesarEntrada();
    void limpiarMundo();
    void finalizar(bool completado, const QString& mensaje);

    ConfigDificultad m_dificultad;
    QTimer m_temporizador;
    QElapsedTimer m_temporizadorTranscurrido;
    std::unique_ptr<Jugador> m_jugador;
    std::vector<std::unique_ptr<Entidad>> m_entidades;
    QSet<int> m_teclasPresionadas;

    double m_sueloY;
    double m_segundosTranscurridos;
    double m_temporizadorObstaculo;
    double m_temporizadorRecolectable;
    int m_puntaje;
    bool m_corriendo;
};

#endif // ESCENAJUEGO_H
