#ifndef QPROCESSOSEND_H
#define QPROCESSOSEND_H

#include <QThread>
#include "Processa.h"

class QProcessoSend : public QThread
{
    Q_OBJECT
public:
    explicit QProcessoSend(CProcessa *newProcessa,QObject *parent);
    void ProcessoRecivedData(QByteArray);
    void run();

signals:

public slots:

private:
   CProcessa *Processa;

};

#endif // QPROCESSOSEND_H
