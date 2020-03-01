#include "StdAfx.h"
#include "Processa.h"
#include "DataFile.h"
#ifndef NUOVA_CLASSE
#include "Arc6010.h"
#include "Arc6011.h"
#endif
#include "MainCostanti.h"
#include <unistd.h>
#include <sys/stat.h>
#ifdef QT_APP
#include "QDebug"
#include<QThread>
#endif


CProcessa::CProcessa( int newtipoapp,int newTipoImpianto)
{

  Impianto=newTipoImpianto;
  TipoApp=newtipoapp;
  ResetTickCount();
  m_SetGasOffSet=false;
  m_StandByCatodi=false;
  m_ArcIsOn=false;
  m_MainTimer.Reset();
  m_PlcErrore=false;
#ifndef NUOVA_CLASSE_PROCESSO
  m_IsLoadDaFile=false;
  m_StepCount=0;
  m_StepIndex=0;
#endif
  Debug=true;
  setImpianto(Impianto);

  struct DataType *p=&pData;
  memset(p,0,sizeof(struct DataType));
  m_Tempo=2147483647L;
  m_End=false;
  pData.ComandoManuale=false;
  pData.ComandoAttivato=1;
  pData.StatoVuoto=-1;
  pData.StatoMacchina=-1;
  pData.ComandoAttivato=-1;
  pData.ComandoEseguito=-1;
  pData.ComandoRicevuto=-1;
  pData.File.ProcessoId=0;
  pData.LastComandoLen=0;
  pData.LastComandoIndex=0;
  pData.ProcessoIsOn=false;
  setProcessoIsOn(pData.ProcessoIsOn);
  pData.ErrPreProcesso=false;
  pData.ErrProcesso=false;
  pData.StepCount=0;
  pData.MessaggioLen=0;
  pData.StepTime=0;
  pData.MainTime=0;
  pData.StatoMacchina=0;
  pData.Simula=false;
  pData.ProStepCount=0;
  pData.RecordCount=0;

  m_PreSet=false;


  m_TimerPreSet(2000);
  m_NewEvento=false;
  m_IsInNextStep=false;
  m_PowOn=false;
  IsPerSimula=false;

  pData.Errore=NULL;
  pData.ErrCount=0;
  memset(Msg,0,D_MSG);

  PowOutdig=NULL;
  Reset=NULL;

  //pErrore.Clear();
  pErrore.append(E_MODBUS_PORTA,"E_MODBUS_PORTA");
  pErrore.append(E_MODBUS_LETTURA,"E_PLC_LETTURA");
  pErrore.append(E_MODBUS_SCRITTURA,"E_PLC_SCRITTURA");
  pErrore.append(E_PRESET_ERRORE,"ERRORE_IN_PRESET");
  pErrore.append(E_CATODO_CONFLITTO,"E_CATODO_CONFLITTO");
  pErrore.append(E_NEXT_STEP,"E_NEXT_STEP");
  pErrore.append(E_CATODO_COTRA_SPENTI,"E_CATODO_COTRAPPOSTI_SPENTI");  //E_BIAS_IS_NOT_ON
  pErrore.append(E_BIAS_IS_NOT_ON,"E_BIAS_IS_NOT_ON");
  pErrore.append(E_PROCESSO_CORROTTO,"E_PROCESSO_CORROTTO");
  pErrore.append(E_LICENZA_SCADUTA,"E_LICENZA_SCADUTA");
  pErrore.append(E_TIME_OUT,"E_TIME_OUT");
  pErrore.append(E_PROCESSO_NO_FILE,"PROCESSO_NO_FILE");
  pErrore.append(E_STEP_GAS_ZERO,"E_STEP_GAS_ZERO");
  pErrore.append(E_RESET,"E_RESET");
  pErrore.append(E_FILE_CICLO,"E_FILE_CICLO");
  pErrore.append(E_MAX_RISCALDO_COUNT,"E_MAX_RISCALDO_COUNT");

  Macchina_InitSegnali(newTipoImpianto);

}

int CProcessa::Macchina_InitSegnali(int newImpianto)
{

#ifndef NUOVA_CLASSE
    if(newImpianto==ARC_6010)
      Init_Arc6010(this,BP5_BIAS);
    else if(newImpianto==ARC_6011)
      Init_Arc6011(this,MDXII_BIAS);
    else
      return 0;

    Plc.OpenPort();
#endif


  #ifdef CON_MODBUS
    int j,i;
    if(getRunPath()!=NULL){
        std::string nomeFile=getRunPath();
        nomeFile.append(PLC_MONITORIZZA);
        FILE* fd = fopen(nomeFile.data(), "r");
        if (fd){
            if((int)fread(&pData.File,sizeof (pData.File),1, fd)<0){
               MostraEvento("Impossibile Leggere InitPlc",Color::FG_RED) ;
            }else{
                if(pData.File.TavGiri==0)
                   pData.File.TavGiri=DEF_TAVOLA_GIRI_SECONDO;
                Vuoto.Meccanica.OnTime(pData.File.MeccanicaOnTime);
                Vuoto.Turbo.OnTime(pData.File.TurboOnTime);
                Vuoto.Tavola.OnTime(pData.File.TavOnTime);
                Vuoto.Tavola.OffTime(pData.File.TavOffTime);
                for(i=0;i<SorgenteCount;i++){
                  for(j=0;j<Bias->Sorgente[i].CatodoCount;j++){
                    Bias->Sorgente[i].Catodo[j]->TrigCount(pData.File.TriggerCount[i+j]);
                    Bias->Sorgente[i].Catodo[j]->Ah(pData.File.Ah[i+j]);
                  }
                }
                if(pData.File.ProcessoId){
                    Processo_LoadDaFile();
                }else
                   pData.File.ProcessoId=0;
                pData.EventoProcessoCount=0;
                pData.EventoProcessoDi=0;
                if(pData.File.Ciclo){
                    std::string path=getHomePath();
                    path.append(PLC_CICLO_PATH);
                    path.append(PLC_EVENTO);
                    path.append(intToString(pData.File.CicloId));
                    FILE* fp = fopen(path.data(), "r");
                    if (fp){
                        while (!feof(fp)) {
                            char buffer[D_MSG];
                            char *pBuffer=buffer;
                            fread(pBuffer, D_MSG, 1, fp);
                            pData.EventoProcessoCount++;
                        }
                        fclose(fp);
                    }
                    setMostraEventoProcessoCount(pData.EventoProcessoCount);
                }

            }
        }
        else{
           memset(&pData.File,0,sizeof(FileType));
           pData.File.TavGiri=DEF_TAVOLA_GIRI_SECONDO;
           MostraEvento("File Inesistente InitPlc",Color::FG_RED) ;
        }
        nomeFile=getRunPath();
        nomeFile.append(PLC_REGOLA);
        fd = fopen(nomeFile.data(), "r");
        if (fd){
            int i=0;
            while (!feof(fd)) {
                double pBuffer;
                fread(&pBuffer, sizeof(double), 1, fd);
                if(pBuffer){
                    if(i< SorgenteCount)
                      Bias->Sorgente[i++].SetI->setRegola(pBuffer);
                }
            }
            fclose(fd);
        }

    }
  #endif

   if(TipoApp==TipoApp_Plc){
     for(int i=0;i<GasCount;i++)
        GGas[i].CeckStato();
     Vuoto.CeckStato();
     for(int i=0;i<SorgenteCount;i++){
        Bias->Sorgente[i].CeckStato();
     }
     pData.StatoMacchina=Vuoto.Stato();
     switch(pData.StatoMacchina){
      case  StatoMacchina_Null:
         pData.Comando=TipoComando_Nessuno;
         pData.ComandoAttivato=TipoComando_Nessuno;
         pData.ComandoRicevuto=TipoComando_Nessuno;
         break;
      case  StatoMacchina_Vacuum:
         pData.Comando=TipoComando_Vacuum;
         pData.ComandoAttivato=TipoComando_Vacuum;
         pData.ComandoRicevuto=TipoComando_Vacuum;
         break;
      case StatoMacchina_InVacuum:
         pData.Comando=TipoComando_Vacuum;
         pData.ComandoRicevuto=TipoComando_Vacuum;
         pData.ComandoAttivato=TipoComando_Vacuum;
         break;
      case StatoMacchina_StopVuoto:
         pData.Comando=TipoComando_StopVuoto;
         pData.ComandoAttivato=TipoComando_StopVuoto;
         pData.ComandoRicevuto=TipoComando_StopVuoto;
      case StatoMacchina_InStopVuoto:
         pData.Comando=TipoComando_StopVuoto;
         pData.ComandoRicevuto=TipoComando_StopVuoto;
         pData.ComandoAttivato=TipoComando_StopVuoto;
         break;

    }

    Vuoto.Tavola.GiriSecondo(pData.File.TavGiri);
    Vuoto.Tavola.OnTime(pData.File.TavOnTime);
    Vuoto.Tavola.OffTime(pData.File.TavOffTime);
   }

   Vuoto.Debug=Debug;
   Bias->Debug=Debug;
   for(int i=0; i<SorgenteCount;i++){
       Bias->Sorgente[i].Catodo[0]->Debug=Debug;
   }


   m_Macchina_ResetGas=false;
   m_OnOffStepTimer(500);
   m_PowerTimeOut(500*(SorgenteCount+1));
   m_PowerTimeOut.Reset();
   m_OnOffStepTimer.Reset();
   m_GasTimeOut(GAS_TIME_RESET_GAS*(GasCount+1));
   Ciclo_ResetErrore();


   return newImpianto;
}

void CProcessa::Macchina_SalvaRegola(){

    bool isStabile=true;
    for( int i=0; i<SorgenteCount; i++)
        isStabile=__min(isStabile,Bias->Sorgente[i].SetI->IsStabile());

    if(isStabile){
        std::string nomeFile=getRunPath();
        nomeFile.append(PLC_REGOLA);
        FILE* fd = fopen(nomeFile.data(), "w");
        if (fd){
            for( int i=0; i<SorgenteCount; i++){
               double val=Bias->Sorgente[i].SetI->getRegola();
               val*=i*0.34569;
               fwrite(&val, sizeof(double), 1, fd);
            }
            fclose(fd);
        }
    }


}


WORD CProcessa::mainMessaggio(char *Buffer, int len)
{
  BYTE PrimoMsg;
  char *pMsg=Buffer;
  PrimoMsg=*pMsg;
  pData.MessaggioRicevuto=PrimoMsg;

  switch (PrimoMsg){
      case TipoMessaggio_Test:
         return len;
         break;
      case TipoMessaggio_SetComando :
         mainComando(++pMsg);
         return len;
         break;
      case TipoMessaggio_CaricaProcesso :
         return Processo_Load(++pMsg);
         break;
      case TipoMessaggio_GetData:
         return Macchina_GetNetData(pMsg);
         break;
      case TipoMessaggio_GetCostanti:
         //return GetNetCostanti(pMsg);
         break;
      case TipoMessaggio_ModbusComunica:
         //return m_Plc.ModBus.Comunica(pMsg);
         break;
      case TipoMessaggio_InitFile:
         return Macchina_InitFile(pMsg);
         break;
      case TipoMessaggio_GetStep:
         return Processo_GetStep(pMsg);
         break;
      case TipoMessaggio_GetProcMsg:
         return Ciclo_GetMsg(pMsg);
         break;
      case TipoMessaggio_GetProcRecord:
        return Processo_GetRecord(pMsg);
        break;
      case TipoMessaggio_Simula:
         char *pSimulaBuffer=SimulaBuffer;
         memcpy(pSimulaBuffer,pMsg,len);
         IsPerSimula=true;
         return len;
         break;

  }

  MostraEvento("TipoMessaggio Non Valido",Color::FG_RED);
  return Macchina_GetNetData(pMsg);


}

