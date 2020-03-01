#include "Vuoto.h"
#include "MainCostanti.h"



CVuoto::CVuoto()
{

    m_InSet=false;
    m_InReset=false;
    m_Errore=false;
    m_Set=false;
    m_Reset=true;
    m_VuotoTime=(WORD)(VAC_T_TURBO_SET+540000);
    m_StopVuotoTime=(WORD)VAC_T_TURBO_RESET+(WORD)VAC_PRE_STOPVUOTO;
    m_TimerEvent(m_VuotoTime);
    m_EnabledVenting=true;
    m_TimerStopVuoto(VAC_PRE_STOPVUOTO);
    m_BaratronOffSet=0;
    Debug=false;
    ManVuoto=NULL;
    IdVuoto=0;

    pErrore.append(E_PORTA_CHIUSA,"E_PORTA_CHIUSA");
    pErrore.append(E_INTERLOCK,"E_INTERLOCK");
    pErrore.append(E_PENNING_START,"E_PENNING_START");
    pErrore.append(E_PIRANI_SET,"E_PIRANI_SET");
    pErrore.append(E_MEC_RELE,"E_MEC_RELE");
    pErrore.append(E_MEC_TERMICO,"E_MEC_TERMICO");
    pErrore.append(E_TURBO_RELE_POWER,"E_TURBO_RELE_POWER");
    pErrore.append(E_TURBO,"E_TURBO");
    pErrore.append(E_TAVOLA,"E_TAVOLA");
    pErrore.append(E_PRESSIONE_ALTA,"E_PRESSIONE_ALTA");
    pErrore.append(E_TEMP_TURBO,"E_TEMP_TURBO");
    pErrore.append(E_TEMP_CAMERA,"E_TEMP_CAMERA");
    pErrore.append(E_TEMP_CATODO,"E_TEMP_CATODO");
    pErrore.append(E_TURBO_NON_PARTE,"E_TURBO_NON_PARTE");
    pErrore.append(E_TAVOLA_MANCA_IMPARA,"E_TAVOLA_MANCA_IMPARA");
    pErrore.append(E_TURBO_ACC_TEMPO,"E_TURBO_ACC_TEMPO");
    pErrore.append(E_PROCESS_START_ENABLED,"E_PROCESS_START_ENABLED");
    pErrore.append(E_NESSUN_SENSORE,"E_NESSUN_SENSORE");

    Penning = new CPenning(&pErrore);

    //int vsl=0;

}


void CVuoto::ResetErrore(){

    pErrore.Reset();
}


void CVuoto::CeckStato()
{

   Meccanica.CeckStato();
   Turbo.CeckStato();
   if(Meccanica.PowIsOn->Segnale()){
     if(Turbo.Normale->Segnale()){//CStatoMacchina::Vacuum
        m_Set=true;
        m_Reset=false;
        m_InSet=false;
        m_InReset=false;
        m_EnabledVenting=false;
     }
     else if(Turbo.Acc->Segnale()){//CStatoMacchina::InVacuum
        m_Set=false;
        m_Reset=false;
        m_InSet=true;
        m_InReset=false;
        m_EnabledVenting=false;
     }
     else{//CStatoMacchina::InStopVuoto
        m_Set=false;
        m_Reset=false;
        m_InSet=false;
        m_InReset=true;
        m_EnabledVenting=false;
     }

   }
   else{//CStatoMacchina::StopVuoto;
       m_Set=false;
       m_Reset=true;
       m_InSet=false;
       m_InReset=false;
       m_EnabledVenting=true;
   }


}

int CVuoto::Stato()
{
   if(Meccanica.PowIsOn->Segnale()){
     if(Turbo.Normale->Segnale())
        return CStatoMacchina::Vacuum;
     return CStatoMacchina::InVacuum;
   }else{
       if(Camera.Valv->Segnale()){
           return CStatoMacchina::Ventila;
       }else{
           if(Camera.PortaChiusa){
              if(EnabledVenting())
                 return CStatoMacchina::EnabledVent;
              else
                 return CStatoMacchina::StopVuoto;
           }else
               return CStatoMacchina::MacchinaAperta;
       }
   }

   return CStatoMacchina::Null;

}


DWORD CVuoto::StopVuotoTime()
{
      return m_StopVuotoTime;
}


DWORD CVuoto::VuotoTime()
{
    return m_VuotoTime;
}

bool CVuoto::EnabledVenting()
{
    if(!m_Reset)
        return false;

    if(m_EnabledVenting)
       return true;

    if(m_TimerStopVuoto.Timer()){
       m_EnabledVenting=true;
       //Errore();
       return true;
      }
    else
      return false;
}




