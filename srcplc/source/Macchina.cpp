#include "Macchina.h"
#include "Arc6010.h"
#include "Arc6011.h"
#include "Arc6012.h"
#include "../consoleplc/Initfile.h"



CMacchina::CMacchina(int newtipoapp,int newTipoImpianto,int argc, char *argv[])
{

    CCInitFile *Init = new CCInitFile();

    IdVuoto=0;
    IdCiclo=0;
    IdPro=0;
    IdRun=0;
    pImpianto=NULL;
    CamereCount=1;
    DogNonAbilitato= Init->isDog(argc, argv);
    Debug=Init->isDebug(argc, argv);
    MacchinaInManuale=Init->isManuale(argc, argv);
    PorteAperte=Init->isPorta(argc, argv);

    TipoApp=newtipoapp;
    Impianto=newTipoImpianto;
    ResetTickCount();
    setImpianto(Impianto);


    m_PowOn=false;
    m_ResetGas=false;
    m_Macchina_ResetGas=false;
    NewEvento=false;
    PowOutdig=NULL;
    m_Comado=255;
    m_ManualeSet=false;

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
    pErrore.append(E_MAX_RISCALDO_COUNT,"E_MAX_RISCALDO_COUNT"); //E_FILE_INIT_NON_ESISTE
    pErrore.append(E_TEMP_ON_ACQUA_ALLARM,"E_TEMP_ON_ACQUA_ALLARM");
    pErrore.append(E_FILE_INIT_NON_ESISTE,"E_FILE_INIT_NON_ESISTE");
    pErrore.append(E_FILE_PROCESSI_TROPPI,"E_FILE_PROCESSI_TROPPI");
    pErrore.append(E_ACQUA_NON_FLUSSA,"E_ACQUA_NON_FLUSSA");
    pErrore.append(E_RESET_MACCHINA,"E_RESET_MACCHINA");
    pErrore.append(E_TROPPI_CATODI_SPENTI,"E_TROPPI_CATODI_SPENTI");
    pErrore.append(E_TERMOSTATO,"E_TERMOSTATO");

    pErroreDue.append(E_DOG,"E_DOG");
    pErroreDue.append(E_MANUALE,"E_MANUALE");
    pErroreDue.append(E_PORTE_APERTE,"E_PORTE_APERTE");
    pErroreDue.append(E_COMADO_ERRATO,"E_COMADO_ERRATO");
    pErroreDue.append(E_MACCHINA_IN_PROCESSO,"E_MACCHINA_IN_PROCESSO");
    pErroreDue.append(E_MACCHINA_IN_RUN,"E_MACCHINA_IN_RUN");
    pErroreDue.append(E_MACCHINA_DUE_PORTA,"E_MACCHINA_DUE_PORTA");
    pErroreDue.append(E_MACCHINA_BIAS_NEXT,"E_MACCHINA_BIAS_NEXT");


    Macchina_InitSegnali();
    Macchina_PrintErrori();


    int slot;
    int point;
    int valore;
    bool IsSegnale=Init->isSetSegnale(argc, argv,&slot,&point,&valore);
    if(IsSegnale){
        pModulo[slot].setSegnale(point,valore);
    }

}


std::string   CMacchina::Macchina_GetInitFileNome(){

    std::string nomeFile=getRunPath();
    nomeFile.append(PLC_MONITORIZZA);
    nomeFile.append("_");
    nomeFile.append(intToString(getImpianto()));

    return nomeFile;
}

std::string   CMacchina::Macchina_GetRegolaFileNome(){

    std::string nomeFile=getRunPath();
    nomeFile.append(PLC_REGOLA);
    nomeFile.append("_");
    nomeFile.append(intToString(getImpianto()));

    return nomeFile;
}

