/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplcserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QNetworkInterface>


QT_USE_NAMESPACE



QPlcServer::QPlcServer(CProcessa *newProcesso,QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(Q_NULLPTR),
    lastcomando(0),
    m_clients()
{
    lastcomando=0;
    Processo=newProcesso;
}

void QPlcServer::connetti(){

    QString Indirizzo;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            Indirizzo = ipAddressesList.at(i).toString();
            break;

        }
    }

    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("SSL Echo Server"),
                                              QWebSocketServer::SecureMode,
                                              this);
    QSslConfiguration sslConfiguration;
    QFile certFile(QStringLiteral(":/localhost.cert"));
    QFile keyFile(QStringLiteral(":/localhost.key"));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);

    if (m_pWebSocketServer->listen(QHostAddress::Any, getPlcListenPort()))
    {
        std::string str="SSL Echo Server listening on Ip Porta ";
        str.append(intToString(getPlcListenPort()));

        MostraEvento( str ); //+ Indirizzo +  " Port " + QString::number(PLC_LISTEN_PORTA));
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &QPlcServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                this, &QPlcServer::onSslErrors);
    }else{
        if(m_pWebSocketServer->isListening()){
            std::string str="STO ANCORA LISTE SU PORTA ";
            str.append(intToString(getPlcListenPort()));
            MostraEvento(str);
        }else{
            std::string str="IMPOSSIBILE FAR PARTIRE IL SERVER SU ";
            str.append(intToString(getPlcListenPort()));
            MostraEvento(str);
            emit noSingleApp();
        }


    }

}

QPlcServer::~QPlcServer()
{

    //m_pWebSocketServer->destroyed();
    qDeleteAll(m_clients.begin(), m_clients.end());
    m_clients.clear();
    m_pWebSocketServer->close();
    MostraEvento("Chiudo  Sochet");

}


void QPlcServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    MostraEvento("Client connected: ");// + pSocket->peerAddress().toString() );

    connect(pSocket, &QWebSocket::textMessageReceived, this, &QPlcServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &QPlcServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &QPlcServer::socketDisconnected);
    //connect(pSocket, &QWebSocket::pong, this, &SslEchoServer::processPong);
    if(!pSocket){
       m_clients << pSocket;
    }

}

void QPlcServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        pClient->sendTextMessage(message);
    }
}

void QPlcServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        if(message.length()>D_BUFFER)
            return;

        BYTE Buffer[D_BUFFER];
        char *pBuffer=Buffer;
        uint len;
        memset(pBuffer,'\0',D_BUFFER);
        memcpy(pBuffer,message.data(),message.length());
        BYTE PrimoMsg=*pBuffer;
        if(PrimoMsg!=TipoMessaggio_GetData   && PrimoMsg!=0){
           pBuffer++;
           PrimoMsg=*pBuffer;
           //if(lastcomando==Processo->pData.ComandoEseguito)
           //    lastcomando=0;
           if(lastcomando!=PrimoMsg){
              ;;//MostraEvento(Processo->ComandoToString(message.data()) );
           }
           lastcomando=PrimoMsg;
        }
        len=(uint)Processo->setMessaggio(Buffer,message.length());
        QByteArray msgout;
        msgout.setRawData(Buffer,len);

        pClient->sendBinaryMessage(msgout);
    }
}



void QPlcServer::socketDisconnected()
{

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        MostraEvento("Client disconnected " );//+ pClient->peerAddress().toString()  );
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void QPlcServer::onSslErrors(const QList<QSslError> &)
{
    MostraEvento("Ssl errors occurred");
}