bool CProcessa::Macchina_PowIsOn()
{   

  if(m_PowOn){
      return m_PowOn;
  }

  bool RetVal=true;
  for(int i=0;i<SorgenteCount;i++)
    RetVal=__min(RetVal,Bias->Sorgente[i].IsOn());
  RetVal=__min(RetVal,Bias->IsOn());

  if( RetVal  )
      MostraEvento("POWER NON È ON ",Color::FG_RED);

  return RetVal;
}

bool CProcessa::Macchina_PowIsOff()
{
  if(!m_PowOn){
      return FalseTrue( m_PowOn);
  }

  bool RetVal=true;

  for(int i=0;i<SorgenteCount;i++)
    RetVal=__min(RetVal,FalseTrue( Bias->Sorgente[i].IsOn()));
  RetVal=__min(RetVal,FalseTrue( Bias->IsOn()));

  if( ! RetVal  )
      MostraEvento("POWER NON È OFF",Color::FG_RED);

  return RetVal;
}


bool CProcessa::Macchina_PowOff(){


    if(!m_PowOn)
        return FalseTrue( m_PowOn);

    if(m_PowerTimeOut.Timer()){
       Bias->ceckPowerError();
       for(int i=0;i<SorgenteCount;i++)
          Bias->Sorgente[i].ceckPowerError();
       m_PowerTimeOut.Reset();
       m_OnOffStepTimer.Reset();
       MostraEvento("ERRORE POWER OFF",Color::FG_RED);
       return FalseTrue( m_PowOn);
    }

    bool isReset=true;

    for(int i=0;i<SorgenteCount;i++)
      isReset=__min(isReset,Bias->Sorgente[i].SetOff());
    if(!isReset)
        return false;

    isReset=__min(isReset,Bias->SetOff());
    if(!isReset)
        return false;

    isReset=__min(isReset,Resistenza.SetOff());

    if(!isReset)
        return false;

    m_PowOn=false;
    m_PowOn=__max(m_PowOn,Resistenza.IsOn());
    for(int i=0;i<SorgenteCount;i++){
       m_PowOn=__max(m_PowOn,Bias->Sorgente[i].IsOn());
    }
    m_PowOn=__max(m_PowOn,Bias->IsOn());
    m_PowOn=__max(m_PowOn, FalseTrue( Vuoto.Camera.AriaComp->Set(false)));

    if( !m_PowOn ){
       if(Debug)
          MostraEvento("SET POWER OFF ",Color::FG_DEFAULT);
       m_PowerTimeOut.Reset();
       m_OnOffStepTimer.Reset();
    }

    return FalseTrue(m_PowOn);
}

bool CProcessa::Macchina_PowOn()
{

  if(m_PowOn)
      return m_PowOn;

  if(m_PowerTimeOut.Timer() ){
      Bias->ceckPowerError();
      for(int i=0;i<SorgenteCount;i++)
         Bias->Sorgente[i].ceckPowerError();
      m_PowerTimeOut.Reset();
      m_OnOffStepTimer.Reset();
      MostraEvento("ERRORE POWE ON",Color::FG_RED);
      return m_PowOn;
  }

  bool IsOnTrigger=true;
  IsOnTrigger=__min(IsOnTrigger,Vuoto.Camera.AriaComp->Set(true));


  for(int i=0;i<SorgenteCount;i++){
     for(int j=0;j<Bias->Sorgente[i].CatodoCount;j++){
         IsOnTrigger=__min(IsOnTrigger,Bias->Sorgente[i].Catodo[j]->Trigger->Set(false));
     }
  }
  if(!IsOnTrigger)
      return false;

  for(int i=0;i<SorgenteCount;i++)
      Bias->Sorgente[i].ceckCatodoPowErr();

  if(m_OnOffStepTimer.Timer()){
    for(int i=0;i<SorgenteCount;i++){
      if(!Bias->Sorgente[i].IsOn()){
        Bias->Sorgente[i].SetOn();
        m_OnOffStepTimer.Reset();
        return false;
      }
    }
    if(!Bias->IsOn()){
      Bias->SetOn();
      m_OnOffStepTimer.Reset();
      return false;
    }
   }
   else
      return false;

  m_PowOn=true;
  for(int i=0;i<SorgenteCount;i++){
     m_PowOn=__min(m_PowOn,Bias->Sorgente[i].IsOn());
  }
  m_PowOn=__min(m_PowOn,Bias->IsOn());


  if( m_PowOn  ){
     if(Debug)
       MostraEvento("SET POWER ON ",Color::FG_DEFAULT);
     m_PowerTimeOut.Reset();
     m_OnOffStepTimer.Reset();
  }

  return m_PowOn;
}


void CProcessa::mainMacchina(void)
{
    bool RetVal=false;
    int i,j;

    if(getErrore()){
        *Vuoto.Camera.LuceRossa=true;
    }else {
        *Vuoto.Camera.LuceRossa=false;
    }

    if(pData.ProcessoIsOn){
        *Vuoto.Camera.LuceGialla=true;
    }else{
        *Vuoto.Camera.LuceGialla=false;
        if(pData.StatoVuoto==StatoMacchina_EnabledVent){
            *Vuoto.Camera.LuceVerde=false;
        }else{
            *Vuoto.Camera.LuceVerde=true;
        }
    }

    pData.TimeEvent=(DWORD)TimerEvent.Time();
    pData.TimeStopEvent=(DWORD)TimerEvent.StopTime();
    pData.MainTime=(DWORD)m_MainTimer.Time();
    pData.Time=(DWORD)m_Timer.Time();
    pData.StopTime=(DWORD)m_StepTimer.StopTime();
    pData.StepTime=(DWORD)m_StepTimer.Time();
    pData.StopStepTime=(DWORD)m_StepTimer.StopTime();
    pData.TipoStep=m_Step[m_StepIndex].TipoStep;
    pData.StepIndex=m_StepIndex;
    pData.RepitCount=m_Step[m_StepIndex].RepitCount;
    pData.Repit=m_Step[m_StepIndex].Repit;
    pData.Resistenza=Resistenza.Potenza();
    pData.File.MeccanicaOnTime=Vuoto.Meccanica.OnTime();
    pData.File.TurboOnTime=Vuoto.Turbo.OnTime();
    for(i=0;i<SorgenteCount;i++){
      for(j=0;j<Bias->Sorgente[i].CatodoCount;j++){
          pData.File.TriggerCount[i+j]=Bias->Sorgente[i].Catodo[j]->TrigCount();
          pData.File.Ah[i+j]=Bias->Sorgente[i].Catodo[j]->Ah();
	  }
	}

#ifdef  CON_MODBUS
   if((pData.MainTime<pData.LastTime) || (pData.MainTime>=pData.LastTime+10) ){
	  pData.LastTime=pData.MainTime;
      pData.File.TavOnTime=Vuoto.Tavola.OnTime();
      pData.File.TavOffTime=Vuoto.Tavola.OffTime();
      if(getRunPath()!=NULL){
          std::string nomeFile=getRunPath();
          nomeFile.append(PLC_MONITORIZZA);
          FILE* fd = fopen(nomeFile.data(), "w");
          if (fd){
             fwrite(&pData.File, sizeof(pData.File), 1, fd);
             fclose(fd);
          }
      }else{
          MostraEvento("File Inesistente InitPlc",Color::FG_DEFAULT) ;
      }

   }
 #endif
	
	if(Reset->Segnale()){
        for(i=0;i<SorgenteCount;i++)
          Bias->Sorgente[i].SetOff();
        Bias->SetOff();
		for(i=0;i<GasCount;i++)
           GGas[i].SetOff();
		Resistenza.SetOff();
		Vuoto.Turbo.SetOff();
		Vuoto.Meccanica.SetOff();
        Vuoto.Tavola.Reset();
		m_StepCount=0;
		m_StepIndex=0;
        pData.StatoMacchina=StatoMacchina_Reset;
		m_NewEvento=false;
        pData.Comando=TipoComando_Reset;
		return;
	}


    if(pData.Comando!=TipoComando_StartProcesso)
	   *Vuoto.Penning.PowOn=true;
	pData.StatoVuoto=Vuoto.Stato(); 
    Macchina_FlussaAcqua();
	if(m_NewEvento){
      TimerEvent.Timer();
    }

    m_MainTimer.Time();

    switch(pData.Comando){
     case TipoComando_ReStart:
        system("shutdown -r");
        break;
     case TipoComando_Manuale:
        if(getSimula()){
            Ciclo_ResetErrore();
            if(pData.ComandoManuale)
             pData.ComandoManuale=false;
            else
             pData.ComandoManuale=true;
        }
        break;
      case TipoComando_Impara:
		if(Vuoto.Tavola.Impara()){
          pData.Comando=TipoComando_Reset;
		  Vuoto.Tavola.SetOff();
		  pData.File.TavOnTime=Vuoto.Tavola.OnTime();
		  pData.File.TavOffTime=Vuoto.Tavola.OffTime();
		}
		break;
      case TipoComando_GiraTavola:
        Vuoto.Tavola.Set();
		if(Vuoto.Tavola.Errore()){
           Vuoto.Tavola.SetOff();
           pData.Comando=TipoComando_Reset;
		}
		break;
      case TipoComando_Reset:
        pData.ComandoAttivato=TipoComando_Reset;
		Vuoto.Tavola.SetOff();
        if(Ciclo_Reset()){
           pData.StatoMacchina=StatoMacchina_Reset;
           pData.ComandoAttivato=TipoComando_Reset;
		   TimerEvent(0);
		   m_NewEvento=false;
		}
		else{
           pData.StatoMacchina=StatoMacchina_InReset;
		   if(Vuoto.Errore())
             pData.Comando=TipoComando_StopVuoto;
		}
		break;
      case TipoComando_StopProcesso:
        if(Ciclo_Reset()){
           pData.StatoMacchina=StatoMacchina_StopProcesso;
           pData.ComandoAttivato=TipoComando_StopProcesso;
           pData.Comando=TipoComando_StendBy;
		   TimerEvent(0);
		   m_NewEvento=false;
		}
		break;
      case TipoComando_StendBy:
        //Vuoto.StendBy();
		if(Vuoto.Errore()){
           pData.Comando=TipoComando_StopVuoto;
		   return;
		}
		RetVal=true;
		for(i=0;i<SorgenteCount;i++)
          RetVal=__min(RetVal,Bias->Sorgente[i].StendBy());
		for(i=0;i<GasCount;i++)
		  RetVal=__min(RetVal,GGas[i].StendBy());
        RetVal=__min(RetVal,Bias->StendBy());
		RetVal=__min(RetVal,Resistenza.StendBy());
		if(RetVal){
           pData.ComandoAttivato=TipoComando_StendBy;
	       m_Timer.StendBy();
	       m_StepTimer.StendBy();
           Vuoto.Stato();
		   m_NewEvento=false;
		}
		else
           pData.StatoMacchina=StatoMacchina_InStendby;
		break;
      case TipoComando_ResetStendBy:
		RetVal=true;
		for(i=0;i<SorgenteCount;i++)
          RetVal=__min(RetVal,Bias->Sorgente[i].ResetStendBy());
		for(i=0;i<GasCount;i++)
		  RetVal=__min(RetVal,GGas[i].ResetStendBy());
        RetVal=__min(RetVal,Bias->ResetStendBy());
		RetVal=__min(RetVal,Resistenza.ResetStendBy());
		if(RetVal){
           pData.ComandoAttivato=TipoComando_ResetStendBy;
           m_Timer.ResetStendBy();
	       m_StepTimer.ResetStendBy();
           pData.StatoMacchina=StatoMacchina_Reset;
           if(pData.NextComando==TipoComando_StartProcesso)
              pData.Comando=TipoComando_StartProcesso;
		   m_NewEvento=false;
		}
		else
           pData.StatoMacchina=StatoMacchina_StartProcesso;
		break;
      case TipoComando_Vacuum:
		if(Resistenza.Errore()){
           pData.Comando=TipoComando_StopVuoto;
		   return;
		}
        if(Vuoto.Errore()){
           pData.Comando=TipoComando_StopVuoto;
		   return;
		}
        if(Vuoto.Set()){
          pData.ComandoEseguito=TipoComando_Vacuum;
		  TimerEvent(0);
          m_NewEvento=false;
        }
        pData.StatoMacchina=pData.StatoVuoto;
		break; 
      case TipoComando_StartProcesso:
        mainCiclo();
        pData.StatoMacchina=StatoMacchina_StartProcesso;
        break;		
      case TipoComando_NextStep:
        mainCiclo();
        pData.StatoMacchina=StatoMacchina_StartProcesso;
        break;	
      case TipoComando_Preset:
		if(Vuoto.Errore()){
           pData.Comando=TipoComando_StopVuoto;
		   return;
        }
        Ciclo_PreSet();
		break;
      case TipoComando_StopVuoto:
        if(!Ciclo_Reset())
            return;
        if(!Vuoto.Reset()){
            pData.StatoVuoto=StatoMacchina_InStopVuoto;
            pData.StatoMacchina=StatoMacchina_InStopVuoto;
        }else{
            if(Vuoto.EnabledVenting()){
               pData.ComandoEseguito=TipoComando_StopVuoto;
               TimerEvent(Vuoto.getVentigTime());
               }
            else{
               TimerEvent(0);
               pData.ComandoEseguito=StatoMacchina_InStopVuoto;
            }
        }
        pData.StatoMacchina=pData.StatoVuoto;
        m_NewEvento=false;
		break;
      case TipoComando_Ventila:
        Vuoto.Ventila();
        pData.StatoMacchina=pData.StatoVuoto;
		break;
      case TipoComando_StopVentila:
        Vuoto.StopVentila();
        pData.StatoMacchina=pData.StatoVuoto;
		break;
      case TipoComando_UnLoad:
        Processo_UnLoad();
        break;
	}

}

