#ifndef QSETTING_H
#define QSETTING_H
#include "NetPlc.h"

#include <QDialog>

namespace Ui {
class QSetting;
}

class QSetting : public QDialog
{
    Q_OBJECT

public:
    explicit QSetting(QNetPlc *newProcessa,int newIdMacchina,QWidget *parent = 0);
    ~QSetting();

private slots:
    void on_Impara_clicked();
    void on_Chiudi_clicked();

private:
    QNetPlc *Processa;
    int IdMacchina;
    Ui::QSetting *ui;
};

#endif // QSETTING_H
