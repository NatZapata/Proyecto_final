#include <QApplication>
#include <QMessageBox>
#include <exception>

#include "gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Toy Story Run");

    try {
        MainWindow ventana;
        ventana.show();
        return app.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error", e.what());
        return 1;
    }
}
