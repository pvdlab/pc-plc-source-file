#ifndef QSETCOMPOENTE_H
#define QSETCOMPOENTE_H

#include <QDialog>
#include "NetPlc.h"

namespace Ui {
class QSetCompoente;
}

class QSetCompoente : public QDialog
{
    Q_OBJECT

public:
    explicit QSetCompoente(QNetPlc *newProcessa,int newIdMacchina,CIOSegnale *newSegnale,QWidget *parent = 0);
    explicit QSetCompoente(QNetPlc *newProcessa,int newIdMacchina,CIOSegnale *new_pSegnale,int newSegnale,int max, QString Descrizione,QWidget *parent = 0);
    ~QSetCompoente();

private slots:
    void on_Set_clicked();
    void on_Chiudi_clicked();
    void on_ValoreSet_sliderMoved(int position);
    void on_TimeOutSet_sliderMoved(int position);   
    void on_Valore_editingFinished();

private:
    int IdMacchina;
    int TipoComp;
    int TimeOut;
    QNetPlc *Processa;
    CIOSegnale *pSegnale;
    Ui::QSetCompoente *ui;
};

#endif // QSETCOMPOENTE_H
