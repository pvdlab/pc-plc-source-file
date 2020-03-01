#ifndef CAMERA_H
#define CAMERA_H
#include "Segnale.h"


class CCamera
{
public:
    CCamera();
    virtual ~CCamera();
    bool Set();

public:
    CIOSegnale  *Acqua;
    CIOSegnale  *AcquaIsOn;
    CIOSegnale  *Temp;
    CIOSegnale  *TempCatodo;
    CIOSegnale  *PortaChiusa;
    CIOSegnale  *AriaComp;
    CIOSegnale  *Valv;
    CIOSegnale  *Interlock;
    CIOSegnale  *LuceRossa;
    CIOSegnale  *LuceGialla;
    CIOSegnale  *LuceVerde;
    CIOSegnale  *Reset;
    CIOSegnale  *Termostato;
    CIOSegnale  *Gas;
    CIOSegnale  *SchermoTurbo;

};

#endif // CAMERA_H
