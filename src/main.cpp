#include <QApplication>
#include "qfactureimpl.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QfactureImpl win;

    win.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &win, SLOT(doQuit()));

    return app.exec();
}
