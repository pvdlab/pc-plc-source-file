#ifndef STDAFX_H
#define STDAFX_H

#include <stdint.h>
#include <ostream>

//#define TEST_BIAS_ARC_OFF

namespace TipoCamera {
    enum Code {
        Prima      = 0,
        Seconda    = 1
    };
}

namespace TipoImpianto {
    enum Code {
        Primo      = 6010,
        Secondo    = 6012,
        Ronzoni    = 6011
    };
}

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
}

#define VERSIONESOFTWARE 230

void setBuffer(char *pBuffer, int len);
char *getBuffer();


#define BYTE  char
#define WORD  uint16_t
#define DWORD uint32_t

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#ifdef QT_APP
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/time.h>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include "DataFile.h"

void setTipoStep(int newTipoStep);
int getTipoStep();
void setMacchinaInVuoto(bool newIsMacchinaInVuoto);
bool getMacchinaInVuoto();
void setAcquaErrore(bool newAcquaFlussa);
bool getAcquaErrore();
void   setPenning(double newPressione);
double getPenning();
void   setBaratron(double newBaratron);
double getBaratron();
void   setBias_V(double newBias);
double getBias_V();
void   setBias_I(double newBias);
double getBias_I();
void   setTipoApp(int newTipoApp);
int    getTipoApp();
void setVersioneRidotta(bool newVersioneRidotta);
bool getVersioneRidotta();
void getFoglio(int *w, int *h );
void setFoglio(int w, int h );
void creaDir();
int  getdir (std::string dir,std::string ext, std::vector<std::string> &files);
bool getErrore();
void setErrore(bool newerrore);
bool IsCompleto();
void setIsCompleto(bool);
int  getImpianto();
void setImpianto(int);
int  getSorgenteCount();
void setSorgenteCount(int newSorgenteCount);
int  getGasCount();
void setGasCount(int newGasCount);
int  getResistenzaCount();
void setResistenzaCount(int newResistenzaCount);
void setLicenza(int newanno,int newmese);
bool isLicenza();
void setProcessoIsOn(bool val);
bool getProcessoIsOn();
void setSalvaSuFile(bool);
void setEventoFileName(std::string nome);

std::string ComandoToString(char *msg );
bool ConnettiDB(QString nome, QString newUtente,QString newPassoword,QString newhost);


void MostraEvento(std::string msg,int colore);
void MostraEventoClear();

void setEventoProcessoFileName(int );
std::string getEventoProcessoFileName();
void  setMostraEventoProcessoCount(WORD );
WORD  getMostraEventoProcessoCount();
void MostraEventoProcessoReset();
void MostraEventoProcesso(std::string msg,int colore);
void setErrorFileName(std::string nome);
void MostraErrore(std::string msg);
int  ErrorCount();
void ErrorCountReset();
std::string intToString(int i);
std::string doubleToString(double i);
std::string MainTimeToString();
#else
void MostraEvento(const char * Buffer);
#endif


#define   PLC_LISTEN_PORTA 2350
#define   PC_LISTEN_PORTA 4000

#include "Modulo.h"
#include "Timer.h"


#define __min(a,b) (((a)<(b))?(a):(b))
#define __max(a,b) (((a)>(b))?(a):(b))
 

#define PLC_INIT "/.consoleplc"
#define PLC_MONITORIZZA "/PlcInit"
#define PLC_ERRORE "/PlcErrore.dat"
#define PLC_MAIN_EVENTO "/PlcEvento.dat"
#define PLC_REGOLA "/regola"


#define PLC_CICLO_PATH "/plc-ciclo"
#define PLC_EVENTO "/evento_"
#define PLC_PROCESSO "/processo_"
#define PLC_CICLO "/ciclo_"
#define PLC_PROCESSO_PATH "/plc-processo"
#define PLC_PROCESSO_EXT ".pro"
#define PLC_TARGET_PATH "/plc-target"
#define PLC_TARGET "/target"
#define PLC_TARGET_EXT ".target"



std::vector<std::string> mSystem(std::string msg);

void setPlcListenPort(int newporta);
int  getPlcListenPort();
void setPcListenPort(int newporta);
int  getPcListenPort();
char *getRunPath();
void setRunPath(const char *path);
char *getHomePath();
void setHomePath(const char *path);
void setSimula(int newval);
int  getSimula();
void setDebug(bool newval);
bool getDebug();


void setBiasIsLV(bool newval);
bool getBiasIsLV();


WORD   StrWORD(BYTE **pStr);
DWORD  StrDWORD(BYTE **pStr);
BYTE   StrByte(BYTE **pStr);
double StrDouble(BYTE **pStr);
void   StrDescrizione(char *Descrizione,BYTE **pStr);

BYTE *ByteStr(BYTE *pStr,BYTE SVal,int *len);
BYTE *WORDStr(BYTE *pStr,WORD SVal,int *len);
BYTE *DWORDStr(BYTE *pStr,DWORD SVal,int *len);
BYTE *DoubleStr(BYTE *pStr, double DVal,int *len);
char *DescrizioneStr(char *pStr, char *Descrizione,int *len);

bool  GetBit(BYTE b, BYTE position);
BYTE  SetBit(BYTE b, BYTE position, bool newBitValue);
void  SetBit(BYTE *b, BYTE Point, bool Segnale);
void  SetChrBit(BYTE *b, BYTE position, bool newBitValue);
bool  GetChrBit(BYTE *b, BYTE position);
BYTE  GetBits(BYTE x, BYTE p,int n);

short StrShort(BYTE **pStr);
BYTE *ShortStr(BYTE *pStr,short SVal,int *len);

void setMainTime(DWORD time);
DWORD getMainTime();
DWORD GetTickCount();
void  ResetTickCount();

#define FalseTrue(val) (val ? 0 : 1)
#define Confronto(a,b) ((a)<(b) ? 0 : 1)


#endif
