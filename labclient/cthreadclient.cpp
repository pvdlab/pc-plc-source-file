#include "cthreadclient.h"
#include "StdAfx.h"
#include <QMessageBox>
#include <QDebug>
#include "Segnale.h"
#include <signal.h>
#include <poll.h>

int m_connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len){

    connect ( __fd,  __addr,  __len);

}


#define SEND_SIZE 100



CThreadClient::CThreadClient(QNetPlc *NewProcessa,   QString newHost, int   newPorta,QObject *parent) : QThread(parent)
{

    Porta=newPorta;
    Processa=NewProcessa;
    Host=newHost;
    IsMsgNullo=false;
    IsMsgTimeout=false;
    MsgNulloCount=0;
    startMsg=0;
    ProcessoRecord[0]=0;
    ProcessoRecord[1]=0;
    IsTempoOn=false;
    IsProcessiDisponibili=false;
    IsConnesioneInterrotta=false;

}

//#define ESCLUDI_GET_RECORD

void CThreadClient::run(){

    signal(SIGPIPE, SIG_IGN);
    struct hostent *hostnm;
    struct timeval  Timeout;
    Timeout.tv_sec = 10;
    Timeout.tv_usec = 0;
    fd_set fdset;


    hostnm = gethostbyname(Host.toUtf8());
    if(hostnm==NULL){
        emit Errore("Host Non Valido " + Host + "\n Impossibile Connettersi");
        IsConnesso=false;
        return;
    }
    cli_addr.sin_family      = AF_INET;
    cli_addr.sin_port        = htons(Porta);
    cli_addr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    if ((ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        emit Errore(strerror(errno));
        IsConnesso=false;
        return;
    }

    if(fcntl(ClientSocket, F_SETFL, O_NONBLOCK)<0){
        goto Esci;
    }


    m_connect(ClientSocket, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

    FD_ZERO(&fdset);
    FD_SET(ClientSocket, &fdset);

    if (select(ClientSocket + 1, NULL, &fdset, NULL, &Timeout) == 1){
        int so_error;
        socklen_t len = sizeof so_error;

        if(getsockopt(ClientSocket, SOL_SOCKET, SO_ERROR, &so_error, &len)<0){
            goto Esci;
        }

        if (so_error == 0) {
            Processa->IsConnesso=true;
            emit Connesso(true);
        }else{
            goto Esci;
        }
    }else{
        goto Esci;
    }

    if(Processa->IsLocale)
       SleepTime=100;
    else
       SleepTime=200;
    if(Processa->IsConnesso){
        for(;;){
            bool IsProcessoRecord=false;
            for(int i=0; i< Processa->CamereCount; i++){
                if(Processa->pData[i].File.ProcessoRecord!=ProcessoRecord[i])
                    IsProcessoRecord=true;
            }
            if(!Processa->IsVisibile){
                std::string risposta;
                risposta=spedisciMsg(CTipoMessaggio::GetData,0,0);
                char *pBuffer=Converti(risposta);
                if(Processa->IsMsgData(pBuffer)){
                   Processa->SetNetData(pBuffer);
                }
            }else{
                std::string risposta;
                if(Processa->msgPerPlc.count()){
                    CMsg pMsg=Processa->msgPerPlc.at(0);
                    TipoComando=pMsg.Secondo;
                    TipoMessaggio=pMsg.Primo;
                    Processa->TipoComando=TipoComando;
                    if(TipoMessaggio==CTipoMessaggio::CaricaProcesso ){
                       Processa->msgPerPlc.clear();
                       ProcessoSpedisci(pMsg.Macchina);
                    }else{
                        if(pMsg.Tentativi>5){
                           Processa->msgPerPlc.removeAt(0);
                        }else{
                           pMsg.Tentativi++;
                           Processa->msgPerPlc.replace(0,pMsg);
                           risposta=spedisciMsg(pMsg.Primo,pMsg.Macchina,pMsg.Secondo,pMsg.Terzo, pMsg.pMsg.data(),pMsg.pMsg.size());
                           if(!IsMsgNullo){
                               if(IsUguale(risposta,pMsg)){
                                   char *pBuffer=Converti(risposta);
                                   ElaboraMsg(pBuffer);
                                   Processa->msgPerPlc.removeAt(0);
                               }
                           }
                        }
                    }
                }else{
                   if(startMsg>Processa->pData->EventoProcessoCount){
                       if(Processa->VersioneRidotta)
                           startMsg=Processa->pData->EventoProcessoCount;
                       else
                           startMsg=0;
                   }
                   if(!IsTempoOn ){
                       risposta=spedisciMsg(CTipoMessaggio::GetOnTime,0,1);
                       char *pBuffer=Converti(risposta);
                       Processa->SetOnTime(pBuffer);
                       IsTempoOn=true;
                       TipoMsgSpedito=CClientMsg::TempoOn;
                   }else if(!IsProcessiDisponibili){
                       if(Processa->VersioneRidotta)
                          IsProcessiDisponibili=true;
                       else{
                          risposta=spedisciMsg(CTipoMessaggio::GetProcessiDisponibili,0,0);
                          IsProcessiDisponibili=true;
                          char *pBuffer=Converti(risposta);
                          pBuffer++;
                          int count=(int)StrByte(&pBuffer);
                          risposta.clear();
                          risposta.append(pBuffer);
                          emit processiDisponibili(count,risposta.c_str());
                       }
                   }else if(startMsg<Processa->pData->EventoProcessoCount){
                      risposta=spedisciMsg(CTipoMessaggio::GetProcMsg,0,0,startMsg++);
                      TipoMsgSpedito=CClientMsg::Msg;
                   }else if(IsProcessoRecord) {
                       for(int i=0; i< Processa->CamereCount; i++){
                           if(Processa->pData[i].File.ProcessoRecord!=ProcessoRecord[i]){
                             if(Processa->VersioneRidotta | Processa->CamereCount >1)
                                ProcessoRecord[i]=Processa->pData[i].File.ProcessoRecord;
                            else{
                              //qDebug()  << Processa->StepIndex;
                              risposta=spedisciMsg(CTipoMessaggio::GetProcRecord,i,0,ProcessoRecord[i]++);
                              char *pBuffer=Converti(risposta);
                              if(ProcessoRecord[i]==1)
                                  Processa->Processo_UnLoad();
                              pBuffer+=3;
                              Processa->Processo_Load(pBuffer);
                              int num=Processa->pData[i].File.ProcessoRecord;
                              if(num==ProcessoRecord[i]){
                                  emit processoCaricato();
                              }
                              risposta=spedisciMsg(CTipoMessaggio::GetData,0,0);
                              TipoMsgSpedito=CClientMsg::ProcessoRecord;
                           }
                          }
                       }


                   }else{
                      risposta=spedisciMsg(CTipoMessaggio::GetData,0,0);
                   }
                }
                char *pBuffer=Converti(risposta);
                if(Processa->IsMsgData(pBuffer)){
                   Processa->SetNetData(pBuffer);
                }

                if(IsMsgNullo | IsMsgTimeout){
                    if(TipoMsgSpedito==CClientMsg::Msg){
                       startMsg=Processa->pData->EventoProcessoCount;
                       IsMsgNullo=false;
                       IsMsgTimeout=false;
                       emit Errore("Impossibile Caricare Messaggi Plc");
                    }else if(TipoMsgSpedito==CClientMsg::TempoOn){
                       IsMsgNullo=false;
                       IsMsgTimeout=false;
                       IsTempoOn=true;
                       emit Errore("Impossibile Caricare TempoOn");
                    }else if(TipoMsgSpedito==CClientMsg::ProcessoRecord){
                       IsMsgNullo=false;
                       IsMsgTimeout=false;
                       //Processa->pData->File.ProcessoRecord=ProcessoRecord;
                       emit Errore("Impossibile Caricare ProcessoRecord");
                    }else
                       goto Esci;
                }
                if(IsConnesioneInterrotta){
                    emit ConnesioneInterrotta();
                    goto Esci;
                }
                msleep(SleepTime);
            }
         }

    }

Esci:
    Processa->msgPerPlc.clear();
    Processa->IsConnesso=false;
    msleep(SleepTime);
    shutdown (ClientSocket, 0x02);
    close(ClientSocket);
    return;

}


CThreadClient::~CThreadClient()
{
    shutdown(ClientSocket, 0x02);
    close(ClientSocket);
}


void CThreadClient::ElaboraMsg(char *Buffer){

    char  *pBuffer=Buffer;
    BYTE  pTipoMessaggio=*pBuffer;

    if(pTipoMessaggio==CTipoMessaggio::GetOnTime)
        Processa->SetOnTime(Buffer);
    else if(pTipoMessaggio==CTipoMessaggio::CambiaTarget)
        IsTempoOn=false;

}

std::string CThreadClient::spedisciMsg( int tipomsg,int Macchina,int secondomsg){

    int len=0;
    char buffer[SEND_SIZE];
    char *pBuffer=buffer;
    memset(pBuffer,0,SEND_SIZE);
    len=0;
    pBuffer=ByteStr(pBuffer,tipomsg ,&len);
    pBuffer=ByteStr(pBuffer,Macchina ,&len);
    pBuffer=ByteStr(pBuffer,secondomsg ,&len);
    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);
    return spedisciMsg(buffer,len);

}


std::string CThreadClient::spedisciMsg(int tipomsg,int Macchina,int secondomsg, int Terzo){

    int len=0;
    char buffer[SEND_SIZE];
    char *pBuffer=buffer;
    memset(pBuffer,0,SEND_SIZE);
    len=0;
    pBuffer=ByteStr(pBuffer,tipomsg ,&len);
    pBuffer=ByteStr(pBuffer,Macchina ,&len);
    pBuffer=ByteStr(pBuffer,secondomsg ,&len);
    pBuffer=WORDStr(pBuffer,Terzo,&len);
    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);
    return spedisciMsg(buffer,len);

}

