#ifndef QSTARTPROCESSO_H
#define QSTARTPROCESSO_H

#include <QDialog>
#include <QBasicTimer>
#include "NetPlc.h"
#include "qvuoto.h"
#include "qpotenza.h"

namespace Ui {
class QStartProcesso;
}

class QStartProcesso : public QDialog
{
    Q_OBJECT

public:
    explicit QStartProcesso(QWidget *parent,QNetPlc *newProcessa);
    ~QStartProcesso();

protected:
    void timerEvent(QTimerEvent *timerEvent);
    void paintEvent(QPaintEvent *event);

private:
    QNetPlc     *Processa;
    QBasicTimer   timer;
    QGas          *Gas[3];
    QBias         *Bias;
    QCatodi       *Catodo[4];
    Ui::QStartProcesso *ui;
};

#endif // QSTARTPROCESSO_H
