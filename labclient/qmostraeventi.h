#ifndef QMOSTRAEVENTI_H
#define QMOSTRAEVENTI_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class QMostraEventi;
}

class QMostraEventi : public QDialog
{
    Q_OBJECT

public:
    explicit QMostraEventi(QWidget *parent =0 );
    ~QMostraEventi();
    void addLista(QListWidget *lista);
    void addLista(QString str);
    void clear();

private slots:
    void on_ButtonElimina_clicked();
    void on_ButtonAnnulla_clicked();

    void on_pushButton_clicked();

private:
    QListWidget       *lista;
    Ui::QMostraEventi *ui;
};

#endif // QMOSTRAEVENTI_H
