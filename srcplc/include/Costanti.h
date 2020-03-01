
#include "Slot_170.h"

//#include "Errori.h"
 
/*#define MSG_SETCOMANDO            1
#define MSG_GETDATA               2 
#define MSG_GETPROCESSO           3 
#define MSG_SETSEGNALE            4 
#define MSG_GETINIT               5 
#define MSG_SETPROCESSO           6 
#define MSG_SETCOSTANTI           7
#define MSG_GETCOSTANTI           8
#define MSG_MODBUS_COMUNICA       9
#define MSG_MODBUS_START_COMUNICA 10
#define MSG_INITFILE              11


	
//Dimensioni
#define D_MSG               200
#define D_BUFFER            500
#define D_FILE              60
#define D_CATODI            3
#define D_SORGENTI          1
#define D_GAS               3
#define D_STEP              15
#define D_COMP_STEP         6
#define D_DESCR             20
#define D_MAX_ANALOG	    4095
#define D_MAX_ANALOG_MODBUS	10000


#define SlotInD      1
#define SlotOutD     2
#define SlotInA      3
#define SlotOutA     4
#define ModSlotInD   5
#define ModSlotOutD  6
#define ModSlotInA   7
#define ModSlotOutA  8

//Errori Macchina
#define E_MODBUS_PORTA			0
#define E_MODBUS_LETTURA		1
#define E_MODBUS_SCRITTURA		2
#define E_COMANDO_RIFIUTATO   	3
#define E_RESET					13

#define E_TIME					5000*/
//Errori Processo
/*#define E_LOW_BIAS				0
#define E_STOP_PER_LOW_V_BIAS	1
#define E_STOP_PER_LOW_I_BIAS	2
#define E_PRESIONE				3
#define E_STOP_PER_PRESIONE		4
#define E_STOP_PER_TEMP_CAMERA	5
#define E_STOP_PER_PLC			6

//Errori Vuoto
#define E_PORTA_CHIUSA			0
#define E_SWITCH_VUOTO			1
#define E_PIRANI_PRESET			2
#define E_PIRANI_SET			3
#define E_MEC_RELE				4
#define E_MEC_TERMICO			5
#define E_TURBO_RELE_POWER		6	
#define E_TURBO					7
#define E_TAVOLA				8
#define E_PRESSIONE_ALTA  		9
#define E_TEMP_TURBO		    10
#define E_TEMP_CAMERA		    11
#define E_TURBO_NON_PARTE		12


//Errori Resistenza
#define E_RES_RELE_POWER		0
#define E_RES_TERMICO_POWER		1
#define E_RES_TEMP_TURBO		2
#define E_RES_TEMP_CAMERA		3



//Errori Bias
#define E_BIAS_RELE_POWER		0
#define E_BIAS_TERMICO_POWER	1
#define E_BIAS_IN_ETCHING		2
#define E_BIAS_CORTO_TAVOLA	    3
#define E_BIAS_NO_CORRENTE	    4
#define E_BIAS_TROPPI_LOW_V	    5
#define E_BIAS_ALLARM	        6
#define W_BIAS_IS_LOW			13

//Errori Gas
#define E_GAS_INSTABILE	    	0
#define E_GAS_MINIMO        	1
#define E_GAS_PIRANI_ALLARM     2

//Errori Sorgenti
#define E_SOR_TERMICO_POW	    0
#define E_SOR_RELE_POW			1
#define E_SOR_CATODI_OFF		2
#define E_SOR_PRESET			3

//Errori Catodi
#define E_CAT_POWER				0
#define E_CAT_BACKARC			1
#define E_CAT_TEMP				2
#define E_CAT_TROPPI_TRIG		3
#define E_CAT_POWER_SPENTA		4
#define E_CAT_ARCO_NO_OFF		13


///DEFINIZIONI
//DA BIAS
#define BIAS_LOW_PRIMO		 100
#define BIAS_LOW_SECONDO	 50
#define BIAS_LOW_TERZO		 10

//DA CATODO
#define CAT_T_TRIGGER                 200
#define CAT_T_ASPETTA                 1000
#define CAT_T_OFF_ARC                 20000
#define CAT_T_ON                      500
#define CAT_ARC_ONOFF_COUNT           10
#define CAT_ETCHING_ARC_ONOFF_TIME    2
#define CAT_COATING_ARC_ONOFF_TIME    60
#define CAT_MAX_TRIGGER_COUNT         45

//DA GAS
#define GAS_T_STABILIZZA      10000
#define GAS_T_RESET_GAS       4000
#define GAS_T_PRESET_GAS      10000 
#define GAS_T_PRESTEP_GAS     10000 
#define GAS_T_MAX             4000
#define GAS_T_MIN             100
#define GAS_PIRANI_ALLARM     5500
#define GAS_MAX_GETGAS        5000

//DA PROCESSA
#define PRO_TimeOutTime				   1000
#define PRO_TEMP_ON_ACQUA_CAMERA   450
#define PRO_TEMP_OFF_ACQUA_CAMERA  400
#define PRO_TEMP_ON_ACQUA_TURBO    400
#define PRO_TEMP_OFF_ACQUA_TURBO   350
#define PRO_ONTIME_BIAS            10000
#define PRO_ONTIME_CATODI          1000
#define PRO_ONTIME_GAS             10000
#define PRO_DT_RISCALDO            300000 //AGGIUNGO 5 MINUTI SE NON RAGGIUNGE LA PRESSIONE

//DA RESISTENZA
#define RES_T_ONOFF_RES       1000
#define RES_MAX_TEMP          6000
#define RES_RISCALDO_CAMERA	  400

//DA VUOTO
#define VAC_MAX_TEMP_CAMERA    600
#define VAC_MAX_TEMP_TURBO     500

#define VAC_PIRANI_SET        8000
#define VAC_BARATRON_SET      7000
#define VAC_BARATRON_ALLARME  11000  // 9.5 10-2 mbar

#define VAC_PENNING_SET       3000
#define VAC_PENNING_ERRORE    3500
#define VAC_PENNING_RESET     3000

#define VAC_T_PIRANI_PRESET   60000 //1 minuti(60000 ms)  per accorgersi di un eventule grossa perdita
#define VAC_T_PIRANI_SET      300000 //3 minuti(300000 ms)  per raggiungere la pressione di SET
#ifdef PER_PLC
#define VAC_T_TURBO_SET       60000   //10 Secondi di attesa dopo aver raggiunto la pressione di partenza 
#define VAC_T_TURBO_RESET     30000  //La turbo si spegne 30 sec prima della meccanica
#else
#define VAC_T_TURBO_SET       10000 
#define VAC_T_TURBO_RESET     5000
#endif

#ifdef PER_PLC
#define VAC_PRE_STOPVUOTO        300000 //5 minuti
#define VAC_PRE_HVALV_STOPVUOTO  300000 //5 minuti
#else
#define VAC_PRE_STOPVUOTO        6000 //1 minuti(60000 ms)  per accorgersi di un eventule grossa perdita
#define VAC_PRE_HVALV_STOPVUOTO  6000 //1 minuti(60000 ms)  per accorgersi di un eventule grossa perdita
#endif*/








