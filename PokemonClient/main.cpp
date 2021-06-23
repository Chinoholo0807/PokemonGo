#include "logwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogWidget w;
    w.show();
//    StackWidget s;
//    s.show();
//    HallWidget h;
//    h.show();
//    FightingWidget f;
//    f.show();
  //  f->show();
//    FightWidget f;
//    f.show();
    return a.exec();
}