void CMacchina::Macchina_Main(){

    bool RetVal=false;

    if(IdVuoto==1){
        DataType      *mData=&pData[IdVuoto];
        IdVuoto=1;
    }
    else{
       DataType      *mData=&pData[IdVuoto];
       IdVuoto=0;
    }

    bool isMacchinaInvuoto=false;
    for(int i=0;i<CamereCount;i++){
        isMacchinaInvuoto=__max(isMacchinaInvuoto,Vuoto[i].Meccanica.PowIsOn->Segnale());
    }
    setMacchinaInVuoto(isMacchinaInvuoto);


    if((pData[0].MainTime<pData[IdVuoto].LastTime)
            | (pData[0].MainTime>=pData[IdVuoto].LastTime+10) ){
        pData[0].LastTime=pData[0].MainTime;
        FILE* fd = fopen(Macchina_GetInitFileNome().data(), "w");
        if (fd){
           for(int i=0; i<CamereCount; i++){
               FileType     *mData=&pData[i].File;
               pData[i].File.TavOnTime=Vuoto[i].Tavola.OnTime();
               pData[i].File.TavOffTime=Vuoto[i].Tavola.OffTime();
               pData[i].File.MeccanicaOnTime=Vuoto[i].Meccanica.OnTime();
               pData[i].File.TurboOnTime=Vuoto[i].Turbo.OnTime();
               fwrite(&pData[i].File, sizeof(pData[i].File), 1, fd);
           }

        }
        fclose(fd);
    }

    pErroreDue.Errore(DogNonAbilitato,E_DOG,E_TIME);
    pErroreDue.Errore(MacchinaInManuale,E_MANUALE,E_TIME);
    pErroreDue.Errore(PorteAperte,E_PORTE_APERTE,E_TIME);
    pErroreDue.Errore();
    //Macchina_PrintErrori();
    pData[IdVuoto].EnabledVenting =Vuoto[IdVuoto].EnabledVenting();
    pData[IdVuoto].EventoProcessoCount=getMostraEventoProcessoCount();
    pData[IdVuoto].ErrCount=ErrorCount();
    pData[IdVuoto].ProcessoIsEnabled=Vuoto[IdVuoto].Penning->ProcessoIsEnabled();
    pData[IdVuoto].TimeEvent=(DWORD)TimerEvent[IdVuoto].Time();
    pData[IdVuoto].TimeEventStop=(DWORD)TimerEvent[IdVuoto].TimeStop();
    pData[IdVuoto].MainTime=(DWORD)MainTimer.Time();
    pData[IdVuoto].Time=(DWORD)Timer[IdVuoto].Time();
    pData[IdVuoto].TimeStop=(DWORD)Timer[IdVuoto].TimeStop();
    pData[IdVuoto].StepTime=(DWORD)StepTimer[IdVuoto].Time();
    pData[IdVuoto].StepTimeStop=(DWORD)StepTimer[IdVuoto].TimeStop();
    pData[IdVuoto].Resistenza=Vuoto[IdVuoto].Resistenza->Potenza();
    pData[IdVuoto].File.MeccanicaOnTime=Vuoto[IdVuoto].Meccanica.OnTime();
    pData[IdVuoto].File.TurboOnTime=Vuoto[IdVuoto].Turbo.OnTime();

    pData[IdVuoto].ErrMacchina=pErrore.Errore();

    //DataType mData=pData[IdVuoto];
    pData[IdVuoto].StatoVuoto=Vuoto[IdVuoto].Stato();
    Macchina_FlussaAcqua();
    if(NewEvento){
      TimerEvent[IdVuoto].Timer();
    }

    if((pData[IdVuoto].Comando!=CTipoComando::Manuale) & (pData[IdVuoto].Comando!=CTipoComando::Automatico))
         m_Comado=pData[IdVuoto].Comando;

    pData[IdVuoto].StatoVuoto=Vuoto[IdVuoto].Stato();
    MainTimer.Time();

    if(Vuoto[IdVuoto].Camera.Reset->Segnale())
        pData[IdVuoto].Comando=CTipoComando::SpegniTutto;


    switch(pData[IdVuoto].Comando){
      case CTipoComando::NextStep:
      case CTipoComando::Reset :
      case CTipoComando::StartProcesso:
      case CTipoComando::StopProcesso:
      case CTipoComando::Preset:
        break;
      case CTipoComando::ReStart:
        system("shutdown -r");
        break;
      case CTipoComando::SpegniTutto:
        for(int i=0; i<ModuloCount; i++)
            pModulo[i].SetOff();
        for(int i=0;CamereCount; i++){
            Vuoto[i].Tavola.SetOff();
            Vuoto[i].Turbo.SetOff();
            Vuoto[i].Meccanica.SetOff();
            for(int j=0;j<SorgenteCount;j++)
              Bias[i].Sorgente[j].SetOff();
            Bias[i].SetOff();
            Vuoto[i].Resistenza->SetOff();
        }
        for(int i=0;i<GasCount;i++)
          GGas[i].SetOff();
        break;
      case CTipoComando::PlcManule:
        pData[IdVuoto].StatoMacchina=CStatoMacchina::PlcManule;
        break;
      case CTipoComando::Manuale:
        if((pData[IdVuoto].StatoVuoto==CStatoMacchina::Vacuum)
                | (pData[IdVuoto].StatoVuoto==CStatoMacchina::InVacuum))
            Vuoto[IdVuoto].Set();
        Macchina_Run();
        pData[IdVuoto].StatoMacchina=CStatoMacchina::Manuale;
        break;
      case CTipoComando::Automatico:
        if(!Macchina_Reset())
            break;
        Macchina_ResetErrore();
        pData[IdVuoto].Comando=m_Comado;
        pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
        m_ManualeTimeOut.Reset();
        m_IsStartProcesso=false;
        break;
      case CTipoComando::Impara:
        if(Vuoto[IdVuoto].Tavola.Impara()){
          pData[IdVuoto].Comando=CTipoComando::Reset;
          Vuoto[IdVuoto].Tavola.Reset();
          pData[IdVuoto].File.TavOnTime=Vuoto[IdVuoto].Tavola.OnTime();
          pData[IdVuoto].File.TavOffTime=Vuoto[IdVuoto].Tavola.OffTime();
        }else{
          pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
        }
        break;
      case CTipoComando::GiraTavola:
        Vuoto[IdVuoto].Tavola.Set();
        if(Vuoto[IdVuoto].Tavola.Errore()){
           Vuoto[IdVuoto].Tavola.Reset();
           pData[IdVuoto].Comando=CTipoComando::Reset;
        }
        break;
      case CTipoComando::StendBy:
        if(Vuoto[IdVuoto].Errore()){
           pData[IdVuoto].Comando=CTipoComando::StopVuoto;
           break;
        }
        if(!m_IsStartProcesso){
            RetVal=true;
            for(int i=0;i<SorgenteCount;i++)
              RetVal=__min(RetVal,Bias[IdCiclo].Sorgente[i].StendBy());
            for(int i=0;i<GasCount;i++)
              RetVal=__min(RetVal,GGas[i].StendBy());
            RetVal=__min(RetVal,Bias[IdCiclo].StendBy());
            RetVal=__min(RetVal,Vuoto[IdVuoto].Resistenza->StendBy());
            if(RetVal){
               pData[IdVuoto].ComandoAttivato=CTipoComando::StendBy;
               Timer[IdVuoto].StendBy();
               StepTimer[IdVuoto].StendBy();
               Vuoto[IdVuoto].Stato();
               NewEvento=false;
            }
            else
               pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
        }else{
            Vuoto[IdVuoto].Resistenza->StendBy();
        }
        break;
      case CTipoComando::ResetStendBy:
        if(!m_IsStartProcesso){
            RetVal=true;
            for(int i=0;i<SorgenteCount;i++)
              RetVal=__min(RetVal,Bias[IdCiclo].Sorgente[i].ResetStendBy());
            for(int i=0;i<GasCount;i++)
              RetVal=__min(RetVal,GGas[i].ResetStendBy());
            RetVal=__min(RetVal,Bias[IdCiclo].ResetStendBy());
            RetVal=__min(RetVal,Vuoto[IdVuoto].Resistenza->ResetStendBy());
            if(RetVal){
               pData[IdVuoto].ComandoAttivato=CTipoComando::ResetStendBy;
               Timer[IdVuoto].ResetStendBy();
               StepTimer[IdVuoto].ResetStendBy();
               pData[IdVuoto].StatoMacchina=CStatoMacchina::Reset;
               if(pData[IdVuoto].NextComando==CTipoComando::StartProcesso)
                  pData[IdVuoto].Comando=CTipoComando::StartProcesso;
               NewEvento=false;
            }
            else
               pData[IdVuoto].StatoMacchina=Vuoto[IdVuoto].Stato();
        }

        break;
      case CTipoComando::Vacuum:
        Vuoto[IdCiclo].Camera.SchermoTurbo->Set(false);
        pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
       /* if(Vuoto[IdVuoto].Resistenza->Errore()){
           pData[IdVuoto].Comando=CTipoComando::StopVuoto;
           break;
        }*/
        if(Vuoto[IdVuoto].Errore() | !Vuoto[IdVuoto].Camera.PortaChiusa->Segnale()){
           pData[IdVuoto].Comando=CTipoComando::StopVuoto;
           break;
        }
        if(Vuoto[IdVuoto].Set()){
          pData[IdVuoto].ComandoEseguito=CTipoComando::Vacuum;
          TimerEvent[IdVuoto].operator ()(0);
          NewEvento=false;
        }
        break;
      case CTipoComando::StopVuoto:
        if(!Macchina_Reset())
            break;
        pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
        if(pData[IdVuoto].ProcessoIsOn){
            pData[IdVuoto].Comando=CTipoComando::StopProcesso;
            break;
        }
        if(!Vuoto[IdVuoto].Reset()){
            pData[IdVuoto].StatoVuoto=CStatoMacchina::InStopVuoto;
            pData[IdVuoto].StatoMacchina=CStatoMacchina::InStopVuoto;
            pData[IdVuoto].TimeStopVuoto=MainTimer.Time();
        }else{
            Vuoto[IdVuoto].ResetErrore();
            if(Vuoto[IdVuoto].EnabledVenting()){
               pData[IdVuoto].ComandoEseguito=CTipoComando::StopVuoto;
               TimerEvent[IdVuoto].operator ()(Vuoto[IdVuoto].getVentigTime());
            }else{
               TimerEvent[IdVuoto].operator ()(0);
               pData[IdVuoto].ComandoEseguito=CStatoMacchina::InStopVuoto;
            }
        }
        NewEvento=false;
        break;
      case CTipoComando::Ventila:
        Vuoto[IdVuoto].Ventila();
        pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
        break;
      case CTipoComando::StopVentila:
        Vuoto[IdVuoto].StopVentila();
        pData[IdVuoto].StatoMacchina=pData[IdVuoto].StatoVuoto;
        break;
      default:
        //int mComando=pData[IdVuoto].Comando;
        pErroreDue.Errore(true,E_COMADO_ERRATO,0);
        pData[IdVuoto].Comando=CTipoComando::Reset;
        break;
    }


    if(CamereCount!=1){
        if(IdVuoto==0){
            IdVuoto++;
        }else{
            IdVuoto=0;
        }
        Vuoto[IdVuoto].IdVuoto=IdVuoto;
    }

}

