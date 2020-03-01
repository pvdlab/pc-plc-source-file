#ifndef CPLC_H
#define CPLC_H

#include "Errore.h"
#include "StdAfx.h"
#include "Segnale.h"
#ifdef CON_MODBUS
#include "thmodbus.h"
#include "Initfile.h"
#endif



class CPlc
{
public:
    CPlc();
    void ScanModulo();
    void InitScanModulo();
    void ResetModbus();
    bool IsSimula(){return m_IsSimula;}
    void OpenPort();
    bool Errore() {return m_Errore;}
    int  OffSet(int Slot, int Point);
    int  SizeData();
    int  GetPlcSegnali(BYTE *);
    int  SetPlcSegnali(BYTE **);
    int  GetLenPlcSegnali();
    void SetSegnale(BYTE *pStr);
    CIOSegnale    *Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala );
    CIOSegnale    *Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala, double a,double b );

public:
    CModulo *pModulo;
    int  ScanTime;
    int  ModuloCount;
    CErrore pErroreModulo;
#ifdef CON_MODBUS
    thModbus *pScanModulo;
#endif
    bool DogNonAbilitato;


private:
    bool    m_Errore;
    bool    m_IsSimula;
    bool    m_Debug;


};

#endif
