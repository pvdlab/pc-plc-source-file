#ifndef ARC6010_SLOT_H
#define ARC6010_SLOT_H

//#define UPDATE_MODULI

#define VERSIONEBIAS        0

//Slot 0 8 Out Digitali
#define S_BiasPowOn         "BiasPowOn",0,0,0
#define S_ResPowOn          "ResPowOn",0,1,0
#define S_PaPowOn           "PaPowOn",0,2,0
#define S_PbPowOn           "PbPowOn",0,3,0
//#define S_MecPowOn          "MecPowOn",0,4,0
#define S_AriaCompOn    	"AriaCompOn",0,5,0    //da 0,9,0
#define S_BiasNext          "BiasNext",0,6,0  //da 0,12
#define S_ValGasCamera      "GasCamera",0,7,0     //NC da 0,13
#define S_ReleDx            "ReleDx",0,8,0        //da 6,0
#define S_ReleSx            "ReleSx",0,9,0        //da 6,4
#define S_MecPowOn          "MecPowOn",0,10,0


//Slot 1 8 Out Digitali
#define S_TurboStart		"TurboStart",1,0,0
#define S_TurboStop         "TurboStop",1,1,0
#define S_BSxCTrigger		"BSxCTrigger",1,2,0
#define S_BDxPTrigger		"BDxPTrigger",1,3,0
#define S_MSxCSchermo       "MSxCSchermo",1,4,0
#define S_MDxCSchermo		"MDxCSchermo",1,5,0
#define S_AcetiValv         "AcetiValv",1,6,0
#define S_AzotoValv         "AzotoValv",1,7,0
#define S_ArgonValv         "ArgonValv",1,8,0
#define S_BaratronValv		"BaratronValv",1,9,0
#define S_AcquaValv         "AcquaCamera",1,10,0
#define S_BSxPTrigger		"BSxPTrigger",1,11,0
#define S_MSxCTrigger		"MSxCTrigger",1,12,0
#define S_BDxCTrigger		"BDxCTrigger",1,13,0
#define S_MDxCTrigger		"MDxCTrigger",1,14,0
#define S_AcquaTurbo		"AcquaTurbo",1,15,0


//Slot 2 16 In Digitali
#define S_Reset             "Reset",2,0,0
#define S_PortaChiusa		"PortaChiusa",2,1,0
#define S_SwitchVuoto		"SwitchVuoto",2,2,0
#define S_MecPowErr         "MecPowErr",2,3,0
#define S_MecPowIsOn		"MecPowIsOn",2,4,0
#define S_BiasPowIsOn		"BiasPowIsOn",2,5,0  // da 2,8
#define S_BiasPowErr		"BiasPowErr",2,6,0   // da 2,9
#define S_ResPowIsOn		"ResPowIsOn",2,7,0   // da 2,10
#define S_ResPowErr		    "ResPowErr",2,8,0    // da 2,11
#define S_PaPowIsOn         "PaPowIsOn",2,9,0    // da 2,12
#define S_PaPowErr          "PaPowErr",2,10,0    // da 2,13
#define S_PbPowIsOn         "PbPowIsOn",2,11,0   // da 2,14
#define S_PbPowErr          "PbPowErr",2,12,0    // da 2,15
#define S_BiasIsNext        "S_BiasIsNext",2,14,0  //da 0,12


//Slot 3 16 In Digitali
#define S_TurboAcc          "TurboAcc",3,0,0     // da 2,5
#define S_TurboErr          "TurboErr",3,1,0     // da 2,6
#define S_TurboNorm         "TurboNorm",3,2,0    // da 2,7
#define S_MSxCIsOnArc		"MSxCIsOnArc",3,3,0  // da 3,7
#define S_BSxPIsOnArc		"BSxPIsOnArc",3,4,0  // da 3,9
#define S_BSxCIsOnArc		"BSxCIsOnArc",3,5,0  // da 3,10
#define S_MSxCErrPow		"MSxCErrPow",3,6,0   // da 3,11
#define S_BSxPErrPow		"BSxPErrPow",3,7,0   // da 3,12
#define S_BSxCErrPow		"BSxCErrPow",3,8,0   // da 3,13


//Slot 4 8 Out Analogici
#define S_BDxPSetI          "BDxPSI",4,0,10000,0.008128,1
#define S_MDxCSetI          "MDxCSI",4,2,10000,0.008128,1
#define S_BSxPSetI          "BSxPSI",4,4,10000,0.008128,1
#define S_BDxCSetI          "BDxCSI",4,6,10000,0.008128,1
#define S_BSxCSetI          "BSxCSI",4,5,10000,0.008128,1
#define S_MSxCSetI          "MSxCSI",4,1,10000,0.008128,1
#define S_BiasSetV          "BiasSV",4,3,10000,0.12,0.25


