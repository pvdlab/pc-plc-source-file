#include "Processo.h"
#include <unistd.h>
#include <sys/stat.h>


CProcesso::CProcesso(int newtipoapp,int newTipoImpianto,int argc, char *argv[]) : CMacchina( newtipoapp,newTipoImpianto,argc, argv)
{

    StepIndex=0;
    StepCount=0;
    IsLoadDaFile=false;
    mProcessoId=0;

    if(newtipoapp==CTipoApp::Server){
        if(CamereCount==1){
            if(pData[IdPro].File.ProcessoId){
                Processo_LoadDaFile();
            }else{
               pData[IdPro].File.ProcessoId=0;
               pData[IdPro].File.ProcessoRecord=0;
            }
        }else{
            for(int i=0; i< CamereCount; i++){
                IdPro=i;
                FILE* fp = fopen(Processo_GetFileNome().data(), "rb");
                if (!fp){
                    pData[IdPro].File.ProcessoId=0;
                    pData[IdPro].File.ProcessoRecord=0;
                }else
                   fclose(fp);
            }
        }

    }

}


WORD CProcesso::Processo_Load(char *pStr)
{

    if( pStr != NULL ){
      pStr++;
      if(*pStr=='S')
        return Processo_LoadStep(pStr);
      else if(*pStr=='D')
        return Processo_LoadCompStep(pStr);
    }

   return 0;

}

void CProcesso::Processo_UnLoad()
{
    for(int i=0; i< SorgenteCount;i++ )
        Bias[IdPro].Sorgente[i].SetI->UnLoad(0);

    for(int i=0; i< GasCount;i++ )
        GGas[i].SetG->UnLoad(0);

    Bias[IdPro].SetV->UnLoad(0);

    return;

}


WORD CProcesso::Processo_LoadCompStep(char *Buffer)
{
   CompFileStepType pStep;
   CIOSegnale *pSegnale=NULL;
   int CompTipo, CompIndex;

   char *pStr=Buffer;
   pStr++;
   IdPro=*pStr++;
   if(IdPro>CamereCount-1){
       MostraErrore("IdPro NON VALIDO");
       IdPro=0;
       return BufferLen;
   }
   if(TipoApp==CTipoApp::Server)
     pData[IdPro].File.ProcessoRecord++;

   pData[IdPro].StepCount++;
   pStep.Slot=*pStr++;
   pStep.Point=*pStr++;
   CompTipo=*pStr++;
   CompIndex=*pStr++;
   pData[IdPro].ComStepLoad=CompTipo*10+CompIndex;

   if(CompTipo==CTipoComp::Bias){
       pSegnale=Bias[IdPro].SetV;
   }else if(CompTipo==CTipoComp::Sorgente){
     if(CompIndex<SorgenteCount)
        pSegnale=Bias[IdPro].Sorgente[CompIndex].SetI;
   }else if(CompTipo==CTipoComp::Gas){
      if(CompIndex<GasCount)
        pSegnale=GGas[CompIndex].SetG;
   }else if(CompTipo==CTipoComp::SchermoTurbo){
      pSegnale=Vuoto[IdPro].Camera.SchermoTurbo;
   }

   if(pSegnale==NULL | *pStr==';'){
       goto ProcessoCorrotto;
   }

   pSegnale->UnLoad(StepIndex);
   pStep.Comp.ProStep=0;
   pStep.Comp.Step=0;
   do{
        if(pStep.Comp.ProStep!=*pStr)
            pStep.Comp.Step=0;
        pStep.Comp.ProStep=*pStr;
        if((pStep.Comp.ProStep>D_STEP )| (*pStr==';'))
            break;
        pStr++;
        pStep.Comp.Inf=StrWORD(&pStr);
        pStep.Comp.Start=StrWORD(&pStr);
        pStep.Comp.Stop=StrWORD(&pStr);
        pStep.Comp.Rampa=(DWORD)StrWORD(&pStr);
        if(TipoApp==CTipoApp::Server){
            if(pStep.Comp.ProStep>=StepIndex){
               pSegnale->Load( pStep.Comp.Inf,pStep.Comp.ProStep,pStep.Comp.Step, pStep.Comp.Start, pStep.Comp.Stop, pStep.Comp.Rampa);
            }
        }else{
            pSegnale->Load( pStep.Comp.Inf,pStep.Comp.ProStep,pStep.Comp.Step, pStep.Comp.Start, pStep.Comp.Stop, pStep.Comp.Rampa);
        }

        if(pStep.Comp.Step++>D_STEP)
            break;
   }while(*pStr!=';');
   pSegnale->Aggiornato();

   if(Debug & (TipoApp==CTipoApp::Server)){
       std::string str;
       str.append(intToString((int)CompTipo));
       str.append(intToString((int)CompIndex) + "\t");
       pData[IdPro].ComStepLoad=CompTipo*10;
       pData[IdPro].ComStepLoad+=CompIndex;
       if(CompTipo==CTipoComp::Bias)
         str.append("Carico CompStep Bias" );
       else if(CompTipo==CTipoComp::Gas)
         str.append("Carico CompStep Gas" );
       else if(CompTipo==CTipoComp::Sorgente)
         str.append("Carico CompStep Sorgente" );
       else if(CompTipo==CTipoComp::SchermoTurbo)
         str.append("Carico CompStep SchermoTurbo" );
       if(pData[IdPro].ProcessoIsOn)
         MostraEventoProcesso(str,Color::FG_GREEN);
       else
         MostraEvento(str,Color::FG_GREEN);
       pSegnale->PrintStep();
   }

   pStr=Buffer;
   pStr-=2;
   return Processo_GetMsgSendCompStep(pStr,IdPro,pSegnale,StepCount);

ProcessoCorrotto:
   pData[IdPro].File.ProcessoRecord=0;
   pData[IdPro].File.ProcessoId=0;
   remove(Processo_GetFileNome().data());
   Processo_UnLoad();
   pStr=Buffer;
   pStr++;
   memcpy(pStr,FINE_MSG,strlen(FINE_MSG));
   MostraEvento("Carico Step Processo Corrotto",Color::FG_RED);
   return 8;

}

