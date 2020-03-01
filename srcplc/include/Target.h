#ifndef TARGET_H
#define TARGET_H
#include "StdAfx.h"


namespace CTargetMeteriale{
    enum Code {
        Titanio =1,
        TiAl=2,
        CrAl=3,
        Cromo=4
    };
}


struct CTarget{
    BYTE    TipoTarget;
    double  OnTime;
    bool    Cambia;
};

class CTargetLista
{
public:
    CTargetLista(int newTargetCount);
    virtual ~CTargetLista();
    CTarget *pTarget;

private:
    int m_TargetCount;
};

class CScorta
{
public:
    CScorta(int newTargetCount);
    bool    Apri();
    bool    Salva();
    CTarget *Target(int index);
    double  OnTime(int index,double onTime);
    double  OnTime(int index);
    int     Cambia(char *Buffer);
    bool    Cerca(BYTE **pStr);
    void    Print();
    int     TipoTraget(int index);

private:
    std::vector<CTargetLista *> pScorta;
    int m_Index;
    int m_TargetCount;

};

#endif // TARGET_H
