#include "Ciclo.h"
#include <QDebug>



CCiclo::CCiclo(int newtipoapp,int newTipoImpianto,int argc, char *argv[]) : CProcesso( newtipoapp, newTipoImpianto, argc, argv)
{
    m_IsInNextStep=false;
    m_PreSet=false;
    m_InPreSet=false;
    m_GasInPreset=false;
    m_PowInPreset=false;
    m_BiasInPreset=false;
    m_Macchina_ResetGas=false;
    m_SetGasOffSet=false;
    m_StandByCatodi=false;
    m_Riparti=false;
    m_StopVuoto=false;
    StepIndex=0;
    m_RiscaldoCount=0;
    m_OnTime=0;
    m_NextStep=0;
    m_NextTimeStep=0;
    m_CatodoOn=0;
    setProcessoIsOn(false);
}


void CCiclo::Ciclo_Main(){

    Macchina_Main();
    //DataType mData=pData[IdCiclo];
    switch(pData[IdCiclo].Comando){
      case CTipoComando::StartProcesso:
      case CTipoComando::NextStep:
      case CTipoComando::Preset:
        pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
        if(!Ciclo_PreSet())
            return;
        Ciclo_Run();
        pData[IdCiclo].ComandoEseguito=CTipoComando::StartProcesso;
        pData[IdCiclo].StatoMacchina=CStatoMacchina::StartProcesso;
        break;
      case CTipoComando::Reset:
       pData[IdCiclo].ComandoAttivato=CTipoComando::Reset;
       Vuoto[IdCiclo].Tavola.Reset();
       if(Ciclo_Reset()){
         pData[IdCiclo].StatoMacchina=CStatoMacchina::Reset;
         pData[IdCiclo].Comando=CTipoComando::StopVuoto;
         TimerEvent[IdCiclo].operator()(0); //Timer[IdCiclo].operator()(0);
       }else{
         pData[IdCiclo].ComandoEseguito=CTipoComando::Reset;
         pData[IdCiclo].StatoMacchina=CStatoMacchina::InReset;
       }
       break;
      case CTipoComando::StopProcesso:
       pData[IdCiclo].ComandoAttivato=CStatoMacchina::StopProcesso;
       if(Ciclo_Reset()){
         pData[IdCiclo].StatoMacchina=Vuoto[IdCiclo].Stato();
         pData[IdCiclo].Comando=CTipoComando::StendBy;
         TimerEvent[IdCiclo].operator()(0);
         NewEvento=false;
         pData[IdCiclo].ComandoEseguito=CStatoMacchina::StopProcesso;
         if(m_StopVuoto){
            pData[IdCiclo].Comando=CTipoComando::StopVuoto;
            m_StopVuoto=false;
         }
       }else
         pData[IdCiclo].StatoMacchina=CStatoMacchina::StopProcesso;
       break;
    }

}