void CProcesso::Processo_LoadStep(int StepIndex,int CTipoStep,int Repit,int Tempo,int Pressione,int Potenza,int Temperatura)
{

    Step[IdPro][StepIndex].Tempo=Tempo;
    Step[IdPro][StepIndex].CTipoStep=CTipoStep;
    Step[IdPro][StepIndex].Step=StepIndex;
    Step[IdPro][StepIndex].Pressione=Pressione;
    Step[IdPro][StepIndex].Potenza=Potenza;
    Step[IdPro][StepIndex].Temperatura=Temperatura;
    Step[IdPro][StepIndex].Repit=1;
    Step[IdPro][StepIndex].RepitCount=Repit;
    Step[IdPro][StepIndex].PerBiasOnTime=100;
    Step[IdPro][StepIndex].SorgentiOn=0;

}




#include<stdio.h>

WORD  CProcesso::Processo_Elimina(char *Buffer){

    char *pStr=Buffer;
    IdPro=*pStr++;
    if(IdPro>CamereCount-1){
        MostraErrore("IdPro NON VALIDO");
        IdPro=0;
        return BufferLen;
    }
    pData[IdPro].File.ProcessoId=StrWORD(&pStr);
    memset(pData[IdPro].File.ProcessoNome,0,UTENTE_SIZE);
    memcpy(pData[IdPro].File.ProcessoNome,pStr,UTENTE_SIZE);
    remove(Processo_GetFileNome().data());
    Processo_UnLoad();
    pData[IdPro].File.ProcessoId=0;
    return BufferLen;
}


