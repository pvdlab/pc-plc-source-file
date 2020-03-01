#ifndef CTHREADCLIENT_H
#define CTHREADCLIENT_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <QMessageBox>
#include "NetPlc.h"

#define DEFAULT_PROTO            SOCK_STREAM //SOCK_DGRAM //SOCK_STREAM
#define MAX_PENDING_CONNECTS     4
#define INVALID_SOCKET           -1
#define SOCKET_ERROR             -1
#define BUFFER_SEND_SIZE         1024

namespace CClientMsg {
    enum Code {
        Data =1,
        Msg=2,
        ProcessoRecord=3,
        TempoOn=4,
        CaricaProcesso=5
    };
}

class CThreadClient : public QThread
{
    Q_OBJECT
public:
    explicit CThreadClient(QNetPlc *NewProcessa,   QString newHost, int   newPorta,QObject *parent = 0);
    void run();
    virtual ~CThreadClient();



signals:
    void Connesso(bool val);
    void StepSpediti(int, int);
    void Errore(QString msg);
    void nuovoMsg();
    void processoCaricato();
    void processoSpedito();
    void processiDisponibili(int count,QString str);
    void ConnesioneInterrotta();


private:
    void Main();
    std::string spedisciMsg(char *Buffer, int len);
    std::string spedisciMsg( int tipomsg,int Macchina,int secondomsg);
    std::string spedisciMsg(int tipomsg,int Macchina,int secondomsg, int Terzo);
    std::string spedisciMsg( int tipomsg,int Macchina,int secondomsg,int terzomsg, char *Buffer,int Buffer_len);
    char *Converti(std::string risposta);
    void ProcessoSpedisci(int IdMacchina);
    bool ProcessoGetMsg();
    bool IsUguale(std::string risposta,CMsg s_msg);
    void ElaboraMsg(char *Buffer);

private:
    int          Porta;
    QString      Host;
    QNetPlc      *Processa;
    sockaddr_in  cli_addr;
    int          ClientSocket;
    int          SockType;
    struct sockaddr_in  From;
    int          FromLen;
    char         ClienIp[INET_ADDRSTRLEN];
    bool         IsConnesso;
    char         Buffer[BUFFER_SEND_SIZE];
    int          SleepTime;
    int          TipoMessaggio;
    int          TipoComando;
    bool         ProcessoNonValido;
    bool         IsMsgNullo;
    bool         IsMsgTimeout;
    int          MsgNulloCount;
    int          startMsg;
    int          ProcessoRecord[2];
    bool         IsTempoOn;
    int          TipoMsgSpedito;
    bool         IsProcessiDisponibili;
    bool         IsConnesioneInterrotta;

};

#endif // CTHREADCLIENT_H
