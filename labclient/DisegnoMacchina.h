#ifndef CDISEGNOMACCHINA_H
#define CDISEGNOMACCHINA_H

#include <QWidget>
#include <QPointF>
#include <QList>
#include <QTimer>
#include "NetPlc.h"

#define PROVA_RESISTENZA

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif


TipoIcona{
   enum TypeTipoIcona {
         Disattiva=1,
         On=2,
         Off=3
    };
 };

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif


MacchinaBottoni{
   enum TypeMacchinaBottoni {
         ProcessoStart=8,
         ProcessoScegli=12,
         Setting = 2,
         Manuale= 10,
         Errore=6,
         Note=4,
         VuotoStart=14,
         Venting=16
    };
 };




#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif


MacchinaLabel{
   enum TypeMacchinaLabel {
         ProcessoPlc=0,
         Processo=2,
         StartProcesso=4,
         MainTime=6,
         Ciclo=8,
         Sito=10,
         Firma=12,
         MenoTempoProcesso=14,
         MenoStepProcesso=16,
         Step=18
    };
 };



#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif



QTipoStatoComponente {

 enum TypeStatoComponente  {
    Acceso=0,
    Spento=1,
    Init=2,
    Errore=3
 };
};

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif



QTipoComponente {

 enum TypeComponente  {
    Camera=1,
    Catodo=2,
    Resistenza=3,
    Schutter=4
 };
};

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif



QTipoCavoGas {

 enum TypeTipoCavoGas  {
    Gas=1,
    Sensore=2,
    SensoreValvola=3,
    Valvola=4
 };
};


class QPunto
{
public:
    QPunto() {}
    QPunto(qreal newx,qreal newy) {x=newx;y=newy;}

public:
    qreal x;
    qreal y;

};

class QCavo
{
public:
    QCavo(int newTipoCavo) {TipoCavo=newTipoCavo;}

public:
    QList <QPunto *> dData;
    QList <QPunto *> Label;
    QPunto  Ultimo;
    QString Descrizione;
    QColor  Colore;
    int TipoCavo;

};

class QDisegnoTavola
{
public:
    QDisegnoTavola() {}

public:
    QPunto *Centro;
    qreal  Raggio;
    QList <QPunto *> Label;

};

class QDisegnoResistenza
{
public:
    QDisegnoResistenza() {}

public:
    QList <QPunto *> dData;
    QColor  Colore;
    QString Descrizione;

};

class QDisegnoComponente
{
public:
    QDisegnoComponente() {}

public:
    QList <QPunto *> dData;
    QList <QPunto *> Centro;
    QList <QPunto *> Label;
    QPunto  *Trasla;
    qreal   Angolo;
    QColor  Colore;

};


class QDisegnoSensore
{
public:
    QDisegnoSensore() {}

public:
    QList <QPunto *> dData;
    QList <QPunto *> Label;
    QPunto * Centro;
    QColor  Colore;

};

class QDisegnoCaviGas
{
public:
    QDisegnoCaviGas() {}

public:
    QList <QCavo *> Cavo;
    QPunto * Turbo;

};

class QDisegnoSorgente
{
public:
    QDisegnoSorgente() {}

public:
    QList <QPunto *> Target;
    QList <QPunto *> Cavi;
    QList <QPunto *> Label;
    QColor  Colore;
    QString Descrizione;

};

class QComponentCavi
{
public:
    QComponentCavi() {}

public:
    QList <QPunto *> Cavi;
    CIOSegnale *pSegnale;


};

class QComponente
{
public:
    QComponente() {Trasla= new QPunto();pSegnale=NULL;}

public:
    QList <QPunto *> Label;
    QString Descrizione;
    QComponentCavi *Cavi;
    QPunto      *Trasla;
    qreal       Angolo;
    QColor      Colore;
    QRect       Rect;
    CIOSegnale *pSegnale;
    CIOSegnale *pIsOn;
    int TipoCavo;

};