WORD CProcesso::Processo_LoadStep(char *Buffer)
{
   DWORD DTime;
   pData[IdPro].StepCount=1;

   TimeStop=0;
   char *pStr=Buffer;
   pStr++;
   IdPro=*pStr++;
   if(IdPro>CamereCount-1){
       MostraErrore("IdPro NON VALIDO");
       IdPro=0;
       return BufferLen;
   }
   if(TipoApp==CTipoApp::Server)
      pData[IdPro].File.ProcessoRecord=1;
   pData[IdPro].ComStepLoad=0;
   pData[IdPro].File.ProcessoId=StrWORD(&pStr);
   if(TipoApp==CTipoApp::Server){
       if(pScorta->Cerca(&pStr)){
           for(int i=0; i< SorgenteCount;i++){
               Bias[IdPro].Sorgente[i].Catodo->OnTime(pScorta->OnTime(i));
               Bias[IdPro].Sorgente[i].Catodo->TipoTraget=pScorta->TipoTraget(i);
           }
           if(Debug)
               pScorta->Print();
       }
   }else{
       pStr+=SorgenteCount;
   }
   memset(pData[IdPro].File.ProcessoNome,0,UTENTE_SIZE);
   memcpy(pData[IdPro].File.ProcessoNome,pStr,UTENTE_SIZE);
   pStr+=UTENTE_SIZE;
   StepCount=*pStr++;
   pData[IdPro].ProStepCount=StepCount;

   if(StepCount > D_STEP){
      StepCount= D_STEP ;
   }

   pData[IdPro].ProcessoCorrotto=false;
   ///CONTROLLA FILE
   for(int i=0;i< StepCount; i++){
       StepType pStep;
       memcpy(&pStep,pStr,sizeof(StepType));
       pStr+=sizeof(StepType);
       if(pStep.ProStep!=i){
           pData[IdPro].ProcessoCorrotto=true;
           goto ProcessoCorrotto;
       }
   }


   if(!pData[IdPro].ProcessoIsOn){
      for(int j=0;j<CamereCount;j++){
         for(int i=0;i<D_STEP;i++){
            memset(&Step[j][i],0,sizeof(StepType));
         }
      }
      for(int i=0;i<GasCount;i++)
          GGas[i].SetG->UnLoad(0);
      for(int i=0;i<SorgenteCount;i++)
          Bias[IdPro].Sorgente[i].SetI->UnLoad(0);
      Bias[IdPro].SetV->UnLoad(0);
   }


   pStr=Buffer;
   pStr+=(5+UTENTE_SIZE+SorgenteCount);

   for(int i=0;i< StepCount; i++){
       int OldRepit=Step[IdPro][i].Repit;
       StepType pStep;
       memcpy(&pStep,pStr,sizeof(StepType));
       Step[IdPro][i]=pStep;
       pStr+=sizeof(StepType);
       if(Step[IdPro][i].RepitCount==0)
          Step[IdPro][i].RepitCount=1;
       if(Step[IdPro][i].Tempo==0)
          Step[IdPro][i].Tempo=10000;
       DTime=(DWORD)(Step[IdPro][i].Tempo);
       if(i>=StepIndex){
         if((i==StepIndex) & StepTimer[IdPro].IsAttivo()){
           if(DTime>StepTimer[IdPro].Time()){
             StepTimer[IdPro].TimeStop(DTime);
             Step[IdPro][i].Tempo=DTime;
           }
           Step[IdPro][i].Repit=OldRepit;
           Vuoto[IdPro].Resistenza->GoToStep(Step[IdPro][i].Potenza,Step[IdPro][i].Temperatura,Step[IdPro][i].Pressione);
           Vuoto[IdPro].GoToStep(Step[IdPro][i].Pressione);
         }else{
           Step[IdPro][i].Repit=1;
         }
       }
   }

   TimeStop=0;
   for(int i=0;i<StepCount;i++)
      TimeStop+=Step[IdPro][i].Tempo*Step[IdPro][i].RepitCount;

   if(Timer[IdPro].IsAttivo())
       Timer[IdPro].TimeStop(TimeStop);
   else
       Timer[IdPro].operator()(TimeStop);

   if(Debug & (TipoApp==CTipoApp::Server)){
       std::string str;
       str.append("Processo PLC ");
       str.append(pData[IdPro].File.ProcessoNome);
       str.append(" ProcessoId ");
       str.append(intToString(pData[IdPro].File.ProcessoId));
       MostraEvento(str,Color::FG_GREEN);
       for(int i=0;i< StepCount; i++){
           std::string str;
           str.append("Step ");
           str.append(intToString(i));
           str.append(" CTipoStep ");
           str.append(intToString(Step[IdPro][i].CTipoStep));
           str.append(" Tempo ");
           str.append(intToString(Step[IdPro][i].Tempo));
           str.append(" Repit ");
           str.append(intToString(Step[IdPro][i].RepitCount));
           str.append(" Potenza ");
           str.append(intToString(Step[IdPro][i].Potenza));
           str.append(" Temperatura ");
           str.append(intToString(Step[IdPro][i].Temperatura));
           str.append(" CompStepCount ");
           str.append(intToString(Step[IdPro][i].CompStepCount));
           MostraEvento(str,Color::FG_DEFAULT);
       }
   }


   pStr=Buffer;
   pStr-=2;
   return Processo_GetMsgSendStep(pStr,IdPro,pData[IdPro].File.ProcessoId,pData[IdPro].File.ProcessoNome,Step[IdPro],StepCount);

ProcessoCorrotto:
   pData[IdPro].File.ProcessoRecord=0;
   pData[IdPro].File.ProcessoId=0;
   remove(Processo_GetFileNome().data());
   Processo_UnLoad();
   pStr=Buffer;
   pStr++;
   memcpy(pStr,FINE_MSG,strlen(FINE_MSG));
   MostraEvento("Carico Step Processo Corrotto",Color::FG_RED);
   return 8;

}


