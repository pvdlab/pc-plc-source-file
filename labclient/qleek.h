#ifndef QLEEK_H
#define QLEEK_H

#include <QWidget>
#include <QSqlRecord>
#include <QSqlQuery>
#include "NetPlc.h"
#include "qgraficoriquadro.h"
#include "ui_qleek.h"

namespace Ui {
class QLeek;
}

class QLeek : public QWidget
{
    Q_OBJECT

public:
    explicit QLeek(QWidget *parent = 0);
    ~QLeek();
    void Init(QNetPlc *newProcessa,QSqlDatabase  newDbProcesso, int newIdVuoto);
    void Edit();

signals:
    void caricaRecord(int perc);

protected:
    //void timerEvent(QTimerEvent *timerEvent);
    void resizeEvent(QResizeEvent *event);

private slots:
    void StatoVuoto(int);
    void timeEvent();
    void on_ButtonAutomatico_clicked();
    void on_ButtonStatoVuoto_clicked();
    void on_Volume_currentIndexChanged(const QString &arg1);


private:
    QSqlDatabase  DbProcesso;
    int mStatoVuoto;
    Ui::QLeek *ui;
    QNetPlc *Processa;
    CIOSegnale *pleek;
    CIOSegnale *pvuoto;
    CIOSegnale *pventing;
    QTimer   timer;
    double lastleek;
    double LastVent;
    QTime  MainTime;
    int    lastTempo;
    int    m_MainTime;
    int    Volume;
    bool   Manuale;
    int    stato;
    int    IdVuoto;
};

#endif // QLEEK_H
