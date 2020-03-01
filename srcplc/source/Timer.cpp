
#include "StdAfx.h"
#include "Timer.h"
#include <stdio.h>


CTimer::CTimer ()
{
	
  m_CountStep=0;
  m_ActTime=0;
  m_Start=0;
  m_StepTime=0;
  m_Stop=0;
  m_ActStep=0;
  m_ActStep=false;
  m_Attivato=false;
  m_FineAttivita=false;
  m_Init=false;
  m_LeftTime=0;
  m_IsAttivo=false;
   
}


CTimer& CTimer::operator()(DWORD Time )
{

	if(Time>2147483647L)
       Time=2147483647L;
    
	
	m_Time=Time;
	m_StepTime=m_Time;
    m_CountStep=1;
	m_Attivato=false;
    m_FineAttivita=false;          
	m_ActStep=m_CountStep;
	m_Init=true;
	m_ActTime=0;
    m_Stop=Time;
	m_LeftTime=Time;
	m_IsAttivo=false;

	return *this;
}

CTimer& CTimer::operator()(DWORD Time,DWORD StepTime )
{
    if(m_Attivato)
      return *this; 
	
	if(Time>2147483647L)
       Time=2147483647L;
	
	if(StepTime>Time)
       StepTime=Time;

	m_Time=Time;
	m_StepTime=StepTime;
    
	m_Attivato=false;
    m_FineAttivita=false;
	m_Init=true;
	m_IsAttivo=false;

	m_LeftTime=Time;
    
	if(m_StepTime)
      m_CountStep=m_Time/m_StepTime;
	else
      m_CountStep=1;
    
	m_ActStep=0;
	m_ActTime=0;

	return *this;
}

bool CTimer::Stop(){

    m_ActTime=Clock();
    m_LeftTime=m_Stop-m_ActTime;
    m_Stop=m_ActTime;
    m_FineAttivita=true;

    return m_FineAttivita;

}

bool  CTimer::Timer(bool Segnale){

    if(!Segnale)
        return false;
    else
        return Timer();
}

bool  CTimer::Timer()
{
   /* if(m_Time==0){
       m_IsAttivo=true;
       m_FineAttivita=true;
       return m_FineAttivita;
    }*/

	if(!m_Init)
		return false;

	if(m_FineAttivita)
		return m_FineAttivita;

    if(!m_Attivato){
       m_systime = GetTickCount();
       m_ActTime=Clock();
	   m_Start=m_ActTime;
	   m_Stop=m_Start+m_Time;
	   m_Attivato=true;
	   if(m_Stop-m_ActTime>0)
	     m_LeftTime=m_Stop-m_ActTime;
	   else
         m_LeftTime=0;
	   m_IsAttivo=true;
	}
	else{
	    m_ActTime=Clock();
		m_LeftTime=m_Stop-m_ActTime;
	    if(m_ActTime-m_Start>=m_Stop){
           m_FineAttivita=true;
		   m_IsAttivo=false; 
        }else
           m_FineAttivita=false;
		
	}
	
    return m_FineAttivita;
}

void CTimer::Time(DWORD New_Time)
{
   m_systime-=New_Time-m_ActTime;
   m_ActTime=Clock();
   m_FineAttivita=false;
   m_Attivato=true;
   if(m_StepTime)
      m_ActStep=(int)(m_ActTime-m_Start)/m_StepTime;

} 


bool CTimer::IsAttivo()
{
	return m_IsAttivo;
}

WORD Aumneta(double percent){

}

void CTimer::TimeStop(DWORD New_Time)
{
	

	if(New_Time>2147483647L){
       New_Time=2147483647L;
	   return;
    }

    m_Time=New_Time;
    m_StepTime=m_Time;
    m_FineAttivita=false;
    m_Stop=New_Time;
    m_LeftTime=New_Time;

}


void CTimer::AddTime(DWORD New_Time) 
{
    if(m_Start+m_Time+New_Time>m_Stop)
	   m_Stop=m_Start+m_Time+New_Time;
}


int  CTimer::TimerStep()
{
	int DStep=0;

	if(!m_Init)
		return DStep;

	if(m_FineAttivita)
	   return GetDStep();
	else if(!m_Attivato){
       m_systime = GetTickCount();
       m_ActTime=Clock();
	   m_Start=m_ActTime;
	   m_Stop=m_Start+m_Time;
	   m_Attivato=true;
	   m_IsAttivo=true;
	   return 0;
	}
	else{
	    m_ActTime=Clock();
        m_LeftTime=m_Stop-m_ActTime;
	    if(m_ActTime-m_Start>=m_StepTime*(m_ActStep+1)){
          if( m_ActStep<m_CountStep){
            DStep=GetDStep();
			if((m_ActStep+=DStep)==m_CountStep)
			  m_FineAttivita=true;
			  m_IsAttivo=false;
			}
		  else
            m_FineAttivita=true;
		}
	}

	return DStep;

}

int  CTimer::GetDStep()
{
  int DStep=1;
  int ActStep;

  if(m_StepTime)
    ActStep=(int)(m_ActTime-m_Start)/m_StepTime;
  DStep=ActStep-m_ActStep;
  
  if(DStep>m_CountStep)
     DStep=m_CountStep;

  return DStep;
}

DWORD CTimer::Clock()
{

  DWORD Actsystime =GetTickCount();
  return (DWORD)__min((Actsystime-m_systime),2147483647L);

}