WORD CProcesso::Processo_GetMsgSendCompStep(char *Buffer,int IdMacchina,CIOSegnale *pSegnale,int pStepCount){

    char *pBuffer=Buffer;
    int len=0;

    pBuffer=ByteStr(pBuffer,CTipoMessaggio::CaricaProcesso ,&len);
    pBuffer=ByteStr(pBuffer,CTipoComando::Load,&len);
    pBuffer=ByteStr(pBuffer,'D',&len);
    pBuffer=ByteStr(pBuffer,IdMacchina,&len);
    pBuffer=ByteStr(pBuffer,pSegnale->Slot(),&len);
    pBuffer=ByteStr(pBuffer,pSegnale->Point(),&len);

    std::string s;
    std::stringstream out;
    out << pSegnale->TipoComp;
    s = out.str();
    if(s.size()<2)
        return 0;
    pBuffer=ByteStr(pBuffer,s.at(0)-48,&len);
    pBuffer=ByteStr(pBuffer,s.at(1)-48,&len);


    for(int i=0; i< pStepCount;i++){
        for(int j=0;j<pSegnale->m_Step[i].StepCount;j++ ){
          pBuffer=ByteStr(pBuffer,i,&len);
          int rampa=pSegnale->m_Step[i].Step[j].Rampa;
          rampa/=1000;
          pBuffer=WORDStr(pBuffer,pSegnale->m_Step[i].Step[j].Inf,&len);
          pBuffer=WORDStr(pBuffer,pSegnale->m_Step[i].Step[j].Start,&len);
          pBuffer=WORDStr(pBuffer,pSegnale->m_Step[i].Step[j].Stop,&len);
          pBuffer=WORDStr(pBuffer,rampa,&len);
          if(len>(D_BUFFER-strlen(FINE_MSG))){
              pData[IdPro].ProcessoCorrotto=true;
              break;
          }
        }
    }

    if( Debug & (TipoApp==CTipoApp::Server)){
        std::string str="Spedisco CompStep " ;
        str.append(intToString((int)pSegnale->TipoComp) + "\t");
        MostraEvento(str,Color::FG_DEFAULT);
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    if(!IsLoadDaFile & (TipoApp==CTipoApp::Server)){
        std::string processo=getRunPath();
        if(pData[IdPro].ProcessoIsOn){
            processo.append(PLC_CICLO_PATH);
            processo.append(PLC_PROCESSO);
            processo.append(intToString(pData[IdPro].File.CicloId));
        }else{
            processo=Processo_GetFileNome();
        }
        FILE* fd = fopen(processo.data(), "ab");
        if (fd){
             fwrite(Buffer, D_PROCESSO, 1, fd);
             fclose(fd);
        }
    }

    return len;

}

WORD CProcesso::Processo_GetMsgElimaProcesso(char *Buffer ,int IdMacchina,int idprocesso,char *processo){

    char *pBuffer=Buffer;
    int len=0;

    pBuffer=ByteStr(pBuffer,CTipoMessaggio::EliminaProcesso ,&len);
    pBuffer=ByteStr(pBuffer,IdMacchina,&len);
    pBuffer=WORDStr(pBuffer,idprocesso,&len);
    memcpy(pBuffer,processo,UTENTE_SIZE);
    pBuffer+=UTENTE_SIZE;
    len+=UTENTE_SIZE;
    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    return len;

}

WORD CProcesso::Processo_GetMsgSendStep( char *Buffer ,int IdMacchina,int idprocesso,char *processo,StepType pStep[], int pStepCount){

    char *pBuffer=Buffer;
    int len=0;

    pBuffer=ByteStr(pBuffer,CTipoMessaggio::CaricaProcesso ,&len);//MSG_SETPROCESSO
    pBuffer=ByteStr(pBuffer,CTipoComando::Load,&len);
    pBuffer=ByteStr(pBuffer,'S',&len);
    pBuffer=ByteStr(pBuffer,IdMacchina,&len);
    pBuffer=WORDStr(pBuffer,idprocesso,&len);
    for(int i=0; i< SorgenteCount; i++){
       BYTE TipoTarget;
       if(TipoApp==CTipoApp::Server)
           TipoTarget=pScorta->Target(i)->TipoTarget;
       else
           TipoTarget=pTarget[i].TipoTarget;
       pBuffer=ByteStr(pBuffer,TipoTarget,&len);
    }
    memcpy(pBuffer,processo,UTENTE_SIZE);
    pBuffer+=UTENTE_SIZE;
    len+=UTENTE_SIZE;
    pBuffer=ByteStr(pBuffer,pStepCount,&len);
    for(int i=0; i< pStepCount;i++){
        StepType mStep=pStep[i];
        memcpy(pBuffer,&pStep[i],sizeof(StepType));
        pBuffer+=sizeof(StepType);
        len+=sizeof(StepType);
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);


    if(!IsLoadDaFile & (TipoApp==CTipoApp::Server)){
        pBuffer=Buffer;
        std::string processo=getRunPath();
        if(pData[IdPro].ProcessoIsOn){
            processo.append(PLC_CICLO_PATH);
            processo.append(PLC_PROCESSO);
            processo.append(intToString(pData[IdPro].File.CicloId));
        }else{
            processo=Processo_GetFileNome();
        }
        FILE* fd = fopen(processo.data(), "wb");
        if (fd){
             fwrite(pBuffer, D_PROCESSO, 1, fd);
             fclose(fd);
        }
    }

    return len;

}

WORD CProcesso::Processo_LoadDaFile(char *Buffer){

    int OldProcessoId=pData[IdPro].File.ProcessoId;
    char OldProcessoNome[UTENTE_SIZE];
    char *pStr=OldProcessoNome;

    if(BufferLen<UTENTE_SIZE+2){
        MostraErrore("NOME PRECESSO NON VALIDO");
        return BufferLen;
    }

    pStr=Buffer;
    pStr++;
    IdPro=StrByte(&pStr);
    if(IdPro>CamereCount-1){
        MostraErrore("IdPro NON VALIDO");
        IdPro=0;
        return BufferLen;
    }

    pData[IdPro].File.ProcessoId=StrWORD(&pStr);

    if(mProcessoId==pData[IdPro].File.ProcessoId)
        return BufferLen;

    mProcessoId=pData[IdPro].File.ProcessoId;

    pStr++;
    memset(pData[IdPro].File.ProcessoNome,0,UTENTE_SIZE);
    memcpy(pData[IdPro].File.ProcessoNome,pStr,UTENTE_SIZE);

    if(!Processo_LoadDaFile()){
       pData[IdPro].File.ProcessoId = OldProcessoId;
       char *pStr=pData[IdPro].File.ProcessoNome;
       memset(pStr,0,UTENTE_SIZE);
       memcpy(pStr,OldProcessoNome,UTENTE_SIZE);
    }

    pStr=Buffer;
    memset(pStr,0,D_BUFFER);
    int len=0;
    pStr=ByteStr(pStr,CTipoMessaggio::LoadDaFileProcesso ,&len);
    pStr=ByteStr(pStr,1 ,&len);
    pStr=WORDStr(pStr,pData[IdPro].File.ProcessoId ,&len);
    memcpy(pStr,pData[IdPro].File.ProcessoNome,UTENTE_SIZE);
    pStr+=UTENTE_SIZE;
    len+=UTENTE_SIZE;
    memcpy(pStr,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    return len;
}


bool CProcesso::Processo_LoadDaFile(){

    struct stat buf;
    stat(Processo_GetFileNome().data(), &buf);
    int Record=buf.st_size;
    Record/=D_PROCESSO;
    if(Record<255){
       pData[IdPro].File.ProcessoRecord=Record;
    }else{
        pErrore.Errore(true,E_PROCESSO_NO_FILE,0);
        return false;
    }

    FILE* fp = fopen(Processo_GetFileNome().data(), "rb");
    if (fp){
        char buffer[D_PROCESSO];
        IsLoadDaFile=true;
        for (int i=0; i<Record;i++) {
            char *pBuffer=buffer;
            fseek(fp, i*D_PROCESSO , SEEK_SET);
            size_t result;
            result=fread(pBuffer, D_PROCESSO, 1, fp);
            //if(StepIndex>0)
            //   StepIndex=0;
            if(!Processo_Load(++pBuffer) | (result == 0)){
               pData[IdPro].File.ProcessoId=0;
               fclose(fp);
               pErrore.Errore(true,E_PROCESSO_CORROTTO,0);
               pErrore.Errore();
               IsLoadDaFile=false;
               pData[IdPro].ProcessoCorrotto=true;
               return false;
               break;
            }
        }
        fclose(fp);
        IsLoadDaFile=false;
    }else{
        pData[IdPro].File.ProcessoRecord=0;
        pData[IdPro].File.ProcessoId=0;
        pErrore.Errore(true,E_PROCESSO_NO_FILE,0);
        pErrore.Errore();
        return false;
    }

    ///VERIFICA SE CI SONO GLI STEP
    for(int i=0; i<pData[IdPro].ProStepCount;i++ ){
        if((Step[IdPro][i].CTipoStep==CTipoStep::Coating) | (Step[IdPro][i].CTipoStep==CTipoStep::Etching)){
            int StepCount=Bias[IdPro].SetV->StepCount(i);
            for(int j=0;j<SorgenteCount; j++){
                StepCount+=Bias[IdPro].Sorgente[j].SetI->StepCount(i);
            }
            for(int j=0;j<GasCount; j++){
                StepCount+=GGas[j].SetG->StepCount(i);
            }
            StepCount+=Vuoto[IdPro].Camera.SchermoTurbo->StepCount(i);
            if(StepCount!=Step[IdPro][i].CompStepCount){
                if(Debug){
                    std::string str;
                    str.append("ERRORE Step ");
                    str.append(intToString(i));
                    str.append(" CTipoStep ");
                    str.append(intToString(Step[IdPro][i].CTipoStep));
                    str.append(" CompStepCount ");
                    str.append(intToString(StepCount));
                    str.append(" Su ");
                    str.append(intToString(Step[IdPro][i].CompStepCount));
                    MostraEvento(str,Color::FG_RED);
                }
                pErrore.Errore(true,E_PROCESSO_CORROTTO, 0);
                pErrore.Errore();
                pData[IdPro].ProcessoCorrotto=true;
            }

        }
    }

    ////VERIFICA BIAS NULLO
    for(int i=0; i<pData[IdPro].ProStepCount;i++ ){
        if((Step[IdPro][i].CTipoStep==CTipoStep::Coating) | (Step[IdPro][i].CTipoStep==CTipoStep::Etching)){
            WORD TempoOn=0;
            for(int j=0;j<SorgenteCount; j++){
                //if(Bias[IdPro].Sorgente[j].Enabled)
                TempoOn=__max(TempoOn, Bias[IdPro].Sorgente[j].SetI->OnTime(i));
            }
            int TempoOnBias=(int)Bias[IdPro].SetV->OnTime(i);
            if(TempoOn>TempoOnBias){
                pData[IdPro].ProcessoCorrotto=true;
                pErrore.Errore(true,E_BIAS_IS_NOT_ON, 0);
                pErrore.Errore();
                return false;

          }
        }
    }

    ///VERIFICA GAS NULLO
 /*   bool GasIsZero=false;
    for(int i=0;i<m_StepCount;i++){
        if((m_Step[i].CTipoStep==CTipoStep::Coating) | (m_Step[i].CTipoStep==CTipoStep::Etching) ){
            GasIsZero=true;
            break;
        }
    }


    for(int i=0;i<m_StepCount;i++){
       if((m_Step[i].CTipoStep==CTipoStep::Coating) | (m_Step[i].CTipoStep==CTipoStep::Etching) ){
           for(int j=0;j<GasCount;j++)
            GasIsZero=__min(GasIsZero,GGas[j].SetG->IsZero(m_Step[i].Step));
       }

    }

    if(GasIsZero){
        pData[IdPro].ProcessoCorrotto=true;
        pErrore.Errore(true,E_STEP_GAS_ZERO, 0);
        return false;
    }*/

    return true ;
}

std::string CProcesso::Processo_GetFileNome(){

    std::string processo=getRunPath();
    processo.append(PLC_PROCESSO_PATH);
    processo.append("/");
    processo.append(intToString(pData[IdPro].File.ProcessoId));
    processo.append("_");
    processo.append(pData[IdPro].File.ProcessoNome);
    processo.append("_");
    processo.append(intToString(IdPro));
    processo.append("_");
    processo.append(intToString(getImpianto()));
    processo.append(PLC_PROCESSO_EXT);

    if(Debug)
        MostraEvento(processo,Color::FG_GREEN);

    return processo;
}


#include <sys/types.h>
#include <dirent.h>

WORD CProcesso::Processo_GetMsgProcessiDisponibili(char *pStr){

    char *pBuffer=pStr;
    int len=0;
    int count=0;
    memset(pBuffer,0,D_BUFFER);
    pBuffer=ByteStr(pBuffer,CTipoMessaggio::GetProcessiDisponibili,&len);
    pBuffer=ByteStr(pBuffer,count,&len);

    std::string path=getRunPath();
    path.append(PLC_PROCESSO_PATH);
    path.append("/");

    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
           std::string nome= dp->d_name;
           int found=nome.find(PLC_PROCESSO_EXT);
           if(found>0){
               if(len+found+1+strlen(FINE_MSG)>D_BUFFER){
                   pErrore.Errore(true,E_FILE_PROCESSI_TROPPI,0);
                   pErrore.Errore();
                   break;
               }
               if(found){
                  memcpy(pBuffer,nome.data(),found);
                  pBuffer+=found;
                  len+=found;
                  memcpy(pBuffer,";",1);
                  pBuffer++;
                  len++;
                  count++;
               }
           }


    }
    closedir(dirp);

    if(count){
        pBuffer-=len;
        pBuffer++;
        memcpy(pBuffer,&count,sizeof(BYTE));
        pBuffer+=len-2;
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    return len;
}


WORD CProcesso::Processo_GetMsgRecord(char *pStr){

    ///
    char tmp[20];
    char *ptmp=tmp;
    memcpy(ptmp,pStr,BufferLen);

    if( pStr != NULL  ){
       pStr+=2;
       int record=StrWORD(&pStr);
       pStr-=2;
       FILE* fp = fopen(Processo_GetFileNome().data(), "rb");
       if (fp){
           fseek(fp, record*D_PROCESSO , SEEK_SET);
           fread(pStr, D_PROCESSO, 1, fp);
           fclose(fp);
           return D_PROCESSO;
       }else{
           return Macchina_GetNetData(pStr);
       }
    }

    return Macchina_GetNetData(pStr);
}

WORD CProcesso::Processo_GetStep(char *pStr){

    if( pStr != NULL ){
       pStr++;
       int step=*pStr;
       if(pData[IdPro].File.ProcessoId){
           std::string processo=getRunPath();
           processo.append(PLC_PROCESSO);
           FILE* fp = fopen(processo.data(), "r");
           if (fp){
               while (!feof(fp)){
                   pStr-=2;
                   char *pBuffer=pStr;
                   fread(pBuffer, D_BUFFER, 1, fp);
                   if(!step--){
                       fclose(fp);
                       return D_BUFFER;
                   }
               }
               fclose(fp);
               return 5;
           }else
               return 5;
       }
    }
    return 5;
}