WORD CProcessa::mainComando(char *pStr)
{
    BYTE NewComando;
    NewComando=*pStr;

    m_NewEvento=true;

    switch( NewComando ){
      case TipoComando_Impara:
          TimerEvent(40000);
          pData.Comando=NewComando;
          break;
      case TipoComando_GiraTavola:
          TimerEvent(60000);
          pData.Comando=NewComando;
          break;
      case TipoComando_NextStep:
          if(pData.Comando==TipoComando_StartProcesso){
             pData.ComandoAttivato=TipoComando_NextStep;
             m_NextStep=*pStr++;
             m_NextTimeStep=(DWORD)StrWORD(&pStr)*1000;
             pData.NextComando=TipoComando_StartProcesso;
             pData.Comando=NewComando;
          }
          break;
      case TipoComando_Reset:
          Ciclo_ResetErrore();
          if(pData.ComandoAttivato!=TipoComando_Reset || pData.ComandoAttivato!=TipoComando_Reset){
            m_MainTimer.Reset();
            TimerEvent(40000);
          }
          pData.Comando=NewComando;
          break;
      case TipoComando_StendBy:
          Ciclo_ResetErrore();
          if(pData.Comando==TipoComando_StartProcesso)
             pData.NextComando=TipoComando_StartProcesso;
          pData.Comando=NewComando;
          break;
      case TipoComando_Vacuum:
          Ciclo_ResetErrore();
          if(!Vuoto.Camera.PortaChiusa->Segnale()){
             Vuoto.pErrore.Errore(true,E_PORTA_CHIUSA,0);
             pData.ComandoAttivato=TipoComando_Vacuum;
             pData.ComandoEseguito=TipoComando_Vacuum;
             return 0;
          }
          if(pData.Comando!=TipoComando_Vacuum ){
            m_MainTimer.Reset();
            TimerEvent(Vuoto.VuotoTime());
          }
          pData.ComandoAttivato=TipoComando_Vacuum;
          pData.Comando=TipoComando_Vacuum;
          pData.NextComando=TipoComando_Nessuno;
          Vuoto.Tavola.Reset();
          break;
      case TipoComando_Load:
          //Load(++pStr);
          break;
      case TipoComando_StartProcesso:
        {
         pStr++;
         WORD ProcessoID=StrByte(&pStr);
         if(ProcessoID){
            pData.File.ProcessoId =ProcessoID;
            pStr++;
            memset(pData.File.ProcessoNome,0,UTENTE_SIZE);
            memcpy(pData.File.ProcessoNome,pStr,UTENTE_SIZE);
            if(Debug)
               MostraEvento("Carico " + Processo_GetFileNome(),Color::FG_BLUE);
         }
         if(!m_StepCount |  !Processo_LoadDaFile()){
             pData.ErrPreProcesso=true;
             pData.ComandoAttivato=TipoComando_StendBy;
             pData.Comando=TipoComando_StendBy;
             pData.NextComando=TipoComando_Nessuno;
             break;
          }
          Ciclo_ResetErrore();
          m_RipartiCount=0;
          TimerEvent.Reset();
          memset(pData.Errore,'\0',m_ErrSize);
          ResetTickCount();
          m_MainTimer.Reset();
          m_Timer(m_Tempo,m_Tempo);
          Vuoto.Tavola.Reset();
          m_BiasIsLow=false;
          pStr++;
          pData.File.Ciclo=StrWORD(&pStr);
          pData.Comando=TipoComando_Preset;
          pData.ComandoAttivato=TipoComando_Preset;
          pData.NextComando=TipoComando_StartProcesso;
          break;
          }
      case TipoComando_ResetStendBy:
          pData.Comando=TipoComando_ResetStendBy;
          pData.NextComando=TipoComando_StartProcesso;
          break;
      case TipoComando_StopVuoto:
          if(pData.Comando!=TipoComando_StopVuoto ){
            TimerEvent(Vuoto.StopVuotoTime());
          }
          pData.ComandoAttivato=TipoComando_StopVuoto;
          pData.Comando=TipoComando_StopVuoto;
          pData.NextComando=TipoComando_Nessuno;
          Vuoto.Tavola.Reset();
          break;
      case TipoComando_Ventila:
          pData.Comando=NewComando;
          break;
      case TipoComando_StopVentila:
          pData.Comando=NewComando;
          break;
      case TipoComando_StopProcesso:
          MostraEventoProcesso("Comado Stop Processo",Color::FG_BLUE);
          pData.Comando=NewComando;
          break;
      case TipoComando_SetSegnale:
          Macchina_SetSegnale(pStr);
          pData.NextComando=TipoComando_Nessuno;
          break;
      }

      pData.ComandoRicevuto=NewComando;
      return pData.Comando;
}

void CProcessa::Ciclo_ResetErrore()
{ 

    MostraEvento("Reset Errore",Color::FG_DEFAULT);
    for(int i=0;i<SorgenteCount;i++)
       Bias->Sorgente[i].ResetErrore();
    for(int i=0;i<GasCount;i++)
	   GGas[i].ResetErrore();
    Bias->ResetErrore();
	Resistenza.ResetErrore();
    pErrore.Reset();
    Vuoto.pErrore.Reset();
    memset(pData.Errore,'\0',m_ErrSize);
    m_ErrorePreset=false;
    m_PlcErrore=false;
    pData.ErrProcesso=false;
    pData.ErrPreProcesso=false;
    setErrore(false);

}

void CProcessa::Ciclo_Allunga(){

    int catodicount=0;
    for(int i=0;i< SorgenteCount;i++){
        for(int j=0;j< Bias->Sorgente[i].CatodoCount;j++){
            if(Bias->Sorgente[i].Catodo[j]->Warnig())
                catodicount++;
        }
    }

    if(!catodicount)
         return;

    for(int i=0;i< SorgenteCount;i++){
        for(int j=0;j< Bias->Sorgente[i].CatodoCount;j++){
            Bias->Sorgente[i].Catodo[j]->resetWarnig();
        }
    }

}