void CTimer::Reset(void)
{
    
    m_Attivato=false;
    m_FineAttivita=false;
	m_ActStep=0;
    m_ActTime=0;
	m_IsAttivo=false;

}


void CTimer::StendBy(void)
{
  if(m_Attivato){  
    m_StartStandBy=Clock();
  }

}

void CTimer::ResetStendBy(void)
{
  if(m_Attivato){  
	m_ActTime=Clock();
	m_systime+=m_ActTime-m_StartStandBy;
  }

}

CSoglia::CSoglia()
{
  m_Attivato=false;
  m_FineAttivita=false;
  m_Init=false;
  m_Tempo=0;

}


CSoglia& CSoglia::operator()(DWORD Tempo,WORD StopSegnale ,WORD Errore)
{
  
  if(!m_Attivato){
    m_Tempo=Tempo;
    m_StopSegnale=StopSegnale;
    m_Errore=Errore;
    m_Attivato=false;
    m_FineAttivita=false;
    m_Superato=false;
    m_Init=true;
  }

  return *this;
}


bool CSoglia::Soglia(WORD Segnale)
{
	if(!m_Init)
		return false;
	
	if(m_FineAttivita)
		return m_FineAttivita;
	else if(!m_Attivato){
      m_Timer(m_Tempo,m_Tempo);
	  m_Attivato=true;
    }
	else{
        if(m_Timer.Timer() || Segnale<m_StopSegnale){ 
		  if(Segnale<m_StopSegnale)
            m_Superato=true;
		  else
            m_Superato=false;
		  m_FineAttivita=true;
		}
	}

    return m_FineAttivita;
}

WORD CSoglia::Errore()
{
	if(!m_Attivato || !m_FineAttivita) 
	   return 0;	

	if(m_Superato)
      return 0;
  	else
      return m_Errore;
}

void CSoglia::Reset()
{
    m_Attivato=false;
    m_FineAttivita=false;
    m_Superato=false;
	m_Timer.Reset();
}


CSTimer::CSTimer ()
{

  m_StartTime=GetTickCount();
  m_ActTime=m_StartTime;

}


DWORD CSTimer::Time(void)
{
  m_ActTime=Clock();
  return m_ActTime;

}


void CSTimer::Reset(void)
{

  m_StartTime = GetTickCount();
  m_ActTime = m_StartTime;

}


DWORD CSTimer::Clock()
{
  m_ActTime = GetTickCount();

  return (DWORD)__min(((m_ActTime)/1000),2147483647L);
}



COnTimer::COnTimer ()
{

  m_IsOn=false;
  m_OnTime=0;
  m_StartTime = Clock();
  m_ActTime = m_StartTime;
  m_Fattore=1;

}


double COnTimer::Time(void)
{
    if(m_IsOn){
        m_ActTime=Clock();
        double Tempo=m_ActTime-m_StartTime;
        Tempo*=m_Fattore;
        return m_OnTime+Tempo;
    }else{
        m_StartTime = Clock();
        m_ActTime = m_StartTime;
        return m_OnTime;
    }


}

double COnTimer::Timer(bool newIsOn,double newFattore){

    if(newFattore!=0)
      m_Fattore=newFattore;
    return Timer(newIsOn);

}


double COnTimer::Timer(bool newIsOn){

    if(!newIsOn & (newIsOn!=m_IsOn)){
        double Tempo=m_ActTime-m_StartTime;
        Tempo*=m_Fattore;
        m_OnTime+=Tempo;
        m_IsOn=newIsOn;
    }

    m_IsOn=newIsOn;

    return Time();

}


void COnTimer::Reset(void)
{
    
    m_OnTime=0;

}

void COnTimer::OnTime(double newOnTime){

    m_OnTime=newOnTime;
    m_StartTime=Clock();
    m_ActTime = m_StartTime;

}


DWORD COnTimer::Clock()
{

    return (DWORD)__min(((GetTickCount())/1000),2147483647L);

}


//CBTimert
CBTimer::CBTimer ()
{
   m_Time=0;
}


CBTimer& CBTimer::operator()(DWORD Time)
{
	
	if(Time>2147483647L)
       Time=2147483647L;	

	m_Time=Time;
    m_StartTime = GetTickCount();
	m_TimeStop = m_StartTime+Time;
	m_ActTime=m_StartTime;
	m_StandByTime=m_ActTime-m_StartTime;

	return *this;
}

void CBTimer::Timer()
{
 
   Clock();
   m_StandByTime=m_ActTime-m_StartTime;

}


WORD CBTimer::PercentOffTime(void)
{
  double DTime;

  DTime=(double)__min(((m_ActTime-m_StartTime)),2147483647L);
  if(m_Time)
    DTime/=(double)m_Time;
  DTime*=100;

  return (WORD)__min(DTime,100);

}

WORD CBTimer::OffTime(void)
{

  return (WORD)__min(((m_ActTime-m_StartTime)),2147483647L);

}

void CBTimer::StendBy()
{
    

    m_ActTime = GetTickCount();

	m_StartTime = m_ActTime-m_StandByTime;

	m_TimeStop = m_StartTime+m_Time;

}

DWORD CBTimer::Clock()
{
  m_ActTime = GetTickCount();

  return (DWORD)__min(((m_ActTime-m_StartTime)),2147483647L);
}

void CBTimer::Reset()
{
	
    m_StartTime = GetTickCount();
	m_TimeStop = m_StartTime+m_Time;
	m_ActTime=m_StartTime;
	m_StandByTime=m_ActTime-m_StartTime;

}