std::string CThreadClient::spedisciMsg( int tipomsg,int Macchina,int secondomsg,int terzomsg, char *Buffer,int Buffer_len){

    int len=0;
    char buffer[SEND_SIZE];
    char *pBuffer=buffer;
    memset(pBuffer,0,SEND_SIZE);
    len=0;
    pBuffer=ByteStr(pBuffer,tipomsg ,&len);
    pBuffer=ByteStr(pBuffer,Macchina ,&len);
    pBuffer=ByteStr(pBuffer,secondomsg ,&len);
    pBuffer=WORDStr(pBuffer,terzomsg ,&len);
    if(Buffer!=NULL){
        if(Buffer_len>0){
           memcpy(pBuffer,Buffer,Buffer_len);
           len+=Buffer_len;
           pBuffer+=Buffer_len;
        }
    }
    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);
    return spedisciMsg(buffer,len);

}

std::string CThreadClient::spedisciMsg(char *Buffer, int len){

    if (send(ClientSocket, Buffer, len, MSG_NOSIGNAL) < 0){
        //emit Errore(strerror(errno));
        IsConnesioneInterrotta=true;
        return "";
    }

    //trovato:https://stackoverflow.com/questions/36893807/setting-timeout-for-socket-recv-tcp
    std::string msg;
    int cut;
    while((cut = msg.find(FINE_MSG)) == -1)
    {
        timeval timeout = { 6, 0};
        fd_set in_set;

        FD_ZERO(&in_set);
        FD_SET(ClientSocket, &in_set);
        char tmpBuffer[D_BUFFER];

        // select the set
        int cnt = select(ClientSocket + 1, &in_set, NULL, NULL, &timeout);

        if (FD_ISSET(ClientSocket, &in_set))
        {
            int numBytes = recv(ClientSocket, tmpBuffer, D_BUFFER, 0);
            if (numBytes <= 0)
            {
                if(TipoMessaggio!=CTipoMessaggio::GetData){
                    IsMsgNullo=true;
                    break;
                }
            }
            msg.append(tmpBuffer, numBytes);
        }
        else
        {
            if(TipoMessaggio!=CTipoMessaggio::GetData){
                IsMsgTimeout=true;
            }
            break;
        }
    }

    if(msg.length()==0)
        return FINE_MSG;
    else
        return msg;

}

