#include "clientenet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainClientnet w;
    w.show();

    return a.exec();
}
