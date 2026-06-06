#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GameWidget.h"
#include "GestorRecursos.h"
#include "GestorSonido.h"
#include "../logica/Dificultad.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui_(new Ui::MainWindow),
    juego_(nullptr),
    recursos_(new GestorRecursos()),
    sonido_(new GestorSonido(this)) {

    ui_->setupUi(this);
    setWindowTitle("Al infinito... Y a correr - Atletismo en el mundo de Toy Story");
    setFixedSize(960, 540);

    ui_->comboDificultad->addItem("Facil",   static_cast<int>(logica::NivelDificultad::Facil));
    ui_->comboDificultad->addItem("Media",   static_cast<int>(logica::NivelDificultad::Normal));
    ui_->comboDificultad->addItem("Dificil", static_cast<int>(logica::NivelDificultad::Dificil));
    ui_->comboDificultad->setCurrentIndex(0);

    auto* layJuego = new QVBoxLayout(ui_->gameContainer);
    layJuego->setContentsMargins(0, 0, 0, 0);
    layJuego->setSpacing(0);
    juego_ = new GameWidget(recursos_, sonido_, ui_->gameContainer);
    layJuego->addWidget(juego_);

    connect(juego_, &GameWidget::volverAlMenu, this, &MainWindow::mostrarMenu);
    connect(ui_->btnNivel1, &QPushButton::clicked, this, [this]{ jugarNivel(1); });
    connect(ui_->btnNivel2, &QPushButton::clicked, this, [this]{ jugarNivel(2); });
    connect(ui_->btnCreditos, &QPushButton::clicked, this, &MainWindow::mostrarCreditos);
    connect(ui_->btnSalir, &QPushButton::clicked, this, &QWidget::close);
    connect(ui_->chkMute, &QCheckBox::toggled, this, &MainWindow::alternarSonido);

    ui_->stackedWidget->setCurrentWidget(ui_->pageMenu);
    sonido_->reproducirFondo("bg_menu");
}

MainWindow::~MainWindow() {
    delete recursos_;
    delete ui_;
}

logica::NivelDificultad MainWindow::dificultadSeleccionada() const {
    return static_cast<logica::NivelDificultad>(ui_->comboDificultad->currentData().toInt());
}

void MainWindow::jugarNivel(int numeroNivel) {
    juego_->iniciarNivel(numeroNivel, dificultadSeleccionada());
    ui_->stackedWidget->setCurrentWidget(ui_->pageGame);
    sonido_->reproducirFondo("bg_level");
    juego_->setFocus();
}

void MainWindow::mostrarMenu() {
    ui_->stackedWidget->setCurrentWidget(ui_->pageMenu);
    sonido_->reproducirFondo("bg_menu");
}

void MainWindow::alternarSonido(bool silenciado) {
    sonido_->setSilenciado(silenciado);
    if (!silenciado && ui_->stackedWidget->currentWidget() == ui_->pageMenu) {
        sonido_->reproducirFondo("bg_menu");
    }
}

void MainWindow::mostrarCreditos() {
    QMessageBox::information(
        this,
        "Créditos",
        "Desarrollo y programación\n"
        "  Natalia Zapata Zapata\n"
        "  Valentina Posada Vásquez\n\n"
        "Proyecto académico para Informática II\n"
        "Universidad de Antioquia, 2026\n\n"
        "Recursos audiovisuales\n"
        "  Sprites e imagenes inspirados en Toy Story.\n"
        "  Efectos de sonido basados en la banda sonora de Toy Story.\n\n"
        "Notas\n"
        "  Desarrollado con fines exclusivamente educativos y sin animo de lucro.\n"
        "  Todos los recursos externos pertenecen a sus respectivos autores.\n\n"
        "Agradecimientos\n"
        "  A nuestro docente, compañeros y a quienes probaron el juego.\n\n"
        "¡Gracias por jugar!"
        );
}
