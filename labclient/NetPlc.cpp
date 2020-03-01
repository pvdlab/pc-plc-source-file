#include "NetPlc.h"
#include <QDebug>
#include <QDir>

QNetPlc::QNetPlc(int newtipoapp,int newTipoImpianto,int argc, char *argv[],QObject *parent) :  CProcesso( newtipoapp,newTipoImpianto,argc, argv) , QObject(parent)
{

  ClientIsConnect=false;
  IsConnesso=false;
  IsVisibile=false;
  Pc_StepCount=0;


  Pc_Gas = new CIOSegnale[GasCount];
  Pc_Sorgenti = new CIOSegnale[SorgenteCount];
  Pc_SchermoTurbo = new CIOSegnale[CamereCount];
  pTarget = new CTarget[SorgenteCount];

  for(int i=0;i< GasCount;i++){
      Pc_Gas[i] =*Segnale(GGas[i].SetG->Descrizione(),GGas[i].SetG->Slot(),GGas[i].SetG->Point(),
                         GGas[i].SetG->FondoScala(),GGas[i].SetG->Fattore(),GGas[i].SetG->SecondoFattore());
      Pc_Gas[i].TipoComp=GGas[i].SetG->TipoComp;
      Pc_Gas[i].PenColor=GGas[i].SetG->PenColor;
      Pc_Gas[i].PenWidth=GGas[i].SetG->PenWidth;
  }

  for(int i=0;i< SorgenteCount;i++){
      Pc_Sorgenti[i] =*Segnale(Bias->Sorgente[i].SetI->Descrizione(),Bias->Sorgente[i].SetI->Slot(),Bias->Sorgente[i].SetI->Point(),
                         Bias->Sorgente[i].SetI->FondoScala(),Bias->Sorgente[i].SetI->Fattore(),Bias->Sorgente[i].SetI->SecondoFattore());
      Pc_Sorgenti[i].TipoComp=Bias->Sorgente[i].SetI->TipoComp;
      Pc_Sorgenti[i].PenColor=Bias->Sorgente[i].SetI->PenColor;
      Pc_Sorgenti[i].PenWidth=Bias->Sorgente[i].SetI->PenWidth;
  }

  Pc_Bias=Segnale(Bias->SetV->Descrizione(),Bias->SetV->Slot(),Bias->SetV->Point(),
                  Bias->SetV->FondoScala(),Bias->SetV->Fattore(),Bias->SetV->SecondoFattore());
  Pc_Bias->TipoComp=Bias->SetV->TipoComp;
  Pc_Bias->PenColor=Bias->SetV->PenColor;
  Pc_Bias->PenWidth=Bias->SetV->PenWidth;

  for(int i=0;i<CamereCount;i++){
      CIOSegnale *pSegnale=Vuoto[i].Camera.SchermoTurbo;
      if(pSegnale!=NULL){
          Pc_SchermoTurbo[i]=*Segnale(pSegnale->Descrizione(),
                                  pSegnale->Slot(),pSegnale->Point(),
                                  pSegnale->FondoScala(),
                                  pSegnale->Fattore(),
                                  pSegnale->SecondoFattore());
          Pc_SchermoTurbo[i].TipoComp=pSegnale->TipoComp;
          Pc_SchermoTurbo[i].PenColor=pSegnale->PenColor;
          Pc_SchermoTurbo[i].PenWidth=pSegnale->PenWidth;
          Pc_SchermoTurbo[i].ForzaToStep();
      }else{
          Pc_SchermoTurbo[i]=NULL;
      }

  }



}

void  QNetPlc::SetOnTime(char *Buffer)
{

    char  *pBuffer=Buffer;
    pBuffer++;
    setSimula(StrByte(&pBuffer));
    Vuoto->Turbo.OnTime(StrDWORD(&pBuffer));
    Vuoto->Meccanica.OnTime(StrDWORD(&pBuffer));
    for(int i=0; i< SorgenteCount;i++){
        double Tempo=StrDouble(&pBuffer);
        Bias->Sorgente[i].Catodo->OnTime(Tempo);
    }for(int i=0; i< SorgenteCount;i++)
        Bias->Sorgente[i].Catodo->TrigCount(StrWORD(&pBuffer));

}

void  QNetPlc::AzzeraNetData(){
    DataType *ppData=pData;
    memset(ppData,0,sizeof(DataType));
}

