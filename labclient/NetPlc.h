#ifndef QNETPLC_H
#define QNETPLC_H

#include <QObject>
#include "Processo.h"
#include "Target.h"

#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif


QMacchina{
   enum TypeMacchina{
         Destra=0,
         Sinistra=1
    };
 };

class CMsg
    {
    public:
        int Tentativi;
        int Primo;
        int Macchina;
        int Secondo;
        int Terzo;
        QByteArray pMsg;

    };


class QNetPlc :  public QObject , public  CProcesso
{

    Q_OBJECT

public:
    explicit QNetPlc(int newtipoapp,int newTipoImpianto,int argc, char *argv[], QObject *parent = Q_NULLPTR);

    int        ComandoToInt( QByteArray msg );
    int        MessaggioToInt( QByteArray msg  );
    int        getPercentSpedito(QByteArray msg );
    bool       VerificaMsg(QByteArray buffer);
    void       AddMsg(int Primo, int Macchina, int Secondo);
    void       AddMsg(int Primo, int Macchina, int Secondo, int Terzo);
    void       AddMsg(int Primo,int Macchina,  QByteArray msg);
    void       AddMsg(int Primo, int Macchina,int Secondo, QByteArray pMsg);
    void       AddMsg(int Primo, int Macchina,  int Secondo, int Terzo, QByteArray pMsg);
    bool       IsMsgData(QByteArray msg);
    int        TipoMessaggio(QByteArray msg);
    int        MsgDataToInt(QByteArray msg);
    //void       AddLastComando(QByteArray msg);
    void       AddStrMsgPerClient(int primomsg,QString msg);
    void       AddMsgPerClient(QByteArray msg);
    QByteArray GetStrMsgPerClient(int i);
    //QString    GetMsgPerClient();
    void       AddMsgSetSegnale(int slot,int point,int segnale);
    WORD       PlcComando(char *Buffer);
    void       SetNetData(char *Buffer);
    WORD       GetImpianto(char *Buffer);
    void       AzzeraNetData();
    QByteArray setPercentSpedito(int di, int su);
    void       ResetComando(int i);
    void       ResetErrore();
    bool       ControllaRecivedStep( QByteArray );
    bool       ControllaRecivedCompStep( QByteArray ,CIOSegnale *);
    void       SetOnTime(char *Buffer);
    int        TipoComando;
    bool       IsConnesso;
    bool       IsInVuoto();
    bool       IsVisibile;
    int        ProcessoId;
    bool       Spedito;
    bool       VersioneRidotta;
    QString    getStatoMacchina(int IdMacchina);
    QString    getTipoStep(int IdMacchina);
    QSqlQuery  queryData;

private:
    bool cercaFileProcesso(int IdMacchina);

signals:
    void ComandoAttivato(int,int );
    void ComandoEseguito(int,int );
    void ComandoRicevuto(int,int );
    void MessaggioRicevuto(int );
    void StatoMacchina();
    void StatoVuoto(int);
    void emitEvento(const QString &msg);
    void ProcessoIsOn(int);
    void ProcessoIsOff(int);
    void Processo(int id, int Macchina);
    void CompStepLoad(int);
    void CambiaCTipoStep(int);
    void AddTempo(int);
    void nuovoMsg(QByteArray msg);
    void NuovaData();
    void nuovoErrore(int);
    void MsgRicevuti(int, int, QString msg);
    void ProcessoCorrotto(int,int);
    void MacchinainInManuale(bool );


private slots:
    void mostraEvento(const QString &msg);


public:
    CIOSegnale          *Pc_Gas;
    CIOSegnale          *Pc_Sorgenti;
    CIOSegnale          *Pc_Bias;
    CIOSegnale          *Pc_SchermoTurbo;
    bool                IsLocale;
    int                 ServerPorta;
    QString             ProcessoStr;
    QList<CMsg >        msgPerPlc;
    QList<CMsg >        msgPerClient;
    QList<CIOSegnale*>  *pListaSegnali;
    WORD                Pc_StepCount;
    int                 Pc_StepIndex;
    StepType            Pc_Step[D_STEP];
    bool                ClientIsConnect;
    DWORD               MainTimer;
    WORD                EventTime;
    WORD                TimeEventStop;
    WORD                Time;
    WORD                TimeStop;
    WORD                StepTime;
    WORD                StepTimeStop;

private:
    struct NetDataType  m_Data;

};

#endif // QNetPlc_H