//Slot 5 8 Out Digitali
#define S_BDxPOnPow         "BDxPOnPow",5,0,0  // da 6,1
#define S_TavolaOn          "TavolaOn",5,1,0   // da 6,2
#define S_BSxPOnPow         "BSxPOnPow",5,2,0  // da 6,3
#define S_BiasOn            "BiasOnOff",5,3,0  // da 6,5
#define S_Interlock         "Interlock",5,4,0  // da 6,6
#define S_BiasHVolt         "BiasHVolt",5,5,0  // da 6,7
#define S_MSxCOnPow         "MSxCOnPow",5,6,0  // da 6,8
#define S_PowOutDig         "PowOutdig",5,7,0

//Slot 12 8 Out Digitali
#define S_BSxCOnPow         "BSxCOnPow",11,0,0  // da 6,9
#define S_MDxCOnPow         "MDxCOnPow",11,1,0  // da 6,10
#define S_BDxCOnPow         "BDxCOnPow",11,2,0  // da 6,11
#define S_BiasLVolt         "BiasLVolt",11,3,0  // da 6,12


//Slot 6 12 In Digitali
#define S_BiasIsHVolt       "BiasIsHVolt",6,0,0  // da 7,0
#define S_BiasAllarme       "BiasAllarme",6,1,0      // da 7,1
#define S_BDxPIsOnArc		"BDxPIsOnArc",6,2,0  // da 7,2
#define S_BDxPErrPow		"BDxPErrPow",6,3,0   // da 7,4
#define S_BDxPTemp          "BDxPTemp",6,4,0     // da 7,5
#define S_MDxCIsOnArc		"MDxCIsOnArc",6,5,0  // da 7,6
#define S_BDxCIsOnArc		"BDxCIsOnArc",6,6,0  // da 7,7
#define S_MDxCTemp          "MDxCTemp",6,7,0     // da 7,8
#define S_BDxCTemp          "BDxCTemp",6,8,0     // da 7,9
#define S_MDxCErrPow		"MDxCErrPow",6,9,0   // da 7,10
#define S_BDxCErrPow		"BDxCErrPow",6,10,0  // da 7,11
#define S_BiasIsLVolt       "BiasIsLVolt",6,11,0 // da 7,14

//Slot 7 in Analogici
#define S_BiasGetV           "BiasGV",7,0,10000,0.12,0.25
#define S_BiasGetI           "BiasGI",7,1,10000,0.0004,4

#define S_BDxCGetI           "BDxCGI",7,2,10000,0.01,1
#define S_MSxCGetI           "MSxCGI",7,3,10000,0.01,1
#define S_BSxCGetI           "BSxCGI",7,4,10000,0.01,1
#define S_BDxPGetI           "BDxPGI",7,5,10000,0.01,1
#define S_BSxPGetI           "BSxPGI",7,6,10000,0.01,1
#define S_MDxCGetI           "MDxCGI",7,7,10000,0.01,1

//Slot 8 in Analogici
#define S_TempResistenza      "TRes",8,0,10000,0.1,1
#define S_TempCamera          "TCam",8,1,10000,0.1,1
#define S_TempTurbo           "TTur",8,2,10000,0.1,1
#define S_TempAmbiente        "TAmb",8,3,10000,0.1,1

//Slot 9 in Analogici
#define S_AzotoGetG           "Azoto",9,0,10000,0.14,1
#define S_ArgonGetG           "Argon",9,1,10000,0.10,1
#define S_AcetiGetG           "Aceti",9,2,10000,1,1
//#define S_AcetiGetG           "Aceti",9,2,10000,0.08,1
#define S_Penning             "Penn",9,3,10000,1.667,11.33
#define S_Pirani              "Pira",9,4,10000,0.04427,0.2101
#define S_Baratron            "Bara",9,5,10000,0.00001,1



//Slot 10 out Analogici
#define S_ArgonSetG           "Argon",10,3,10000,0.10,1
#define S_AzotoSetG           "Azoto",10,1,10000,0.14,1
#define S_AcetiSetG           "Aceti",10,2,10000,1,1
//#define S_AcetiSetG           "Aceti",10,2,10000,0.08,1

#define S_TavolaRuota         "TavolaRuota",13,0,0
#define S_Termostato          "Termostato",13,1,0
#define S_AcquaIsOn           "AcquaIsOn",13,3,0
//Slot 12 4 In Digitali
//NON USATI

//Slot 13 4 In Digitali
//NON USATI

///////////////////

#endif // ARC6010_SLOT
