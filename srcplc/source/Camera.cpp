#include "Camera.h"


CCamera::CCamera()
{

    Acqua= NULL;
    Temp= NULL;
    TempCatodo= NULL;
    PortaChiusa= NULL;
    AriaComp= NULL;
    Valv= NULL;
    Interlock= NULL;
    LuceRossa=NULL;
    LuceGialla=NULL;
    LuceVerde=NULL;
    Reset=NULL;
    AcquaIsOn=NULL;
    Gas=NULL;
    SchermoTurbo=NULL;

}



CCamera::~CCamera()
{

}

bool CCamera::Set(){

    return SchermoTurbo->Set();
}
