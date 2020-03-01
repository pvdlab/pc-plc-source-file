#ifndef QGRAFICO_H
#define QGRAFICO_H


#include <QWidget>
#include <QSlider>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtWidgets>
#include <QSqlRecord>
#include "Segnale.h"
#include "StdAfx.h"
#include "MainCostanti.h"

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif

QTipoScala {

 enum TypeScala {
    Lineare=1,
    Logaritmica=2
 };
};

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif

QTipoGrafico {

 enum TypeGrafico {
    Vuoto=1,
    Riscaldo=2,
    Gas=3,
    LowI=4,
    Volt=5,
    HeghtI=6
 };
};


class QGraficoLabel : public QWidget
{
    Q_OBJECT
public:
    explicit QGraficoLabel(QWidget *parent = 0);
    ~QGraficoLabel();
    void   setSegnali(QList<CIOSegnale*> pnewListaSegnali){pListaSegnali=pnewListaSegnali;}
    void   append(CIOSegnale* pSegnale){pListaSegnali.append(pSegnale);}
    void   Edit(QSqlRecord record);
    void   Edit(){update();}
    int    TipoScala;


protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QList<CIOSegnale*> pListaSegnali;
    void      refresch();
    void      resizePixmap(QImage *image, const QSize &newSize);
    int       Varcount;
    qreal     spazio;
    QPixmap   *pixmap;
};


class QGrafico : public QWidget
{
    Q_OBJECT

public:
    explicit QGrafico(QWidget *parent = 0);
    ~QGrafico();

    void  setSegnali(QList<CIOSegnale*> pnewListaSegnali);
    qreal getydivcount(){ return ydivcount;}
    qreal getydiv(){ return ydiv;}
    qreal getymax(){ return ymax;}
    qreal gettmax(){ return tmax;}
    void setMaxOut(qreal newymaxout,int newtiposcala);
    void setTempo(int newtempo);
    void setTempo(int newtempo,qreal newtzoom);
    void addTempo(int newtempo);

    void setRiquadro(int t, int y,int divt,int divy);
    void DisegnaGriglia();
    void drawCompStep(qreal t,int step);
    QString getVal(qreal newval);
    QList<CIOSegnale*> pListaSegnali;
    void   append(CIOSegnale* pSegnale){
        pListaSegnali.append(pSegnale);
    }
    void Clear(){pListaSegnali.clear();}
    void Edit(int newtime);
    void Edit(int newtime,int index);
    void Edit(int newtime, int index,qreal val);
    void Load(qreal time,QSqlRecord record);
    void cursoreMoveto(int x);


signals:
    void gotoTempo(int newtempo);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);


private:
    void     drawCursore(const QPoint &endPoint);
    qreal    ymax,ydiv,ydivcount,yscala,ymaxout;
    qreal    tmax,tdiv,tdivcount;
    int      myPenWidth;
    QColor   myPenColor;
    QImage   *image;
    QPointF  lastPoint;
    QPointF  lastCursore;
    int      Varcount;
    int      TipoScala;
    qreal    tzoom;
};



class QGraficoRiquadro : public QWidget
{
    Q_OBJECT
public:
    explicit QGraficoRiquadro(QWidget *parent = 0);
    ~QGraficoRiquadro();
    void    reset();
    void    setSegnali(QList<CIOSegnale*> pnewListaSegnali);
    void    append(CIOSegnale* pSegnale);
    void    setMaxOut(qreal newymaxout, int newtiposcala){
                Grafico->setMaxOut(newymaxout,newtiposcala);
                TipoScala=newtiposcala;
                LabelX->TipoScala=newtiposcala;
    }
    void    setTempo(int tempo);
    void    Edit(int newtime){Grafico->Edit(newtime);
                           LabelX->Edit();}
    void    scrollTempo(int tempo);
    void    scrolltoTempo(int x,int newtempo);
    QGrafico  *Grafico;
    QGraficoLabel  *LabelX;
    void Clear();

signals:
    void gotoTempo(int x,int newtempo);

private slots:
    void goTo(int newtempo);
    void valueChanged(int value);

protected:
    void    paintEvent(QPaintEvent *event);
    void    resizeEvent(QResizeEvent *event);

private:
    QWidget        *LabelY;
    QScrollArea    *ScrollArea ;
    int            myPenWidth;
    QColor         myPenColor;
    qreal          ymax,ydiv,ydivcount;
    int            TipoScala;
    int            MaxTempo;
    int            OldTempo;
};


#endif // QGRAFICO_H