void CCiclo::Ciclo_Run(){


    if(!pData[IdCiclo].ProcessoIsOn)
        return;

    bool retval=true;
    bool isMinGas=true;

    pData[IdCiclo].ErrProcesso=false;
    Macchina_FlussaAcqua();
    Vuoto[IdCiclo].Set();
    Ciclo_CeckErrori();
    if(pData[IdCiclo].ErrProcesso)
        goto Errore;

    Timer[IdCiclo].Timer();
    setMainTime(Timer[IdCiclo].Time());
    if(!Ciclo_NextStepForza())
        return;

    pData[IdCiclo].StepIndex=StepIndex;
    pData[IdCiclo].TipoStep=Step[IdCiclo][StepIndex].CTipoStep;
    pData[IdCiclo].StepIndex=StepIndex;
    pData[IdCiclo].RepitCount=Step[IdCiclo][StepIndex].RepitCount;
    pData[IdCiclo].Repit=Step[IdCiclo][StepIndex].Repit;


    pData[IdCiclo].ProcessoIsOn=true;
    setProcessoIsOn(pData[IdCiclo].ProcessoIsOn);
    pData[IdCiclo].TipoStep=Step[IdCiclo][StepIndex].CTipoStep;
    if(StepTimer[IdCiclo].Timer()){
      switch( Step[IdCiclo][StepIndex].CTipoStep){
       case CTipoStep::Vuoto:
         Vuoto[IdCiclo].Camera.SchermoTurbo->Set(false);
         pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
         if(!Vuoto[IdCiclo].Set())
           return;
         break;
       case CTipoStep::Riscaldo:
         Vuoto[IdCiclo].Camera.SchermoTurbo->Set(false);
         pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
         if(!Vuoto[IdCiclo].Resistenza->Set(Step[IdCiclo][StepIndex].CTipoStep))
             return;
         if(!m_IsInNextStep){
             if(Step[IdCiclo][StepIndex].Pressione < Vuoto[IdCiclo].Penning->Segnale()->Segnale()){
                StepTimer[IdCiclo].TimeStop(StepTimer[IdCiclo].TimeStop()+PRO_DT_RISCALDO);
                TimeStop+=PRO_DT_RISCALDO;
                m_RiscaldoCount++;
                if(m_RiscaldoCount==PRO_MAX_RISCALDO_COUNT){
                    pErrore.Errore(true,E_MAX_RISCALDO_COUNT,0);
                }
                return;
             }
         }
         break;
       case CTipoStep::Etching:
       case CTipoStep::Coating:
         pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
         for(int i=0;i<SorgenteCount;i++)
           retval=__min(retval,Bias[IdCiclo].Sorgente[i].ResetSegnali());
         for(int i=0;i<GasCount;i++)
           retval=__min(retval,GGas[i].ResetSegnali());
         if(!m_SetGasOffSet){
           for(int i=0;i<GasCount;i++)
             GGas[i].Offset(Vuoto[IdCiclo].Baratron.Pressione());
           Vuoto[IdCiclo].BaratronOffSet();
           m_SetGasOffSet=true;
         }
         retval=__min(retval,Bias[IdCiclo].ResetSegnali());
         Vuoto[IdCiclo].Resistenza->ResetSegnali();
         break;
       case CTipoStep::Venting:
         pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
         pData[IdCiclo].Comando=CTipoComando::StopVuoto;
         break;
      }

      if(StepIndex + 1 < StepCount){
        if(!Ciclo_NextStepGoto()){
           if(pData[IdCiclo].ErrProcesso)
               goto Errore;
           return;
        }
        pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
      }
      else{ //Se non vi sono altri step  la macchina va in StandBy
        Timer[IdCiclo].Reset();
        pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
        pData[IdCiclo].Comando=CTipoComando::StopProcesso;
        m_StopVuoto=true;
        return;
      }
    }
    else{//Continua Il ciclo
      switch( Step[IdCiclo][StepIndex].CTipoStep){
        case CTipoStep::Vuoto:
          Vuoto[IdCiclo].Set();
          break;
        case CTipoStep::Riscaldo:
          Vuoto[IdCiclo].Resistenza->Set(Step[IdCiclo][StepIndex].CTipoStep);
          break;
        case CTipoStep::Coating:
          Vuoto[IdCiclo].Camera.Set();
          if(!Macchina_PowOn(IdCiclo))
              return;
          if(!Macchina_PowIsOn(IdCiclo)){
              pData[IdCiclo].ErrProcesso=true;
              goto Errore;
          }
          Bias[IdCiclo].Repit(Step[IdCiclo][StepIndex].Repit);
          for(int i=0;i<GasCount;i++){
            GGas[i].Set();
            isMinGas=__min(isMinGas,GGas[i].IsMin());
          }
          if(isMinGas)
              Bias[IdCiclo].SetArcOff();
          else
              Bias[IdCiclo].Set();
          Vuoto[IdCiclo].Resistenza->Set(Step[IdCiclo][StepIndex].CTipoStep);
          Ciclo_AllungaCiclo();
        case CTipoStep::Etching:
          Vuoto[IdCiclo].Camera.Set();
          for(int i=0;i<GasCount;i++){
            GGas[i].Set();
            isMinGas=__min(isMinGas,GGas[i].IsMin());
          }
          Vuoto[IdCiclo].Resistenza->Set(Step[IdCiclo][StepIndex].CTipoStep);
          if(isMinGas)
              Bias[IdCiclo].SetArcOff();
          else{
              Bias[IdCiclo].Set();
              if(Bias[IdCiclo].Riparti()){
                  if(StepIndex-1>0){
                     StepIndex--;
                     StepTimer[IdCiclo].TimeStop(StepTimer[IdCiclo].TimeStop()+StepTimer[IdCiclo].Time());
                     TimeStop+=StepTimer[IdCiclo].Time();
                     StepTimer[IdCiclo].Stop();
                     Step[IdCiclo][StepIndex].Tempo=TimeStop;
                     return;
                  }
              }
          }
          Ciclo_AllungaCiclo();
          break;
        case CTipoStep::Venting:
          pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
          pData[IdCiclo].Comando=CTipoComando::StopProcesso;
          m_StopVuoto=true;
          break;
      }
    }

Errore:
    if(pData[IdCiclo].ErrProcesso ){
       m_StopVuoto=true;
       pData[IdCiclo].Comando=CTipoComando::StopProcesso;
       pData[IdCiclo].SorgentiOn=pData[IdCiclo].File.SorgentiOn;
       MostraEventoProcesso("Fermo per Errore Processo",Color::FG_RED);
       return ;
     }

}

