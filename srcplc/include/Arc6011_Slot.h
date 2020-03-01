#ifndef ARC6011_SLOT_H
#define ARC6011_SLOT_H


///SOLT 1 OUT DIGITALE////
#define S_BiasPowOn     "BiasPowOn",0,0,0
#define S_ResPowOn      "ResPowOn ",0,1,0
#define S_PaPowOn       "PaPowOn",0,2,0
#define S_PbPowOn       "PbPowOn",0,3,0
#define S_PcPowOn       "PcPowOn",0,4,0
#define S_MecPowOn      "MecPowOn",0,5,0
#define S_TurboPowOn	"TurboPowOn",0,6,0
#define S_HVentValv     "HVentValv",0,7,0

///SOLT 2 OUT DIGITALE////
#define S_ArgonValv     "ArgonValv",1,0,0
#define S_AzotoValv     "AzotoValv",1,1,0
#define S_AcetiValv     "AcetiValv",1,2,0
#define S_BaratronValv	"BaratronValv",1,3,0
#define S_AcquaValv     "AcquaValv",1,4,0
#define S_TurboStart	"TurboStart",1,5,0
#define S_TurboStop     "TurboStop",1,6,0
#define S_TavolaOn      "TavolaOn",1,7,0

///SOLT 3 OUT DIGITALE////
#define S_ReleDx        "ReleDx",2,0,0
#define S_ReleSx        "ReleSx",2,1,0
#define S_SxPTrigger	"SxPTrigger",2,2,0
#define S_DxPTrigger	"DxPTrigger",2,3,0
#define S_DxCTrigger	"DxCTrigger",2,4,0
#define S_SxCTrigger	"SxCTrigger",2,5,0
#define S_MidBTrigger	"MidBTrigger",2,6,0
#define S_MidATrigger	"MidATrigger",2,7,0

///SOLT 4 OUT DIGITALE////
#define S_SxPOnPow      "SxPOnPow",3,4,0
#define S_DxPOnPow      "DxPOnPow",3,5,0
#define S_DxCOnPow      "DxCOnPow",3,6,0
#define S_SxCOnPow      "SxCOnPow",3,7,0

///SOLT 5 OUT DIGITALE////
#define S_MidBOnPow     "MidBOnPow",4,0,0
#define S_MidAOnPow     "MidAPOnPow",4,1,0
#define S_LVentValv     "LVentValv",4,4,0
#define S_AcquaTurbo	"AcquaTurbo",4,5,0
#define S_PenningOnOff	"PenningOnOff",4,6,0

///SOLT 5 OUT DIGITALE////
#define S_LuceRossa     "LuceRossa",5,0,0
#define S_LuceGialla    "LuceGialla",5,1,0
#define S_LuceVerde     "LuceVerde ",5,2,0


///SOLT 1 IN DIGITALE////
#define S_VuotoManuale	"VuotoManuale",6,0,0
#define S_VenManuale    "VenManuale",6,1,0
#define S_TavolaIsOn	"TavolaIsOn",6,2,0
#define S_TavolaRuota	"TavolaRuota",6,3,0
#define S_BiasPowIsOn	"BiasPowIsOn",6,4,0
#define S_ResPowIsOn	"ResPowIsOn ",6,5,0
#define S_PaPowIsOn     "PaPowIsOn",6,6,0
#define S_PbPowIsOn     "PbPowIsOn",6,7,0

///SOLT 2 IN DIGITALE////
#define S_PcPowIsOn     "PcPowIsOn",7,0,0
#define S_MecPowIsOn	"MecPowIsOn",7,1,0
#define S_BiasPowErr	"BiasPowErr",7,2,0
#define S_ResPowErr     "ResPowErr",7,3,0
#define S_PaPowErr      "PaPowErr",7,4,0
#define S_PbPowErr      "PbPowErr",7,5,0
#define S_PcPowErr      "PcPowErr",7,6,0
#define S_MecPowErr     "MecPowErr",7,7,0

///SOLT 3 IN DIGITALE////
#define S_TurboErr      "TurboErr",8,0,0
#define S_TurboNorm     "TurboNorm",8,1,0
#define S_TurboPowIsOn	"TurboPowIsOn",8,2,0

#define S_SxPIsOnArc	"SxPIsOnArc",8,3,0
#define S_DxPIsOnArc	"DxPIsOnArc",8,4,0
#define S_DxCIsOnArc	"DxSIsOnArc",8,5,0
#define S_SxCIsOnArc	"SxCIsOnArc",8,6,0
#define S_MidBIsOnArc	"MidBIsOnArc",8,7,0