bool CThreadClient::ProcessoGetMsg(){

    if(Processa->pData->EventoProcessoCount>MAX_MSG)
        return false;

    for(int i=startMsg; i< Processa->pData->EventoProcessoCount; i++){
        std::string risposta=spedisciMsg(CTipoMessaggio::GetProcMsg,0,0,i);
        if(IsMsgNullo | IsMsgTimeout)
            break;
        char *pBuffer=Converti(risposta);
        Processa->SetNetData(pBuffer);
        msleep(10);
    }
    startMsg=Processa->pData->EventoProcessoCount;

    return true;

}


void CThreadClient::ProcessoSpedisci(int IdMacchina){

    QString StrErrore;

    for(int i=0; i<Processa->pListaSegnali->count();i++){
        CIOSegnale *pSegnale=Processa->pListaSegnali->at(i);
        pSegnale->Spedito=false;
    }
    Processa->Spedito=false;
    std::string risposta;
    QByteArray buffer(D_BUFFER,0);
    BYTE *pBuffer=buffer.data();
    Processa->pData[IdMacchina].File.ProcessoId=Processa->ProcessoId;
    int len=Processa->Processo_GetMsgSendStep(pBuffer,IdMacchina,Processa->ProcessoId,
                                           Processa->ProcessoNome,
                                           Processa->Pc_Step,Processa->Pc_StepCount);
    buffer.resize(len);
    risposta=spedisciMsg(buffer.data(),len);
    QByteArray msg(risposta.c_str(), risposta.length());
    int step=1;
    bool spedito=false;

    while(!spedito){
        if(IsMsgNullo | IsMsgTimeout )
            return;
        BYTE *pBuffer=msg.data();
        pBuffer+=2;
        if(*pBuffer=='S'){
            if(Processa->ControllaRecivedStep(msg)){
                Processa->Spedito=true;
                msleep(SleepTime);
            }
            else{
                StrErrore="Impossibile Caricare Processo";
                goto ProcessoErrore;

            }
        }else{
            pBuffer+=4;
            int CompTipo=StrByte(&pBuffer);
            int CompIndex=StrByte(&pBuffer);
            Processa->pData[IdMacchina].ComStepLoad=CompTipo*10;
            Processa->pData[IdMacchina].ComStepLoad+=CompIndex;
            for(int i=0; i<Processa->pListaSegnali->count();i++){
                CIOSegnale *pSegnale=Processa->pListaSegnali->at(i);
                QString comptipo= QString::number(CompTipo) + QString::number(CompIndex);
                if(pSegnale->TipoComp==comptipo.toInt()){
                    if(Processa->ControllaRecivedCompStep(msg,pSegnale)){
                        pSegnale->Spedito=true;
                        break;
                    }
                    else{
                        StrErrore="Impossibile Caricare Step " + QString::number(pSegnale->TipoComp);
                        goto ProcessoErrore;
                    }
                }
            }
        }
        spedito= true;
        for(int i=0; i<Processa->pListaSegnali->count();i++){
           CIOSegnale *pSegnale=Processa->pListaSegnali->at(i);
           if(!pSegnale->Spedito){
               QByteArray buffer(D_BUFFER,0);
               BYTE *pBuffer=buffer.data();
               int len=Processa->Processo_GetMsgSendCompStep(pBuffer,IdMacchina,pSegnale,Processa->Pc_StepCount);
               buffer.resize(len);
               std::string risposta=spedisciMsg(buffer.data(),len);
               msg=QByteArray(risposta.c_str(), risposta.length());
               spedito= false;
               step++;
               emit StepSpediti(step,Processa->pListaSegnali->count()+1);
               msleep(SleepTime);
               break;
           }
        }
        msleep(5);
    }
    IsTempoOn=false;
    ProcessoNonValido=false;
    Processa->pData[IdMacchina].File.ProcessoId=0;

    for(int i=0; i<Processa->pListaSegnali->count();i++){
        CIOSegnale *pSegnale=Processa->pListaSegnali->at(i);
        pSegnale->Spedito=false;
    }

    emit processoSpedito();
    return;

ProcessoErrore:
    Processa->Spedito=false;
    ProcessoNonValido=true;
    for(int i=0; i<Processa->pListaSegnali->count();i++){
        CIOSegnale *pSegnale=Processa->pListaSegnali->at(i);
        pSegnale->Spedito=false;
    }
    buffer=QByteArray(D_BUFFER,0);
    pBuffer=buffer.data();
    //Processa->pData[IdMacchina].File.ProcessoId=Processa->ProcessoId;
    len=Processa->Processo_GetMsgElimaProcesso(pBuffer,IdMacchina,Processa->ProcessoId,
                                           Processa->ProcessoNome);
    buffer.resize(len);
    risposta=spedisciMsg(buffer.data(),len);
    emit Errore(StrErrore);

}

char *CThreadClient::Converti(std::string risposta){

    int len=risposta.length();
    if(len>1000)
        len=1000;
    char *pBuffer = Buffer;
    memcpy(pBuffer,risposta.data(),len);

    return pBuffer;
}

bool CThreadClient::IsUguale(std::string risposta,CMsg s_msg){

    char *pBuffer=QByteArray(risposta.c_str(), risposta.length()).data();
    int primo=*pBuffer++;
    int secondo=*pBuffer++;

    bool uguale=true;
    if(primo!=s_msg.Primo)
        uguale=false;
    if(secondo!=s_msg.Secondo)
        uguale=false;

  /*  if(s_msg.pMsg.size()){
       char *vecchio=s_msg.pMsg.data();
       pBuffer=QByteArray(risposta.c_str(), risposta.length()).data();
       pBuffer+=2;
       int len=s_msg.pMsg.size()-1;
       for(int i=0; i<len;i++){
           if(*pBuffer++!=*vecchio++)
               uguale=false;
       }
    }*/

    return uguale;
}