void CCiclo::Ciclo_CeckErrori(){

    Macchina_PrintErrori();
    bool mErrore=pErrore.Errore();
    mErrore=__max(mErrore,Vuoto[IdCiclo].Errore());

    mErrore=__max(mErrore,Vuoto[IdCiclo].Resistenza->Errore());
    for(int i=0;i<SorgenteCount;i++){
          mErrore=__max(mErrore,Bias[IdCiclo].Sorgente[i].Errore());
    }
    mErrore=__max(mErrore,Bias[IdCiclo].Errore());
    for(int i=0;i<GasCount ;i++){
        mErrore=__max(mErrore,GGas[i].Errore());
    }

    ///CONDUE
    for(int i=0; i < CamereCount; i++){
        if(i!=IdCiclo){
           if(!Vuoto[i].Camera.PortaChiusa->Segnale()) {
             pErroreDue.Errore(true,E_MACCHINA_DUE_PORTA,0);
             mErrore=true;
           }
        }
    }
  /*  if(IdCiclo==0){
        pErroreDue.Errore( Bias[IdCiclo].BiasIsNext->Segnale(),E_MACCHINA_BIAS_NEXT,0);
    }else{
        pErroreDue.Errore( FalseTrue( Bias[IdCiclo].BiasIsNext->Segnale()),E_MACCHINA_BIAS_NEXT,0);
    }*/
    ///CONDUE
    pData[IdCiclo].ErrProcesso=mErrore;

}


bool CCiclo::Ciclo_NextStepForza()
{
    DWORD TmpTempo=0;
    int i;

    if(pData[IdCiclo].Comando!=CTipoComando::NextStep)// || m_NextTimeStep<m_Timer.Time())
        return true;

    if(!m_IsInNextStep){
        pData[IdCiclo].ComandoAttivato=CTipoComando::NextStep;
        if(((Step[IdCiclo][m_NextStep].CTipoStep==CTipoStep::Etching)
            | (Step[IdCiclo][m_NextStep].CTipoStep==CTipoStep::Coating)) & !Macchina_PowIsOn(IdCiclo)){
             TimerEvent[IdCiclo].operator()(30000);
             //m_OnOffStepTimer.Reset();
        }
        else
             TimerEvent[IdCiclo].operator()(5000);
        if(m_NextTimeStep){
          for(i=0;i<m_NextStep;i++)
            TmpTempo+=(Step[IdCiclo][i].Tempo*Step[IdCiclo][i].RepitCount);
          m_NextTimeStep-=TmpTempo;
        }
        m_IsInNextStep=true;
    }
    return Ciclo_NextStepGoto();

}

