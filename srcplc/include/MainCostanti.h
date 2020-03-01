#ifndef MAINCOSTANTI_H
#define MAINCOSTANTI_H

#define PENNING_PTR225 225
#define PENNING_PTR90  90
#define MAX_COM_PORT 4
#define DEF_ERRORE_LETTURA_SCRITTURA 2
#define ARC_6010     6010
#define ARC_6011     6011
#define ARC_6012     6012
#define BP5_BIAS     1
#define MDXII_BIAS   2
#define MAX_MSG 5000
#define GASCOUNT 3
//#define SORGENTECOUNT 6
#define MODULOCOUNT   14
#define SIMULA_STEP_DEFAUL_TIME 200
#define EVENTO_BIAS_LOW         1
#define FINE_MSG  ";\r\n"

#define ZIP_GAS                 0.25
#define ZIP_SORGENTI            1
#define ZIP_TEMP                0.25
#define ZIP_VACUUM              100000
#define ZIP_V_BIAS              0.25
#define ZIP_I_BIAS              10
#define ZIP_PRESSIONE           100000

namespace CCamereComando {
    enum Code {
        Prima =0,
        Seconda=1,
        Entrambe=2
    };
}


namespace CCoating {
    enum Code {
        TiN =1,
        TiAlN=2,
        CrN=3
    };
}


namespace CTipoApp {
    enum Code {
        Server =1,
        Client= 2
    };
}

namespace CTipoComp {
    enum Code {
        Gas =1,
        Sorgente= 2,
        Bias =3,
        SchermoTurbo=6,
        Riscaldo = 5
    };
}

namespace CTipoComDef {
    enum Code {
        PENNING =40,
        BARATRON= 41,
        PIRANI =42,
        GAS=10,
        ARGON=10,
        AZOTO=11,
        ACETI=12,
        SORGENTI=20,
        BIAS_V =30,
        BIAS_I =31,
        TEMP_RESISTENZA=50,
        TEMP_CAMERA=51,
        TEMP_TURBO=52,
        TEMP_CATODO=53,
        SCHERMO_TURBO =60
    };
}


namespace CStatoMacchina {
    enum Code {
         Null=0,
         InVacuum = 1,
         Vacuum  = 2,
         StartProcesso =3,
         InStopVuoto = 4,
         StopVuoto = 5,
         Reset = 6,
         PreSet =7,
         InReset=8,
         Stendby=9,
         InStendby=10,
         EnabledVent=11,
         Ventila =12,
         StopVentila=13,
         StopProcesso=14,
         MacchinaAperta=15,
         Manuale =16,
         PlcManule=17
    };
}


namespace CTipoMessaggio {
    enum Code {
        SetComando= 1,
        GetData=2,
        GetProcesso=3,
        SetSegnale = 4,
        GetInit=5,
        CaricaProcesso=6,
        SetCostanti=7,
        GetCostanti=8,
        ModbusComunica=9,
        ModbusStart=10,
        InitFile=11,
        ModificoProcesso=12,
        Risposta=13,
        Errore=14,
        ResetProcesso=15,
        GetStep=16,
        GetProcMsg=17,
        Simula=18,
        Test=19,
        GetProcRecord=20,
        Debug=21,
        GetOnTime=22,
        CambiaTarget=23,
        GetProcessiDisponibili=24,
        LoadDaFileProcesso=25,
        Manuale=26,
        EliminaProcesso=27
    };
}

namespace CTipoComando {
    enum Code {
        Nessuno=0,
        StendBy=1,
        Vacuum=2,
        Load=3,
        StartProcesso=4,
        Manuale=5,
        StopVuoto=6,
        ResetSegnali=9,
        Reset=10,
        Preset=11,
        resetSorgente=12,
        ResetStendBy=13,
        NextStep=14,
        Enabled=15,
        Ventila=16,
        Impara=17,
        GiraTavola=18,
        StopVentila=19,
        StopProcesso=20,
        ReStart=22,
        CaricaProcesso=23,
        ResetCiclo=24,
        Automatico=25,
        SpegniCatodo=26,
        PlcManule=27,
        SpegniTutto=28,
        Elimina=29
    };
}


