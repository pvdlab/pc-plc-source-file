
#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QTcpServer>
#include "qimpianto.h"
#include "qserverthread.h"


class QServer : public QTcpServer
{
    Q_OBJECT

public:
    QServer(QObject *parent ,QImpianto *newimpianto);
    void Ricevi(QByteArray msg);

protected:
    void incomingConnection(qintptr socketDescriptor);


private:
    QImpianto *Impianto;

};


#endif
