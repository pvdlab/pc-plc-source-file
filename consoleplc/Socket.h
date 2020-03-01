#ifndef CSOCKET_H
#define CSOCKET_H

#include <QThread>
#include "StdAfx.h"
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "Ciclo.h"


class CTcpSockect : public QThread
{

public:
    explicit CTcpSockect(CCiclo *newProcessa,int  newClientSocket,QObject *parent = 0);
    void run();

private:
    std::string Ricevi();
    int Comunica(char *Buffer, int len);

private:
    BYTE         lastcomando;
    bool         NuovoMsg;
    bool         SpedisciMsg;
    bool         Connesso;
    bool         TrasmettiData;
    sockaddr_in  cli_addr;
    int          ClientSocket;
    char         Buffer[D_BUFFER];
    int          SockReadStato;
    int          SockWriteStato;
    int          SockType;
    struct sockaddr_in  From;
    int          FromLen;
    CCiclo       *Processa;
    char         ClienIp[INET_ADDRSTRLEN];
    int          BufferLen;

};



class CServer : public QThread
{

public:
    explicit CServer(CCiclo *newProcessa,QObject *parent = 0);
    void run();


private:
    int          Porta;
    bool         IsThread;
    bool         NuovoMsg;
    bool         SpedisciMsg;
    bool         Connesso;
    bool         TrasmettiData;
    int          ServerSocket;
    int          ClientSocket;
    char         Buffer[D_BUFFER];
    int          SockReadStato;
    int          SockWriteStato;
    int          SockType;
    struct sockaddr_in  From;
    int          FromLen;
    CCiclo      *Processa;
    char        ClienIp[INET_ADDRSTRLEN];

};

class CClient
{
public:
    explicit CClient(QString host, int   newPorta,QObject *parent = 0);
    virtual ~CClient();
    std::string Scrivi(char *Buffer, int len);
    bool  Connesso;

private:
    int          Porta;
    sockaddr_in  cli_addr;
    int          ClientSocket;
    int          SockType;
    struct sockaddr_in  From;
    int          FromLen;
    char         ClienIp[INET_ADDRSTRLEN];

};



#endif // CSOCKET_H
