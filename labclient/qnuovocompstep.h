#ifndef QNUOVOCOMPSTEP_H
#define QNUOVOCOMPSTEP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

namespace Ui {
class QNuovoCompStep;
}

class QNuovoCompStep : public QDialog
{
    Q_OBJECT
public:
    explicit QNuovoCompStep(QWidget *parent,QSqlDatabase NewDbProcesso,QSqlRelationalTableModel *newCompStepMod);
    ~QNuovoCompStep();
    int TipoComp();

private slots:
    void on_ButtonOk_clicked();
    void on_ButtonAnnulla_clicked();

private:
    bool  ok;
    QSqlDatabase DbProcesso;
    QSqlRelationalTableModel  *CompStepMod;
    Ui::QNuovoCompStep *ui;
};

#endif // QNUOVOCOMPSTEP_H