WORD QNetPlc::GetImpianto(char *Buffer){

    char *pBuffer=Buffer;
    StrByte(&pBuffer);
    return StrWORD(&pBuffer);

}

void  QNetPlc::SetNetData(char *Buffer)
{
  int  intComando;
  BYTE byteComando;

  StrByte(&Buffer);
  Impianto=StrWORD(&Buffer);
  setImpianto(Impianto);
  StrByte(&Buffer);
  ScanTime= StrByte(&Buffer);
  pData[0].Simula=StrByte(&Buffer);
  setSimula(pData[0].Simula);

  if(!IsVisibile ) //| !pData[i].IsInit
      return;

  for(int i=0;i<CamereCount;i++){
      IdVuoto=i;

      byteComando=StrByte(&Buffer);
      if(pData[i].StatoMacchina!=byteComando){
        pData[i].StatoMacchina=byteComando;
        emit StatoMacchina();
      }
      byteComando=StrByte(&Buffer);
      if(pData[i].StatoVuoto!=byteComando){
        pData[i].StatoVuoto=byteComando;
        emit StatoVuoto(byteComando);
      }

      pData[i].EnabledVenting=StrByte(&Buffer);

      byteComando=StrByte(&Buffer);
      if(pData[i].Comando!=byteComando){
         pData[i].Comando=byteComando;
         emit ComandoAttivato(byteComando,i);
      }

      byteComando=StrByte(&Buffer);
      if(pData[i].ComandoAttivato!=byteComando){
         pData[i].ComandoAttivato=byteComando;
         emit ComandoAttivato(byteComando,i);
      }
      byteComando=StrByte(&Buffer);
      if(pData[i].ComandoRicevuto!=byteComando){
         pData[i].ComandoRicevuto=byteComando;
         emit ComandoRicevuto(byteComando,i);
      }
      byteComando=StrByte(&Buffer);
      if(pData[i].ComandoEseguito!=byteComando){
         pData[i].ComandoEseguito=byteComando;
         emit ComandoEseguito(byteComando,i);
      }
      byteComando=StrByte(&Buffer);
      if(pData[i].MessaggioRicevuto!=byteComando){
         pData[i].MessaggioRicevuto=byteComando;
         emit MessaggioRicevuto(byteComando);
      }
      pData[i].MessaggioLen=StrByte(&Buffer);

      byteComando=StrByte(&Buffer);
      if(byteComando!=pData[i].SorgentiOn){
          pData[i].SorgentiOn=byteComando;
          for(int j=0;j<SorgenteCount;j++){
            bool isEnabled=GetBit(pData[i].SorgentiOn,j);
            Bias[i].Sorgente[j].Enabled=isEnabled ;
          }
      }

      EventTime=StrWORD(&Buffer);
      TimeEventStop=StrWORD(&Buffer);
      MainTimer=StrDWORD(&Buffer);
      Time=StrWORD(&Buffer);
      pData[i].TimeEvent=EventTime;
      pData[i].TimeEventStop=TimeEventStop;

      int tempo=StrWORD(&Buffer);
      if(TimeStop!=tempo){
         if(tempo-TimeStop>0 && pData[i].ProcessoIsOn)
           ;// emit AddTempo(tempo-TimeStop);
        TimeStop=tempo;
      }

      pData[i].Time=Time;
      pData[i].TimeStop=TimeStop;

      StepTime=StrWORD(&Buffer);
      StepTimeStop=StrWORD(&Buffer);

      pData[i].StepTime=StepTime;
      pData[i].StepTimeStop=StepTimeStop;

      pData[i].TimeStopVuoto=StrDWORD(&Buffer);

      int CTipoStep=StrByte(&Buffer);
      if(pData[i].TipoStep!=CTipoStep){
          pData[i].TipoStep=CTipoStep;
          emit CambiaCTipoStep(CTipoStep);
      }

      pData[i].StepCount=StrByte(&Buffer);
      pData[i].StepIndex=StrByte(&Buffer);
      pData[i].RepitCount=StrByte(&Buffer);
      pData[i].Repit=StrByte(&Buffer);
      Vuoto[i].Resistenza->Potenza(StrWORD(&Buffer));
      Vuoto[i].Resistenza->TempSet(StrWORD(&Buffer));

      intComando=StrWORD(&Buffer);
      if(intComando!=pData[i].File.ProcessoId ){
        pData[i].File.ProcessoId=intComando;
        if(VersioneRidotta){
           cercaFileProcesso(i);
        }
        AddMsg(CTipoMessaggio::GetOnTime,i,1);
        emit Processo(intComando,i);
      }

      byteComando=StrByte(&Buffer);
      if(byteComando!=pData[i].ComStepLoad){
        pData[i].ComStepLoad=byteComando;
        emit CompStepLoad(byteComando);
      }
      pData[i].File.Ciclo=StrDWORD(&Buffer);
      pData[i].File.ProcessoRecord=StrByte(&Buffer);//
      byteComando=StrByte(&Buffer);
      if(byteComando!=pData[i].ProcessoIsOn ){
        if(byteComando){
           pData[i].ProcessoIsOn=byteComando;
           IdPro=i;
           emit ProcessoIsOn(i);
        }
        if(!byteComando && pData[i].ProcessoIsOn){
           pData[i].ProcessoIsOn=byteComando;
           IdPro=0;
           emit ProcessoIsOff(i);
           }
      }

      intComando=StrWORD(&Buffer);
      if(intComando!=pData[i].ErrCount){
        pData[i].ErrCount=intComando;
        emit nuovoErrore(intComando);
      }
      pData[i].ErrPreProcesso=StrByte(&Buffer);
      pData[i].ErrProcesso=StrByte(&Buffer);

      intComando=StrWORD(&Buffer);
      if(intComando!=pData[i].EventoProcessoCount){
        if(intComando>=MAX_MSG){
           intComando=0;
        }else{
            pData[i].EventoProcessoCount=intComando;
            if(pData[i].ProcessoIsOn & (pData[i].EventoProcessoCount!=0)){
            }
        }

      }
      pData[i].EventoProcessoDi=StrWORD(&Buffer);
      pData[i].RecordCount=StrWORD(&Buffer);

      pData[i].ComStepLoad=StrByte(&Buffer);
      byteComando=StrByte(&Buffer);
      if((byteComando!=pData[i].ProcessoCorrotto) & byteComando){
        emit ProcessoCorrotto(pData[i].File.ProcessoId,pData[i].ComStepLoad);
      }
      pData[i].ProcessoCorrotto=byteComando;
      pData[i].ProcessoIsEnabled=StrByte(&Buffer);

      byteComando=StrByte(&Buffer);
      if(byteComando!=MacchinaInManuale){
         MacchinaInManuale=byteComando;
         emit MacchinainInManuale(MacchinaInManuale);
      }


  }

  SetPlcSegnali(&Buffer);
  char *pErrore=pBufferErrore;
  if(pErrore!=NULL){
      memcpy(pBufferErrore,Buffer,ErrSize);
  }

  Buffer+=ErrSize;

  if(pData[0].MessaggioLen){
      char msg[D_MSG];
    char *pmsg=msg;
    memcpy(pmsg,Buffer,__min(pData[0].MessaggioLen,D_MSG));
    QString str=msg;
    emit MsgRicevuti(pData[0].EventoProcessoDi+1, pData[0].EventoProcessoCount-1, str);
  }

  emit NuovaData();


}


