#ifndef __DATAFILR_H
#define __DATAFILR_H

#include "StdAfx.h"
#include <QString>


#define NOME_DB "DATI"
#define HOSTNAME "labserver.jumpingcrab.com"
#define UTENTE_SIZE 16
#define PATH_SIZE 64

struct NetDb
{
    QString    host;
    QString    user;
    QString    password;
    int        porta;
};


struct FileType{
    WORD    Ciclo;
    WORD    CicloId;
    WORD    ProcessoId;
    BYTE    ProcessoRecord;
    BYTE    ProcessoNome[UTENTE_SIZE];
    BYTE    Coating;
    WORD    TavGiri;
    WORD    TavOnTime;
    WORD    TavOffTime;
    DWORD   TurboOnTime;
    DWORD   MeccanicaOnTime;
    BYTE    SorgentiOn;
};


struct DataType{
    BYTE     Comando;
    BYTE     NextComando;
    BYTE     StatoMacchina;
    BYTE     StatoVuoto;
    BYTE     ComandoAttivato;
    BYTE     ComandoEseguito;
    BYTE     ComandoRicevuto;
    BYTE     MessaggioRicevuto;
    BYTE     MessaggioLen;
    BYTE     LastComandoIndex;
    int      LastComandoLen;
    bool     EnabledVenting;
	BYTE     SorgentiOn;
    DWORD    TimeEvent;
    DWORD    TimeEventStop;
    DWORD    MainTime;
    DWORD    LastTime;
    DWORD    Time;
    DWORD    TimeStop;
    DWORD    StepTime;
    DWORD    StepTimeStop;
    DWORD    TimeStopVuoto;
    BYTE     StepCount;
    BYTE     TipoStep;
    BYTE     StepIndex;
    BYTE     RepitCount;
    BYTE     Repit;
    BYTE     ScanTime;
    WORD     Resistenza;
    bool     ProcessoIsOn;
    bool     ProcessoIsEnabled;
    bool     ErrProcesso;
    bool     ErrPreProcesso;
    bool     ErrMacchina;
    WORD     ErrCount;
    BYTE     ComStepLoad;
    BYTE     ProStepCount;
    FileType File;
    BYTE     *LastComando;
    bool     ComandoManuale;
    WORD     EventoProcessoCount;
    WORD     EventoProcessoDi;
    BYTE     SimulaEvento;
    DWORD    SimulaTempo;
    bool     Simula;
    WORD     RecordCount;
    bool     ProcessoCorrotto;
    bool     IsPerSimula;
    WORD     BiasLowCount;
    bool     IsRun;
    bool     PowerIsEnabled;

};

struct NetDataType{
     WORD  Impianto;
     WORD  VersioneSoftware;
     BYTE  StatoMacchina;
     BYTE  StatoVuoto;
     BYTE  EnabledVenting;
     BYTE  Comando;
     BYTE  ComandoAttivato;
     BYTE  ComandoRicevuto;
     BYTE  ComandoEseguito;
     BYTE  MessaggioRicevuto;
     BYTE  MessaggioLen;
     BYTE  SorgentiOn;
     BYTE  ResistenzaPotenza;
     BYTE  BiasLowCount;
     DWORD MainTime;
     DWORD TimeEvent;
     DWORD TimeEventStop;
     DWORD Time;
     DWORD TimeStop;
     DWORD StepTime;
     DWORD StepTimeStop;
     WORD  ScanTime;
     BYTE  TipoStep;
     BYTE  StepCount;
     BYTE  StepIndex;
     BYTE  RepitCount;
     BYTE  Repit;
     BYTE  ComStepLoad;
     WORD  Ciclo;
     WORD  ProcessoId;
     WORD  ProcessoRecord;
     BYTE  ProcessoIsOn;
     BYTE  ErrCount;
     BYTE  ErrPreProcesso;
     BYTE  ErrProcesso;
     BYTE  EventoProcessoCount;
     BYTE  EventoProcessoDi;
     WORD  RecordCount;
     BYTE  ProcessoCorrotto;
     BYTE  Simula;
};


struct StepType
{
    char   Descrizione[UTENTE_SIZE];
    BYTE   Repit;
    BYTE   RepitCount;
	BYTE   SorgentiOn;
    BYTE   CompStepCount;
    DWORD  Tempo;
    BYTE   CTipoStep;
    WORD   Step;
    BYTE   ProStep;
    WORD   Potenza;
    WORD   Temperatura;
    WORD   Pressione;
    WORD   Tavola;
    BYTE   PerBiasOnTime;
    bool   InAggiorna;
};

struct CompType{
    BYTE    TipoComp;
    BYTE    Index;
    BYTE    Slot;
    BYTE    Point;
    BYTE    Enabled;
    WORD    Set;
    WORD    SetSecondo;
    WORD    Info;
    DWORD   TimeOut;

};

struct CompStepType{
	WORD    Inf;
	WORD    Start;
    WORD    Stop;
    DWORD   Rampa;
	BYTE    Step;
	BYTE    ProStep;
};

struct CompFileStepType{
	CompStepType  Comp;
	BYTE          Slot;
	BYTE          Point;
	bool          Cambia;
};



struct TargetType{
    BYTE    TipoTarget;
    double  OnTime;
    bool    Cambia;
};


#endif 
