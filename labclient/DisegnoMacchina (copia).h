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
    Valvola
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


class CDisegnoMacchina : public QWidget
{
    Q_OBJECT
public:
    explicit CDisegnoMacchina(QWidget *parent);
    ~CDisegnoMacchina();
    void RuotaTavola();
    void Edit();
    void Init(QNetPlc *newProcessa);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void StartProcesso();

public slots:
    void TavolaTimeout();

private:
    QString initString(const QString &string);
    QList <QPunto *> CDparse(const QString &string, const QString &codec);
    void Prova(QPunto * Start,QDisegnoComponente  *pComponente);
    void DisegnaTesto(QPunto * Start,QPunto * Stop,QString Testo ,QColor colore);
    void DisegnaMacchina();
    void DisegnaComponente(QPunto *pCentro, qreal Angolo,QDisegnoComponente  *pComponente, QComponente *plabel);
    void DisegnaCatodo(int Index  , bool ConCavi);
    void DisegnaComponentiVuoto(int Index);
    void DisegnaTurbo();
    void DisegnaCatodoLabel(int Index);
    void DisegnaResistenza(int Index);
    void DisegnaTavola();
    void DisegnaTavolaLabel();
    QComponente * NextValvola();
    QComponente * NextSensori();
    QString Descrizione(QComponente pComp);
    QList <QPunto *> parseFromFile(const QString &filename, const QString &codec);
    QColor getColore(int newstato);
    QColor getColoreVuoto();
    QColor getColoreGas(int Index);
    QList <QPunto *> dMacchiana;
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
    QTimer   *TimerTavola;
    QNetPlc *Processa;
    bool Ruota;
    int Impianto;
    int ValvolaIndex;
    int ValvolaCount;
    int ValvolaVenting;
    int SensoreIndex, SensoreCount;
    int hFoglio,wFoglio;
    int hFoglioDisegno,wFoglioDisegno;
    qreal Fattore;
    qreal xTrasla,yTrasla;
    //int hScalaFoglio;

};

#endif // CDisegnoMacchina_H