bool QNetPlc::cercaFileProcesso(int IdMacchina){

    QString path = QDir::homePath() + "/run" + PLC_PROCESSO_PATH + "/";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*_*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()) {
        int pos=dirFile.indexOf("_");
        if(pData[IdMacchina].File.ProcessoId==dirFile.left(pos).toInt()){
            dirFile=dirFile.mid(pos+1,dirFile.length());
            int pos=dirFile.indexOf(".");
            dirFile=dirFile.left(pos);
            char *pBuffer=pData[IdMacchina].File.ProcessoNome;
            memcpy(pBuffer,dirFile.toUtf8().data(),dirFile.length());
            break;
        }


    }

    return true;
}


void QNetPlc::AddMsg(int Primo,int Macchina, int Secondo){

    if(Primo==CTipoMessaggio::GetData)
        return;
    CMsg pMsg ;
    pMsg.Primo=Primo;
    pMsg.Macchina=Macchina;
    pMsg.Secondo=Secondo;
    pMsg.pMsg="";
    pMsg.Tentativi=0;

    for(int i=0; i<msgPerPlc.count() ;i++){
        if((pMsg.Primo==msgPerPlc.at(i).Primo) | (pMsg.Secondo==msgPerPlc.at(i).Secondo)  ){
           return;
        }
    }

    msgPerPlc.append(pMsg);


}

