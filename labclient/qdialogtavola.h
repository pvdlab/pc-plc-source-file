#ifndef QDIALOGTAVOLA_H
#define QDIALOGTAVOLA_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class QDialogTavola;
}

class QDialogTavola : public QDialog
{
    Q_OBJECT

public:
    explicit QDialogTavola(QString CoatingId,QString ciclo, QString Indirizzo,QString newdpuser,QString newdbpassword, QString id, QWidget *parent = 0);
    ~QDialogTavola();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_OkButton_clicked();
    void RowChange(QModelIndex index);
    void on_ButtonChiudi_clicked();

private:
    QSqlQueryModel *TavolaMod ;
    QSqlDatabase *Database ;
    QString Tavola;
    QString Ciclo;
    QString Processo;
    QString Coating;
    QString Indirizzo;
    QString dpuser;
    QString dbpassword;
    QString id;
    Ui::QDialogTavola *ui;
};

#endif // QDIALOGTAVOLA_H