bool CCiclo::Ciclo_NextStepGoto()
{
  bool RetVal=true;
  bool Tmp;
  int i;

  if(!m_IsInNextStep){
     if(Step[IdCiclo][StepIndex].Repit==Step[IdCiclo][StepIndex].RepitCount ){
        if(StepIndex+1< StepCount){
           StepIndex++;
        }else
           return true;
     }else
        Step[IdCiclo][StepIndex].Repit++;
     m_TimerPreSet(30000);
     m_TimerPreSet.Reset();
     m_IsInNextStep=true;
  }

  if(m_TimerPreSet.Timer()){
      goto Errore;
  }

  m_Macchina_ResetGas=false;
  if((Step[IdCiclo][StepIndex].CTipoStep==CTipoStep::Etching)
          | (Step[IdCiclo][StepIndex].CTipoStep==CTipoStep::Coating)){
      if(!Vuoto[IdCiclo].Camera.Gas->Set(true))
          return false;
///CONDUE
      for(int i=0; i < CamereCount; i++){
          if(i!=IdCiclo){
             if(!Vuoto[i].Camera.Gas->Set(false) | !Vuoto[i].Camera.AriaComp->Set(true))
               return false;
          }
      }
///CONDUE
      if(Step[IdCiclo][StepIndex].SorgentiOn!=pData[IdCiclo].SorgentiOn){
       for(i=0;i<SorgenteCount;i++){
         bool isEnabled=GetBit(Step[IdCiclo][StepIndex].SorgentiOn,i);
         if(isEnabled!=Bias[IdCiclo].Sorgente[i].Enabled){
           Bias[IdCiclo].Sorgente[i].SetArcOff();
         }
         Bias[IdCiclo].Sorgente[i].Enabled=isEnabled ;
         Bias[IdCiclo].Sorgente[i].Catodo->SetEnabled(isEnabled);
       }
       Ciclo_SorgentiAccoppiate(StepIndex);
       pData[IdCiclo].SorgentiOn=Step[IdCiclo][StepIndex].SorgentiOn;
       m_CatodoOn=Step[IdCiclo][StepIndex].SorgentiOn;
     }

     if(!Macchina_PowOn(IdCiclo)){
        return false;
     }

     if(!Macchina_PowIsOn(IdCiclo)){
         goto Errore;
     }

     if(Bias[IdCiclo].IsSetTap()){
        if(!Bias[IdCiclo].SetArcOff()){
           return false;
        }
        m_StandByCatodi=true;
        Tmp=Bias[IdCiclo].GoToStep(Step[IdCiclo][StepIndex].Step,Step[IdCiclo][StepIndex].CTipoStep,Step[IdCiclo][StepIndex].PerBiasOnTime);
        RetVal=__min(RetVal,Tmp);
        if(!RetVal)
            return false;
        m_StandByCatodi=false;
     }
     else{
        Tmp=Bias[IdCiclo].GoToStep(Step[IdCiclo][StepIndex].Step,Step[IdCiclo][StepIndex].CTipoStep,Step[IdCiclo][StepIndex].PerBiasOnTime);
        RetVal=__min(RetVal,Tmp);
     }

     for(i=0;i<SorgenteCount;i++){
       Tmp=Bias[IdCiclo].Sorgente[i].GoToStep(Step[IdCiclo][StepIndex].Step,Step[IdCiclo][StepIndex].CTipoStep);
       RetVal=__min(RetVal,Tmp);
     }

     if(Step[IdCiclo][StepIndex].CTipoStep==CTipoStep::Venting){
       if(Bias[IdCiclo].SetArcOff())
          return false;
     }
     for(i=0;i<GasCount;i++){
       Tmp=GGas[i].GoToStep(Step[IdCiclo][StepIndex].Step);
       RetVal=__min(RetVal,Tmp);
     }

   }

  Tmp=Vuoto[IdCiclo].GoToStep(Step[IdCiclo][StepIndex].Pressione);
  RetVal=__min(RetVal,Tmp);
  Tmp=Vuoto[IdCiclo].Resistenza->GoToStep(Step[IdCiclo][StepIndex].Potenza,Step[IdCiclo][StepIndex].Temperatura,Step[IdCiclo][StepIndex].Pressione);
  RetVal=__min(RetVal,Tmp);
  if(Vuoto[IdCiclo].Camera.SchermoTurbo!=NULL)
     RetVal=__min(RetVal,Vuoto[IdCiclo].Camera.SchermoTurbo->GoToStep(Step[IdCiclo][StepIndex].Step));

  if(RetVal){
    if(!m_SetGasOffSet){
        for(i=0;i<GasCount;i++)
          GGas[i].Offset(Vuoto[IdCiclo].Baratron.Pressione());
        Vuoto[IdCiclo].BaratronOffSet();
        m_SetGasOffSet=true;
    }
    for(int i=0; i< SorgenteCount;i++){
        pScorta->OnTime(i,Bias[IdCiclo].Sorgente[i].Catodo->OnTime());
    }

    if(Debug){
       pScorta->Print();
    }

    StepTimer[IdCiclo].operator ()(Step[IdCiclo][StepIndex].Tempo);
    m_IsInNextStep=false;
    m_RiscaldoCount=0;

    std::string str;
    std::stringstream out;
    out << Step[IdCiclo][StepIndex].Step;
    str.append( out.str());
    str.append("\t");
    setTipoStep(Step[IdCiclo][StepIndex].CTipoStep);
    switch (Step[IdCiclo][StepIndex].CTipoStep) {
    case CTipoStep::Vuoto:
         str+=" Vuoto ";
        break;
    case CTipoStep::Riscaldo:
         str+=" Riscaldo ";
        break;
    case CTipoStep::Etching:
         str+=" Etching ";
        break;
    case CTipoStep::Coating:
         str+=" Coating ";
        break;
    default:
        break;
    }
    str.append("Step\t");
    str.append(intToString((int)Step[IdCiclo][StepIndex].Repit) + "/");
    str.append(intToString((int)Step[IdCiclo][StepIndex].RepitCount) );
    MostraEventoProcesso(str,Color::FG_GREEN);
    return true;
  }
  else
     return false;

Errore:
  Bias[IdCiclo].ceckNextStepError();
  for(i=0;i<SorgenteCount;i++)
      Bias[IdCiclo].Sorgente[i].ceckNextStepError();
  for(i=0;i<GasCount;i++)
      GGas[i].ceckNextStepError();
  Vuoto[IdCiclo].Resistenza->ceckNextStepError();
  pErrore.Errore(true,E_NEXT_STEP, 0);
  m_Riparti=false;

  return false;
}


