#include "Turbo.h"

CTurbo::CTurbo()
{

    m_TimerSet(VAC_T_TURBO_SET);
    m_TimerReset(VAC_T_TURBO_RESET);
    m_TimerSetNormale(VAC_T_TURBO_SET_NORMALE);
    m_Set=false;
    m_Reset=true;
    m_ErroreSetNormale=false;

    Start=NULL;
    Stop=NULL;
    Acc=NULL;
    Errore=NULL;
    Normale=NULL;
    PowOn=NULL;
    PowIsOn=NULL;
    Acqua=NULL;
    Temp=NULL;
    Penning=NULL;

}


CTurbo::~CTurbo()
{


}


void CTurbo::Simula(bool newSimula){

    if(newSimula){
        m_TimerSet(VAC_T_TURBO_SET/5);
        m_TimerReset(VAC_T_TURBO_RESET/5);
    }else{
        m_TimerSet(VAC_T_TURBO_SET);
        m_TimerReset(VAC_T_TURBO_RESET);
    }
}

bool CTurbo::isOn(){

   bool on=Start->Segnale();
   on=__max(on,Stop->Segnale());
   on=__max(on,Normale->Segnale());
   on=__max(on,Acc->Segnale());

   return on;
}


void CTurbo::CeckStato()
{

    if(Acc->Segnale() | Errore->Segnale() | Normale->Segnale()){
      m_Reset=false;
      m_Set=true;
    }else{
      m_Reset=true;
      m_Set=false;
    }

}

bool CTurbo::Set()
{

  m_OnTime.Timer(Start->Segnale());

  if(Errore->Segnale()){
    return Reset();
  }

  m_TimerReset.Reset();

  //ACCENDO SOLO DOPO TOT MINUTI
  if(!m_TimerSet.Timer())
      return false;

  m_Set=true;
  m_Set=__min(m_Set,Stop->Set(true));
  m_Set=__min(m_Set,Start->Set(true));
  m_Set=__min(m_Set,Normale->Segnale());

  //VERIFICO SE ACCELLERA IN 12 MINUTI ALTRIMENTI ERRORE
  if(!m_Set){
     m_ErroreSetNormale=m_TimerSetNormale.Timer();
  }

  return m_Set;

}

bool CTurbo::Reset()
{
  m_OnTime.Timer(Start->Segnale());
  m_Reset=true;
  m_Reset=__min(m_Reset,Stop->Set(false));
  m_Reset=__min(m_Reset,Start->Set(false));

  //MI ASSICURO DI AVER SPENTO/////
  if(!m_Reset){
      m_OnTime.Reset();
      return m_Reset;
  }

  m_TimerSet.Reset();
  m_TimerSetNormale.Reset();

  if(!m_TimerReset.Timer())
      return false;

  return m_Reset;

}

bool CTurbo::SetOff()
{

  m_Reset=true;
  m_Reset=__min(m_Reset,Stop->Set(false));
  m_Reset=__min(m_Reset,Start->Set(false));

  m_TimerSet.Reset();
  m_TimerSetNormale.Reset();

  return m_Reset;

}