void CProcessa::mainCiclo()
{
    int i;
    m_ArcIsOn=false;
    bool retval=true;
    bool isMinGas=true;

    pData.ErrProcesso=false;
    Macchina_FlussaAcqua();
    Vuoto.Set();
    Ciclo_Allunga();
    Ciclo_CeckErrori();

	m_Timer.Timer();
    setMainTime(m_Timer.Time());
    if(!Ciclo_NextStepForza())
		return;


    pData.ProcessoIsOn=true;
    setProcessoIsOn(pData.ProcessoIsOn);
    pData.TipoStep=m_Step[m_StepIndex].TipoStep;
	if(m_StepTimer.Timer()){ 
	  switch( m_Step[m_StepIndex].TipoStep){
       case TipoStep_Vuoto:
		 *Vuoto.Penning.PowOn=true;
         pData.ComandoAttivato=TipoComando_StartProcesso;
         if(!Vuoto.Set())
           return;
		 break;
       case TipoStep_Riscaldo:
		 *Vuoto.Penning.PowOn=true;
         pData.ComandoAttivato=TipoComando_StartProcesso;
         if(!Resistenza.Set(m_Step[m_StepIndex].TipoStep))
			 return;
         if(!m_IsInNextStep){
             if(m_Step[m_StepIndex].Pressione < Vuoto.Penning.Segnale->Segnale()){
                m_StepTimer.StopTime(m_StepTimer.StopTime()+PRO_DT_RISCALDO);
                m_StopTime+=PRO_DT_RISCALDO;
                m_RiscaldoCount++;
                if(m_RiscaldoCount==PRO_MAX_RISCALDO_COUNT){
                    pErrore.Errore(true,E_MAX_RISCALDO_COUNT,0);
                }
                return;
             }
         }
		 break;
       case TipoStep_Etching:
       case TipoStep_Coating:
         //*Vuoto.Camera.AriaComp=true;
         pData.ComandoAttivato=TipoComando_StartProcesso;
		 for(i=0;i<SorgenteCount;i++)
           retval=__min(retval,Bias->Sorgente[i].ResetSegnali());
		 for(i=0;i<GasCount;i++)
           retval=__min(retval,GGas[i].ResetSegnali());
		 if(!m_SetGasOffSet){
		   for(i=0;i<GasCount;i++)
             GGas[i].Offset(Vuoto.Baratron.Pressione());
		   Vuoto.BaratronOffSet();
		   m_SetGasOffSet=true;
         }
         retval=__min(retval,Bias->ResetSegnali());
         Resistenza.ResetSegnali();
         break;
       case TipoStep_Venting:
		 *Vuoto.Penning.PowOn=true;
         pData.ComandoAttivato=TipoComando_StartProcesso;
         pData.Comando=TipoComando_StopVuoto;
		 break;	 
	  }

      if(m_StepIndex + 1 < m_StepCount){
        if(!Ciclo_NextStepGoto()){
           if(pData.ErrProcesso)
               goto Errore;
           return;
        }
        pData.ComandoAttivato=TipoComando_StartProcesso;
	  }
	  else{ //Se non vi sono altri step  la macchina va in StandBy
        m_Timer.Reset();
        pData.ComandoAttivato=TipoComando_StartProcesso;
        pData.Comando=TipoComando_StopVuoto;
        pData.ProcessoIsOn=false;
        setProcessoIsOn(pData.ProcessoIsOn);
	    return;
	  }
	}
    else{//Continua Il ciclo
      switch( m_Step[m_StepIndex].TipoStep){
        case TipoStep_Vuoto:
          Vuoto.Set();
		  break;
        case TipoStep_Riscaldo:
          Resistenza.Set(m_Step[m_StepIndex].TipoStep);
		  break;
        case TipoStep_Coating:
          if(!Macchina_PowOn())
              return;
          if(!Macchina_PowIsOn()){
              pData.ErrProcesso=true;
              goto Errore;
          }
          Bias->Repit(m_Step[m_StepIndex].Repit);
          for(i=0;i<GasCount;i++){
            GGas[i].Set();
            isMinGas=__min(isMinGas,GGas[i].IsMinGas());
          }
          if(isMinGas)
              Bias->SetArcOff();
          else
              Bias->Set();
          Resistenza.Set(m_Step[m_StepIndex].TipoStep);
        case TipoStep_Etching:
          for(i=0;i<GasCount;i++){
            GGas[i].Set();
            isMinGas=__min(isMinGas,GGas[i].IsMinGas());
          }
          Resistenza.Set(m_Step[m_StepIndex].TipoStep);
          if(isMinGas)
              Bias->SetArcOff();
          else{
              Bias->Set();
              if(Bias->Riparti()){
                  if(m_StepIndex-1>0){
                     m_StepIndex--;
                     m_StepTimer.StopTime(m_StepTimer.StopTime()+m_StepTimer.Time());
                     m_StopTime+=m_StepTimer.Time();
                     m_StepTimer.Stop();
                     m_Step[m_StepIndex].Tempo=m_StopTime;
                     return;
                  }
              }
          }
		  break;
        case TipoStep_Venting:  
         // PreStopVuoto();
          Macchina_SalvaRegola();
          pData.ComandoAttivato=TipoComando_StartProcesso;
          pData.Comando=TipoComando_StopVuoto;
		  pData.ProcessoIsOn=false;
          setProcessoIsOn(pData.ProcessoIsOn);
		  break;
	  }
	}

Errore:
    if(pData.ErrProcesso ){

       Ciclo_CeckErrori();

      // PreStopVuoto();

      /* bool riparti=m_Riparti;
       riparti=__min(riparti,Vuoto.Riparti());
       riparti=__min(riparti,Resistenza.Riparti());
       riparti=__min(riparti,Bias->Riparti());
       for(i=0;i<GasCount;i++)
          riparti=__min(riparti,GGas[i].Riparti());
       for(i=0;i<SorgenteCount;i++)
          riparti=__min(riparti,Bias->Sorgente[i].Riparti());
       if((riparti==true) & (m_RipartiCount < 3)){
          if(!TimerEvent.IsAttivo()){
             m_RipartiCount++;
             TimerEvent(120000);
             TimerEvent.Timer();
             MostraEventoProcesso("Riparte Processo " + intToString(m_RipartiCount));
             return;
          }else{
             if(TimerEvent.Timer()){
                 Ciclo_CeckErrori();
                 for(int i=0;i<SorgenteCount;i++)
                    Bias->Sorgente[i].ResetErrore();
                 for(int i=0;i<GasCount;i++)
                    GGas[i].ResetErrore();
                 Bias->ResetErrore();
                 Resistenza.ResetErrore();
                 pErrore.Reset();
                 Vuoto.pErrore.Reset();
                 pData.ErrProcesso=false;
                 MostraEventoProcesso("Riparte Processo Reset Errore" + intToString(m_RipartiCount));
                 TimerEvent.Reset();
                 return;
             }
          }
          return;
       }*/
       pData.ProcessoIsOn=false;
       setProcessoIsOn(pData.ProcessoIsOn);
	   Vuoto.Tavola.SetOff();
       pData.Comando=TipoComando_StopVuoto;
       pData.SorgentiOn=pData.File.SorgentiOn;
       pData.ComandoAttivato=TipoComando_StartProcesso;
       //*Vuoto.Camera.AriaComp=false;
       MostraEventoProcesso("Fermo per Errore Processo",Color::FG_RED);
	   return ;
	 }

}

QString Vecchia;

void CProcessa::printErrori(){

    return;

    if(!Debug)
        return;

    char tmp[95];
    char *ptmp=tmp;
    memcpy(ptmp,pData.Errore,95);


    for(int i=0; i<95 ;i+=D_COMP_ERRORE){
        int Tempo=StrWORD(&ptmp);
        BYTE Primo=StrByte(&ptmp);
        BYTE Secondo=StrByte(&ptmp);
        ptmp++;
        if((Tempo!=0) | (Primo!=0) | (Secondo!=0)){
            QString Nuova="INDEX " + QString::number( i ) + " Tempo " + QString::number( Tempo )
                    + " Primo " + QString::number( Primo ) + " Secondo " + QString::number( Secondo );
            if(Nuova!=Vecchia)
              qDebug() << Nuova;
            Vecchia=Nuova;
        }

    }

}

void CProcessa::Ciclo_CeckErrori(){

    printErrori();
    pData.ErrProcesso=pErrore.Errore();
    pData.ErrProcesso=__max(pData.ErrProcesso,Vuoto.Errore());

    pData.ErrProcesso=__max(pData.ErrProcesso,Resistenza.Errore());
    for(int i=0;i<SorgenteCount;i++){
          pData.ErrProcesso=__max(pData.ErrProcesso,Bias->Sorgente[i].Errore());
    }
    pData.ErrProcesso=__max(pData.ErrProcesso,Bias->Errore());
    for(int i=0;i<GasCount ;i++){
        pData.ErrProcesso=__max(pData.ErrProcesso,GGas[i].Errore());
    }

}


bool CProcessa::Ciclo_NextStepForza()
{
    DWORD TmpTempo=0;
    int i;

    if(pData.Comando!=TipoComando_NextStep)// || m_NextTimeStep<m_Timer.Time())
        return true;

    if(!m_IsInNextStep){
        pData.ComandoAttivato=TipoComando_NextStep;
        if((m_Step[m_NextStep].TipoStep==TipoStep_Etching || m_Step[m_NextStep].TipoStep==TipoStep_Coating) && !Macchina_PowIsOn()){
             TimerEvent(30000);
             m_OnOffStepTimer.Reset();
        }
        else
             TimerEvent(5000);
        if(m_NextTimeStep){
          for(i=0;i<m_NextStep;i++)
            TmpTempo+=(m_Step[i].Tempo*m_Step[i].RepitCount);
          m_NextTimeStep-=TmpTempo;
        }
        m_IsInNextStep=true;
    }
    return Ciclo_NextStepGoto();

}
//#define TOGLI_NEXT_TIMER


bool CProcessa::Ciclo_NextStepGoto()
{
  bool RetVal=true;
  bool Tmp;
  int i;


  if(!m_IsInNextStep){
     if(m_Step[m_StepIndex].Repit==m_Step[m_StepIndex].RepitCount ){
        if(m_StepIndex+1< m_StepCount){
           m_StepIndex++;
        }else
           return true;
     }else
        m_Step[m_StepIndex].Repit++;
     m_TimerPreSet(30000);
     m_TimerPreSet.Reset();
     m_IsInNextStep=true;
     Ciclo_ResetErrore();
  }

#ifndef TOGLI_NEXT_TIMER
  if(m_TimerPreSet.Timer()){
      goto Errore;
  }
#endif

  m_Macchina_ResetGas=false;
  if((m_Step[m_StepIndex].TipoStep==TipoStep_Etching) | (m_Step[m_StepIndex].TipoStep==TipoStep_Coating)){
      if(m_Step[m_StepIndex].SorgentiOn!=pData.SorgentiOn){
       for(i=0;i<SorgenteCount;i++){
         bool isEnabled=GetBit(m_Step[m_StepIndex].SorgentiOn,i);
         if(isEnabled!=Bias->Sorgente[i].Enabled){
           Bias->Sorgente[i].SetArcOff();
         }
         Bias->Sorgente[i].Enabled=isEnabled ;
         Bias->Sorgente[i].Catodo[0]->SetEnabled(isEnabled);
       }
       Ciclo_SorgentiAccoppiate(m_StepIndex);
       pData.SorgentiOn=m_Step[m_StepIndex].SorgentiOn;
       Bias->Sorgente[3].SetI->PrintStep(3);
     }

     if(!Macchina_PowOn()){
        return false;
     }

     if(!Macchina_PowIsOn()){
         goto Errore;
     }

     if(Bias->IsSetTap()){
        if(!Bias->SetArcOff()){
           return false;
        }
        m_StandByCatodi=true;
        Tmp=Bias->GoToStep(m_Step[m_StepIndex].Step,m_Step[m_StepIndex].TipoStep,m_Step[m_StepIndex].PerBiasOnTime);
        RetVal=__min(RetVal,Tmp);
        if(!RetVal)
            return false;
        m_StandByCatodi=false;
     }
     else{
        Tmp=Bias->GoToStep(m_Step[m_StepIndex].Step,m_Step[m_StepIndex].TipoStep,m_Step[m_StepIndex].PerBiasOnTime);
        RetVal=__min(RetVal,Tmp);
     }

     for(i=0;i<SorgenteCount;i++){
       Tmp=Bias->Sorgente[i].GoToStep(m_Step[m_StepIndex].Step,m_Step[m_StepIndex].TipoStep);
       RetVal=__min(RetVal,Tmp);
     }

     if(m_Step[m_StepIndex].TipoStep==TipoStep_Venting){
       if(Bias->SetArcOff())
          return false;
     }
     for(i=0;i<GasCount;i++){
       Tmp=GGas[i].GoToStep(m_Step[m_StepIndex].Step);
       RetVal=__min(RetVal,Tmp);
     }

   }

  Tmp=Vuoto.GoToStep(m_Step[m_StepIndex].Pressione);
  RetVal=__min(RetVal,Tmp);
  Tmp=Resistenza.GoToStep(m_Step[m_StepIndex].Potenza,m_Step[m_StepIndex].Temperatura,m_Step[m_StepIndex].Pressione);
  RetVal=__min(RetVal,Tmp);

  if(RetVal){
    if(!m_SetGasOffSet){
        for(i=0;i<GasCount;i++)
          GGas[i].Offset(Vuoto.Baratron.Pressione());
        Vuoto.BaratronOffSet();
        m_SetGasOffSet=true;
    }

    m_StepTimer(m_Step[m_StepIndex].Tempo);
    m_IsInNextStep=false;
    TimerOnArc(5000);
    m_RiscaldoCount=0;

    std::string str;
    std::stringstream out;
    out << m_Step[m_StepIndex].Step;
    str.append( out.str());
    str.append("\t");
    switch (m_Step[m_StepIndex].TipoStep) {
    case TipoStep_Vuoto:
         str+=" Vuoto ";
        break;
    case TipoStep_Riscaldo:
         str+=" Riscaldo ";
        break;
    case TipoStep_Etching:
         str+=" Etching ";
        break;
    case TipoStep_Coating:
         str+=" Coating ";
        break;
    default:
        break;
    }
    str.append("Step\t");
    str.append(intToString((int)m_Step[m_StepIndex].Repit) + "/");
    str.append(intToString((int)m_Step[m_StepIndex].RepitCount) );
    MostraEventoProcesso(str,Color::FG_GREEN);
    return true;
  }
  else
     return false;

Errore:
  Bias->ceckNextStepError();
  for(i=0;i<SorgenteCount;i++)
      Bias->Sorgente[i].ceckNextStepError();
  for(i=0;i<GasCount;i++)
      GGas[i].ceckNextStepError();
  Resistenza.ceckNextStepError();
  pErrore.Errore(true,E_NEXT_STEP, 0);
  m_Riparti=false;

  return false;
}


