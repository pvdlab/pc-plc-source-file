#ifndef QPLCSERVER_H
#define QPLCSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include "Processa.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class QPlcServer
: public QObject

{
   Q_OBJECT
public:
    explicit QPlcServer(CProcessa *newProcesso,QObject *parent = Q_NULLPTR);
    virtual ~QPlcServer();
    void connetti();

signals:
    void evento(QString msg);
    void noSingleApp();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    CProcessa *Processo;
    int lastcomando;

};


#endif
