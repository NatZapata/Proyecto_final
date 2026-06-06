#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../logica/Dificultad.h"

class GameWidget;
class GestorRecursos;
class GestorSonido;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void jugarNivel(int numeroNivel);
    void mostrarMenu();
    void alternarSonido(bool silenciado);
    void mostrarCreditos();

private:
    logica::NivelDificultad dificultadSeleccionada() const;

    Ui::MainWindow* ui_;
    GameWidget*     juego_;
    GestorRecursos* recursos_;
    GestorSonido*   sonido_;
};

#endif // MAINWINDOW_H