void QNetPlc::AddMsg(int Primo,int Macchina, int Secondo, int Terzo){

    int len=0;
    QByteArray buffer(100,0);
    BYTE *pBuffer=buffer.data();
    pBuffer=WORDStr(pBuffer,Terzo,&len);
    buffer.resize(len);
    AddMsg(Primo, Macchina,Secondo,buffer);

}


void QNetPlc::AddMsg(int Primo,int Macchina,  QByteArray msg){

    if(Primo==CTipoMessaggio::GetData)
        return;
    CMsg pMsg ;
    pMsg.Primo=Primo;
    pMsg.Macchina=Macchina;
    pMsg.Secondo=0;
    pMsg.Terzo=0;
    pMsg.pMsg=msg;
    pMsg.Tentativi=0;

    for(int i=0; i<msgPerPlc.count() ;i++){
        if((pMsg.Primo==msgPerPlc.at(i).Primo) | (pMsg.Secondo==msgPerPlc.at(i).Secondo)  ){
           return;
        }
    }

    msgPerPlc.append(pMsg);

}

void QNetPlc::AddMsg(int Primo, int Macchina,  int Secondo, int Terzo, QByteArray msg){

    if(Primo==CTipoMessaggio::GetData)
        return;
    CMsg pMsg ;
    pMsg.Primo=Primo;
    pMsg.Macchina=Macchina;
    pMsg.Secondo=Secondo;
    pMsg.Terzo=Terzo;
    pMsg.pMsg=msg;
    pMsg.Tentativi=0;

    for(int i=0; i<msgPerPlc.count() ;i++){
        if((pMsg.Primo==msgPerPlc.at(i).Primo) | (pMsg.Secondo==msgPerPlc.at(i).Secondo)  ){
           return;
        }
    }

    msgPerPlc.append(pMsg);
}


void QNetPlc::AddMsg(int Primo,int Macchina, int Secondo, QByteArray msg){

    if(Primo==CTipoMessaggio::GetData)
        return;
    CMsg pMsg ;
    pMsg.Primo=Primo;
    pMsg.Macchina=Macchina;
    pMsg.Secondo=Secondo;
    pMsg.Terzo=0;
    pMsg.pMsg=msg;
    pMsg.Tentativi=0;

    for(int i=0; i<msgPerPlc.count() ;i++){
        if((pMsg.Primo==msgPerPlc.at(i).Primo) | (pMsg.Secondo==msgPerPlc.at(i).Secondo)  ){
           return;
        }
    }

    msgPerPlc.append(pMsg);

}

void  QNetPlc::AddMsgSetSegnale(int newslot,int newpoint,int newsegnale){

    int len=0;
    QByteArray buffer(20,0);
    BYTE *pBuffer=buffer.data();
    pBuffer=ByteStr(pBuffer,newpoint ,&len);
    pBuffer=WORDStr(pBuffer,newsegnale,&len);
    buffer.resize(len);
    AddMsg(CTipoMessaggio::SetSegnale,0,newslot,buffer);
}

bool QNetPlc::VerificaMsg(QByteArray buffer){

    BYTE val;
    BYTE *pBuffer=buffer.data();

    val=StrByte(&pBuffer);


    return true;
}

int QNetPlc::TipoMessaggio(QByteArray msg){

    BYTE *pBuffer=msg.data();
    BYTE PrimoMsg=*pBuffer;
    return PrimoMsg;

}

bool QNetPlc::IsMsgData(QByteArray msg){

    BYTE *pBuffer=msg.data();
    BYTE PrimoMsg=*pBuffer;

    if(PrimoMsg==CTipoMessaggio::GetData)
        return true;
    else
        return false;
}

/*void QNetPlc::AddLastComando(QByteArray msg){

    if(!pData[i].LastComando){
        delete pData[i].LastComando;
    }
    pData[i].LastComandoLen=msg.length();
    pData[i].LastComando=new BYTE[pData[i].LastComandoLen+1];

    BYTE *pBuffer=pData[i].LastComando;
    memcpy(pBuffer,msg.data(),pData[i].LastComandoLen);

    if(pData[i].LastComandoIndex+1<255)
       pData[i].LastComandoIndex++;
    else
       pData[i].LastComandoIndex=0;

}*/