void CCiclo::Ciclo_AllungaCiclo(){

    if((Step[IdCiclo][StepIndex].CTipoStep!=CTipoStep::Coating ) & (Step[IdCiclo][StepIndex].CTipoStep!=CTipoStep::Etching))
        return ;

    int SpentiCount=0;
    BYTE CatodoOn=Step[IdCiclo][StepIndex].SorgentiOn;
    for(int i=0; i<SorgenteCount; i++){
        if(Bias[IdCiclo].Sorgente[i].Enabled & !Bias[IdCiclo].Sorgente[i].Catodo->Enabled){
            CatodoOn=SetBit(CatodoOn,i,0);
            if(CatodoOn!=m_CatodoOn){
                //VERIFICO SE E INTERFACCIA
                int id=i;
                bool IsInterfaccia=false;
                for(int j=0;j<SorgenteCount; j++){
                    if(j!=id ){
                        if( Bias[IdCiclo].Sorgente[j].Enabled){ //if(Bias[IdCiclo].Sorgente[j].SetI->Segnale()!=0 & Bias[IdCiclo].Sorgente[j].Enabled)
                            if(Bias[IdCiclo].Sorgente[j].Catodo->TipoTraget!=Bias[IdCiclo].Sorgente[id].Catodo->TipoTraget){
                                Bias[IdCiclo].Sorgente[j].SetI->AzzeraStep();
                                Bias[IdCiclo].Sorgente[j].Catodo->Enabled=false;
                                IsInterfaccia=true;
                                break;
                            }
                        }
                    }
                }
                if(Step[IdCiclo][StepIndex].CTipoStep==CTipoStep::Coating)
                   Bias[IdCiclo].Sorgente[i].SetI->AzzeraStep();
                if(GetBit(m_CatodoOn,i)==1)
                    SpentiCount++;
            }
        }
    }

    if(SpentiCount==0)
        return ;


    int DaAccendere=0;
    int Accesi=0;
    for(int i=0; i<SorgenteCount; i++){
        if(Bias[IdCiclo].Sorgente[i].Enabled)
            DaAccendere++;
        if(GetBit(CatodoOn,i)==1)
            Accesi++;
    }
    double perceent=Accesi;
    perceent/=DaAccendere;
    if(perceent<0.5){
        pErrore.Errore(true,E_TROPPI_CATODI_SPENTI,E_TIME);
        m_CatodoOn=CatodoOn;
        return ;
    }else
        pErrore.Errore(false,E_TROPPI_CATODI_SPENTI,E_TIME);

    if(SpentiCount!=0){
        double  incremento=SpentiCount;
        if((Accesi+SpentiCount)==0){
            m_CatodoOn=CatodoOn;
            return;
        }
        incremento/=(Accesi+SpentiCount);
        incremento=(1-incremento);
        if(incremento==0){
            m_CatodoOn=CatodoOn;
            return;
        }
        incremento=1/incremento;
        //qDebug() << "Incremento di " << QString::number(incremento,'f',2);
        double StopStepTime=StepTimer[IdCiclo].TimeStop();
        DWORD DaTempo=StepTimer[IdCiclo].Time();
        StopStepTime-=StepTimer[IdCiclo].Time();
        StopStepTime*=(incremento);
        StopStepTime=(int)StopStepTime;
        StopStepTime+=StepTimer[IdCiclo].Time();
        int aumemto=StopStepTime;
        aumemto-=StepTimer[IdCiclo].TimeStop();
        StepTimer[IdCiclo].TimeStop(StopStepTime);

        Timer[IdCiclo].TimeStop(Timer[IdCiclo].TimeStop()+aumemto);

        if(Step[IdCiclo][StepIndex].CTipoStep==CTipoStep::Etching){
            for(int i=0;i<SorgenteCount;i++){
                if(Bias[IdCiclo].Sorgente[i].Catodo->Enabled){
                   if(!Bias[IdCiclo].Sorgente[i].SetI->AllungaZero(incremento))
                      Step[IdCiclo][StepIndex].Repit*=(incremento);
                }
            }
            for(int i=0;i<SorgenteCount;i++){
                if(!Bias[IdCiclo].Sorgente[i].Catodo->Enabled)
                   Bias[IdCiclo].Sorgente[i].SetI->AzzeraStep();
            }
        }else{
            for(int i=0;i<SorgenteCount;i++){
                if(Bias[IdCiclo].Sorgente[i].Enabled)
                   Bias[IdCiclo].Sorgente[i].SetI->Allunga(incremento,StepTimer[IdCiclo].Time());
            }
            Bias[IdCiclo].SetV->Allunga(incremento,DaTempo);
            for(int i=0; i< GasCount;i++)
                GGas[i].SetG->Allunga(incremento,StepTimer[IdCiclo].Time());
        }

        std::string str;
        str.append( intToString(SpentiCount) );
        str.append(" Catodo Spento Allungo A ");
        str.append(intToString(StopStepTime/60000));
        str.append(" min  ");
        MostraEventoProcesso(str,Color::FG_GREEN);
    }
    m_CatodoOn=CatodoOn;    

}

