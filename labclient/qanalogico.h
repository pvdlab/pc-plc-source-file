#ifndef QANALOGICO_H
#define QANALOGICO_H

#include <QtWidgets>
#include <QDoubleSpinBox>
#include "Segnale.h"
#include "StdAfx.h"
#include "NetPlc.h"
#include "qled.h"

class QMLabel : public QWidget
{
    Q_OBJECT
public:
    explicit QMLabel(QWidget *parent = 0);
    void setDescrizione( QString newdescrizione);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString Descrizione;

};

class QAnalogico : public QWidget
{
    Q_OBJECT
public:
    explicit QAnalogico(QWidget *parent = 0);
    ~QAnalogico();
    void  setEdit(bool newedit){isEdit=newedit;}
    void  setEnabled(bool newenabled){isEnabled=newenabled;}
    void  setAnalogico(QString newdescrizione, QString newmisura,CIOSegnale *newpSegnale);
    void  setAnalogico(QString newdescrizione, QString newmisura,int newslot, int newpoint,CMacchina *processa);
    void  Edit();
    void  Edit(int );
    void  EditSecondo();
    void  EditSegnale();
    void  setManuale(bool newManuale);
    bool  isOutPut;

protected:
    void paintEvent(QPaintEvent* event);

signals:
    void CambiaSegnale(int newslot,int newpoint, int newsegnale);

public slots:
    void editingFinished();

private:
    CIOSegnale *pSegnale;
    QString    Descrizione;
    QString    Misura;
    short      Segnale;
    double     DSegnale;
    bool       isEdit;
    bool       isEnabled;
    bool       isManuale;
    QDoubleSpinBox *EditBox;

};

class QDigitale : public QWidget
{
    Q_OBJECT
public:
    explicit QDigitale(QWidget *parent =0);
    ~QDigitale();
    void setEdit(bool newedit){isEdit=newedit;}
    void setEnabled(bool newenabled){isEnabled=newenabled;}
    void Edit();
    void Edit(bool );
    void setDigitale(QString newdescrizione, CIOSegnale *newpSegnale);
    void setDigitale(QString newdescrizione, int newslot, int newpoint, int newfalsetrue,CMacchina *processa);
    bool Segnale;
    bool isOutPut;
    void  setManuale(bool newManuale){
        isManuale=newManuale;
        led->setManuale(newManuale);
    }


protected:
    void paintEvent(QPaintEvent* event);

signals:
    void CambiaSegnale(int newslot,int newpoint, int newsegnale);

public slots:
    void mousePress(bool);

private:
    QString    Descrizione;
    CIOSegnale *pSegnale;
    bool       isManuale;
    bool       isEdit;
    bool       isEnabled;
    QLed       *led;

};

class QSensore : public QWidget
{
    Q_OBJECT
public:
    explicit QSensore(QWidget *parent = 0);
    void  setAnalogico(QString newdescrizione, QString newmisura,CIOSegnale *newpSegnale);
    void  Edit();

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void CambiaSegnale( int newslot,int newpoint, int newsegnale);

public slots:

private:
    QString     Descrizione;
    QString     Misura;
    CIOSegnale *pSegnale;
    double     DSegnale;
    WORD       Segnale;

};


#endif // QANALOGICO_H
