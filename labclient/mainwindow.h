#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QObject>
#include <QTimer>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QFileSystemWatcher>
#include <QThread>
#include "qvuoto.h"
#include "qpotenza.h"
#include "qmonitorizza.h"
#include "qprocesso.h"
#include "qmostraeventi.h"
#include "qmostraerrore.h"
#include "qlabservernet.h"
#include "NetPlc.h"
#include "initdialog.h"
#include "qstartprocesso.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QNetPlc *newProcessa,QLabserverNet *newNetServer, int newtipoapp,CNetDb *newpNet,QWidget *parent = 0);
    ~MainWindow();
    bool DbIsConnesso;

private:
    void ConnettiDb();
    void SetAutomatico();
    void MonitorizzaViewInit();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void processiDisponibili(int,QString);
    void ConnettiToDatabase();
    void Edit();
    void mostraEvento(QString);
    void mostraErrore(QString msg);
    void mostraErrore(CErrore *pErrore);
    void mostraListaErrori();
    void ProcessoIsOn(int);
    void ProcessoIsOff(int);
    void ProcessoCorrotto(int ,int);
    void Processo(int,int);
    void AddTempo(int);
    void nuovoMsg(QByteArray);
    void CambiaCTipoStep(int CTipoStep);
    void on_VersioneRidotta(bool newVersioneRidotta);
    void on_ButtonVuoto_clicked();
    void on_ButtonStopVuoto_clicked();
    void on_ButtonVenting_clicked();
    void on_ButtonStartProcesso_clicked();
    void on_ButtonStopProcesso_clicked();
    void on_ButtonManule_clicked();
    void on_ButtonAutomatico_clicked();
    void on_ButtonApriCiclo_clicked();
    void mostraMsgPlc();
    void on_ListaErrori_doubleClicked(const QModelIndex &index);
    void on_tabImpianto_tabBarClicked(int index);
    void sendCambiaSegnale(int newslot,int newpoint, int newsegnale);
    void setprogressBar(int perc);
    void ComandoAttivato(int,int);
    void ComandoEseguito(int,int );
    void ComandoRicevuto(int,int );
    void StatoMacchina();
    void StatoVuoto(int);
    void on_DbConnesso(QSqlDatabase *newDb, QSqlDatabase *newDbProcesso,QSqlQuery *queryEvento,QSqlQuery *queryData,QSqlQuery *queryLeek);//,QSqlQuery *queryData, QSqlQuery *queryLeek);
    void on_StepSpediti(int step, int di);
    void on_MsgRicevuti(int step, int di,QString msg);
    void nuovoErrore(int count);
    void onTimeout();
    void on_processoCaricato();
    void on_processoSpedito();
    void on_MacchinainInManuale(bool);
    void on_Errore(QString);


private slots:
    void on_ButtonImpara_clicked();
    void on_ButtonSimula_clicked();
    void on_ButtonTest_clicked();

private:
    void Memorizza();
    void DisegnaGrafico();
    void SetNetData();

private:
    CNetDb        *pNet;
    int           OldComando;
    QString       strUpdate;
    int           recordCount;
    QSqlDatabase  *DbData;
    QSqlDatabase  *DbProcesso;
    QBasicTimer   timerProcesso;
    QBasicTimer   timerUpdate;
    QNetPlc       *Processa;
    bool          enableNet;
    int           Ciclo;
    int           CicloId;
    int           TipoApp;
    bool          RicevutoData;
    QString       PathCicli;
    QString       PathProcessi;
    QSqlQueryModel *TipoSteoMod ;
    int           lastmsg;
    QString       host;
    bool          IsLocale;
    QString       Impianto;
    //QSqlQuery     queryData;
    QMostraErrore DlgErrore;
    QMostraEventi DlgEventi;
    DataType      *m_pData;
    bool          m_PortaChiusa;
    bool          IsVisibile;
    bool          IsMsgPlcCaricato;
    int           VersioneRidotta;
    CDisegnoMacchina   *Macchina;
    int           IdMacchina;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
