#ifndef QMOSTRAERRORE_H
#define QMOSTRAERRORE_H

#include <QDialog>

namespace Ui {
class QMostraErrore;
}

class QMostraErrore : public QDialog
{
    Q_OBJECT

public:
    explicit QMostraErrore(QWidget *parent = 0 );
    ~QMostraErrore();
    void addErrore(QString Errore);

private slots:
    void on_ButtonOk_clicked();

private:
    QString lasterrore;
    Ui::QMostraErrore *ui;
};

#endif // QMOSTRAERRORE_H
