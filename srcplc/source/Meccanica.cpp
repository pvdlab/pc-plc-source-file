#include "Meccanica.h"

CMeccanica::CMeccanica()
{
  m_Set=false;
  m_Reset=true;
  PowOn=NULL;
  PowIsOn=NULL;
  PowErr=NULL;
  m_TimerSet(VAC_T_TURBO_SET);

}



CMeccanica::~CMeccanica()
{

}


bool CMeccanica::IsOn(){

    return PowIsOn->Segnale();
}

void CMeccanica::CeckStato()
{

    if(PowIsOn->Segnale() | PowErr->Segnale()){
      m_Reset=false;
      m_Set=true;
    }
    else{
      m_Reset=true;
      m_Set=false;
    }

}

bool CMeccanica::Set()
{

  m_OnTime.Timer(PowIsOn->Segnale());

  *PowOn=true;
  if(PowIsOn->Segnale()){
     if(!m_TimerSet.Timer()){
         return false;
     }
     m_Set=PowIsOn->Segnale();
     m_Reset=false;
     return m_Set;
  }

  return m_Set;

}

bool CMeccanica::Reset()
{
   m_OnTime.Timer(PowIsOn->Segnale());
   m_TimerSet.Reset();
   *PowOn=false;

   if(!PowIsOn->Segnale()){
     m_Reset= true;
     m_Set=false;
   }

   return m_Reset;

}



bool CMeccanica::SetOff()
{
    return Reset();

}
