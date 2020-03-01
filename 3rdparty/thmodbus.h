#ifndef THMODBUS_H
#define THMODBUS_H

#include <QThread>
#include "Modulo.h"
#include "StdAfx.h"
#include "modbus.h"
#include "Errore.h"



class thModbus :  public QThread
{
public:
    explicit thModbus(CModulo *pNewModulo, int newModuloCount,CErrore *pNewErrore,bool newDebug,QString newCom,int newPorta,bool newDog);
    void run();
    int ScanModuloTime;
    void Scan(int index);
    bool Errore;

private:
    void getScanTime();
    int  getNext();
    bool ResetModbus();
    void SetRitardo(int);
    void SetWotchDog();
    void ResetAllarmi();
    int  modbus_registro_in_index(CModulo *modulo,int index);
    int  modbus_registro_out_index(CModulo *modulo,int index);

private:
    int   ActIndex;
    QTime *StartTime;
    int   Count;
    QTime *pScanModuloTime;
    int   ScanModulo;
    int   ScanModuloCount;
    int   IndexUrgente;
    int   Index;
    int   IndexUrgenteCount;
    int   IndexCount;
    int   IndexAnalogicoCount;
    int   *pIndexUrgente;
    int   *pIndex;
    int   *pIndexAnalogico;
    int   OgniAnalogico;
    int   OgniUrgente;
    int   IndexAnalogico;
    int   TotScanModuloCount;
    int   ModuloCount;
    bool  Debug;
    bool  DogNonAbilitato;
    CModulo *pModulo;
    QString Com;
    int     Porta;
    modbus_t *m_modbus;
    CErrore  *pErrore;

};

#endif // THMODBUS_H
