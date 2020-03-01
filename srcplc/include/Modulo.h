#ifndef CMODULO_H
#define CMODULO_H

#include "StdAfx.h"
#include "MainCostanti.h"


class CPoint
{

public:
    CPoint();

public:
    int     Point;
    char    Descrizione[D_DESCRIZIONE];

};

class CModulo
{

public:
    CModulo();


public:
    void setSlot(int,int,int , int ,int,int,bool ,const char newnetPlc[], bool, BYTE ,BYTE   newDataIndex);
    void setDescrizione(int ,const char descrizione[]);
    void scritto(){DaScrivere=false;}
    void setDataIndex(BYTE newDataIndex){DataIndex=newDataIndex;}
    WORD  setSegnale(int newPoint,WORD newSegnale);
    WORD  getSegnale(int newPoint);
    void  SetOff();

public:
    BYTE        Index;
    BYTE        DataIndex;
    CPoint      *pPoint;
    int         Count ;
    int         TipoSlot;
    int         Slot;
    BYTE        *Data;
    BYTE        *Scrivo;
    bool        DaScrivere;
    bool        InScrittura;
    int         SizeData;
    bool        IsDigitale;
    int         Versione;
    int         Risposta;
    int         LettiCount;
    int         ErroriCount;
    int         ScanTime;
    bool        Errore;
    int         FondoScala;
    DWORD       systemTime;
    bool        Enabled;
    const char *netPlc;
    bool        Urgente;
    bool        Posizionato;
    BYTE        Porta;
};

#endif
