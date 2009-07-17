#include <QApplication>

#include "mainWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    mainWidget window;
    window.show();
    return app.exec();
}
