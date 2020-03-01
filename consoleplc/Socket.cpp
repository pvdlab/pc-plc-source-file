#include "Socket.h"
#include "Test.h"


#define DEFAULT_PROTO            SOCK_STREAM //SOCK_DGRAM //SOCK_STREAM
#define MAX_PENDING_CONNECTS     4
#define INVALID_SOCKET           -1
#define SOCKET_ERROR             -1

CServer::CServer(CCiclo *newProcessa,QObject *parent) : QThread(parent)
{
    Processa=newProcessa;
    Porta=getPlcListenPort();
}

void CServer::run(){

    struct sockaddr_in  local_sin;
    socklen_t    clilen;
    struct sockaddr_in cli_addr;

    std::string str;
    str.append("Server Locole su porta ");
    str.append(intToString(Porta));
    MostraEvento(str,Color::FG_DEFAULT);

    SockType=SOCK_STREAM;
    ServerSocket = socket(AF_INET, SockType, 0);
    if(ServerSocket != INVALID_SOCKET)
    {
        local_sin.sin_family = AF_INET;
        local_sin.sin_port = htons (Porta);
        local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
        bind (ServerSocket,  (struct sockaddr *) &local_sin, sizeof (local_sin));
        listen (ServerSocket, MAX_PENDING_CONNECTS);
        struct sockaddr_in sin;
        socklen_t len = sizeof(sin);
        if (getsockname(ServerSocket, (struct sockaddr *)&sin, &len) == -1){
            std::string msg;
            msg.append(strerror(errno));
            MostraErrore(msg);
            return;
        }else{
            int porta=ntohs(sin.sin_port);
            if(porta!=Porta){
                std::string msg;
                msg.append("Porta Errata ");
                msg.append(intToString(ntohs(sin.sin_port)));
                MostraErrore(msg);
            }
        }
        do
        {
         clilen = sizeof(cli_addr);
         ClientSocket = accept(ServerSocket,
                             (struct sockaddr *) &cli_addr,
                             &clilen);
         std::string msg;
         msg.append("Connesso Client ");
         msg.append(inet_ntop(AF_INET, &(cli_addr.sin_addr), ClienIp, INET_ADDRSTRLEN));
         MostraEvento(msg,Color::FG_DEFAULT);
         if (ClientSocket == INVALID_SOCKET) {
           MostraErrore("Sock Non Valido");
           SockReadStato=0;
           break;
         }
         CTcpSockect *PlcSocket = new CTcpSockect(Processa,ClientSocket);
         PlcSocket->start();
         msleep(5);
        }while (ServerSocket != INVALID_SOCKET);

    }
    else
      MostraErrore("Errore Connessione");

    MostraEvento("Esco StartListen",Color::FG_DEFAULT);
}

CTcpSockect::CTcpSockect(CCiclo *newProcessa,int  newClientSocket,QObject *parent) : QThread(parent)
{
    Processa=newProcessa;
    ClientSocket=newClientSocket;

}

