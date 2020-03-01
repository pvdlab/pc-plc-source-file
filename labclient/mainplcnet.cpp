#include "plcnet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainPlcnet w;
    w.show();


    return a.exec();
}