WORD CMacchina::Macchina_SetSegnale(BYTE *pStr){

    if(!MacchinaInManuale)
        return BufferLen;

    int slot,point;
    WORD Valore;

    pStr+=2;
    slot=StrByte(&pStr);
    StrWORD(&pStr);
    point=StrByte(&pStr);
    Valore=StrWORD(&pStr);
    pModulo[slot].setSegnale(point,Valore);


    return BufferLen;

}

WORD CMacchina::Macchina_InitSegnali()
{

    SorgenteCount=6;
    GasCount=3;
    if(Impianto==ARC_6010)
      Init_Arc6010(this,BP5_BIAS);
    else if(Impianto==ARC_6011)
      Init_Arc6011(this,MDXII_BIAS);
    else if(Impianto==ARC_6012)
      Init_Arc6012(this,BP5_BIAS);
    else
      return 0;

    pScorta =new CScorta(SorgenteCount);

    if(TipoApp==CTipoApp::Client)
        return Impianto;

    for(int i=1;i<ModuloCount; i++){
        std::string str="MODULO [";
        str.append(intToString(i));
        str.append("] ERRORE");
        pErroreModulo.append(i,str.data());
    }

    OpenPort();

    if(getRunPath()!=NULL){
        FILE* fd = fopen(Macchina_GetInitFileNome().data(), "r");
        if (fd){
            for(int i=0;i<CamereCount;i++){
                if((int)fread(&pData[i].File,sizeof (pData[i].File),1, fd)<0){
                   MostraEvento("Impossibile Leggere InitPlc",Color::FG_RED) ;
                   pErrore.Errore(true,E_FILE_INIT_NON_ESISTE,0);
                   pErrore.Errore();
                }else{
                    //FileType     *mData=&pData[i].File;
                    if(pData[i].File.TavGiri==0)
                       pData[i].File.TavGiri=DEF_TAVOLA_GIRI_SECONDO;
                    Vuoto[i].Meccanica.OnTime(pData[i].File.MeccanicaOnTime);
                    Vuoto[i].Turbo.OnTime(pData[i].File.TurboOnTime);
                    Vuoto[i].Tavola.OnTime(pData[i].File.TavOnTime);
                    Vuoto[i].Tavola.OffTime(pData[i].File.TavOffTime);
                    pData[i].EventoProcessoCount=0;
                    pData[i].EventoProcessoDi=0;
                    if(pData[i].File.Ciclo){
                        std::string path=getRunPath();
                        path.append(PLC_CICLO_PATH);
                        path.append(PLC_EVENTO);
                        path.append(intToString(pData[i].File.CicloId));
                        FILE* fp = fopen(path.data(), "r");
                        if (fp){
                            while (!feof(fp)) {
                                char buffer[D_MSG];
                                char *pBuffer=buffer;
                                fread(pBuffer, D_MSG, 1, fp);
                                pData[i].EventoProcessoCount++;
                            }
                            fclose(fp);
                        }
                        setMostraEventoProcessoCount(pData[i].EventoProcessoCount);
                        if(Debug){
                            std::string str="Tavola OnTime Macchina ";
                            str.append(intToString(i));
                            str.append(" ");
                            str.append(intToString(pData[i].File.TavOnTime));
                            MostraEvento(str,Color::FG_GREEN);
                        }
                    }

                }
            }

        }
        else{
            for(int i=0;i<CamereCount;i++){
                memset(&pData[i].File,0,sizeof(FileType));
                pData[i].File.TavGiri=DEF_TAVOLA_GIRI_SECONDO; //E_FILE_INIT_NON_ESISTE
            }
            pErrore.Errore(true,E_FILE_INIT_NON_ESISTE,0);
            pErrore.Errore();
        }
        fd = fopen(Macchina_GetRegolaFileNome().data(), "r");
        if (fd){
            int i=0;
            while (!feof(fd)) {
                double pBuffer;
                fread(&pBuffer, sizeof(double), 1, fd);
                if(pBuffer){
                    if(i< SorgenteCount)
                      Bias[IdCiclo].Sorgente[i++].SetI->setRegola(pBuffer);
                }
            }
            fclose(fd);
        }
        pScorta->Apri();

    }
    for(int i=0;i<GasCount;i++)
       GGas[i].CeckStato();

    for(int j=0 ; j< CamereCount;j++){
        pData[j].Comando=CTipoComando::StendBy;
        Vuoto[j].CeckStato();
        for(int i=0;i<SorgenteCount;i++){
           Bias[j].Sorgente[i].CeckStato();
        }
        pData[j].StatoMacchina=Vuoto[IdVuoto].Stato();
        switch(pData[j].StatoMacchina){
         case  CStatoMacchina::Null:
            pData[j].Comando=CTipoComando::Nessuno;
            pData[j].ComandoAttivato=CTipoComando::Nessuno;
            pData[j].ComandoRicevuto=CTipoComando::Nessuno;
            break;
         case  CStatoMacchina::Vacuum:
            pData[j].Comando=CTipoComando::Vacuum;
            pData[j].ComandoAttivato=CTipoComando::Vacuum;
            pData[j].ComandoRicevuto=CTipoComando::Vacuum;
            break;
         case CStatoMacchina::InVacuum:
            pData[j].Comando=CTipoComando::Vacuum;
            pData[j].ComandoRicevuto=CTipoComando::Vacuum;
            pData[j].ComandoAttivato=CTipoComando::Vacuum;
            break;
         case CStatoMacchina::StopVuoto:
            pData[j].Comando=CTipoComando::StopVuoto;
            pData[j].ComandoAttivato=CTipoComando::StopVuoto;
            pData[j].ComandoRicevuto=CTipoComando::StopVuoto;
         case CStatoMacchina::InStopVuoto:
            pData[j].Comando=CTipoComando::StopVuoto;
            pData[j].ComandoRicevuto=CTipoComando::StopVuoto;
            pData[j].ComandoAttivato=CTipoComando::StopVuoto;
            break;

       }

       Vuoto[j].Tavola.GiriSecondo(pData[j].File.TavGiri);
       Vuoto[j].Tavola.OnTime(pData[j].File.TavOnTime);
       Vuoto[j].Tavola.OffTime(pData[j].File.TavOffTime);
       Vuoto[j].Debug=Debug;
       Vuoto[j].Meccanica.Debug=Debug;
       Vuoto[j].Turbo.Debug=Debug;
       Bias[j].Debug=Debug;
       Vuoto[j].Penning->Debug=Debug;
       Vuoto[j].Tavola.Debug=Debug;
       Vuoto[j].Resistenza->Debug=Debug;
       for(int i=0; i<SorgenteCount;i++){
           Bias[j].Sorgente[i].Catodo->Debug=Debug;
           Bias[j].Sorgente[i].Catodo->AcquaIsOn=Vuoto[j].Camera.AcquaIsOn;
    }

   }

   m_ResetGas=false;
   m_OnOffStepTimer(500);
   m_PowerTimeOut(500*(SorgenteCount+1));
   m_PowerTimeOut.Reset();
   m_OnOffStepTimer.Reset();
   m_GasTimeOut(GAS_TIME_RESET_GAS*(GasCount+1));
   m_ResetTimerOut(TIMEOUT);
   m_ManualeTimeOut(MANUALE_TIME_OUT);

   return Impianto;
}