namespace CTipoStep {
    enum Code {
        Nessuno=0,
        Vuoto=1,
        Riscaldo=2,
        Etching=3,
        Coating=4,
        Venting=5
    };
}

namespace CTipoSlot {
    enum Code {
        InDigitale=1,
        OutDigitale=2,
        InAnalogico=3,
        OutAnalogico=4,
        OutDigitaleConStep=5
    };
}




#define  Simula_BiasLow             1



//Dimensioni
#define MINUTO              60000
#define MANUALE_TIME_OUT    MINUTO*60
#define TIMEOUT             30000
#define D_DESCRIZIONE       16
#define D_MSG               120

#define D_BUFFER            1024
#define D_FILE              60
#define D_CATODI            3
#define D_SORGENTI          1
#define D_GAS               3
#define D_STEP              15
#define D_COMP_STEP         10
#define D_PROCESSO          (9*D_COMP_STEP*D_COMP_STEP)
#define D_MAX_ANALOG	    4095
#define D_MAX_ANALOG_MODBUS	10000



#define D_COMP_ERRORE           5
#define E_TIME					5000
#define E_TIME_GOTO_STEP		15000


//Errori Macchina
#define E_MODBUS_PORTA			0
#define E_MODBUS_LETTURA		1
#define E_MODBUS_SCRITTURA		2
#define E_PRESET_ERRORE   	    3
#define E_CATODO_CONFLITTO      4
#define E_NEXT_STEP             5
#define E_CATODO_COTRA_SPENTI   6
#define E_BIAS_IS_NOT_ON        7
#define E_PROCESSO_CORROTTO     8
#define E_LICENZA_SCADUTA       9
#define E_TIME_OUT              10
#define E_PROCESSO_NO_FILE      11
#define E_STEP_GAS_ZERO         12
#define E_RESET					13
#define E_FILE_CICLO            14
#define E_MAX_RISCALDO_COUNT    15
#define E_TEMP_ON_ACQUA_ALLARM  16
#define E_FILE_INIT_NON_ESISTE  17
#define E_FILE_PROCESSI_TROPPI  18
#define E_ACQUA_NON_FLUSSA      19
#define E_RESET_MACCHINA        20
#define E_TROPPI_CATODI_SPENTI  21
#define E_TERMOSTATO            22

//SECONDO ERRORI MACCHINA
#define E_DOG                   0
#define E_MANUALE               1
#define E_PORTE_APERTE          2
#define E_COMADO_ERRATO         3
#define E_MACCHINA_IN_PROCESSO  4
#define E_MACCHINA_IN_RUN       5
#define E_MACCHINA_DUE_PORTA    6
#define E_MACCHINA_BIAS_NEXT    7


//Errori Vuoto
#define E_PORTA_CHIUSA			 0
#define E_INTERLOCK              1
#define E_PENNING_START			 2
#define E_PIRANI_SET			 3
#define E_MEC_RELE				 4
#define E_MEC_TERMICO			 5
#define E_TURBO_RELE_POWER		 6
#define E_TURBO					 7
#define E_TAVOLA				 8
#define E_PRESSIONE_ALTA  		 9
#define E_TEMP_TURBO		     10
#define E_TEMP_CAMERA		     11
#define E_TEMP_CATODO		     12
#define E_TURBO_NON_PARTE		 13
#define E_TAVOLA_MANCA_IMPARA	 14
#define E_TURBO_ACC_TEMPO	     15
#define E_PROCESS_START_ENABLED  16
#define E_NESSUN_SENSORE         17