bool CProcessa::Ciclo_SorgentiAccoppiate(int index){

    for(int i=0;i<SorgenteCount;i++){
      int attuale=i;
      for(int j=0;j<SorgenteCount;j++){
        if(j!=attuale) {
            if(Bias->Sorgente[j].Enabled){
               if((Bias->Sorgente[attuale].SetI->Slot()==Bias->Sorgente[j].SetI->Slot()) & (Bias->Sorgente[attuale].SetI->Point()==Bias->Sorgente[j].SetI->Point())){
                  if((Bias->Sorgente[attuale].SetI->StepCount()!=0) & (Bias->Sorgente[j].SetI->StepCount()!=0)){
                      std::string str;
                      if(Bias->Sorgente[attuale].SetI->isStepNull(index)){
                          Bias->Sorgente[attuale].Enabled=false;
                          str.append("ENABLED FALSE SU ");
                          str.append(intToString(attuale) );
                          //MostraEventoProcesso(str);
                      }else if(Bias->Sorgente[j].SetI->isStepNull(index)){
                          Bias->Sorgente[j].Enabled=false;
                          Bias->Sorgente[attuale].Enabled=false;
                          str.append("ENABLED FALSE SU ");
                          str.append(intToString(j) );
                          //MostraEventoProcesso(str);
                      }else{
                          pErrore.Errore(true,E_CATODO_CONFLITTO,0);
                          return true;
                      }
                  }

               }
             }
        }

      }
    }

    return false;

}

bool CProcessa::Macchina_FlussaAcqua()
{

     if(pData.Comando==TipoComando_StendBy)
          return true;

      if(m_Step[m_StepIndex].TipoStep==TipoStep_Etching || m_Step[m_StepIndex].TipoStep==TipoStep_Coating){
          *Vuoto.Turbo.Acqua=true;
      }
	  else{
	      if(Vuoto.Turbo.Temp->Segnale()>PRO_TEMP_ON_ACQUA_TURBO )
             *Vuoto.Turbo.Acqua=true;
	       else if(Vuoto.Turbo.Temp->Segnale()<PRO_TEMP_OFF_ACQUA_TURBO)
			 *Vuoto.Turbo.Acqua=false;
      }


      if(Vuoto.Camera.Temp->Segnale()>PRO_TEMP_ON_ACQUA_CAMERA )
             *Vuoto.Camera.Acqua=true;
	  else if(Vuoto.Camera.Temp->Segnale()<PRO_TEMP_OFF_ACQUA_CAMERA )
			 *Vuoto.Camera.Acqua=false;

	  return true;
}



bool CProcessa::Ciclo_PreSet()
{

  DWORD TempoPreset;

  if(m_PreSet)
	  return m_PreSet;

  
  if(!m_InPreSet){
     if(!Macchina_PowOff())
         return false;
     if(!Macchina_PowIsOff()){
        goto Errore;
     }
     if(!Macchina_ResetGas())
         return false;
	 m_InPreSet=true;
	 m_GasInPreset=false;
	 m_PowInPreset=false;
	 m_BiasInPreset=false;
	 pData.ErrPreProcesso=false;
     m_RiscaldoCount=0;
     TempoPreset=Vuoto.VuotoTime();
	 TempoPreset+=2000; // Aspetto 10 Secondi prima di iniziare il Preset
	 m_TimerPreSet(PRO_ONTIME_GAS);
     for(int i=0;i<GasCount;i++){
       GGas[i].PreSet(true);
	   if(GGas[i].SetG->Load())
		 TempoPreset+=PRO_ONTIME_GAS;
	 }
	 //TempoPreset+=PRO_ONTIME_GAS; // dopo 10 secondi spengo tutte
	 m_OnTime=0;
     for(int i=0;i<SorgenteCount;i++){
       if(Bias->Sorgente[i].Enabled){
		 m_OnTime+=PRO_ONTIME_CATODI;
		 TempoPreset+=PRO_ONTIME_CATODI;
	   }
	 }
     if(Bias->Enabled){
		 m_OnTime+=PRO_ONTIME_BIAS;
		 TempoPreset+=PRO_ONTIME_BIAS;
	 }
     TimerEvent(TempoPreset+30000);
     if(m_MainTimer.Time()<1)
         return m_InPreSet;
  }

  for(int i=0; i<m_StepCount;i++){
      if((m_Step[i].TipoStep==TipoStep_Coating) | (m_Step[i].TipoStep==TipoStep_Etching)){
         if(Ciclo_SorgentiAccoppiate(i)){
            pData.ErrPreProcesso=true;
         }
      }
  }

  if(!Vuoto.Set())
     return false;

  if(TimerEvent.Timer() & !getSimula()){
      pErrore.Errore(true,E_TIME_OUT,0);
      goto Errore;
  }

  pData.StatoMacchina=StatoMacchina_PreSet;

  if(!m_GasInPreset){
      if(!Macchina_PowOn()){
         return false;
      }
      if(!Macchina_PowIsOn()){
          goto Errore;
      }
	 if(m_TimerPreSet.Timer()){
        m_Macchina_ResetGas=false;
        for(int i=0; i<GasCount;i++){
		  if(!GGas[i].SetG->Segnale() && GGas[i].SetG->Load()){
			*GGas[i].OnOffValv=true;
            if(i==0)
               *GGas[i].SetG=30/(GGas[i].SetG->Fattore());
            else if(i==1)
               *GGas[i].SetG=100/(GGas[i].SetG->Fattore());
            else if(i==2)
                *GGas[i].SetG=10/(GGas[i].SetG->Fattore());
            else
                *GGas[i].SetG=100;
            m_TimerPreSet(PRO_ONTIME_GAS);
            m_TimerPreSet.Reset();
            return false;
		  }
		}
		m_TimerPreSet(1000);
        m_GasInPreset=true;
     }else
        return false;
  }

  pErrore.Errore(pData.ProcessoCorrotto,E_PROCESSO_CORROTTO, E_TIME);
  pErrore.Errore(FalseTrue(isLicenza()),E_LICENZA_SCADUTA, E_TIME);
  pData.ErrProcesso=pErrore.Errore();
  pData.ErrProcesso=__max(pData.ErrProcesso,Vuoto.Errore());
  pData.ErrProcesso=__max(pData.ErrProcesso,Resistenza.Errore());

  for(int i=0;i<GasCount ;i++){
      pData.ErrPreProcesso=__max(pData.ErrPreProcesso,GGas[i].Errore());
  }


  if(!m_BiasInPreset){
     if(!Bias->SetTap(1)){
        return false;
     }

     if(!Bias->SetV->Segnale()){
       *Bias->SetV=BIAS_TEST/(Bias->SetV->Fattore());
	   m_TimerPreSet(500);
	 }
	 if(!m_TimerPreSet.Timer() ){
	     return false;
	 }
	 m_TimerPreSet(3*E_TIME);
     m_BiasInPreset=true;
   }

   for(int i=0;i<SorgenteCount ;i++){
      pData.ErrPreProcesso=__max(pData.ErrPreProcesso,Bias->Sorgente[i].Errore());
   }

   if(!m_TimerPreSet.Timer() ){
       pData.ErrPreProcesso=__max(pData.ErrPreProcesso,Bias->Errore());
	   return false;
   }

   if(!Macchina_ResetGas())
       return false;

   if(!Macchina_PowOff())
       return false;
   if(!Macchina_PowIsOff()){
      goto Errore;
   }

   if(!pData.ErrPreProcesso){ 
     m_Timer(m_Tempo);
     m_StepIndex=0;
     m_StepTimer.Reset();
     m_StepTimer(m_Step[m_StepIndex].Tempo);
     Vuoto.GoToStep(m_Step[m_StepIndex].Pressione);
     Resistenza.GoToStep(m_Step[m_StepIndex].Potenza,m_Step[m_StepIndex].Temperatura,m_Step[m_StepIndex].Pressione);
     pData.ErrProcesso=false;
     m_SetGasOffSet=false;
     Vuoto.BaratronOffSet();
     pData.Comando=TipoComando_StartProcesso;
     pData.ComandoAttivato=TipoComando_Preset;
     pData.ComandoAttivato=TipoComando_StartProcesso;
     MostraEventoProcessoReset();
     pData.File.CicloId=0;

   }
   else{ 
     goto Errore;
   }

   for(int i=0;i<GasCount;i++)
     GGas[i].PreSet(false);
   Ciclo_ResetErrore();
   m_PreSet=true;
   m_InPreSet=false;
   TimerEvent(0);

   return m_PreSet;

Errore:
   pErrore.Errore(true,E_PRESET_ERRORE,0);
   pData.Comando=TipoComando_StopVuoto;
   pData.ProcessoIsOn=false;
   setProcessoIsOn(pData.ProcessoIsOn);
   m_PreSet=true;
   m_InPreSet=false;
   return false;

}

bool CProcessa::Macchina_ResetGas(){

    if(m_Macchina_ResetGas)
        return m_Macchina_ResetGas;

    if(m_GasTimeOut.Timer()){
       MostraErrore("ERRORE RESET GAS");
       m_GasTimeOut.Reset();
    }

    m_Macchina_ResetGas=true;
    for(int i=0;i< GasCount;i++)
       m_Macchina_ResetGas=__min(m_Macchina_ResetGas,GGas[i].SetOff());

    if(m_Macchina_ResetGas){
       if(Debug)
         MostraEvento("GAS IS RESET",Color::FG_GREEN);
       m_GasTimeOut.Reset();
    }

    return m_Macchina_ResetGas;

}

