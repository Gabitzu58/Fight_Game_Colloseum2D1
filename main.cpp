#include <QApplication>
#include "GameWindow.h"
#include "SoundManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundManager::instance().playMusic();
    GameWindow w;
    w.show();

    return a.exec();
}
