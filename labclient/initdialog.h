#ifndef INITDIALOG_H
#define INITDIALOG_H

#include <QDialog>

class CNetDb
{
public:
    int wFoglio;
    int hFoglio;
    QString Nome;
    QString Impianto;
    QString DbProcessoHost;
    QString DbProcessoClient;
    QString DbProcessoPass;
    QString DbProcessoPorta;
    QString PortaPlc;
    QString DbDatiHost;
    QString DbDatiClient;
    QString DbDatiPass;
    QString DbDatiPorta;
    bool    IsLocale;
    bool    IsRidotta;

};

namespace Ui {
class QInitDialog;
}

class InitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitDialog(QWidget *parent = 0);
    ~InitDialog();
    int ApriFile();
    bool Connetti;
    CNetDb  Net;

private slots:
    void on_Salva_clicked();
    void on_Annulla_clicked();
    void on_Connetti_clicked();
    void on_Nome_activated(int index);

private:
    void Aggiorna(int );

private:
    Ui::QInitDialog *ui;
    QList <CNetDb *> pNet;
};

#endif // QInitDialog_H
