#ifndef CTIMER_H
#define CTIMER_H

#include "StdAfx.h"

class CTimer  
{
public:

    CTimer();

    CTimer& operator()(DWORD, DWORD);
    CTimer& operator()(DWORD);
    bool    IsAttivo();
    bool    Stop();
    bool    Timer();
    bool    Timer(bool Segnale);
	void    Break() {m_Init=false;} 
	int     TimerStep();
	void    Reset();
    DWORD   Time(){return m_ActTime;}
    DWORD   LeftTime(){return m_LeftTime;}
    void    Time(DWORD New_Time);
	int     Step(){return (m_CountStep-m_ActStep);} 
    DWORD   TimeStop() {return m_Stop;}
    void    TimeStop(DWORD New_Time);
    void    AddTime(DWORD New_Time);
	void    StendBy();
	void    ResetStendBy();
	bool    IsStepTimer(){if(m_CountStep>1) return true; else return false;}
    void    Aumneta(double percent);
    
protected:
    DWORD Clock();
	int  GetDStep();

private:
    DWORD    m_systime;
    DWORD    m_ActTime;
    DWORD    m_Start;
    DWORD    m_Stop;
    DWORD    m_StartStandBy;
    DWORD    m_Time;
    DWORD    m_StepTime;
	int      m_ActStep;
	int      m_CountStep;
	bool     m_Attivato;
	bool     m_IsAttivo;
	bool     m_FineAttivita;
	bool     m_Init;
	bool     m_IsStepTimer;
    DWORD    m_LeftTime;

};



class CSoglia  
{
public:
    CSoglia();

    CSoglia& operator()(DWORD Tempo,WORD StopSegnale ,WORD Errore);
    WORD  Errore();
    DWORD           Tempo() {return m_Timer.Time();}
	void            Break() {m_Init=false;} 
    bool            Soglia(WORD);
	void            Reset();

private:
	CTimer          m_Timer;
    DWORD           m_Tempo;
    WORD            m_Errore;
    WORD            m_StopSegnale;
    bool            m_Attivato;
	bool            m_Superato;
	bool            m_FineAttivita;
	bool            m_Init;
};

class CSTimer
{
public:

    CSTimer();
    DWORD       Time();
    void        Reset();

protected:
    DWORD Clock();

private:
    DWORD    m_StartTime;
    DWORD    m_ActTime;

};

class COnTimer
{
public:

    COnTimer();
    double  Time();
    double  Timer(bool newIsOn);
    double  Timer(bool newIsOn,double newFattore);
    void    Reset();
    void    OnTime(double newOnTime);
    double  OnTime(){return m_OnTime;}

protected:
    DWORD   Clock();

private:
    DWORD    m_StartTime;
    DWORD    m_ActTime;
    double   m_OnTime;
    bool     m_IsOn;
    double   m_Fattore;

};

class CBTimer  
{
public:

    CBTimer();

    CBTimer& operator()(DWORD Tempo);
    WORD   OffTime();
    WORD   PercentOffTime();
    void   Timer();
	void   StendBy();
	void   Reset();

protected:
    DWORD   Clock();

private:
    DWORD   m_StartTime;
    DWORD   m_TimeStop;
    DWORD   m_ActTime;
    DWORD   m_StandByTime;
    DWORD   m_Time;

};

#endif
