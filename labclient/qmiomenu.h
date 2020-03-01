#ifndef QMIOMENU_H
#define QMIOMENU_H

#include <QDialog>
#include "NetPlc.h"

namespace Ui {
class qMioMenu;
}

class qMioMenu : public QDialog
{
    Q_OBJECT

public:
    explicit qMioMenu(QNetPlc *newProcessa,int IdMacchina,QWidget *parent = 0);
    ~qMioMenu();
    QString ProcessoNome;
    int     ProcessoId;

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private :
    void setCoating();

private slots:
    void on_Chiudi_clicked();
    void on_processo_activated(int index);

private:
    QStringList ProcessoLista;
    QNetPlc *Processa;
    Ui::qMioMenu *ui;
};

#endif // QMIOMENU_H