void CTcpSockect::run(){

    int len;
    BYTE m_Buffer[D_BUFFER];


    if (ClientSocket != INVALID_SOCKET )
    {
     Connesso=true;
     NuovoMsg=false;
     SpedisciMsg=false;
     while (1){
            memset(Buffer, 0, D_BUFFER);
            //SockReadStato = recv(ClientSocket , Buffer , D_BUFFER , 0) ;
            //ByteRicevuti=SockReadStato;
            /////
            std::string msg=Ricevi();
            int ByteRicevuti=msg.length();
            if(ByteRicevuti>D_BUFFER)
                ByteRicevuti=D_BUFFER;
            memcpy(Buffer,msg.c_str(),ByteRicevuti);
            SockReadStato=ByteRicevuti;
            /////

            if (SockReadStato  == SOCKET_ERROR ){
               MostraErrore("Errore Ricezione da ");// + QString::fromUtf8(ClienIp));
               shutdown (ClientSocket, 0x02);
               close(ClientSocket);
               Connesso=false;
               NuovoMsg=false;
               SpedisciMsg=false;
               return ;
            }
            else if (SockReadStato == 0) {
               MostraEvento("Client Disconnesso ",Color::FG_DEFAULT);// + QString::fromUtf8(ClienIp));
               shutdown (ClientSocket, 0x02);
               close(ClientSocket);
               Connesso=false;
               NuovoMsg=false;
               SpedisciMsg=false;
               return ;
            }
            else{
                 memcpy(m_Buffer,Buffer,D_BUFFER);
                 char *pBuffer=Buffer;
                 BYTE PrimoMsg=*pBuffer;
                 if(PrimoMsg==CTipoMessaggio::SetComando){
                    pBuffer+=2;
                    PrimoMsg=*pBuffer;
                    if((PrimoMsg!=CTipoMessaggio::GetProcMsg) & (lastcomando!=PrimoMsg) ){
                       MostraEvento(ComandoToString(Buffer),Color::FG_GREEN );
                       lastcomando=PrimoMsg;
                    }
                 }else if(PrimoMsg==CTipoMessaggio::Test){
                     ctest *pTest= new ctest(Processa,"power");
                     pTest->start();
                 }

                 len=Comunica(Buffer,D_BUFFER);
                 SockWriteStato = send(ClientSocket,Buffer,len,MSG_NOSIGNAL);
                 if (SockWriteStato == SOCKET_ERROR ){
                    MostraErrore("Errore In Scrittura da ");// + QString::fromUtf8(ClienIp));
                    shutdown (ClientSocket, 0x02);
                    close(ClientSocket);
                    return ;
                 }
               }
         msleep(5);
        }

    }
    return ;
}

int CTcpSockect::Comunica(char *Buffer, int len){

    BYTE PrimoMsg;
    char *pMsg=Buffer;
    PrimoMsg=*pMsg;
    Processa->pData[0].MessaggioRicevuto=PrimoMsg;
    Processa->BufferLen=BufferLen;
    switch (PrimoMsg){
        case CTipoMessaggio::Test:
           return len;
           break;
        case CTipoMessaggio::Manuale:
           return Processa->Macchina_Set(Buffer);
           break;
        case CTipoMessaggio::SetComando :
           return Processa->Macchina_SetComando(Buffer);
           break;
        case CTipoMessaggio::CaricaProcesso :
           return Processa->Processo_Load(++pMsg);
           break;
        case CTipoMessaggio::EliminaProcesso :
           return Processa->Processo_Elimina(++pMsg);
           break;
        case CTipoMessaggio::LoadDaFileProcesso :
           return Processa->Processo_LoadDaFile(pMsg);
           break;
        case CTipoMessaggio::GetData:
           return Processa->Macchina_GetNetData(pMsg);
           break;
        case CTipoMessaggio::GetStep:
           return Processa->Processo_GetStep(pMsg);
           break;
        case CTipoMessaggio::GetProcMsg:
           return Processa->Ciclo_GetMsg(pMsg);
           break;
        case CTipoMessaggio::GetProcRecord:
          return Processa->Processo_GetMsgRecord(pMsg);
          break;
        case CTipoMessaggio::SetSegnale:
          return Processa->Macchina_SetSegnale(pMsg);
          break;
        case CTipoMessaggio::GetOnTime:
          return Processa->Macchina_GetOnTime(pMsg);
          break;
        case CTipoMessaggio::CambiaTarget:
          return Processa->Macchina_TragetCambia(pMsg);
          break;
        case CTipoMessaggio::GetProcessiDisponibili:
          return Processa->Processo_GetMsgProcessiDisponibili(pMsg);
          break;
    }

    MostraEvento("TipoMessaggio Non Valido",Color::FG_RED);
    return Processa->Macchina_GetNetData(pMsg);

}