void QNetPlc::AddStrMsgPerClient(int primomsg,QString msg){

    QByteArray buffer(msg.length()+1,0);
    BYTE *pBuffer=buffer.data();
    int len=0;
    pBuffer=ByteStr(pBuffer,primomsg ,&len);
    buffer.append(msg);
    AddMsgPerClient(buffer);

}

void QNetPlc::AddMsgPerClient(QByteArray msg){

    if(!ClientIsConnect)
         return;

    for(int i=0; i<msgPerClient.count();i++){
        QByteArray pmsg=msgPerClient.at(i).pMsg;
        if(pmsg==msg)
            return;
    }

    CMsg pMsg;
    pMsg.pMsg.append(msg);
    pMsg.Tentativi=0;
    msgPerClient.append(pMsg);

}

/*QString QNetPlc::GetMsgPerClient(){

    if(pData[i].LastComandoLen>0){
        BYTE *pComando=pData[i].LastComando;
        QByteArray buffer(pData[i].LastComandoLen,0);
        if(pData[i].LastComandoLen==0)
            return NULL;
        BYTE *pBuffer=buffer.data();
        pComando++;
        memcpy(pBuffer,pComando,pData[i].LastComandoLen-1);
        return buffer;
    }
    return NULL;

}*/

QByteArray QNetPlc::GetStrMsgPerClient(int i){

    if(pData[i].LastComandoLen>0){
        BYTE *pComando=pData[i].LastComando;
        QByteArray buffer(pData[i].LastComandoLen,0);
        if(pData[i].LastComandoLen==0)
            return NULL;
        BYTE *pBuffer=buffer.data();
        memcpy(pBuffer,pComando,pData[i].LastComandoLen);
        return buffer;
    }
    return NULL;

}

int QNetPlc::ComandoToInt( QByteArray msg  )
{
    if(msg.length()<2)
        return 0;
    BYTE *pBuffer=msg.data();
    pBuffer++;
    int funcCode =*pBuffer;
    return funcCode;

}

int QNetPlc::getPercentSpedito(QByteArray msg ){

    BYTE *pBuffer=msg.data();
    if(msg.length()<5)
        return 0;
    pBuffer+=2;
    int percent=StrWORD(&pBuffer);
    return percent;

}


void QNetPlc::ResetComando(int i){

    pData[i].ComandoAttivato=1;
    pData[i].StatoVuoto=-1;
    pData[i].StatoMacchina=-1;
    pData[i].ComandoAttivato=-1;
    pData[i].ComandoEseguito=-1;
    pData[i].ComandoRicevuto=-1;
    pData[i].File.ProcessoId=-1;
    pData[i].LastComandoLen=0;
    pData[i].LastComandoIndex=0;
    pData[i].ProcessoIsOn=false;

}

void QNetPlc::ResetErrore(){

    Macchina_ResetErrore();

}

QByteArray QNetPlc::setPercentSpedito(int di, int su){

    QByteArray Buffer(5,0);
    BYTE *pBuffer=Buffer.data();
    int len=0;
    pBuffer=ByteStr(pBuffer,CTipoMessaggio::CaricaProcesso ,&len);
    pBuffer=ByteStr(pBuffer,CTipoComando::Load,&len);
    double perc=di;
    if(di)
      perc/=su;
    else
      perc=0;
    perc*=100;
    int val=perc;
    pBuffer=WORDStr(pBuffer,val,&len);
    pBuffer=ByteStr(pBuffer,';' ,&len);
    return Buffer;

}

int QNetPlc::MessaggioToInt( QByteArray msg  )
{
    if(msg.length()==0)
        return 0;
    BYTE *pBuffer=msg.data();
    int funcCode =*pBuffer;
    return funcCode;

}

int QNetPlc::MsgDataToInt(QByteArray msg){

    BYTE *pBuffer=msg.data();
    int PrimoMsg=*pBuffer;

    return PrimoMsg;
}


QString QNetPlc::getTipoStep(int IdMacchina){

    QString str="";
    switch (pData[IdMacchina].TipoStep) {
      case CTipoStep::Vuoto:
        str="Vuoto";
        break;
      case CTipoStep::Riscaldo:
        str="Riscaldo";
        break;
      case CTipoStep::Etching:
        str="Etching";
        break;
      case CTipoStep::Coating:
        str="Coating";
        break;
      case CTipoStep::Venting:
        str="Venting";
        break;
    }
    return str;

}