//Errori Resistenza
#define E_RES_RELE_POWER		0
#define E_RES_TERMICO_POWER		1
#define E_RES_TEMP_TURBO		2
#define E_RES_TEMP_CAMERA		3
#define E_RES_PIRANI_ALLARM     4
#define E_RES_NEXT_SETP         5
#define E_RES_TEMP              6
#define E_RES_TERMOCOPPIA       7
#define E_RES_TEMP_ACQUA		8
#define E_RES_NON_SCALDA		9
#define E_RES_ALTA_TEMPERATURA	10

//Errori Bias
#define E_BIAS_RELE_POWER		0
#define E_BIAS_TERMICO_POWER	1
#define E_BIAS_IN_ETCHING		2
#define E_BIAS_CORTO_COATING	3
#define E_BIAS_NO_CORRENTE	    4
#define E_BIAS_TROPPI_LOW_V	    5
#define E_BIAS_ALLARM	        6
#define E_BIAS_CORTO_ETCHING	7
#define E_BIAS_NON_CAMBIA_TAP   8
#define E_BIAS_TAP_ERRATO       9
#define E_BIAS_GETV_IS_ZERO     10
#define E_BIAS_CATODI_SPENTI    11
#define E_BIAS_NEXT_STEP        12
#define E_BIAS_CORTO_PRESET     13
#define E_BIAS_ETCHING_TOO_LOW	14
#define W_BIAS_IS_LOW			15

//Errori Gas
#define E_GAS_INSTABILE	    	0
#define E_GAS_MINIMO        	1
#define E_GAS_PIRANI_ALLARM     2
#define E_GAS_NEXT_STEP         3

//Errori Sorgenti
#define E_SOR_TERMICO_POWER	    0
#define E_SOR_RELE_POWER        1
#define E_SOR_CATODI_OFF		2
#define E_SOR_PRESET			3
#define E_SOR_NEXT_STEP			4

//Errori Catodi
#define E_CAT_POWER				0
#define E_CAT_BACKARC			1
#define E_CAT_TEMP				2
#define E_CAT_TROPPI_TRIG		3
#define E_CAT_POWER_SPENTA		4
#define E_CAT_NON_SI_SPEGNE		5
#define E_CAT_NEXT_STEP         6
#define E_CAT_ARCO_NO_OFF		7
#define E_CAT_ALTA_CORRENTE		8
#define E_CAT_TARGET_CONSUMATO  9
#define E_CAT_ACQUA_IS_OFF      10


///DEFINIZIONI
//DA Bias
#define BIAS_LOW_PRIMO		  100
#define BIAS_LOW_SECONDO	  50
#define BIAS_LOW_TERZO		  10
#define BIAS_TEST             500
#define BIAS_ASPETTA          6000
#define BIAS_RISALITA         10000
#define BIAS_GETV_IS_ZERO     10
#define BIAS_MIN_ETCHING_GETV 100
#define BIAS_TAP_TIME         3000

//DA CATODO
#define CAT_T_TRIGGER                 100
#define CAT_T_ASPETTA                 4000
#define CAT_T_SET_ARC_OFF               5000
#define CAT_T_ASPETTA_ISONARC         6000
#define CAT_T_ASPETTA_BECKARC         8000
#define CAT_T_REGOLA                  8000
#define CAT_T_OFF_ARC                 20000
#define CAT_T_ON                      1000
#define CAT_ARC_ONOFF_COUNT           10
#define CAT_ARC_PERCENT_AUMETA        7.50 //PERCENTUALE
#define CAT_ETCHING_ARC_ONOFF_TIME    10
#define CAT_COATING_ARC_ONOFF_TIME    60
#define CAT_MAX_TRIGGER_COUNT         120
#define CAT_REGOLA_RIFERIMENTO        60
#define CAT_MAX_TEMP_ACQUA            400
#define CAT_TARGET_CONSUMATO          1300
#define CAT_MIN_GET_CORRENTE          4000
#define CAT_MAX_GET_CORRENTE          8500
#define CAT_MIN_SET_CORRENTE          1000

