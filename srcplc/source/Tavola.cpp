#include "Tavola.h"

#define DEF_COUNT 6

CTavola::CTavola()
{
  m_Errore=false;
  m_Ruota=false;
  m_Timer(20000);
  m_TimerStart(20000);
  m_Count=0;
  m_TmpErrore=false;
  m_Reset=false;

  OnOff=NULL;
  Ruota=NULL;
  Speed=NULL;
  Manuale=NULL;
}

CTavola::~CTavola()
{

}

bool CTavola::Impara()
{

  if(!OnOff->Segnale() && !m_Count){
      if(!OnOff->Set(true))
          return false;
      *Speed=m_GiriSecondo;
      m_Count=0;
      m_Timer(20000);
      m_Timer.Reset();
      m_Ruota=Ruota->Segnale();
      m_OnTime=0;
      m_OffTime=0;
      m_MinOnTime=2000;
      m_MinOffTime=2000;
      m_TmpErrore=false;
  }

  if(m_Count==50){
     *OnOff=false;
     *Speed=0;
     m_Timer(2*m_OnTime+2*m_OffTime);//m_Timer(m_OnTime+m_OffTime);
     return true;
  }
  else{
      m_Timer.Timer();
      if(Ruota->Segnale()!=m_Ruota){
         if(m_Count>4){
           if(m_Ruota){
             m_OnTime=__max(m_OnTime,(WORD)m_Timer.Time());
             m_MinOnTime=__min(m_OnTime,(WORD)m_Timer.Time());
           }
           else{
             m_OffTime=__max(m_OffTime,(WORD)m_Timer.Time());
             m_MinOffTime=__min(m_OffTime,(WORD)m_Timer.Time());
           }
         }
         m_Timer.Reset();
         m_Ruota=Ruota->Segnale();
         m_Count++;
      }
  }
  return false;

}



bool CTavola::Set()
{
  WORD MinTime;
  WORD MaxTime;

  if(m_Errore)
      return false;

  if(!OnOff->Set(true))
      return false;
  else{
      if(!m_TimerStart.Timer())
          return false;
  }

  m_Reset=false;
  *Speed=m_GiriSecondo;

  if(m_TmpErrore){
     goto Errore;
  }

  if(m_Count<=DEF_COUNT){ // Sincronizza
      if(Ruota->Segnale()!=m_Ruota){
         m_Timer.Reset();
         m_Ruota=Ruota->Segnale();
         m_Count++;
      }
      if(m_Timer.Timer()){
          m_TmpErrore=true;
          goto Errore;
      }

      if(m_Count==DEF_COUNT)
        m_Timer.Reset();
      return true;
  }

  m_Timer.Timer();
  if(m_Ruota){ // VERO
     MinTime=m_OnTime/2;//*0.4;
     MaxTime=m_OnTime*2;
  }
  else{ // FALSE
     MinTime=m_OffTime/2;//*0.4;
     MaxTime=m_OffTime*2;
  }

  if(m_Timer.Time()<MinTime){
    if(Ruota->Segnale()!=m_Ruota){
          goto Errore;
    }
  }
  else if(m_Timer.Time()>MinTime && m_Timer.Time()<MaxTime ){
       if(Ruota->Segnale()!=m_Ruota){
          m_Ruota=Ruota->Segnale();
          m_Timer.Reset();
       }
  }
  else if(m_Timer.Time()>MaxTime){
        if(Ruota->Segnale()==m_Ruota){
           m_TmpErrore=true;
           m_Timer.Reset();
           goto Errore;
         }
  }

  return OnOff->Segnale();

Errore:
  if(Ruota->Segnale()!=m_Ruota){
    m_Timer.Reset();
    m_TmpErrore=false;
    m_Ruota=Ruota->Segnale();
    //if(Debug)
    //   MostraEvento( "Vedo TmpErrore Tavola",Color::FG_GREEN);
    return true;
  }
  if(m_Timer.Timer()){
    m_Errore=true;
    if(Debug)
       MostraEvento( "ERRORE Tavola",Color::FG_RED);
    *Speed=0;
    *OnOff=false;
    m_TimerStart.Reset();
    return false;
  }
  else
     return true;

}


bool  CTavola::Reset()
{
    m_Timer.Reset();
    m_TmpErrore=false;
    m_Errore=false;
    m_Count=DEF_COUNT;

    if(Debug & !m_Reset)
       MostraEvento( "Tavola Reset",Color::FG_DEFAULT);

    m_Reset=true;
    m_Reset=OnOff->Set(false);
    m_Reset=__min(m_Reset, Speed->Set(0));
    m_TimerStart.Reset();

    return m_Reset;

}