bool CCiclo::Ciclo_SorgentiAccoppiate(int index){

    for(int i=0;i<SorgenteCount;i++){
      int attuale=i;
      for(int j=0;j<SorgenteCount;j++){
        if(j!=attuale) {
            if(Bias[IdCiclo].Sorgente[j].Enabled){
               if((Bias[IdCiclo].Sorgente[attuale].SetI->Slot()==Bias[IdCiclo].Sorgente[j].SetI->Slot()) & (Bias[IdCiclo].Sorgente[attuale].SetI->Point()==Bias[IdCiclo].Sorgente[j].SetI->Point())){
                  if((Bias[IdCiclo].Sorgente[attuale].SetI->StepCount()!=0) & (Bias[IdCiclo].Sorgente[j].SetI->StepCount()!=0)){
                      std::string str;
                      if(Bias[IdCiclo].Sorgente[attuale].SetI->isStepNull(index)){
                          Bias[IdCiclo].Sorgente[attuale].Enabled=false;
                          str.append("ENABLED FALSE SU ");
                          str.append(intToString(attuale) );
                          //MostraEventoProcesso(str);
                      }else if(Bias[IdCiclo].Sorgente[j].SetI->isStepNull(index)){
                          Bias[IdCiclo].Sorgente[j].Enabled=false;
                          Bias[IdCiclo].Sorgente[attuale].Enabled=false;
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

bool CCiclo::Ciclo_Reset()
{

  if(!Macchina_PowOff(IdCiclo))
      return false;
  if(!Macchina_PowIsOff(IdCiclo)){
     MostraErrore("Errore Reset Ciclo");
  }

  if(!Macchina_ResetGas())
      return false;
  if(!Vuoto[IdCiclo].Resistenza->Reset())
      return false;

  if(IdCiclo==0){
      bool SchermoIsOpen=true;
      for(int i=0;i<SorgenteCount;i++){
           SchermoIsOpen=__min(SchermoIsOpen,Bias[IdCiclo].Sorgente[i].SchermoApri());
      }
      if(!SchermoIsOpen)
          return false;
  }else{
      bool SchermoIsClose=true;
      for(int i=0;i<SorgenteCount;i++){
           SchermoIsClose=__min(SchermoIsClose,Bias[IdCiclo].Sorgente[i].SchermoChiudi());
      }
      if(!SchermoIsClose)
          return false;
  }


  if(!Vuoto[IdCiclo].Camera.Gas->Set(false))
      return false;

  pData[IdCiclo].ProcessoIsOn=false;
  setProcessoIsOn(pData[IdCiclo].ProcessoIsOn);
  Timer[IdCiclo].Reset();
  StepIndex=0;
  StepTimer[IdCiclo].Reset();
  m_IsInNextStep=false;
  m_PreSet=false;
  m_InPreSet=false;
  m_GasInPreset=false;
  m_PowInPreset=false;
  m_BiasInPreset=false;
  m_Macchina_ResetGas=false;
  m_SetGasOffSet=false;
  m_StandByCatodi=false;
  StepIndex=0;
  pData[IdCiclo].TipoStep=CTipoStep::Nessuno;

  for(int i=0 ; i<StepCount;i++){
      Step[IdCiclo][i].Repit=1;
  }

  for(int i=0; i< SorgenteCount;i++){
      pScorta->OnTime(i,Bias[IdCiclo].Sorgente[i].Catodo->OnTime());
  }

  if(Debug){
     pScorta->Print();
  }
  pScorta->Salva();
  //setProcessoIsOn(false);

  return  true;
}

WORD CCiclo::Ciclo_GetMsg(char *pStr){

    ///
    char tmp[20];
    char *ptmp=tmp;
    memcpy(ptmp,pStr,BufferLen);

    pData[IdCiclo].MessaggioLen=0;
    memset(Msg,0,D_MSG);
    if( pStr != NULL  ){
       pStr+=3;
       int step=StrWORD(&pStr);
       if((step>pData[IdCiclo].EventoProcessoCount) | !pData[IdCiclo].File.CicloId){
           pStr-=5;
           return Macchina_GetNetData(pStr);
       }
       pData[IdCiclo].EventoProcessoDi=step;
       std::string path=getRunPath();
       path.append(PLC_CICLO_PATH);
       path.append(PLC_EVENTO);
       path.append(intToString(pData[IdCiclo].File.CicloId));
       FILE* fp = fopen(path.data(), "r");
       if (fp){
           int pos=D_MSG*step;
           fseek(fp, pos , SEEK_SET);
           fread(Msg, D_MSG, 1, fp);
           pData[IdCiclo].MessaggioLen=D_MSG;
           if(step==2){
               qDebug() << Msg;
           }
           pStr-=5;
           fclose(fp);
       }
    }

    return Macchina_GetNetData(pStr);
}


bool CCiclo::Ciclo_IsTargetConsumato(){

    int Consumato=false;
    for(int i=0; i<SorgenteCount;i++){
        Consumato=__max(Consumato,Bias[IdCiclo].Sorgente[i].Catodo->IsTargetConsumato(Bias[IdCiclo].Sorgente[i].SetI));
    }
    return Consumato;
}


bool CCiclo::Ciclo_PreSet()
{

  DWORD TempoPreset;

  setProcessoIsOn(true);

  if(m_PreSet)
      return m_PreSet;

  if(Vuoto[IdCiclo].Errore()){
     pData[IdCiclo].Comando=CTipoComando::StopVuoto;
     m_PreSet=false;
     return true;
  }


  if(!m_InPreSet){
     Macchina_ResetErrore();
     if(Ciclo_IsTargetConsumato()){
          pData[IdCiclo].Comando=CStatoMacchina::StopProcesso;
          MostraEventoProcesso("Errore Preset IsTargetConsumato",Color::FG_RED);
          goto Errore;
     }
     if(DogNonAbilitato){
         pErroreDue.Errore(true,E_DOG, 0);
         pErroreDue.Errore();
         pData[IdCiclo].Comando=CStatoMacchina::StopProcesso;
         goto Errore;
     }
     if(MacchinaInManuale){
         pErroreDue.Errore(true,E_MANUALE, 0);
         pErroreDue.Errore();
         pData[IdCiclo].Comando=CStatoMacchina::StopProcesso;
         goto Errore;
     }
     if(PorteAperte){
         pErroreDue.Errore(true,E_PORTE_APERTE, 0);
         pErroreDue.Errore();
         pData[IdCiclo].Comando=CStatoMacchina::StopProcesso;
         goto Errore;
     }
     if(CamereCount>1){
         if(IdCiclo==0){
            pData[1].SorgentiOn=0;
         }else{
            pData[0].SorgentiOn=0;
         }
     }
     if(!Macchina_PowOff(IdCiclo))
         return false;
     if(!Macchina_PowIsOff(IdCiclo)){
        goto Errore;
     }
     if(!Macchina_ResetGas())
         return false;

     m_InPreSet=true;
     m_GasInPreset=false;
     m_PowInPreset=false;
     m_BiasInPreset=false;
     pData[IdCiclo].ErrPreProcesso=false;
     m_RiscaldoCount=0;
     TempoPreset=Vuoto[IdCiclo].VuotoTime();
     TempoPreset+=2000; // Aspetto 10 Secondi prima di iniziare il Preset
     m_TimerPreSet(PRO_ONTIME_GAS);
     for(int i=0;i<GasCount;i++){
       GGas[i].PreSet(true);
       if(GGas[i].SetG->Load())
         TempoPreset+=PRO_ONTIME_GAS;
     }
     TempoPreset+=PRO_ONTIME_GAS;
     m_OnTime=0;
     for(int i=0;i<SorgenteCount;i++){
       if(Bias[IdCiclo].Sorgente[i].Enabled){
         m_OnTime+=PRO_ONTIME_CATODI;
         TempoPreset+=PRO_ONTIME_CATODI;
       }
     }
     TempoPreset+=PRO_ONTIME_CATODI;
     if(Bias[IdCiclo].Enabled){
         m_OnTime+=PRO_ONTIME_BIAS;
         TempoPreset+=PRO_ONTIME_BIAS;
     }
     TimerEvent[IdCiclo].operator ()(TempoPreset+30000);
     if(MainTimer.Time()<1)
         return m_InPreSet;
     if(!Processo_LoadDaFile())
         goto Errore;
  }

  for(int i=0; i<StepCount;i++){
      if((Step[IdCiclo][i].CTipoStep==CTipoStep::Coating)
              | (Step[IdCiclo][i].CTipoStep==CTipoStep::Etching)){
         if(Ciclo_SorgentiAccoppiate(i)){
            pData[IdCiclo].ErrPreProcesso=true;
         }
      }
  }

  if(!Vuoto[IdCiclo].Set())
     return false;

  if(TimerEvent[IdCiclo].Timer() ){
      pErrore.Errore(true,E_TIME_OUT,0);
      MostraEventoProcesso("Errore Preset E_TIME_OUT",Color::FG_RED);
      goto Errore;
  }

  pData[IdCiclo].StatoMacchina=CStatoMacchina::PreSet;

  if(!m_GasInPreset){
      if(!Macchina_PowOn(IdCiclo)){
         return false;
      }
      if(!Macchina_PowIsOn(IdCiclo)){
          goto Errore;
      }
      if(!Vuoto[IdCiclo].Camera.Gas->Set(true))
          return false;
      if(m_TimerPreSet.Timer()){
        m_Macchina_ResetGas=false;
        for(int i=0; i<GasCount;i++){
          if(!GGas[i].SetG->Segnale() & GGas[i].SetG->Load()){
            *GGas[i].Valvola=true;
            if(i==0)
               *GGas[i].SetG=(GAS_MIN_ARGON+20);
            else if(i==1)
               *GGas[i].SetG=(GAS_MIN_AZOTO+20);
            else if(i==2)
                *GGas[i].SetG=(GAS_MIN_ACETI+20);
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

  pErrore.Errore(pData[IdCiclo].ProcessoCorrotto,E_PROCESSO_CORROTTO, 0);
  pErrore.Errore(FalseTrue(isLicenza()),E_LICENZA_SCADUTA, 0);
  pData[IdCiclo].ErrProcesso=pErrore.Errore();
  pData[IdCiclo].ErrProcesso=__max(pData[IdCiclo].ErrProcesso,Vuoto[IdCiclo].Errore());
  pData[IdCiclo].ErrProcesso=__max(pData[IdCiclo].ErrProcesso,Vuoto[IdCiclo].Resistenza->Errore());

  for(int i=0;i<GasCount ;i++){
      pData[IdCiclo].ErrPreProcesso=__max(pData[IdCiclo].ErrPreProcesso,GGas[i].Errore());
  }

  if(!m_BiasInPreset){
     if(!Bias[IdCiclo].SetTap(1)){
        return false;
     }
     if(!Bias[IdCiclo].SetV->Segnale()){
       *Bias[IdCiclo].SetV=BIAS_TEST/(Bias[IdCiclo].SetV->Fattore());
       m_TimerPreSet(500);
     }
     if(!m_TimerPreSet.Timer() ){
         return false;
     }
     m_TimerPreSet(1.5*E_TIME);
     m_BiasInPreset=true;
   }

   for(int i=0;i<SorgenteCount ;i++){
      pData[IdCiclo].ErrPreProcesso=__max(pData[IdCiclo].ErrPreProcesso,Bias[IdCiclo].Sorgente[i].Errore());
   }

   pData[IdCiclo].ErrPreProcesso=__max(pData[IdCiclo].ErrPreProcesso,Bias[IdCiclo].Errore());

   if(!m_TimerPreSet.Timer() ){
       return false;
   }

   if(!Macchina_ResetGas())
       return false;
   if(!Macchina_PowOff(IdCiclo))
       return false;

   if(!Macchina_PowIsOff(IdCiclo)){
      goto Errore;
   }

   if(!pData[IdCiclo].ErrPreProcesso){
     StepIndex=0;
     StepTimer[IdCiclo].Reset();
     StepTimer[IdCiclo].operator ()(Step[IdCiclo][StepIndex].Tempo);
     Vuoto[IdCiclo].GoToStep(Step[IdCiclo][StepIndex].Pressione);
     Vuoto[IdCiclo].Resistenza->GoToStep(Step[IdCiclo][StepIndex].Potenza,Step[IdCiclo][StepIndex].Temperatura,Step[IdCiclo][StepIndex].Pressione);
     pData[IdCiclo].ErrProcesso=false;
     m_SetGasOffSet=false;
     Vuoto[IdCiclo].BaratronOffSet();
     pData[IdCiclo].Comando=CTipoComando::StartProcesso;
     pData[IdCiclo].ComandoAttivato=CTipoComando::StartProcesso;
     MostraEventoProcessoReset();
     pData[IdCiclo].File.CicloId=0;
     MostraEventoProcesso("Start Processo",Color::FG_GREEN);
   }
   else{
     goto Errore;
   }

   for(int i=0;i<GasCount;i++)
     GGas[i].PreSet(false);
   Macchina_ResetErrore();
   m_PreSet=true;
   m_InPreSet=false;
   TimerEvent[IdCiclo].operator ()(0);
   pData[IdCiclo].ProcessoIsOn=true;
   //setProcessoIsOn(pData[IdCiclo].ProcessoIsOn);

   return m_PreSet;

Errore:
   Bias[IdCiclo].ceckNextStepError();
   Bias[IdCiclo].ceckPowerError();
   pErrore.Errore(true,E_PRESET_ERRORE,0);
   pData[IdCiclo].Comando=CTipoComando::Reset;
   pData[IdCiclo].ProcessoIsOn=false;
   setProcessoIsOn(pData[IdCiclo].ProcessoIsOn);
   m_PreSet=false;
   m_InPreSet=false;
   MostraEventoProcesso("Errore Preset",Color::FG_RED);
   return false;

}


