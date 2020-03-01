#ifndef QSIMULA_H
#define QSIMULA_H

#include <QDialog>
#include "NetPlc.h"

namespace Ui {
class QSimula;
}

class QSimula : public QDialog
{
    Q_OBJECT

public:
    explicit QSimula(QNetPlc *newProcessa,QWidget *parent = 0);
    ~QSimula();

private slots:
    void on_BiasLow_clicked();

    void on_chiudi_clicked();

private:
    Ui::QSimula *ui;
    QNetPlc *Processa;
};

#endif // QSIMULA_H