QString QNetPlc::getStatoMacchina(int IdMacchina){

     QString str="";
     switch (pData[IdMacchina].StatoMacchina) {
        case CStatoMacchina::InVacuum  :
            str="StartVuoto" ;
            break;
        case CStatoMacchina::Vacuum :
            str="Vuoto" ;
            break;
        case CStatoMacchina::StartProcesso:
            str="Start Processo" ;
            break;
        case CStatoMacchina::InStopVuoto:
            str="InStopVuoto" ;
            break;
        case CStatoMacchina::StopVuoto:
            str="StopVuoto" ;
            break;
        case CStatoMacchina::Reset:
            str="StandBy" ;
            break;
        case CStatoMacchina::PreSet:
            str="PreSet" ;
            break;
        case CStatoMacchina::InReset:
            str="InReset" ;
            break;
        case CStatoMacchina::Stendby:
            str="Manuale" ;
            break;
        case CStatoMacchina::EnabledVent:
            str="EnabledVent" ;
            break;
        case CStatoMacchina::Ventila:
            str="InVenting" ;
            break;
        case CStatoMacchina::StopVentila:
            str="StopVentila" ;
            break;
        case CStatoMacchina::StopProcesso:
            str="Processo Stop" ;
            break;
        case CStatoMacchina::MacchinaAperta:
            str="Macchina Aperta" ;
            break;
        case CStatoMacchina::Manuale:
            str="Macchina In Manuale" ;
             break;
        }

     return str;


}


void QNetPlc::mostraEvento(const QString &msg){

   emit emitEvento(msg);

}

bool QNetPlc::ControllaRecivedCompStep(QByteArray Buffer,CIOSegnale *pSegnale){

    BYTE *pBuffer=Buffer.data();

    pBuffer+=8;

    for(int i=0; i< Pc_StepCount;i++){
        for(int j=0;j<pSegnale->m_Step[i].StepCount;j++ ){
          //CompStepType   Step=pSegnale->m_Step[i].Step[j];
          if(StrByte(&pBuffer)!=i)
              return false;
          if(pSegnale->m_Step[i].Step[j].Inf!=StrWORD(&pBuffer))
              return false;
          if(pSegnale->m_Step[i].Step[j].Start!=StrWORD(&pBuffer))
              return false;
          if(pSegnale->m_Step[i].Step[j].Stop!=StrWORD(&pBuffer))
              return false;
          int tempo=StrWORD(&pBuffer);
          int rampa=pSegnale->m_Step[i].Step[j].Rampa;
          if(rampa/1000!=tempo)
              return false;
        }
    }

    return true;
}


bool QNetPlc::ControllaRecivedStep( QByteArray Buffer ){

    BYTE *pBuffer=Buffer.data();
    pBuffer+=3;
    int IdMachina=StrByte(&pBuffer);
    int processo_id=StrWORD(&pBuffer);
    if(pData[IdMachina].File.ProcessoId!=processo_id)
        return false;
    pBuffer+=SorgenteCount;
    pBuffer+=UTENTE_SIZE;
    if(StrByte(&pBuffer)!=Pc_StepCount)
        return false;

    for(int i=0; i< Pc_StepCount;i++){
        StepType pStep;
        memcpy(&pStep,pBuffer,sizeof(StepType));
        pBuffer+=sizeof(StepType);
        if(pStep.Step!=i)
            return false;
        if(pStep.CTipoStep!=Pc_Step[i].CTipoStep)
            return false;
        if(pStep.RepitCount!=Pc_Step[i].RepitCount)
            return false;
        if(pStep.Tempo!=Pc_Step[i].Tempo)
            return false;
        if(pStep.Pressione!=Pc_Step[i].Pressione)
            return false;
        if(pStep.Potenza!=Pc_Step[i].Potenza)
            return false;
        if(pStep.Temperatura!=Pc_Step[i].Temperatura)
            return false;
        if(pStep.Tavola!=Pc_Step[i].Tavola)
            return false;
        if(pStep.PerBiasOnTime!=Pc_Step[i].PerBiasOnTime)
            return false;
        if(pStep.SorgentiOn!=Pc_Step[i].SorgentiOn)
            return false;
    }

    return true;

}

bool QNetPlc::IsInVuoto(){

    return Vuoto->Turbo.Normale->Segnale();
}



