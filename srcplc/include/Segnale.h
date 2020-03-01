#ifndef CIOSEGNALE_H
#define CIOSEGNALE_H

#include "Timer.h"
#include "Modulo.h"
#include "DataFile.h"
#include "MainCostanti.h"
#ifdef QT_APP
#include <QPointF>
#include <QColor>
#endif

struct IoStepType{
    WORD          StepCount;
    bool          Attivo;
    bool          InAggiorna;
	CompStepType  Step[D_COMP_STEP];	
};



class CIOSegnale
{


public:
    CIOSegnale();
    CIOSegnale&   operator()(BYTE  Slot,BYTE  Point,const char *Descrizione,WORD FondoScala, BYTE Tipo,BYTE OffSet,CModulo *NewModulo );
    CIOSegnale&   operator=(WORD);
    CIOSegnale&   operator+=(WORD);
    CIOSegnale&   operator*=(double);
    CIOSegnale&   operator-=(WORD);
    double        getCompDouble();
    WORD          getCompZipDouble();
    void          setRegola(double newRegola){ m_Regola=newRegola;}
    double        getRegola(){return m_Regola;}
    bool          IsStabile(){return m_IsStabile;}
    void          PrintStep();
    void          PrintStep(int);
    void          Aggiornato();
    int           getNoZeroStepCount();
    int           Segnale();
    WORD          FondoScala(){return m_FondoScala;}
    bool          isLetto(WORD tempo);
    double        DSegnale();
    void          DSegnale(double newsegnale){ m_DSegnale=newsegnale;}
    WORD          ActProStep() { return m_ActProStep; }
    WORD          ActStep() { return m_ActStep; }
    IoStepType    Step();
    WORD          StepCount();
    WORD          StepCount(int Step);
    int           FSegnale(int FondoScala);
    char          *Descrizione(){ char *pBuffer=m_Descrizione; return pBuffer;}
    DWORD         Load(WORD,WORD,WORD,WORD,WORD ,DWORD );
    void          Allunga(double incremento,DWORD DaTempo);
    bool          AllungaZero(double incremento);
    void          AzzeraStep();
    void          append(WORD ProStep);
    bool          reLoad(WORD,WORD,WORD,WORD,WORD ,DWORD );
    DWORD         reLoad(WORD ProStep,WORD Step,DWORD);
    DWORD         Rampa(WORD ProStep,WORD Step);
    bool          Load() { return m_Load; }
    bool          GoToStep(WORD Step,DWORD Tempo);
    bool          GoToStep(WORD Step);
    void          UnLoad(int StepIndex);
    bool          IsStart();
    bool          Set();
    bool          Set(int newSegnale);
    bool          SetStart( );
    void          SetStabile();
    int           Get();
    bool          Reset();
    bool          ResetSegnali();
    void          StendBy();
    void          ResetStendBy();
    bool          Attivo() {return m_Step[m_ActProStep].Attivo;}
    bool          IsZero(WORD Step);
    bool          IsPrimoStep();
    bool          Enabled();
    int           Info();
    int           InfoGotoStep(int Step);
    bool          IsRampa();
    DWORD         OnTime(WORD);
    DWORD         OnTime();
    int           OnTimeAh();
    BYTE          OffSet() { return m_OffSet; }
    WORD          Slot() { return (WORD)m_Slot; }
    WORD          Point() { return (WORD)m_Point; }
    double        Fattore();
    double        SecondoFattore();
    double        Zip;
    void          setFattore(double val){m_Fattore=val;}
    void          setSecondoFattore(double val){m_SecondoFattore=val;}
    IoStepType    *SteData(){return &m_Step[m_ActProStep];}
    WORD          ProStep(){return m_ActProStep;}
    int           GraficoIndex;
    WORD          TipoComp;
    IoStepType    *m_Step;
    bool          Spedito;
#ifdef QT_APP
    void         setColor(int index);
    int          PenWidth;
    QColor       PenColor;
    QPointF      lastPoint;
    QString      FieldNome(){ QString str;
                             str= QString::number(m_Slot)
                                  + "_" +QString::number(m_Point) ;
                             return str;}
#endif
    void         setStep(IoStepType *newStep){m_Step=newStep;}
    bool         isStepNull(WORD index);
    void         ForzaToStep();


private:
	void         SetStepTempo(void);
    void         getSegnale();
    void         setSegnale();

private:
    char         m_Descrizione[D_DESCRIZIONE];
    CModulo      *m_Modulo;
	BYTE         m_TipoSlot;
    int          m_Segnale;
    int          m_SetSegnale;
    bool         m_IsStabile;
    double       m_DSegnale;
    double       m_Regola;
	WORD         m_StendBySegnale;
	BYTE         m_Slot;//
    BYTE         m_Point;//
    WORD         m_FondoScala;//
    CTimer       m_Timer;
    WORD         m_ActStep;
    WORD         m_ActProStep;
	bool         m_Set;
	bool         m_Reset;
    bool         m_Enabled;
	bool         m_StendBy;
	BYTE         m_OffSet; //
	bool         m_Load;
    bool         m_FalseTrue;
    double       m_Fattore;
    double       m_SecondoFattore;
    bool         m_IsStart;


};


#endif 