void CVuoto::Ventila()
{
    if(!EnabledVenting())
        return;

    Baratron.Ventila();
    if(!Camera.Valv->Segnale()){
       *Camera.Valv=true;
       m_TimerStopVuoto(VAC_PRE_HVALV_STOPVUOTO);
    }
    else{
        if(!m_TimerStopVuoto.Timer())
            return;
        //else
          //  *Camera.HValv=true;

    }


}

bool CVuoto::Set()
{

  if(Errore() | !Camera.PortaChiusa->Segnale()){
    m_InSet=false;
    Reset();
    return false;
  }

  m_EnabledVenting=false;

  if(!m_InSet){
    m_InSet=true;
    m_InReset=false;
    m_Reset=false;
    m_InStopVuoto=false;
    m_StopVuoto=false;
    m_TimerEvent(m_VuotoTime);
    m_TimerEvent.Reset();
    m_TimerStopVuoto.Reset();
  }

  if(!Camera.Valv->Set(false))
      return m_Set;

  Tavola.Set();
  Penning->Set();
  if(Baratron.Segnale)
    Baratron.Set(Penning->Pressione());
  m_Set =Meccanica.Set();
  if(m_Set){
     m_Set=Turbo.Set();
  }
  if(m_Set){
      m_BaratronOffSet=Baratron.Pressione();
      m_InSet=false;
  }
  return m_Set;

}



bool CVuoto::GoToStep(WORD NewPressione)//,WORD NewTemperatura,WORD NewSpeed
{

  m_Pressione=NewPressione;
  return true;

}



void CVuoto::StendBy()
{
  m_Errore=false;

  Tavola.Reset();
  Errore();

}



bool CVuoto::Reset()
{

  if(!m_InReset){
    m_Set=false;
    m_InReset=true;
    m_InSet=false;
    m_TimerEvent(m_StopVuotoTime,m_StopVuotoTime);
    m_TimerEvent.Reset();
  }

  m_TimerEvent.Timer();

  if(!m_InStopVuoto && m_Reset){
    m_InStopVuoto=true;
    m_TimerStopVuoto(VAC_PRE_STOPVUOTO);
    m_TimerEvent(VAC_PRE_STOPVUOTO);
  }

  Tavola.Reset();
  m_Reset=Turbo.Reset();
  Baratron.Reset(Penning->Pressione());
  Penning->Reset();


  if(m_Reset)
     m_Reset=Meccanica.Reset();
  if(m_Reset){
     m_InSet=false;
     }
  if(m_Reset){
     m_Set=false;
     m_InReset=false;
     //pErrore.Reset();
  }



  m_TimerStopVuoto.Timer();


  return m_Reset;
}



bool CVuoto::Errore()
{

  if(Meccanica.PowOn->Segnale()){
    pErrore.Errore(FalseTrue(Meccanica.PowIsOn->Segnale()),E_MEC_RELE,E_TIME);
    pErrore.Errore(FalseTrue(Camera.PortaChiusa->Segnale()),E_PORTA_CHIUSA,E_TIME);
  }
  pErrore.Errore(Meccanica.PowErr->Segnale(),E_MEC_TERMICO,E_TIME);
  pErrore.Errore(Turbo.Errore->Segnale(),E_TURBO,E_TIME);
  pErrore.Errore(Turbo.ErroreSetNormale(),E_TURBO_ACC_TEMPO,E_TIME);


  if(Turbo.Start->Segnale() & Turbo.Stop->Segnale()){
    if(!Turbo.Acc->Segnale() & !Turbo.Normale->Segnale())
      pErrore.Errore(false,E_TURBO_NON_PARTE,300);
  }

  if(Turbo.PowOn->Segnale()){
    pErrore.Errore(FalseTrue(Turbo.PowIsOn->Segnale()),E_TURBO_RELE_POWER,E_TIME);
  }

  if(!getSimula() & IdVuoto==0){
     if(Tavola.OnTime()+ Tavola.OffTime()> 20000)
        pErrore.Errore(true,E_TAVOLA_MANCA_IMPARA,E_TIME);
     pErrore.Errore(Tavola.Errore(),E_TAVOLA,E_TIME);
  }


  pErrore.Errore(Confronto(Camera.Temp->Segnale(),VAC_MAX_TEMP_CAMERA),E_TEMP_CAMERA,E_TIME);
  pErrore.Errore(Confronto(Turbo.Temp->Segnale(),VAC_MAX_TEMP_TURBO),E_TEMP_TURBO,E_TIME);
  pErrore.Errore(Confronto(Camera.TempCatodo->Segnale(),VAC_MAX_TEMP_CATODO),E_TEMP_CATODO,E_TIME);


  m_Errore=pErrore.Errore();
  return m_Errore;

}
