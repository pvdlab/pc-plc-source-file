#ifndef ARC6012_SLOT_H
#define ARC6012_SLOT_H

#define MODULI_AGGIUNTI 6
#include "Arc6010_Slot.h"

//DA MODULI PRECEDENTI
#define S_PennOnOffDue         "PenningOnOff",11,4,0
#define S_PiraniDue            "Pirani",9,7,10000,0.778,4.779254
#define S_PenningDue           "Penn",9,6,10000,0.751879699,9.473684211

//OUT DIGITALI
#define S_ResPowOnDue          "ResPowOn",14,0,0
#define S_MecPowOnDue          "MecPowOn",14,1,0
#define S_TurboStartDue		   "TurboStart",14,2,0
#define S_TurboStopDue         "TurboStop",14,3,0
#define S_AriaCompOnDue    	   "AriaCompOn",14,4,0
#define S_ReleDxDue            "ReleDx",14,5,0
#define S_ReleSxDue            "ReleSx",14,6,0
#define S_DxPSchermoDue        "DxPSchermo",14,7,0

//OUT DIGITALI
#define S_DxCSchermoDue		   "DxCSchermo",15,0,0
#define S_MSxSchermoDue		   "MDxSchermo",15,1,0
#define S_SchermoTurboDue	   "SchermoTurbo",15,2,0
#define S_ValGasCameraDue	   "GasCamera",15,3,0
#define S_TavolaOnDue          "TavolaOn",15,5,0

//IN DIGITALI
#define S_TavolaRuotaDue       "TavolaRuota",16,0,0
#define S_PortaChiusaDue	   "PortaChiusa",16,1,0
#define S_TurboAccDue          "TurboAcc",16,2,0
#define S_TurboErrDue          "TurboErr",16,3,0
#define S_TurboNormDue         "TurboNorm",16,4,0
#define S_MecPowIsOnDue        "MecPowIsOn",16,5,0
#define S_MecPowErrDue         "MecPowErr",16,6,0
#define S_ResPowIsOnDue        "ResPowIsOn",16,7,0

//IN DIGITALI
#define S_ResPowErrDue		   "ResPowErr",17,0,0
#define S_TermostatoDue        "Termostato",17,1,0



//Slot 8 in Analogici
#define S_TempResistenzaDue    "TRes",18,0,10000,0.1,1
#define S_TempCameraDue        "TCam",18,1,10000,0.1,1
#define S_TempTurboDue         "TTur",18,2,10000,0.1,1
#define S_TempAmbienteDue      "TAmb",18,3,10000,0.1,1

//OUT DIGITALI
#define S_BDxPTriggerDue	   "BDxPTrigger",19,0,0
#define S_BDxCTriggerDue	   "BDxCTrigger",19,1,0
#define S_MSxCTriggerDue	   "MSxCTrigger",19,2,0
#define S_BSxCTriggerDue	   "BSxCTrigger",19,3,0
#define S_BSxPTriggerDue	   "BSxPTrigger",19,4,0
#define S_MDxCTriggerDue	   "MDxCTrigger",19,5,0

#define S_VentValvDue          "VentValv",19,6,0
//#define S_InterlockDue         "Interlock",19,7,0

//INT DIGITALI
#define S_DxPSchermoDueAperto  "DxPSchermoAperto",20,0,0
#define S_DxPSchermoDueChiuso  "DxPSchermoChiuso",20,1,0
#define S_DxCSchermoDueAperto  "DxCSchermoAperto",20,2,0
#define S_DxCSchermoDueChiuso  "DxCSchermoChiuso",20,3,0
#define S_MSxSchermoDueAperto  "MSxSchermoAperto",20,4,0
#define S_MSxSchermoDueChiuso  "MSxSchermoChiuso",20,5,0
#define S_ScheTurboDueAperto   "ScheTurboDueAperto",20,6,0
#define S_ScheTurboDueChiuso   "ScheTurboDueChiuso",20,6,0


#endif // ARC6012_SLOT
