#ifndef CERRORE_H
#define CERRORE_H

#define BUFF_SIZE	  3

#include "StdAfx.h"
#include "MainCostanti.h"

class CErrore
{

public:
    CErrore();
    CErrore& operator()(BYTE *pErr,CSTimer *pMainTimer,int NewTime, const char *newDescrizione);
    void    append(int,const char *);
    void    Clear();
    int     Edit(std::vector<std::string> *newLista);
    void    Edit();
    void    first();
    bool    next(int &newErrore);
    int     getErrore();
    bool    IsErrore(int newErrore);
    void	Errore(bool NewErrore,BYTE Pos,DWORD NewTime);
    bool	Errore();
    void	Aspetta(int NewTime);
	void	Reset();
	BYTE    *pErrore(){ return m_pErr;}
	void    Informa(bool NewErrore,BYTE Pos);
    int     Tempo(){ return m_ErrTimer.Time();}
    int     MainTempo(){ return m_pMainTimer->Time();}
    char    *Descrizione(){ char *pBuffer=m_Descrizione; return pBuffer;}

private:
    char    m_Descrizione[D_DESCRIZIONE];
    std::vector<std::string> m_Lista;
    int      m_ListaCount;
    BYTE     *m_pErr;
	CSTimer  *m_pMainTimer;
    CTimer   m_ErrTimer;
	bool     m_Aspetta;
    int      m_Time;
	bool     m_Errore;
    int      m_PowerTimeOut;
    BYTE     m_bErrore[BUFF_SIZE];
    BYTE     m_emitErrore[D_COMP_ERRORE];
    bool     m_TmpErrore;
    short    index;
    DWORD    m_LastAttivita;

};

#endif
