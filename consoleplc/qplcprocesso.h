#ifndef QPLCPROCESSO_H
#define QPLCPROCESSO_H

#include <QObject>
#include <QThread>
#include <QHostAddress>
#include <QNetworkReply>
#include "Processa.h"
#include "Simula.h"
#include "qplcserver.h"

class QPlcProcesso
: public QObject

{

   Q_OBJECT

public:
    explicit QPlcProcesso(CProcessa *newProcesso,QObject *parent = 0);

signals:
    void finished();

public slots:
    void Scan();


private:
    void updateIp();


private:
    QString      ipAddress;
    QHostAddress ipEoloAddress;
    CProcessa    *Processa;
    QPlcServer   *Server;
    CSimula      Simula;
};

#endif // QPROCESSO_H