int CMacchina::Macchina_TragetCambia(char *Buffer){

    int len=pScorta->Cambia(Buffer);
    for(int i=0; i< SorgenteCount;i++){
          Bias[IdCiclo].Sorgente[i].Catodo->OnTime(pScorta->OnTime(i));
    }
    if(Debug)
         pScorta->Print();

    return  len;

}

WORD  CMacchina::Macchina_GetOnTime(char *Buffer)
{
    std::string str ="OnTime Turbo ";
    int len=0;
    char  *pBuffer=Buffer;
    pBuffer++;
    int IdCamera=*pBuffer;
    pBuffer--;
    if(IdCamera>CamereCount-1){
        MostraErrore("Macchina_GetOnTime IdCamera Errata");
        return BufferLen;
    }
    memset(pBuffer,'\0',D_BUFFER);
    pBuffer=ByteStr(pBuffer,CTipoMessaggio::GetOnTime,&len);
    pBuffer=ByteStr(pBuffer,1,&len);
    int tempo=Vuoto[IdCamera].Turbo.OnTime();
    str.append( intToString(tempo));
    pBuffer=DWORDStr(pBuffer,(DWORD)tempo,&len);
    tempo=Vuoto[IdCamera].Meccanica.OnTime();
    str.append( " Meccanica ");
    str.append( intToString(tempo));
    pBuffer=DWORDStr(pBuffer,(DWORD)tempo,&len);
    for(int i=0; i< SorgenteCount;i++){
        double Tempo=Bias[IdCamera].Sorgente[i].Catodo->OnTime();
        pBuffer=DoubleStr(pBuffer,Tempo,&len);
    }
    for(int i=0; i< SorgenteCount;i++)
        pBuffer=WORDStr(pBuffer,Bias[IdCamera].Sorgente[i].Catodo->TrigCount(),&len);

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    if(Debug)
      ;//MostraEvento(str,Color::FG_DEFAULT);

    return len;
}

WORD  CMacchina::Macchina_SetComando(char *Buffer){


    char  *pBuffer=Buffer;
    pBuffer++;
    int CameraId=StrByte(&pBuffer);
    if(CameraId>CamereCount-1){
        MostraErrore("SET-COMENDO MACCHINA NON VALIDA");
        return BufferLen;
    }
    BYTE Comando=StrByte(&pBuffer);

    for(int i=0;i< CamereCount; i++){
        if(pData[i].Comando==CTipoComando::SpegniTutto)
            return BufferLen;
    }


    if(Comando==CTipoComando::StartProcesso){
        m_IsStartProcesso=true;
        for(int i=0; i < CamereCount; i++){
            if(i!=CameraId){
                if(pData[i].Comando==CTipoComando::StartProcesso){
                    pErroreDue.Errore(true,E_MACCHINA_IN_PROCESSO,0);
                    return BufferLen;
                }
                if(pData[i].Comando==CTipoComando::Manuale){
                    pErroreDue.Errore(true,E_MANUALE,0);
                    return BufferLen;
                }
            }
        }
        IdCiclo=CameraId;
        IdPro=CameraId;
        int ProcessoId=StrWORD(&pBuffer);
        if(ProcessoId!=0){
            pData[IdPro].File.ProcessoId=0;
            memset(pData[IdPro].File.ProcessoNome,0,UTENTE_SIZE);
            if((*pBuffer!=';')  & (BufferLen>16)  ){
                pData[IdPro].File.ProcessoId=ProcessoId;
                memcpy(pData[IdPro].File.ProcessoNome,pBuffer,UTENTE_SIZE);
            }

        }

    } else if(Comando==CTipoComando::Manuale) {
        for(int i=0; i < CamereCount; i++){
            if(i!=CameraId){
                if(pData[i].Comando==CTipoComando::Manuale){
                    pErroreDue.Errore(true,E_MACCHINA_IN_RUN,0);
                    return BufferLen;
                }
                if(pData[i].Comando==CTipoComando::StartProcesso){
                    pErroreDue.Errore(true,E_MACCHINA_IN_PROCESSO,0);
                    return BufferLen;
                }
            }
            m_IsStartProcesso=true;
        }
    }

    //SE IL PROCESSO È IN CORSO ACCETA SOLO STOP PROCESSO
    if(pData[CameraId].ProcessoIsOn){
        if(Comando==CTipoComando::SpegniCatodo){
           pBuffer+=2;
           int IdCatodo=StrWORD(&pBuffer);
           if(IdCatodo<SorgenteCount)
              Bias[CameraId].Sorgente[IdCatodo].Catodo->Enabled=false;
           return BufferLen;
        }else if(Comando!=CTipoComando::StopProcesso){
           m_IsStartProcesso=false;
           return BufferLen;
        }
    }


    if(CameraId==CCamereComando::Entrambe){
       for(int i=0; i< CamereCount; i++)
          pData[i].Comando=Comando;
    }else
       pData[CameraId].Comando=Comando;

    NewEvento=true;
    return BufferLen;
}