//DA GAS
#define GAS_TIME_STABILIZZA      10000
#define GAS_T_RESET_GAS          4000
#define GAS_T_PRESET_GAS         10000
#define GAS_T_PRESTEP_GAS        10000
#define GAS_T_MAX                5000
#define GAS_T_MIN             100
#define GAS_PIRANI_ALLARM     5350 //5-6 10-2 mbar
#define GAS_BARATRON_ALLARM   5000
#define GAS_MAX_GETGAS        5000
#define GAS_MIN_GETGAS        1000
#define GAS_MIN_ARGON         100
#define GAS_MIN_AZOTO         700
#define GAS_MIN_ACETI         100
#define GAS_TIME_STABILIZZA      10000
#define GAS_TIME_RESET_GAS       4000
#define GAS_TIME_PRESET_GAS      10000
#define GAS_TIME_PRESTEP_GAS     10000
#define GAS_TIME_MAX             4000
#define GAS_TIME_MIN             100



//DA PROCESSA
//#define PRO_TimeOutTime			   1000
#define PRO_TEMP_ON_ACQUA_ALLARM   500
#define PRO_TEMP_ON_ACQUA_CAMERA   400
#define PRO_TEMP_OFF_ACQUA_CAMERA  380
#define PRO_TEMP_ON_ACQUA_TURBO    400
#define PRO_TEMP_OFF_ACQUA_TURBO   380
#define PRO_ONTIME_BIAS            10000
#define PRO_ONTIME_CATODI          1000
#define PRO_ONTIME_GAS             10000
#define PRO_DT_RISCALDO            300000 //AGGIUNGO 5 MINUTI SE NON RAGGIUNGE LA PRESSIONE
#define PRO_MAX_RISCALDO_COUNT     10

//DA Resistenza
#define RES_T_ONOFF_RES       1000
#define RES_MAX_TEMP          5200
#define RES_RISCALDO_CAMERA	  400
#define RES_TEMP_ACQUA        400
#define RES_TIME_SET    	  300000

//DA VUOTO
#define DEF_TAVOLA_GIRI_SECONDO    2500
#define VAC_MAX_TEMP_CAMERA        500
#define VAC_MAX_TEMP_TURBO         450
#define VAC_MAX_TEMP_CATODO        400

#define VAC_PRESSIONE_MAX           1000   //900 mbar
#define VAC_PRESSIONE_IS_START      900   //900 mbar
#define VAC_PRESSIONE_SET           1     //1 mbar
#define VAC_PRESSIONE_PRO_SET       0.010 //1x10-2
#define VAC_PRESSIONE_ALLARME       0.18  // 1,8 10-1 mbar
#define VAC_BARATRON_SET            0.4    // 4 10-1 mbar
#define VAC_PENNING_SET_ON          0.1   // 1,0 10-1 mbar
#define VAC_PENNING_SET_OFF         0.2   // 2,0 10-1 mbar
#define VAC_PENNING_SET             0.005  // 1,0 10-2 mbar
#define VAC_PENNING_RESET           0.006  // 1,0 10-2 mbar
#define VAC_RESISCALDO_ALLARM       0.05   // 5 10-2 mbar


#define VAC_T_PRESSIONE_START       60000  //1 minuti(60000 ms)  per accorgersi di un eventule grossa perdita
#define VAC_T_PRESSIONE_PRO_START   540000 //9 minuti(300000 ms)  per razzingere la pressione per far partire il proceso
#define VAC_T_PRESSIONE_SET         300000 //3 minuti(300000 ms)  per raggiungere la pressione di SET

#define VAC_T_TURBO_SET          60000
#define VAC_T_TURBO_RESET        60000
#define VAC_T_TURBO_SET_NORMALE  660000 //11 monuti prima di poter ARRIVARE A NORMALE
#define VAC_PRE_STOPVUOTO        120000 //2 monuti prima di poter aprire la  valvola di venting
#define VAC_PRE_HVALV_STOPVUOTO  120000 //2 monuti prima di poter aprire la  valvola di venting

#endif // PROVA_H
