#ifndef QPROCESSO_H
#define QPROCESSO_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
#include <QItemDelegate>
#include <QtWidgets>
#include "NetPlc.h"
#include "Segnale.h"
#include "MainCostanti.h"
#include "qprocessosend.h"
#include "Processo.h"


class QTipoComp{

public:
    QString Descrizione;
    int index;

};



class ColorDelegate: public QItemDelegate
{
public:
    ColorDelegate(QObject *parent ,QList <QTipoComp> newTipoComp) : QItemDelegate(parent) {
        TipoComp=newTipoComp;
    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    virtual void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QList <QTipoComp> TipoComp;
};

namespace Ui {
class QProcesso;
}

class QProcesso : public QWidget
{
    Q_OBJECT

public:
    explicit QProcesso(QWidget *parent = 0);
    ~QProcesso();
    void setDbProcesso(QSqlDatabase *NewDbProcesso,int newIdMacchina, int newIdProcesso);
    void Init(QNetPlc *newProcessa){Processa=newProcessa;}
    void ProcessoRecivedData(QByteArray);
    bool ProcessoApri(int id);
    void ProcessoCaricaDaPlc();
    void SetTipoApp(int newtipoapp){TipoApp=newtipoapp;}
    QString CTipoStep(int index);
    int ProcessoId;
    QString ProcessoNome;
    int CoatingId;
    QString Coating;
    void ProcessoIsOn();
    QString getProcesso();
    void Edit();
    int  getPercent(int CompStep);
    void Aggiorna();
    bool CompStepBiasIsOn();
    void nuovoProcesso(QString Id, int macchina);
    void getProcessi();


signals:
    void ProcessoCaricaSuPlc(QByteArray ,int );
    void getNuovaConnesione();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void processiDisponibili(int,QString);
    void Salva();
    void StepAggiorna();
    void StepAdd();
    void StepElimina();
    void StepDuplica();
    void StepSpostaSu();
    void StepSpostaGiu();
    void CompStepAggiorna();
    void CompStepAdd();
    void CompStepElimina();
    void CompStepAddTempo();
    void CompStepDelTempo();
    void CompStepSalvaCome();
    void CompStepCambia();
    void ProcessoNuovo();
    void ProcessoDuplica();
    void ProcessoElimina();
    void ProcessoTmp();
    void ProcessoCaricaSuPlc();
    void StepModBeforeUpdate(int index, QSqlRecord &record);
    void CompStepModBeforeUpdate(int index, QSqlRecord &record);



private slots:
    void CompStepColumnChanged ( QModelIndex ,  QModelIndex  );
    void CompStepRowChange(QModelIndex );
    void StepColumnChanged ( QModelIndex ,  QModelIndex  );
    void StepRowChange(QModelIndex );
    void on_TabProcesso_activated(int index);
    void onTimeOut();
    void on_Macchina_activated(int index);


private:
    QSqlRecord CompRecord;
    QTimer *DbTimer;
    bool IsRollBack;
    bool IsModificato;
    int  WAIT_TIME;
    //void getProcessi();
    void ProcessoApri();
    void ProcessoDisegna();
    QString ControllaStep(CIOSegnale *pPc_Segnale,CIOSegnale *pSegnale);
    bool LoadSegnale(CIOSegnale *pSegnale, bool load,int info, int i,int j, int start, int stop,int rampa);
    QString getTipoComp(QString str);
    CIOSegnale *getSegnale(QString str);
    void addSegnale(QString id,CIOSegnale *segnale);
    QList<CIOSegnale*>        pListaSegnali;
    QSqlDatabase              *DbProcesso;
    //QSqlDatabase              DpRecenti;
    QSqlRelationalTableModel  *StepMod;
    QSqlRelationalTableModel  *CompStepMod;
    QSqlQueryModel            *ProcessoMod;
    QSqlQueryModel            *CompMod;
    QSqlQueryModel            *CTipoStepMod;
    QSqlRecord                CompStepRecord;
    QSqlRecord                StepRecord;
    int                       TempoProcesso;
    int                       StepId;
    int                       StepMaxId,CompStepMaxId;
    QNetPlc                   *Processa;
    bool                      Info;
    QString                   CompStepNome;
    int                       CompStepTempoCount;
    QProcessoSend             *pThreadSend;
    QTimer                    timeOut;
    QByteArray                sendData;
    bool                      spedito;
    int                       dperc;
    int                       TipoApp;
    bool                      ProcessoNonValido;
    QStringList               ListaProcessiCaricati;
    int                       IdMacchina;
    Ui::QProcesso             *ui;
};

#endif // QPROCESSO_H