WORD  CMacchina::Macchina_GetNetData(char *Buffer)
{
    int len=0,Tmp=0;
    char  *pBuffer;
    pBuffer=Buffer;
    memset(pBuffer,'\0',D_BUFFER);

    pBuffer=ByteStr(pBuffer,CTipoMessaggio::GetData,&len);
    pBuffer=WORDStr(pBuffer,Impianto,&len);
    pBuffer=ByteStr(pBuffer,VERSIONESOFTWARE,&len);
    pBuffer=ByteStr(pBuffer,ScanTime,&len);
    pBuffer=ByteStr(pBuffer,IsSimula(),&len);


    for(int i=0; i<CamereCount;i++){
        pBuffer=ByteStr(pBuffer,pData[i].StatoMacchina,&len);
        pBuffer=ByteStr(pBuffer,pData[i].StatoVuoto,&len);
        pBuffer=ByteStr(pBuffer,pData[i].EnabledVenting,&len);
        pBuffer=ByteStr(pBuffer,pData[i].Comando,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ComandoAttivato,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ComandoRicevuto,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ComandoEseguito,&len);
        pBuffer=ByteStr(pBuffer,pData[i].MessaggioRicevuto,&len);
        pBuffer=ByteStr(pBuffer,pData[i].MessaggioLen,&len);
        pBuffer=ByteStr(pBuffer,pData[i].SorgentiOn,&len);
        pBuffer=WORDStr(pBuffer,(WORD)(pData[i].TimeEvent /1000),&len);
        pBuffer=WORDStr(pBuffer,(WORD)(pData[i].TimeEventStop/1000),&len);
        pBuffer=DWORDStr(pBuffer,(DWORD)(pData[i].MainTime),&len);
        pBuffer=WORDStr(pBuffer,(WORD)(pData[i].Time/1000),&len);
        pBuffer=WORDStr(pBuffer,(WORD)(pData[i].TimeStop/1000),&len);
        pBuffer=WORDStr(pBuffer,(WORD)(pData[i].StepTime/1000),&len);
        pBuffer=WORDStr(pBuffer,(WORD)(pData[i].StepTimeStop/1000),&len);
        pBuffer=DWORDStr(pBuffer,(DWORD)(pData[i].TimeStopVuoto),&len);
        pBuffer=ByteStr(pBuffer,pData[i].TipoStep,&len);
        pBuffer=ByteStr(pBuffer,pData[i].StepCount,&len);
        pBuffer=ByteStr(pBuffer,pData[i].StepIndex,&len);
        pBuffer=ByteStr(pBuffer,pData[i].RepitCount,&len);
        pBuffer=ByteStr(pBuffer,pData[i].Repit,&len);
        pBuffer=WORDStr(pBuffer,(WORD)(Vuoto[i].Resistenza->Potenza()),&len);
        pBuffer=WORDStr(pBuffer,(WORD)(Vuoto[i].Resistenza->TempSet()),&len);
        pBuffer=WORDStr(pBuffer,pData[i].File.ProcessoId,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ComStepLoad,&len);
        pBuffer=DWORDStr(pBuffer,pData[i].File.Ciclo,&len);
        pBuffer=ByteStr(pBuffer,pData[i].File.ProcessoRecord,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ProcessoIsOn,&len);
        pBuffer=WORDStr(pBuffer,pData[i].ErrCount,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ErrPreProcesso,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ErrProcesso,&len);
        pBuffer=WORDStr(pBuffer,pData[i].EventoProcessoCount,&len);
        pBuffer=WORDStr(pBuffer,pData[i].EventoProcessoDi,&len);
        pBuffer=WORDStr(pBuffer,pData[i].RecordCount,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ComStepLoad,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ProcessoCorrotto,&len);
        pBuffer=ByteStr(pBuffer,pData[i].ProcessoIsEnabled,&len);
        pBuffer=ByteStr(pBuffer,MacchinaInManuale,&len);
    }


    Tmp=GetPlcSegnali(pBuffer);
    len+=Tmp;
    pBuffer+=Tmp;

    char *pErrore=pBufferErrore;
    if(pErrore!=NULL)
       memcpy(pBuffer,pBufferErrore,ErrSize);
    len+=ErrSize;
    pBuffer+=ErrSize;

    if(pData[IdVuoto].MessaggioLen+len<D_BUFFER ){
      if(pData[IdVuoto].MessaggioLen){
          char *pMsg=Msg;
          if(pMsg!=NULL){
              memcpy(pBuffer,Msg,pData[IdVuoto].MessaggioLen);
              len+=pData[IdVuoto].MessaggioLen;
              pBuffer+=pData[IdVuoto].MessaggioLen;
          }
          memset(Msg,0,D_MSG);
          pData[IdVuoto].MessaggioLen=0;
      }
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    return len;

}



void CMacchina::Macchina_PrintErrori(){

    return;

    if(!Debug)
        return;

    char *ptmp=pBufferErrore;

    for(int i=0; i<ErrSize/D_COMP_ERRORE ;i++){
        int Tempo=StrWORD(&ptmp);
        BYTE Primo=StrByte(&ptmp);
        BYTE Secondo=StrByte(&ptmp);
        BYTE Terzo=StrByte(&ptmp);
        if((Tempo!=0) & ((Primo!=0) | (Secondo!=0) | (Terzo!=0))){
            QString Nuova="INDEX " + QString::number( i*D_COMP_ERRORE ) + " Tempo " + QString::number( Tempo )
                    + " Primo " + QString::number( Primo ) + " Secondo " + QString::number( Secondo ) + " Terzo " + QString::number( Terzo );
            std::string str;
            str.append(Nuova.toUtf8());
            if(m_oldstr!=str)
               MostraEvento(str,Color::FG_DEFAULT);
            m_oldstr=str;
        }

    }

}


void CMacchina::Macchina_ResetErrore()
{

    MostraEvento("Reset Errore",Color::FG_DEFAULT);
    for(int i=0;i<SorgenteCount;i++)
       Bias[IdCiclo].Sorgente[i].ResetErrore();
    for(int i=0;i<GasCount;i++)
       GGas[i].ResetErrore();
    Bias[IdCiclo].ResetErrore();
    Vuoto[IdCiclo].Resistenza->ResetErrore();
    pErrore.Reset();
    Vuoto[IdCiclo].pErrore.Reset();
    char *ptmp=pBufferErrore;
    memset(ptmp,'\0',ErrSize);
    pData[IdCiclo].ErrProcesso=false;
    pData[IdCiclo].ErrPreProcesso=false;
    setErrore(false);
    ErrorCountReset();

}


void CMacchina::Macchina_SalvaRegola(){

    bool isStabile=true;
    for( int i=0; i<SorgenteCount; i++)
        isStabile=__min(isStabile,Bias[IdCiclo].Sorgente[i].SetI->IsStabile());

    if(isStabile){
        FILE* fd = fopen(Macchina_GetRegolaFileNome().data(), "w");
        if (fd){
            for( int i=0; i<SorgenteCount; i++){
               double val=Bias[IdCiclo].Sorgente[i].SetI->getRegola();
               val*=i*0.34569;
               fwrite(&val, sizeof(double), 1, fd);
            }
            fclose(fd);
        }
    }


}

bool CMacchina::Macchina_OnePowIsOn(){

    for(int i=0;i<SorgenteCount;i++){
      if(Bias[IdCiclo].Sorgente[i].IsOn())
          return true;
    }

    return false;
}

bool CMacchina::Macchina_OneGasIsOn(){

    for(int i=0; i< GasCount; i++){
        if(GGas[i].SetG->Segnale()){
            return true;
        }
    }
    return false;

}


bool CMacchina::Macchina_PowIsOn(int Index)
{

  if(m_PowOn){
      return m_PowOn;
  }

  bool RetVal=true;
  for(int i=0;i<SorgenteCount;i++)
    RetVal=__min(RetVal,Bias[Index].Sorgente[i].IsOn());
  RetVal=__min(RetVal,Bias[Index].IsOn());

  if( RetVal  )
      MostraEvento("POWER NON È ON ",Color::FG_RED);

  return RetVal;
}

bool CMacchina::Macchina_PowIsOff(int Index)
{
  if(!m_PowOn){
      return FalseTrue( m_PowOn);
  }

  bool RetVal=true;

  for(int i=0;i<SorgenteCount;i++)
    RetVal=__min(RetVal,FalseTrue( Bias[Index].Sorgente[i].IsOn()));
  RetVal=__min(RetVal,FalseTrue( Bias[Index].IsOn()));

  if( ! RetVal  )
      MostraEvento("POWER NON È OFF",Color::FG_RED);

  return RetVal;
}


bool CMacchina::Macchina_PowOff(int Index){

    if(!m_PowOn)
      return FalseTrue( m_PowOn);

    if(m_PowerTimeOut.Timer()){
       Bias[Index].ceckPowerError();
       for(int i=0;i<SorgenteCount;i++)
          Bias[Index].Sorgente[i].ceckPowerError();
       m_PowerTimeOut.Reset();
       m_OnOffStepTimer.Reset();
       MostraEvento("ERRORE POWER OFF",Color::FG_RED);
       pErrore.Errore(true,E_RESET_MACCHINA,0);
       pData[IdCiclo].ErrMacchina=pErrore.Errore();
       return true;
    }

    bool isReset=true;

    for(int i=0;i<SorgenteCount;i++)
      isReset=__min(isReset,Bias[Index].Sorgente[i].SetOff());
    if(!isReset)
        return false;

    isReset=__min(isReset,Bias[Index].SetOff());
    if(!isReset)
        return false;

    isReset=__min(isReset,Vuoto[Index].Resistenza->SetOff());

    if(!isReset)
        return false;
    if(CamereCount!=1)
      Bias[Index].BiasNext->Set(false);

    m_PowOn=false;
    m_PowOn=__max(m_PowOn,Vuoto[Index].Resistenza->IsOn());
    for(int i=0;i<SorgenteCount;i++){
       m_PowOn=__max(m_PowOn,Bias[Index].Sorgente[i].IsOn());
    }
    m_PowOn=__max(m_PowOn,Bias[Index].IsOn());
    for(int i=0; i<CamereCount;i++)
      m_PowOn=__max(m_PowOn, FalseTrue( Vuoto[i].Camera.AriaComp->Set(false)));

    if( !m_PowOn ){
       if(Debug)
          MostraEvento("SET POWER OFF ",Color::FG_DEFAULT);
       m_PowerTimeOut.Reset();
       m_OnOffStepTimer.Reset();
    }

    return FalseTrue(m_PowOn);
}

bool CMacchina::Macchina_PowOn(int Index)
{

  if(m_PowOn)
      return m_PowOn;

  if(m_PowerTimeOut.Timer() ){
      Bias[Index].ceckPowerError();
      for(int i=0;i<SorgenteCount;i++)
         Bias[Index].Sorgente[i].ceckPowerError();
      m_PowerTimeOut.Reset();
      m_OnOffStepTimer.Reset();
      MostraEvento("ERRORE POWE ON",Color::FG_RED);
      pErrore.Errore(true,E_RESET_MACCHINA,0);
      pData[Index].ErrMacchina=pErrore.Errore();
      //*Vuoto[IdVuoto].Turbo.Acqua=false;
      return true;
  }

  bool IsOnTrigger=true;
  for(int i=0;i< CamereCount;i++){
    IsOnTrigger=__min(IsOnTrigger,Vuoto[i].Camera.AriaComp->Set(true));
    for(int j=0;j<SorgenteCount;j++){
       IsOnTrigger=__min(IsOnTrigger,Bias[i].Sorgente[j].Catodo->Trigger->Set(false));
    }
  }

  if(!IsOnTrigger)
      return false;

  for(int i=0;i<SorgenteCount;i++)
      Bias[Index].Sorgente[i].ceckCatodoPowErr();

  if(m_OnOffStepTimer.Timer()){
    for(int i=0;i<SorgenteCount;i++){
      if(!Bias[Index].Sorgente[i].IsOn()){
        Bias[Index].Sorgente[i].SetOn();
        m_OnOffStepTimer.Reset();
        return false;
      }
    }
    if(!Bias[Index].IsOn()){
      Bias[Index].SetOn();
      m_OnOffStepTimer.Reset();
      return false;
    }
   }
   else
      return false;

  m_PowOn=true;
  for(int i=0;i<SorgenteCount;i++){
     m_PowOn=__min(m_PowOn,Bias[Index].Sorgente[i].IsOn());
  }
  m_PowOn=__min(m_PowOn,Bias[Index].IsOn());

  if(Index==0){
      m_PowOn=__min(m_PowOn,Bias[Index].BiasNext->Set(false));
     /* if(!m_PowOn)
          return m_PowOn;
      m_PowOn=__min(m_PowOn,FalseTrue( Bias[IdCiclo].BiasIsNext->Segnale()));*/
  }else{
      m_PowOn=__min(m_PowOn,Bias[Index].BiasNext->Set(true));
      /*if(!m_PowOn)
          return m_PowOn;
      m_PowOn=__min(m_PowOn, Bias[IdCiclo].BiasIsNext->Segnale());*/
  }


  if( m_PowOn  ){
     if(Debug)
       MostraEvento("SET POWER ON ",Color::FG_DEFAULT);
     m_PowerTimeOut.Reset();
     m_OnOffStepTimer.Reset();
  }

  return m_PowOn;
}

bool CMacchina::Macchina_FlussaAcqua(){

    WORD TurboTemp=Vuoto[0].Turbo.Temp->Segnale();
    WORD CameraTemp=Vuoto[0].Camera.Temp->Segnale();
    bool IsTempCameraRotta=Vuoto[0].Resistenza->IsTempCameraRotta();
    bool IsTermostaAperto=FalseTrue( Vuoto[0].Camera.Termostato->Segnale());
    bool IsAcquaAperta=Vuoto[0].Camera.Acqua->Segnale();
    bool IsAcquaFlussa=Vuoto[0].Camera.AcquaIsOn->Segnale();
    for(int i=1; i<CamereCount; i++){
        TurboTemp=__max(TurboTemp,Vuoto[i].Turbo.Temp->Segnale());
        CameraTemp=__max(CameraTemp,Vuoto[i].Camera.Temp->Segnale());
        IsTempCameraRotta=__max(IsTempCameraRotta,Vuoto[i].Resistenza->IsTempCameraRotta());
        IsTermostaAperto==__max(IsTermostaAperto,FalseTrue( Vuoto[i].Camera.Termostato->Segnale()));
        IsAcquaAperta==__max(IsAcquaAperta,Vuoto[i].Camera.Acqua->Segnale());
        IsAcquaFlussa==__max(IsAcquaFlussa,Vuoto[i].Camera.AcquaIsOn->Segnale());
    }

    pErrore.Errore(IsTermostaAperto,E_TERMOSTATO,60*E_TIME); //ASPETTO 5 MINUTI PRIMA DI FAR VEDERE ERRORE
    if(IsAcquaAperta){
       pErrore.Errore(FalseTrue( IsAcquaFlussa),E_ACQUA_NON_FLUSSA,4*E_TIME);
       if(pErrore.IsErrore(E_ACQUA_NON_FLUSSA) & pErrore.Errore())
           setAcquaErrore(true);
       else{
           setAcquaErrore(false);
       }
    }else{
       pErrore.Errore(false,E_ACQUA_NON_FLUSSA,2*E_TIME);
       setAcquaErrore(false);
    }

    if(!Macchina_OnePowIsOn()  ){
        if(TurboTemp>PRO_TEMP_ON_ACQUA_TURBO ){
            for(int i=0; i<CamereCount; i++){
                *Vuoto[i].Turbo.Acqua=true;
            }
        }else if(TurboTemp<PRO_TEMP_OFF_ACQUA_TURBO){
            if(!MacchinaInManuale){
                for(int i=0; i<CamereCount; i++){
                    *Vuoto[i].Turbo.Acqua=false;
                }
            }

        }
    }else{
        for(int i=0; i<CamereCount; i++){
            *Vuoto[i].Turbo.Acqua=true;
        }
    }


    if((CameraTemp>PRO_TEMP_ON_ACQUA_CAMERA) | IsTempCameraRotta | IsTermostaAperto){
        for(int i=0; i<CamereCount; i++){
            *Vuoto[i].Camera.Acqua=true;
            if(getSimula()){
                *Vuoto[i].Camera.AcquaIsOn=true;
            }
        }
    }else if(CameraTemp<PRO_TEMP_OFF_ACQUA_CAMERA ){
        for(int i=0; i<CamereCount; i++){
            if(!MacchinaInManuale)
              *Vuoto[i].Camera.Acqua=false;
        }
    }

    if(getErrore()){
        *Vuoto[IdVuoto].Camera.LuceRossa=true;
    }else {
        *Vuoto[IdVuoto].Camera.LuceRossa=false;
    }

    if(pData[IdVuoto].ProcessoIsOn){
        *Vuoto[IdVuoto].Camera.LuceGialla=true;
    }else{
        *Vuoto[IdVuoto].Camera.LuceGialla=false;
        if(pData[IdVuoto].StatoVuoto==CStatoMacchina::EnabledVent){
            *Vuoto[IdVuoto].Camera.LuceVerde=false;
        }else{
            *Vuoto[IdVuoto].Camera.LuceVerde=true;
        }
    }

    return true;
}

void CMacchina::Macchina_Run(){

///CONDUE
    for(int i=0; i < CamereCount; i++){
        if(i!=IdRun){
           *Vuoto[i].Camera.Gas=false;
           *Vuoto[i].Camera.AriaComp=true;
           if(!Vuoto[i].Camera.PortaChiusa->Segnale()){
              pErroreDue.Errore(true,E_MACCHINA_DUE_PORTA,0);
              pData[i].Comando=CTipoComando::Automatico;
              pData[i].IsRun=false;
              return;
           }
        }
    }
///CONDUE

    bool mErrore=Vuoto[IdRun].Errore();

    mErrore=__max(mErrore,Vuoto[IdRun].Resistenza->Errore());
    for(int i=0;i<SorgenteCount;i++){
          mErrore=__max(mErrore,Bias[IdRun].Sorgente[i].Errore());
    }
    mErrore=__max(mErrore,Bias[IdRun].Errore());
    for(int i=0;i<GasCount ;i++){
        mErrore=__max(mErrore,GGas[i].Errore());
    }

    if(mErrore){
        pData[IdRun].Comando=CTipoComando::Automatico;
        pData[IdRun].IsRun=false;
        m_IsStartProcesso=false;
        return;
    }

    if(!pData[IdRun].ProcessoIsEnabled){
        m_ManualeTimeOut.Reset();
        m_IsStartProcesso=false;
        return;
    }

    if(!Vuoto[IdRun].Set()){
        m_ManualeTimeOut.Reset();
        m_IsStartProcesso=false;
        return;
    }


    if(m_ManualeTimeOut.Timer()){
        pData[IdRun].Comando=CTipoComando::Automatico;
        pData[IdRun].IsRun=false;
        m_IsStartProcesso=false;
        return;
    }

    TimerEvent[IdRun].TimeStop(m_ManualeTimeOut.TimeStop());
    TimerEvent[IdRun].Time(m_ManualeTimeOut.Time());

    if(m_ManualeSet){
        //CompType pComp=m_pComp;
        if((m_pComp.TipoComp==CTipoComp::Bias) | (m_pComp.TipoComp==CTipoComp::Gas) | (m_pComp.TipoComp==CTipoComp::Sorgente)){
            if((Bias[IdRun].SetV->Slot()==(int)m_pComp.Slot) & (Bias[IdRun].SetV->Point()==(int)m_pComp.Point)){
                if(m_pComp.Set){
                    if(!Macchina_PowOn(IdRun))
                        return;
                    if(!Bias[IdRun].SetTap(m_pComp.Info))
                        return ;
                    for(int i=0;i<SorgenteCount;i++ ){
                        Bias[IdRun].Sorgente[i].Enabled=false;
                    }
                }else{
                    if(!Macchina_PowOff(IdRun))
                        return;
                    pData[IdRun].Comando=CTipoComando::Automatico;
                    m_ManualeSet=false;
                }
            }
            if(m_pComp.TipoComp==CTipoComp::Gas){
                if(!Vuoto[IdRun].Camera.Gas->Set(true))
                    return ;
                if(!Macchina_OneGasIsOn()){
                  if(m_pComp.Set<GGas[(int)m_pComp.Index].Min())
                    m_pComp.Set=GGas[(int)m_pComp.Index].Min()+1;
                }
            }else if(m_pComp.TipoComp==CTipoComp::Sorgente){
                if(m_pComp.Set<Bias[IdRun].Sorgente[(int)m_pComp.Index].Catodo->Min())
                    m_pComp.Set=Bias[IdRun].Sorgente[(int)m_pComp.Index].Catodo->Min()+10;
            }
            pModulo[(int)m_pComp.Slot].setSegnale((int)m_pComp.Point,(int)m_pComp.Set);
            if(m_pComp.Set==(WORD)pModulo[(int)m_pComp.Slot].getSegnale((int)m_pComp.Point))
                m_ManualeSet=false;
            bool IsSorgenteOn=false;
            for(int i=0;i<SorgenteCount;i++ ){
                if((Bias[IdRun].Sorgente[i].SetI->Slot()==(int)m_pComp.Slot)
                        & (Bias[IdRun].Sorgente[i].SetI->Point()==(int)m_pComp.Point)){
                    IsSorgenteOn=true;
                }
            }
            if(IsSorgenteOn){
               m_ManualeSet=FalseTrue(Vuoto[IdRun].Turbo.Acqua->Set(true));
               for(int i=0;i<SorgenteCount;i++ ){
                   bool isEnabled=GetBit(m_pComp.Enabled,i);
                   Bias[IdRun].Sorgente[i].Enabled=isEnabled ;
               }
               pData[IdRun].SorgentiOn=m_pComp.Enabled;
            }
        }else{
            if(m_pComp.TipoComp==CTipoComp::Riscaldo){
                Vuoto[IdRun].Resistenza->TempSet(m_pComp.Set);
                Vuoto[IdRun].Resistenza->Potenza(100);
                m_ManualeSet=false;
            }
        }
        m_ManualeTimeOut.TimeStop(m_pComp.TimeOut);
    }

    Macchina_FlussaAcqua();
    Vuoto[IdRun].Resistenza->ManualeRun();
    Bias[IdRun].ManualeRun();
    for(int i=0; i< SorgenteCount;i++){
        if(Bias[IdRun].Sorgente[i].Enabled)
           Bias[IdRun].Sorgente[i].ManualeRun();
    }
    for(int i=0; i< GasCount;i++){
        if(GGas[i].SetG->Segnale()){
            if(!Vuoto[IdRun].Camera.AriaComp->Set(true))
                    return;
        }
        GGas[i].ManualeRun();
    }

}

WORD CMacchina::Macchina_Set(char *Buffer){

    char *pBuffer=Buffer;


    ////
    char Tmp[20];
    char *pTmp=Tmp;
    memset(pTmp, 0,20);
    memcpy(pTmp,pBuffer,20);

    pBuffer++;
    int Index=*pBuffer;
    if(CamereCount==2){
        if(((Index==0) & pData[1].IsRun) | ((Index==1 )& pData[0].IsRun)){
            pBuffer--;
            return 20;
        }
    }
    pData[Index].IsRun=true;
    IdRun=Index;
    pBuffer+=4;

    BYTE    TipoComp=StrByte(&pBuffer);
    if((TipoComp==CTipoComp::Gas ) | (TipoComp==CTipoComp::Bias )
            | (TipoComp==CTipoComp::Sorgente ) | (TipoComp==CTipoComp::Riscaldo )){
        m_pComp.TipoComp=TipoComp;
        m_pComp.Index=StrByte(&pBuffer);
        m_pComp.Slot=StrByte(&pBuffer);
        m_pComp.Point=StrByte(&pBuffer);
        m_pComp.Enabled=StrByte(&pBuffer);
        m_pComp.Set=StrWORD(&pBuffer);
        m_pComp.SetSecondo=StrWORD(&pBuffer);
        m_pComp.Info=StrWORD(&pBuffer);
        m_pComp.TimeOut=StrDWORD(&pBuffer);
        m_ManualeSet=true;
    }else{
        MostraEvento("TipoComp Errato",Color::FG_RED);
    }


    int len=0;
    pBuffer=Buffer;
    pBuffer=ByteStr(pBuffer,CTipoMessaggio::Manuale,&len);
    pBuffer=ByteStr(pBuffer,1,&len);
    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    return len;
}

bool CMacchina::Macchina_Reset(){


    if(pData[IdCiclo].ComandoAttivato==CTipoComando::StartProcesso)
         return true;

    if(m_ResetTimerOut.Timer()){
       MostraErrore("ERRORE RESET MACCHINA");
       pErrore.Errore(true,E_RESET_MACCHINA,0);
       pData[IdVuoto].ErrMacchina=pErrore.Errore();
       m_ResetTimerOut.Reset();
       return true;
    }

    if(!Macchina_PowOff(IdCiclo))
        return false;
    if(!Macchina_PowIsOff(IdCiclo)){
       MostraErrore("Errore Reset Ciclo");
    }

    if(!Macchina_ResetGas())
        return false;

    if(!Vuoto[IdVuoto].Resistenza->Reset())
        return false;

    m_ResetTimerOut.Reset();
    return true;
}

bool CMacchina::Macchina_ResetGas(){

    if(m_GasTimeOut.Timer()){
       MostraErrore("ERRORE RESET GAS");
       pErrore.Errore(true,E_RESET_MACCHINA,0);
       pData[IdVuoto].ErrMacchina=pErrore.Errore();
       m_GasTimeOut.Reset();
       return true;
    }

    m_Macchina_ResetGas=true;
    for(int i=0;i< GasCount;i++)
       m_Macchina_ResetGas=__min(m_Macchina_ResetGas,GGas[i].SetOff());

    for(int i=0;i<CamereCount;i++)
       m_Macchina_ResetGas=__min(m_Macchina_ResetGas,Vuoto[i].Camera.Gas->Set(false));

    if(m_Macchina_ResetGas){
       if(Debug)
         MostraEvento("GAS IS RESET",Color::FG_GREEN);
       m_GasTimeOut.Reset();
    }

    return m_Macchina_ResetGas;

}


void CMacchina::Macchina_PowOutdig(){

    *PowOutdig=true;
}