class QMacchinaCavi
{
public:
    QMacchinaCavi() {}

public:
    QList <QComponentCavi *> Cavo;
    QList <QPunto *>         Label;
    QList <QComponente  *>   pTarget;
    QList <QComponente  *>   pGas;
    QList <QComponente  *>   pValvole;
    QList <QComponente  *>   pSensori;
    QString Descrizione;
    QString File;
    QString FileLabel;
    int LabelIndex;
    int TargetIndex;
    int GasIndex;
    int ValvolaIndex;
    int SensoreIndex;

};


class CDisegnoMacchina : public QWidget
{
    Q_OBJECT
public:
    explicit CDisegnoMacchina(QWidget *parent);
    ~CDisegnoMacchina();
    void RuotaTavola();
    void Edit();
    void Init(QNetPlc *newProcessa,int newIndex);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void StartProcesso();
    void MostraEvento();
    void MostraErrore();

public slots:
    void TavolaTimeout();

private:
    void BottoniDisegna();
    void BottoniDisegna(QRect rect ,QString str);
    void MouseEvento_Bottoni(QMouseEvent *event);
    void MouseEvento_Manuale(QMouseEvent *event);
    void DisegnaTesto(QPunto * Start,QPunto * Stop,QString Testo ,QColor colore);
    void DisegnaTesto(QPunto * Start,QPunto * Stop,QString Testo ,QColor colore,QColor Beckcolore, int Alliniamento );
    void DisegnaSensoreLabel(int Index);
    void DisegnaMacchina();
    void DisegnaComponente(QPunto *pCentro, qreal Angolo,QDisegnoComponente  *pComponente, QComponente *plabel);
    void DisegnaCatodo(int Index  , bool ConCavi);
    void DisegnaComponentiVuoto(int Index);
    void DisegnaTurbo();
    void DisegnaResistenza(int Index);
    void DisegnaTavola();
    void DisegnaTavolaLabel();
    QRect BottoneRect(QPunto *pStart,QPunto *pStop);
    QComponente * NextValvola();
    QComponente * NextSensori();
    QString Descrizione(QComponente pComp);
    QList <QPunto *> PuntiDaFile(const QString &filename, const QString &codec);
    QList <QMacchinaCavi *> MacchinaCaviDaFile(const QString &filename, const QString &codec);
    QColor getColore(int newstato);
    QColor getColoreVuoto();
    QColor getColoreGas(int Index);
    QList <QPunto *> dMacchiana;
    QList <QPunto *> dLabel;
    QList <QPunto *> dBottoni;
    QList <QDisegnoSorgente *> dSorgente;
    QList <QDisegnoResistenza *> dRiscaldo;
    QDisegnoTavola   *dTavola;
    QDisegnoComponente *dTarget;
    QDisegnoComponente *dResistenza;
    QDisegnoComponente *dValvola;
    QDisegnoComponente *dFlussimetro;
    QDisegnoComponente *dSensore;
    QDisegnoComponente *dTurbo;
    QColor  dTavolaColore;
    QComponente *pTarget;
    QComponente *pGas;
    QComponente *pValvole;
    QComponente *pBias;
    QComponente *pSensori;
    QComponente *pTurbo;
    QComponente *pRes;
    QComponente *pVuoto;
    QDisegnoCaviGas *dCaviGas;
    QImage  *image;
    int SorgenteCount;
    int ResistenzeCount;
    int SensoriCount;
    int FlangiaCount;
    int TavolaGradi;
    int  RuotaGradi;
    bool Ruota;
    QTimer   *TimerTavola;
    QNetPlc *Processa;

    int Impianto;
    int ValvolaIndex;
    int ValvolaCount;
    int ValvolaVenting;
    int ValvolaBaratron;
    int Penning;
    int Baratron;
    int SensoreIndex, SensoreCount;
    int hFoglio,wFoglio;
    int hFoglioDisegno,wFoglioDisegno;
    qreal Fattore;
    qreal xTrasla,yTrasla;
    QRect   RectTavola;
    QColor ColoreCamera;
    bool  StartProcessoIsVisible;
    QString ProcessoNome;
    bool    Manuale;
    int     IdMacchina;
    bool    ProcessoIsEnabled;
    int     IdProcesso;

};

#endif // CDisegnoMacchina_H