bool CProcessa::Ciclo_Reset()
{

  if(!Macchina_PowOff())
      return false;
  if(!Macchina_PowIsOff()){
     MostraErrore("Errore Reset Ciclo");
  }

  if(!Macchina_ResetGas())
      return false;

  bool SchermoIsOpen=true;
  for(int i=0;i<SorgenteCount;i++){
       SchermoIsOpen=__min(SchermoIsOpen,Bias->Sorgente[i].SchermoApri());
  }
  if(!SchermoIsOpen)
      return false;

  pData.ProcessoIsOn=false;
  setProcessoIsOn(pData.ProcessoIsOn);
  m_Timer.Reset();
  m_StepIndex=0;
  m_StepTimer.Reset();
  m_IsInNextStep=false;
  m_PreSet=false;
  m_InPreSet=false;
  for(int i=0 ; i<m_StepCount;i++){
      m_Step[i].Repit=1;
  }

  return  true;
}

void CProcessa::Macchina_SetSegnale(BYTE *pStr)
{
  int slot,point;
  WORD Valore;
  
  pStr++;
  if(pStr)
    slot=*pStr++;
  if(pStr)
    point=*pStr++;
  Valore=StrWORD(&pStr);
#ifndef NUOVA_CLASSE
  Plc.setSegnale(slot, point, Valore);
#endif

}