std::string CTcpSockect::Ricevi(){

    //trovato:https://stackoverflow.com/questions/36893807/setting-timeout-for-socket-recv-tcp
    std::string msg;
    int cut;
    while((cut = msg.find(FINE_MSG)) == -1)
    {
        timeval timeout = { 300, 0};
        fd_set in_set;

        FD_ZERO(&in_set);
        FD_SET(ClientSocket, &in_set);
        char tmpBuffer[300];
        select(ClientSocket + 1, &in_set, NULL, NULL, &timeout);

        if (FD_ISSET(ClientSocket, &in_set))
        {
            BufferLen = recv(ClientSocket, tmpBuffer, 300, 0);
            if (BufferLen <= 0) {
                //MostraErrore("Nulla è stato Ricevuto");
                break;
            }
            msg.append(tmpBuffer, BufferLen);
        }
        else{
            MostraErrore("Time Out Nessuna Risposta");
            break;
        }
    }

    if(cut){
       msg=msg.substr(0,cut+3);
    }
    return msg;


}

CClient::CClient(QString host, int   newPorta,QObject *parent)
{

    Porta=newPorta;
    struct hostent *hostnm;
    struct timeval  Timeout;
    Timeout.tv_sec = 10;
    Timeout.tv_usec = 0;
    fd_set fdset;

    hostnm = gethostbyname(host.toUtf8());
    cli_addr.sin_family      = AF_INET;
    cli_addr.sin_port        = htons(Porta);
    cli_addr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    if ((ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
       std::string msg;
       msg.append(strerror(errno));
       MostraErrore(msg);
       Connesso=false;
       return;
    }

    if(fcntl(ClientSocket, F_SETFL, O_NONBLOCK)<0){
       std::string msg;
       msg.append(strerror(errno));
       MostraErrore(msg);
       Connesso=false;
       return;
    }

    connect(ClientSocket, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

    FD_ZERO(&fdset);
    FD_SET(ClientSocket, &fdset);

    if (select(ClientSocket + 1, NULL, &fdset, NULL, &Timeout) == 1){
       int so_error;
       socklen_t len = sizeof so_error;

       if(getsockopt(ClientSocket, SOL_SOCKET, SO_ERROR, &so_error, &len)<0){
           std::string msg;
           msg.append(strerror(errno));
           MostraErrore(msg);
           Connesso=false;
           return;
       }

       if (so_error == 0) {
           printf(" is open\n");
           Connesso=true;
       }else{
           MostraErrore("Errore di connessione");
           Connesso=false;
       }
    }else{
       Connesso=false;
       MostraErrore("Connessione TimeOut");
       close(ClientSocket);
    }

}

CClient::~CClient()
{
    shutdown (ClientSocket, 0x02);
    close(ClientSocket);
}

std::string CClient::Scrivi(char *Buffer, int len){

    if (send(ClientSocket, (char *)Buffer, len, MSG_NOSIGNAL) < 0){
        std::string msg;
        msg.append(strerror(errno));
        MostraErrore(msg);
    }

    //trovato:https://stackoverflow.com/questions/36893807/setting-timeout-for-socket-recv-tcp
    std::string msg;
    int cut;
    while((cut = msg.find(FINE_MSG)) == -1)
    {
        timeval timeout = { 5, 0};
        fd_set in_set;

        FD_ZERO(&in_set);
        FD_SET(ClientSocket, &in_set);
        char tmpBuffer[300];

        // select the set
        select(ClientSocket + 1, &in_set, NULL, NULL, &timeout);

        if (FD_ISSET(ClientSocket, &in_set))
        {
            int numBytes = recv(ClientSocket, tmpBuffer, 300, 0);
            if (numBytes <= 0)
            {
                //MostraErrore("Nulla è stato Ricevuto");
                break;
            }

            msg.append(tmpBuffer, numBytes);
        }
        else
        {
            MostraErrore("Time Out Nessuna Risposta");
            break;
        }
    }

    return msg;

}