///SOLT 4 IN DIGITALE////
#define S_MidAIsOnArc	"MidAIsOnArc",9,0,0
#define S_SxPErrPow     "SxPErrPow",9,3,0
#define S_DxPErrPow	    "DxPErrPow",9,4,0
#define S_DxCErrPow	    "DxCErrPow",9,5,0
#define S_SxCErrPow     "SxCErrPow",9,6,0
#define S_MidBErrPow	"MidBErrPow",9,7,0

///SOLT 5 IN DIGITALE////
#define S_MidAErrPow	"MidAErrPow",10,0,0
#define S_TurboAcc		"TurboAcc",10,3,0
//#define S_BiasIsHVolt	"IsHVolt",10,4,0
//#define S_BiasIsLVolt	"IsLVolt",10,5,0
#define S_BiasOnEnabled "OnEnabled ",10,6,1 ///abilito FalseTrue
#define S_BiasArc       "Arc",10,7,1        ///abilito FalseTrue

///SOLT 6 IN DIGITALE////
///NON USATO/////////////


///SOLT 7 OUT ANALOGICO////
#define S_ArgonSetG     "Argon",12,0,10000,0.10,1
#define S_AzotoSetG     "Azoto",12,1,10000,0.14,1
#define S_AcetiSetG     "Aceti",12,2,10000,0.08,1
#define S_BiasSetV      "Bias",12,3,10000,0.20,1
#define S_TavolaSpeed	"Tavola",12,4,10000,0.01,1

///SOLT 8 OUT ANALOGICO////
#define S_SxPSetI       "SxPSetI",13,0,10000,0.008128,1
#define S_DxPSetI	    "DxPSetI",13,1,10000,0.008128,1
#define S_DxCSetI       "DxCSetI",13,2,10000,0.008128,1
#define S_SxCSetI       "SxCSetI",13,3,10000,0.008128,1
#define S_MidBSetI      "MidBSetI",13,4,10000,0.008128,1
#define S_MidASetI      "MidASetI",13,5,10000,0.008128,1


///SOLT 9 IN ANALOGICO////
#define S_ArgonGetG     "Argon",14,0,10000,0.1,1
#define S_AzotoGetG     "Azoto",14,1,10000,0.14,1
#define S_AcetiGetG     "Aceti",14,2,10000,0.08,1
#define S_BiasGetI      "BiasGetI",14,3,10000,0.0048,1
#define S_BiasGetV      "BiasGetV",14,4,10000,0.20,1
#define S_Penning       "Penning",14,5,10000,1.667,11.33
#define S_Pirani        "Pirani",14,6,10000,0.04427,0.2101
#define S_Baratron      "Baratron",14,7,10000,1.0E-4,1

///SOLT 10  IN ANALOGICO////
#define S_SxPGetI       "SxP",15,0,10000,0.01,1
#define S_DxPGetI       "DxP",15,1,10000,0.01,1
#define S_DxCGetI       "DxC",15,2,10000,0.01,1
#define S_SxCGetI       "SxC",15,3,10000,0.01,1
#define S_MidBGetI      "MidB",15,4,10000,0.01,1
#define S_MidAGetI      "MidA",15,5,10000,0.01,1

#define S_Reset         "Reset",16,0,0
#define S_AcquaIsOn     "AcquaIsOn",16,2 ,0
#define S_PortaChiusa	"PortaChiusa",16,3,0
#define S_Termostato    "Termostato",16,4,0


///SOLT 11 IN DIGITALE////
#define S_BiasOn        "BiasOn",17,0,0
#define S_BiasHVolt     "BiasHVolt",17,1,0
#define S_BiasLVolt     "BiasLVolt",17,2,0
#define S_Interlock     "Interlock ",17,3,0

///SOLT 12 TERMOCOPPIA////
#define S_TempResistenza "TRes",18,0,10000,0.1,1
#define S_TempCamera 	 "TCam",18,1,10000,0.1,1
#define S_TempTurbo 	 "TTur",18,2,10000,0.1,1
#define S_TempAmbiente 	 "TPor",18,3,10000,0.1,1

///SOLT 13 TERMOCOPPIA////
#define S_BSxPTemp   "BSxPTemp",19,0,10000,0.1,1
#define S_BDxPTemp 	 "BDxPTemp",19,1,10000,0.1,1
#define S_BDxCTemp 	 "BDxCTemp",19,2,10000,0.1,1
#define S_BSxCTemp   "BSxCTemp",19,3,10000,0.1,1
#define S_MidBTemp 	 "MidBTemp",19,4,10000,0.1,1
#define S_MidATemp 	 "MidATemp",19,5,10000,0.1,1


#endif // ARC6011_SLOT_H