#ifndef NUOVA_CLASSE_PROCESSO
WORD CProcessa::Processo_GetStep(char *pStr){

    if( pStr != NULL ){
       pStr++;
       int step=*pStr;
       if(pData.File.ProcessoId){
           std::string processo=getHomePath();
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


WORD CProcessa::Processo_GetSendCompStep(char *Buffer,CIOSegnale *pSegnale){

    char *pBuffer=Buffer;
    int len=0;

    pBuffer=ByteStr(pBuffer,TipoMessaggio_CaricaProcesso ,&len);
    pBuffer=ByteStr(pBuffer,TipoComando_Load,&len);
    pBuffer=ByteStr(pBuffer,'D',&len);
    pBuffer=ByteStr(pBuffer,pSegnale->Slot(),&len);
    pBuffer=ByteStr(pBuffer,pSegnale->Point(),&len);

    std::string s;
    std::stringstream out;
    out << pSegnale->TipoComp;
    s = out.str();
    pBuffer=ByteStr(pBuffer,s.at(0)-48,&len);
    pBuffer=ByteStr(pBuffer,s.at(1)-48,&len);


    for(int i=0; i< m_StepCount;i++){
        for(int j=0;j<pSegnale->m_Step[i].StepCount;j++ ){
          pBuffer=ByteStr(pBuffer,i,&len);
          int rampa=pSegnale->m_Step[i].Step[j].Rampa;
          rampa/=1000;
          pBuffer=WORDStr(pBuffer,pSegnale->m_Step[i].Step[j].Inf,&len);
          pBuffer=WORDStr(pBuffer,pSegnale->m_Step[i].Step[j].Start,&len);
          pBuffer=WORDStr(pBuffer,pSegnale->m_Step[i].Step[j].Stop,&len);
          pBuffer=WORDStr(pBuffer,rampa,&len);
          if(len>D_BUFFER-strlen(FINE_MSG)){
              pErrore.Errore(true,E_PROCESSO_CORROTTO,0);
              break;
          }
        }
    }

    if((TipoApp==TipoApp_PlcNet) & Debug){
        std::string str="Spedisco CompStep " ;
        str.append(intToString((int)pSegnale->TipoComp) + "\t");
        MostraEvento(str,Color::FG_DEFAULT);
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    if(!m_IsLoadDaFile){
        std::string processo=getHomePath();
        if(pData.ProcessoIsOn){
            processo.append(PLC_CICLO_PATH);
            processo.append(PLC_PROCESSO);
            processo.append(intToString(pData.File.CicloId));
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


std::string CProcessa::Processo_GetFileNome(){

    std::string processo=getHomePath();
    processo.append(PLC_PROCESSO_PATH);
    processo.append("/");
    processo.append(intToString(pData.File.ProcessoId));
    processo.append("_");
    processo.append(pData.File.ProcessoNome);


    return processo;
}


WORD CProcessa::Processo_GetRecord(char *pStr){

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

bool CProcessa::Processo_LoadDaFile(){

    struct stat buf;
    stat(Processo_GetFileNome().data(), &buf);
    int Record=buf.st_size;
    Record/=D_PROCESSO;
    if(Record<255){
       pData.File.ProcessoRecord=Record;
    }else{
        pErrore.Errore(true,E_PROCESSO_NO_FILE,0);
        return false;
    }

    FILE* fp = fopen(Processo_GetFileNome().data(), "rb");
    if (fp){
        char buffer[D_PROCESSO];
        m_IsLoadDaFile=true;
        for (int i=0; i<Record;i++) {
            char *pBuffer=buffer;
            fseek(fp, i*D_PROCESSO , SEEK_SET);
            fread(pBuffer, D_PROCESSO, 1, fp);
            if(!Processo_Load(++pBuffer)){
               pData.File.ProcessoId=0;
               fclose(fp);
               pErrore.Errore(true,E_PROCESSO_CORROTTO,0);
               m_IsLoadDaFile=false;
               pData.ProcessoCorrotto=true;
               return false;
               break;
            }
        }
        fclose(fp);
        m_IsLoadDaFile=false;
    }else{
        pData.File.ProcessoRecord=0;
        pData.File.ProcessoId=0;
        pErrore.Errore(true,E_PROCESSO_NO_FILE,0);
        return false;
    }

    ///VERIFICA SE CI SONO GLI STEP
    for(int i=0; i<pData.ProStepCount;i++ ){
        if((m_Step[i].TipoStep==TipoStep_Coating) | (m_Step[i].TipoStep==TipoStep_Etching)){
            int StepCount=Bias->SetV->StepCount(i);
            for(int j=0;j<SorgenteCount; j++){
                StepCount+=Bias->Sorgente[j].SetI->StepCount(i);
            }
            for(int j=0;j<GasCount; j++){
                StepCount+=GGas[j].SetG->StepCount(i);
            }
            if(StepCount!=m_Step[i].CompStepCount){
                if(Debug){
                    std::string str;
                    str.append("ERRORE Step ");
                    str.append(intToString(i));
                    str.append(" TipoStep ");
                    str.append(intToString(m_Step[i].TipoStep));
                    str.append(" CompStepCount ");
                    str.append(intToString(StepCount));
                    str.append(" Su ");
                    str.append(intToString(m_Step[i].CompStepCount));
                    MostraEvento(str,Color::FG_RED);
                }
                pErrore.Errore(true,E_PROCESSO_CORROTTO, 0);
                pData.ProcessoCorrotto=true;
            }

        }
    }

    ////VERIFICA BIAS NULLO
    for(int i=0; i<pData.ProStepCount;i++ ){
        if((m_Step[i].TipoStep==TipoStep_Coating) | (m_Step[i].TipoStep==TipoStep_Etching)){
            WORD TempoOn=0;
            for(int j=0;j<SorgenteCount; j++){
                //if(Bias->Sorgente[j].Enabled)
                TempoOn=__max(TempoOn, Bias->Sorgente[j].SetI->OnTime(i));
            }
            int TempoOnBias=(int)Bias->SetV->OnTime(i);
            if(TempoOn>TempoOnBias){
                pData.ProcessoCorrotto=true;
                pErrore.Errore(true,E_BIAS_IS_NOT_ON, 0);
                return false;

          }
        }
    }

    ///VERIFICA GAS NULLO
 /*   bool GasIsZero=false;
    for(int i=0;i<m_StepCount;i++){
        if((m_Step[i].TipoStep==TipoStep_Coating) | (m_Step[i].TipoStep==TipoStep_Etching) ){
            GasIsZero=true;
            break;
        }
    }


    for(int i=0;i<m_StepCount;i++){
       if((m_Step[i].TipoStep==TipoStep_Coating) | (m_Step[i].TipoStep==TipoStep_Etching) ){
           for(int j=0;j<GasCount;j++)
            GasIsZero=__min(GasIsZero,GGas[j].SetG->IsZero(m_Step[i].Step));
       }

    }

    if(GasIsZero){
        pData.ProcessoCorrotto=true;
        pErrore.Errore(true,E_STEP_GAS_ZERO, 0);
        return false;
    }*/

    return true ;
}


WORD CProcessa::Processo_Load(char *pStr)
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

void CProcessa::Processo_UnLoad()
{
    for(int i=0; i< SorgenteCount;i++ )
        Bias->Sorgente[i].SetI->UnLoad();

    for(int i=0; i< GasCount;i++ )
        GGas[i].SetG->UnLoad();

    Bias->SetV->UnLoad();

    return;

}


WORD CProcessa::Processo_LoadCompStep(char *Buffer)
{
   CompFileStepType pStep;
   CIOSegnale *pSegnale=NULL;
   int CompTipo, CompIndex;

   pData.File.ProcessoRecord++;
   pData.StepCount++;
   char *pStr=Buffer;
   pStr++;

   pStep.Slot=*pStr++;
   pStep.Point=*pStr++;
   CompTipo=*pStr++;
   CompIndex=*pStr++;
   if((CompTipo==2) & (CompIndex==3))
       CompIndex=3;
   pData.ComStepLoad=CompTipo*10+CompIndex;

   if(CompTipo==TipoComp_Bias){
       pSegnale=Bias->SetV;
   }
   else if(CompTipo==TipoComp_Sorgente){
	 if(CompIndex<SorgenteCount)
        pSegnale=Bias->Sorgente[CompIndex].SetI;
	 else
        pData.ProcessoCorrotto=true;
   }
   else if(CompTipo==TipoComp_Gas){
	  if(CompIndex<GasCount)
	    pSegnale=GGas[CompIndex].SetG;
	  else
        pData.ProcessoCorrotto=true;
   }
   else
       pData.ProcessoCorrotto=true;

   if(pSegnale==NULL)
       pData.ProcessoCorrotto=true;

   if(*pStr==';')
      pData.ProcessoCorrotto=true;


   if(pData.ProcessoCorrotto){
       pStr=Buffer;
       pStr++;
       memcpy(pStr,FINE_MSG,strlen(FINE_MSG));
       return 8;
   }

   pSegnale->UnLoad();
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
        if(pStep.Comp.ProStep>=m_StepIndex){
			pSegnale->Load( pStep.Comp.Inf,pStep.Comp.ProStep,pStep.Comp.Step, pStep.Comp.Start, pStep.Comp.Stop, pStep.Comp.Rampa);
		}
		if(pStep.Comp.Step++>D_STEP)
            break;
   }while(*pStr!=';');
   pSegnale->Aggiornato();

   if(Debug){
       std::string str;
       str.append(intToString((int)CompTipo));
       str.append(intToString((int)CompIndex) + "\t");
       pData.ComStepLoad=CompTipo*10;
       pData.ComStepLoad+=CompIndex;
       if(CompTipo==TipoComp_Bias)
         str.append("Carico CompStep Bias" );
       else if(CompTipo==TipoComp_Gas)
         str.append("Carico CompStep Gas" );
       else if(CompTipo==TipoComp_Sorgente)
         str.append("Carico CompStep Sorgente" );

       if(pData.ProcessoIsOn)
         MostraEventoProcesso(str,Color::FG_GREEN);
       else
         MostraEvento(str,Color::FG_GREEN);
       pSegnale->PrintStep();
   }

   pStr=Buffer;
   pStr-=2;
   return Processo_GetSendCompStep(pStr,pSegnale);

}

void CProcessa::Processo_LoadStep(int Step,int TipoStep,int Repit,int Tempo,int Pressione,int Potenza,int Temperatura)
{

    m_Step[Step].Tempo=Tempo;
    m_Step[Step].TipoStep=TipoStep;
    m_Step[Step].Step=Step;
    m_Step[Step].Pressione=Pressione;
    m_Step[Step].Potenza=Potenza;
    m_Step[Step].Temperatura=Temperatura;
    m_Step[Step].Repit=1;
    m_Step[Step].RepitCount=Repit;
    m_Step[Step].PerBiasOnTime=100;
    m_Step[Step].SorgentiOn=0;

}

WORD CProcessa::Processo_LoadStep(char *Buffer)
{
   DWORD DTime;
   pData.StepCount=1;
   pData.File.ProcessoRecord=1;

   m_StopTime=0;
   char *pStr=Buffer;
   pStr++;
   pData.ComStepLoad=0;
   pData.File.ProcessoId=StrWORD(&pStr);
   memset(pData.File.ProcessoNome,0,UTENTE_SIZE);
   memcpy(pData.File.ProcessoNome,pStr,UTENTE_SIZE);
   pStr+=UTENTE_SIZE;
   m_StepCount=*pStr++;
   pData.ProStepCount=m_StepCount;
   pData.ProSubStepCount=*pStr++;

   if(m_StepCount > D_STEP){
      m_StepCount= D_STEP ;
   }

   pData.ProcessoCorrotto=false;
   ///CONTROLLA FILE
   for(int i=0;i< m_StepCount; i++){
       StepType pStep;
       memcpy(&pStep,pStr,sizeof(StepType));
       pStr+=sizeof(StepType);
       if(pStep.ProStep!=i){
           pData.ProcessoCorrotto=true;
           break;
       }
   }

   if(pData.ProcessoCorrotto==true ){
       pStr=Buffer;
       pStr++;
       memcpy(pStr,FINE_MSG,strlen(FINE_MSG));
       MostraEvento("Carico Step Processo Corrotto",Color::FG_RED);
       return 8;
   }

   if(!pData.ProcessoIsOn){
      for(int i=0;i<D_STEP;i++){
        memset(&m_Step[i],0,sizeof(StepType));
      }
      for(int i=0;i<GasCount;i++)
          GGas[i].SetG->UnLoad();
      for(int i=0;i<SorgenteCount;i++)
          Bias->Sorgente[i].SetI->UnLoad();
      Bias->SetV->UnLoad();
   }


   pStr=Buffer;
   pStr+=(5+UTENTE_SIZE);

   for(int i=0;i< m_StepCount; i++){
       int OldRepit=m_Step[i].Repit;
       memcpy(&m_Step[i],pStr,sizeof(StepType));
       pStr+=sizeof(StepType);
       if(m_Step[i].RepitCount==0)
          m_Step[i].RepitCount=1;
       if(m_Step[i].Tempo==0)
          m_Step[i].Tempo=10000;
       DTime=(DWORD)(m_Step[i].Tempo);
       if(i>=m_StepIndex){
         if(i==m_StepIndex && m_StepTimer.IsAttivo()){
           if(DTime>m_StepTimer.Time()){
             m_StepTimer.StopTime(DTime);
             m_Step[i].Tempo=DTime;
           }
           m_Step[i].Repit=OldRepit;
           Resistenza.GoToStep(m_Step[i].Potenza,m_Step[i].Temperatura,m_Step[i].Pressione);
           Vuoto.GoToStep(m_Step[i].Pressione);
         }else{
           m_Step[i].Repit=1;
         }
       }
   }

   m_StopTime=0;
   for(int i=0;i<m_StepCount;i++)
      m_StopTime+=m_Step[i].Tempo*m_Step[i].RepitCount;

   if(m_Timer.IsAttivo())
       m_Timer.StopTime(m_StopTime);
   else
       m_Timer(m_StopTime);

   if(Debug){
       std::string str;
       str.append("Processo PLC ");
       str.append(pData.File.ProcessoNome);
       str.append(" ProcessoId ");
       str.append(intToString(pData.File.ProcessoId));
       MostraEvento(str,Color::FG_GREEN);
       for(int i=0;i< m_StepCount; i++){
           std::string str;
           str.append("Step ");
           str.append(intToString(i));
           str.append(" TipoStep ");
           str.append(intToString(m_Step[i].TipoStep));
           str.append(" Tempo ");
           str.append(intToString(m_Step[i].Tempo));
           str.append(" Repit ");
           str.append(intToString(m_Step[i].RepitCount));
           str.append(" Potenza ");
           str.append(intToString(m_Step[i].Potenza));
           str.append(" Temperatura ");
           str.append(intToString(m_Step[i].Temperatura));
           str.append(" CompStepCount ");
           str.append(intToString(m_Step[i].CompStepCount));
           str.append(" Pressione ");
           str.append(intToString(m_Step[i].Pressione));
           MostraEvento(str,Color::FG_DEFAULT);
       }
   }


   pStr=Buffer;
   pStr-=2;
   return Processo_GetSendStep(pStr,pData.File.ProcessoId,pData.File.ProcessoNome);
}

WORD CProcessa::Processo_GetSendStep( char *Buffer ,int idprocesso,char *processo){

    char *pBuffer=Buffer;
    int len=0;

    pBuffer=ByteStr(pBuffer,TipoMessaggio_CaricaProcesso ,&len);//MSG_SETPROCESSO
    pBuffer=ByteStr(pBuffer,TipoComando_Load,&len);
    pBuffer=ByteStr(pBuffer,'S',&len);
    pBuffer=WORDStr(pBuffer,idprocesso,&len);
    memcpy(pBuffer,processo,UTENTE_SIZE);
    pBuffer+=UTENTE_SIZE;
    len+=UTENTE_SIZE;
    pBuffer=ByteStr(pBuffer,m_StepCount,&len);
    pBuffer=ByteStr(pBuffer,0,&len);

    for(int i=0; i< m_StepCount;i++){
        memcpy(pBuffer,&m_Step[i],sizeof(StepType));
        pBuffer+=sizeof(StepType);
        len+=sizeof(StepType);
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);


    if((TipoApp==TipoApp_Plc) & !m_IsLoadDaFile){
        pBuffer=Buffer;
        std::string processo=getHomePath();
        if(pData.ProcessoIsOn){
            processo.append(PLC_CICLO_PATH);
            processo.append(PLC_PROCESSO);
            processo.append(intToString(pData.File.CicloId));
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
#endif



WORD  CProcessa::Macchina_InitFile(char *Buffer)
{
  int len=0;
  char  *pBuffer=Buffer;
  int i;
  
  pBuffer++;
  if(*pBuffer){
    pBuffer++;
    pData.File.Ciclo=StrWORD(&pBuffer);
    pData.File.TavGiri=StrWORD(&pBuffer);
    pData.File.TavOnTime=StrWORD(&pBuffer);
    pData.File.TavOffTime=StrWORD(&pBuffer);
    pData.File.TurboOnTime=StrDWORD(&pBuffer);
    pData.File.MeccanicaOnTime=StrDWORD(&pBuffer);
    pData.File.ModBusPorta=StrDWORD(&pBuffer);
	pData.File.ModBusComunica=StrByte(&pBuffer);
	pData.File.SorgentiOn=StrByte(&pBuffer);
    for(i=0; i<10;i++)
        pData.File.TriggerCount[i]=StrDWORD(&pBuffer);
    for(i=0; i<10;i++)
        pData.File.Ah[i]=StrDWORD(&pBuffer);
    for(i=0; i<3;i++)
		pData.File.Enabled[i]=StrByte(&pBuffer);


  }
  pBuffer=Buffer;
  memset(pBuffer,'\0',D_BUFFER);
  pBuffer=ByteStr(pBuffer,TipoMessaggio_InitFile,&len);
  pBuffer=WORDStr(pBuffer,pData.File.Ciclo,&len);
  pBuffer=WORDStr(pBuffer,pData.File.TavGiri,&len);
  pBuffer=WORDStr(pBuffer,pData.File.TavOnTime,&len);
  pBuffer=WORDStr(pBuffer,pData.File.TavOffTime,&len);
  pBuffer=DWORDStr(pBuffer,pData.File.TurboOnTime,&len);
  pBuffer=DWORDStr(pBuffer,pData.File.MeccanicaOnTime,&len);
  pBuffer=DWORDStr(pBuffer,pData.File.ModBusPorta,&len);
  pBuffer=ByteStr(pBuffer,pData.File.ModBusComunica,&len);
  pBuffer=ByteStr(pBuffer,pData.File.SorgentiOn,&len);
  for(i=0; i<10;i++)
      pBuffer=DWORDStr(pBuffer,pData.File.TriggerCount[i],&len);
  for(i=0; i<10;i++)
      pBuffer=DWORDStr(pBuffer,pData.File.Ah[i],&len);
  for(i=0; i<3;i++)
	  pBuffer=ByteStr(pBuffer,pData.File.Enabled[i],&len);

  memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
  len+=strlen(FINE_MSG);

  return len;
}



WORD CProcessa::Ciclo_GetMsg(char *pStr){

    pData.MessaggioLen=0;
    memset(Msg,0,D_MSG);
    if( pStr != NULL  ){
       pStr+=2;
       int step=StrWORD(&pStr);
       if((step>pData.EventoProcessoCount) | !pData.File.CicloId){
           pStr-=4;
           return Macchina_GetNetData(pStr);
       }
       pData.EventoProcessoDi=step;
       std::string path=getHomePath();
       path.append(PLC_CICLO_PATH);
       path.append(PLC_EVENTO);
       path.append(intToString(pData.File.CicloId));
       FILE* fp = fopen(path.data(), "r");
       if (fp){
           int pos=D_MSG*step;
           fseek(fp, pos , SEEK_SET);
           fread(Msg, D_MSG, 1, fp);
           pData.MessaggioLen=D_MSG;
           pStr-=4;
           fclose(fp);
       }
    }

    return Macchina_GetNetData(pStr);
}

#ifdef NUOVE_MODIFICHE
WORD  CProcessa::Macchina_GetNetData(char *Buffer)
{
  int len=0,Tmp=0;
  char  *pBuffer;

  pData.EnabledVenting =Vuoto.EnabledVenting();
  pData.EventoProcessoCount=getMostraEventoProcessoCount();
  pData.ErrCount=ErrorCount();
#ifndef NUOVA_CLASSE
  pData.Simula=Plc.IsSimula();
#endif
  pBuffer=Buffer;
  memset(pBuffer,'\0',D_BUFFER);
  pBuffer=ByteStr(pBuffer,TipoMessaggio_GetData,&len);
  pBuffer=WORDStr(pBuffer,Impianto,&len);
  pBuffer=ByteStr(pBuffer,VERSIONESOFTWARE,&len);
  pBuffer=ByteStr(pBuffer,pData.StatoMacchina,&len);
  pBuffer=ByteStr(pBuffer,pData.StatoVuoto,&len);
  pBuffer=ByteStr(pBuffer,pData.EnabledVenting,&len);
  pBuffer=ByteStr(pBuffer,pData.Comando,&len);
  pBuffer=ByteStr(pBuffer,pData.ComandoAttivato,&len);
  pBuffer=ByteStr(pBuffer,pData.ComandoRicevuto,&len);
  pBuffer=ByteStr(pBuffer,pData.ComandoEseguito,&len);
  pBuffer=ByteStr(pBuffer,pData.MessaggioRicevuto,&len);
  pBuffer=ByteStr(pBuffer,pData.MessaggioLen,&len);
  pBuffer=ByteStr(pBuffer,pData.SorgentiOn,&len);



  pBuffer=WORDStr(pBuffer,(WORD)(TimerEvent.Time()/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(TimerEvent.StopTime()/1000),&len);
  pBuffer=DWORDStr(pBuffer,(DWORD)(m_MainTimer.Time()),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_Timer.Time()/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_StopTime/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_StepTimer.Time()/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_StepTimer.StopTime()/1000),&len);

  pBuffer=ByteStr(pBuffer,pData.TipoStep,&len);
  pBuffer=ByteStr(pBuffer,pData.StepCount,&len);
  pBuffer=ByteStr(pBuffer,pData.StepIndex,&len);
  pBuffer=ByteStr(pBuffer,pData.RepitCount,&len);
  pBuffer=ByteStr(pBuffer,pData.Repit,&len);

  pData.Resistenza=Resistenza.Potenza();
  pData.BiasLowCount=Bias->BiasLowCount();
#ifndef NUOVA_CLASSE
  pData.ScanTime=Plc.ScanTime;
#endif
  pBuffer=WORDStr(pBuffer,(WORD)(Resistenza.Potenza()),&len);

  pBuffer=WORDStr(pBuffer,pData.File.ProcessoId,&len);
  pBuffer=ByteStr(pBuffer,pData.ComStepLoad,&len);
  pBuffer=DWORDStr(pBuffer,pData.File.Ciclo,&len);
  pBuffer=ByteStr(pBuffer,pData.File.ProcessoRecord,&len);//
  pBuffer=ByteStr(pBuffer,pData.ProcessoIsOn,&len);
#ifndef NUOVA_CLASSE
  pBuffer=ByteStr(pBuffer,Plc.ScanTime,&len);
#endif
  pBuffer=ByteStr(pBuffer,Bias->BiasLowCount(),&len);
  pBuffer=WORDStr(pBuffer,pData.ErrCount,&len);
  pBuffer=ByteStr(pBuffer,pData.ErrPreProcesso,&len);
  pBuffer=ByteStr(pBuffer,pData.ErrProcesso,&len);
  pBuffer=WORDStr(pBuffer,pData.EventoProcessoCount,&len);
  pBuffer=WORDStr(pBuffer,pData.EventoProcessoDi,&len);
  pBuffer=WORDStr(pBuffer,pData.RecordCount,&len);
  pBuffer=ByteStr(pBuffer,pData.ComStepLoad,&len);
  pBuffer=ByteStr(pBuffer,pData.ProcessoCorrotto,&len);
  pBuffer=ByteStr(pBuffer,pData.Simula,&len);
#ifndef NUOVA_CLASSE
  Tmp=Plc.GetPlcSegnali(pBuffer);
#endif
  len+=Tmp;
  pBuffer+=Tmp;

  printErrori();
  memcpy(pBuffer,pData.Errore,m_ErrSize);
  len+=m_ErrSize;
  pBuffer+=m_ErrSize;

  if(pData.MessaggioLen+len<D_BUFFER ){
      if(pData.MessaggioLen){
          memcpy(pBuffer,Msg,pData.MessaggioLen);
          len+=pData.MessaggioLen;
          pBuffer+=pData.MessaggioLen;
      }
      memset(Msg,0,D_MSG);
      pData.MessaggioLen=0;
  }

  memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
  len+=strlen(FINE_MSG);

  return len;


}
#else
WORD  CProcessa::Macchina_GetNetData(char *Buffer)
{
  int len=0,Tmp=0;
  char  *pBuffer;
  
  pData.EnabledVenting =Vuoto.EnabledVenting();
  pData.EventoProcessoCount=getMostraEventoProcessoCount();
  pData.ErrCount=ErrorCount();
  pData.Simula=Plc.IsSimula();
  pBuffer=Buffer;
  memset(pBuffer,'\0',D_BUFFER);
  pBuffer=ByteStr(pBuffer,TipoMessaggio_GetData,&len);
  pBuffer=WORDStr(pBuffer,Impianto,&len);
  pBuffer=ByteStr(pBuffer,VERSIONESOFTWARE,&len);
  pBuffer=ByteStr(pBuffer,pData.StatoMacchina,&len);
  pBuffer=ByteStr(pBuffer,pData.StatoVuoto,&len);
  pBuffer=ByteStr(pBuffer,pData.EnabledVenting,&len);
  pBuffer=ByteStr(pBuffer,pData.Comando,&len);
  pBuffer=ByteStr(pBuffer,pData.ComandoAttivato,&len);
  pBuffer=ByteStr(pBuffer,pData.ComandoRicevuto,&len);
  pBuffer=ByteStr(pBuffer,pData.ComandoEseguito,&len);
  pBuffer=ByteStr(pBuffer,pData.MessaggioRicevuto,&len);
  pBuffer=ByteStr(pBuffer,pData.MessaggioLen,&len);
  pBuffer=ByteStr(pBuffer,pData.SorgentiOn,&len);

  pData.TimeEvent=TimerEvent.Time()/1000;
  pData.TimeStopEvent=TimerEvent.StopTime()/1000;
  pData.MainTime=m_MainTimer.Time();
  pData.Time=m_Timer.Time()/1000;
  pData.StopTime=m_StopTime/1000;
  pData.StepTime=m_StepTimer.Time()/1000;
  pData.StopStepTime=m_StepTimer.StopTime()/1000;


  pBuffer=WORDStr(pBuffer,(WORD)(TimerEvent.Time()/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(TimerEvent.StopTime()/1000),&len);
  pBuffer=DWORDStr(pBuffer,(DWORD)(m_MainTimer.Time()),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_Timer.Time()/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_StopTime/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_StepTimer.Time()/1000),&len);
  pBuffer=WORDStr(pBuffer,(WORD)(m_StepTimer.StopTime()/1000),&len);

  pBuffer=ByteStr(pBuffer,pData.TipoStep,&len);
  pBuffer=ByteStr(pBuffer,pData.StepCount,&len);
  pBuffer=ByteStr(pBuffer,pData.StepIndex,&len);
  pBuffer=ByteStr(pBuffer,pData.RepitCount,&len);
  pBuffer=ByteStr(pBuffer,pData.Repit,&len);

  pData.Resistenza=Resistenza.Potenza();
  pData.BiasLowCount=Bias->BiasLowCount();
  pData.ScanTime=Plc.ScanTime;
  pBuffer=WORDStr(pBuffer,(WORD)(Resistenza.Potenza()),&len);

  pBuffer=WORDStr(pBuffer,pData.File.ProcessoId,&len);
  pBuffer=ByteStr(pBuffer,pData.ComStepLoad,&len);
  pBuffer=DWORDStr(pBuffer,pData.File.Ciclo,&len);
  pBuffer=ByteStr(pBuffer,pData.File.ProcessoRecord,&len);//
  pBuffer=ByteStr(pBuffer,pData.ProcessoIsOn,&len);
  pBuffer=ByteStr(pBuffer,Plc.ScanTime,&len);
  pBuffer=ByteStr(pBuffer,Bias->BiasLowCount(),&len);
  pBuffer=WORDStr(pBuffer,pData.ErrCount,&len);
  pBuffer=ByteStr(pBuffer,pData.ErrPreProcesso,&len);
  pBuffer=ByteStr(pBuffer,pData.ErrProcesso,&len);
  pBuffer=WORDStr(pBuffer,pData.EventoProcessoCount,&len);
  pBuffer=WORDStr(pBuffer,pData.EventoProcessoDi,&len);
  pBuffer=WORDStr(pBuffer,pData.RecordCount,&len);
  pBuffer=ByteStr(pBuffer,pData.ComStepLoad,&len);
  pBuffer=ByteStr(pBuffer,pData.ProcessoCorrotto,&len);
  pBuffer=ByteStr(pBuffer,pData.Simula,&len);

  Tmp=Plc.GetPlcSegnali(pBuffer);
  len+=Tmp;
  pBuffer+=Tmp;

  printErrori();
  memcpy(pBuffer,pData.Errore,m_ErrSize);
  len+=m_ErrSize;
  pBuffer+=m_ErrSize;

  if(pData.MessaggioLen+len<D_BUFFER ){
      if(pData.MessaggioLen){
          memcpy(pBuffer,Msg,pData.MessaggioLen);
          len+=pData.MessaggioLen;
          pBuffer+=pData.MessaggioLen;
      }
      memset(Msg,0,D_MSG);
      pData.MessaggioLen=0;
  }

  memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
  len+=strlen(FINE_MSG);

  return len;


}
#endif



/*void   CProcessa::Macchina_SetSegnale(CIOSegnale   *newSegnale, int newSegnaleCount){

    pSegnali=newSegnale;
    SegnaleCount=newSegnaleCount;

}*/


CIOSegnale *CProcessa::Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala )
{
#ifdef NUOVA_CLASSE
    return NULL;
#else
	CIOSegnale *pTmpSegnale;

   if(pModulo[Slot].Count>=Point){
	  pTmpSegnale=new CIOSegnale;
      pTmpSegnale->operator () (Slot,Point,Descrizione,FondoScala,pModulo[Slot].TipoSlot,OffSet(Slot,Point) ,&Plc);
      pModulo[Slot].setDescrizione(Point,Descrizione);
      return pTmpSegnale;
    }
   else
	return NULL;
#endif

}



CIOSegnale *CProcessa::Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala, double a,double b )
{

#ifdef NUOVA_CLASSE
    return NULL;
#else
    CIOSegnale *pTmpSegnale;

    if(pModulo[Slot].Count>=Point){
      pTmpSegnale=new CIOSegnale;
      pTmpSegnale->operator () (Slot,Point,Descrizione,FondoScala,pModulo[Slot].TipoSlot,OffSet(Slot,Point) ,&Plc);
      pTmpSegnale->setFattore(a);
      pTmpSegnale->setSecondoFattore(b);
      pModulo[Slot].setDescrizione(Point,Descrizione);
      return pTmpSegnale;
    }
   else
    return NULL;
#endif

}



WORD CProcessa::OffSet(int Slot, int Point)
{ 
    int i;
	BYTE RetVal=0;

	for(i=0;i<Slot;i++){ 
      if(pModulo[Slot].IsDigitale )
        RetVal+=pModulo[Slot].Count/8;
	  else
        RetVal+=2*pModulo[Slot].Count;
    }
    if(pModulo[Slot].IsDigitale)
	   return  RetVal;
    else
       return  RetVal+2*(Point);
}
















