#ifndef CTEST_H
#define CTEST_H

#include <QDebug>
#include <QThread>
#include "Macchina.h"

class ctest : public QThread
{

public:
    explicit ctest(CMacchina *newProcessa, QString newTest,QObject *parent = 0);
    void run();

private:
    void testPower();
    void mainComando(BYTE comando);

private:
    CMacchina *Processa;
    QString Test;
};

#endif // CTEST_H
