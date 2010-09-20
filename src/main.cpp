#include <QApplication>
#include "qfactureimpl.h"

// Code 
int main(int argc, char ** argv)
{
  QApplication app( argc, argv );
  QfactureImpl win;
  win.show(); 
  app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
  return app.exec();
}
