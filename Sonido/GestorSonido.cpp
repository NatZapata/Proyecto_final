#include "GestorSonido.h"

#ifdef TIENE_AUDIO
#include <QSoundEffect>
#include <QUrl>
#endif

GestorSonido::GestorSonido(QObject* parent)
    : QObject(parent), silenciado_(false)
#ifdef TIENE_AUDIO
    , fondo_(nullptr)
#endif
{
}

GestorSonido::~GestorSonido() = default;

#ifdef TIENE_AUDIO

QSoundEffect* GestorSonido::cargar(const QString& archivo) {
    auto it = efectos_.constFind(archivo);
    if (it != efectos_.constEnd()) return *it;

    QSoundEffect* ef = new QSoundEffect(this);
    ef->setSource(QUrl("qrc:/assets/sounds/" + archivo + ".wav"));
    efectos_.insert(archivo, ef);
    return ef;
}

void GestorSonido::reproducirEvento(const QString& clave) {
    if (silenciado_) return;
    QSoundEffect* ef = cargar(clave);
    ef->setLoopCount(1);
    ef->setVolume(0.7);
    ef->play();
}

void GestorSonido::reproducirFondo(const QString& clave) {
    detenerFondo();
    if (silenciado_) return;
    fondo_ = cargar(clave);
    fondo_->setLoopCount(QSoundEffect::Infinite);
    fondo_->setVolume(0.35);
    fondo_->play();
}

void GestorSonido::detenerFondo() {
    if (fondo_) { fondo_->stop(); fondo_ = nullptr; }
}

void GestorSonido::setSilenciado(bool s) {
    silenciado_ = s;
    if (s) detenerFondo();
}

#else

void GestorSonido::reproducirEvento(const QString&) {}
void GestorSonido::reproducirFondo(const QString&) {}
void GestorSonido::detenerFondo() {}
void GestorSonido::setSilenciado(bool s) { silenciado_ = s; }

#endif
