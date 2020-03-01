/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qleek.h"
#include "qmodulo.h"
#include "qmonitorizza.h"
#include "qpotenza.h"
#include "qprocesso.h"
#include "qvuoto.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *ButtonVuoto;
    QPushButton *ButtonStopVuoto;
    QPushButton *ButtonVenting;
    QPushButton *ButtonStartProcesso;
    QPushButton *ButtonStopProcesso;
    QPushButton *ButtonManule;
    QPushButton *ButtonAutomatico;
    QPushButton *ButtonApriCiclo;
    QPushButton *ButtonImpara;
    QComboBox *ListaTipoStep;
    QPushButton *ButtonSimula;
    QProgressBar *progressBar;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *Ciclo;
    QLabel *StepTime;
    QLabel *MainTime;
    QHBoxLayout *horizontalLayout_3;
    QLabel *EventTime;
    QLabel *RepiStep;
    QLabel *Time;
    QLabel *label_8;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_8;
    QToolButton *ButtonErrore;
    QToolButton *ButtonMsgPlc;
    QListWidget *ListaErrori;
    QTabWidget *tabImpianto;
    QWidget *tabVuoto;
    QVBoxLayout *verticalLayout_4;
    QVuoto *VuotoView;
    QWidget *tabPotenza;
    QVBoxLayout *verticalLayout_3;
    QPotenza *PotenzaView;
    QWidget *tabProcesso;
    QVBoxLayout *verticalLayout_7;
    QProcesso *ProcessoView;
    QWidget *tabMonitorizza;
    QVBoxLayout *verticalLayout_2;
    QMonitorizza *MonitorizzaView;
    QWidget *tabLeek;
    QVBoxLayout *verticalLayout_6;
    QLeek *LeekView;
    QWidget *tabModulo;
    QVBoxLayout *verticalLayout_9;
    QModulo *ModuloView;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1030, 582);
        QIcon icon;
        icon.addFile(QStringLiteral(":/plcnetlogo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        widget = new QWidget(MainWindow);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_5 = new QVBoxLayout(widget);
        verticalLayout_5->setSpacing(1);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(1, 1, 1, 1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ButtonVuoto = new QPushButton(widget);
        ButtonVuoto->setObjectName(QStringLiteral("ButtonVuoto"));

        horizontalLayout->addWidget(ButtonVuoto);

        ButtonStopVuoto = new QPushButton(widget);
        ButtonStopVuoto->setObjectName(QStringLiteral("ButtonStopVuoto"));

        horizontalLayout->addWidget(ButtonStopVuoto);

        ButtonVenting = new QPushButton(widget);
        ButtonVenting->setObjectName(QStringLiteral("ButtonVenting"));

        horizontalLayout->addWidget(ButtonVenting);

        ButtonStartProcesso = new QPushButton(widget);
        ButtonStartProcesso->setObjectName(QStringLiteral("ButtonStartProcesso"));

        horizontalLayout->addWidget(ButtonStartProcesso);

        ButtonStopProcesso = new QPushButton(widget);
        ButtonStopProcesso->setObjectName(QStringLiteral("ButtonStopProcesso"));

        horizontalLayout->addWidget(ButtonStopProcesso);

        ButtonManule = new QPushButton(widget);
        ButtonManule->setObjectName(QStringLiteral("ButtonManule"));

        horizontalLayout->addWidget(ButtonManule);

        ButtonAutomatico = new QPushButton(widget);
        ButtonAutomatico->setObjectName(QStringLiteral("ButtonAutomatico"));

        horizontalLayout->addWidget(ButtonAutomatico);

        ButtonApriCiclo = new QPushButton(widget);
        ButtonApriCiclo->setObjectName(QStringLiteral("ButtonApriCiclo"));

        horizontalLayout->addWidget(ButtonApriCiclo);

        ButtonImpara = new QPushButton(widget);
        ButtonImpara->setObjectName(QStringLiteral("ButtonImpara"));

        horizontalLayout->addWidget(ButtonImpara);

        ListaTipoStep = new QComboBox(widget);
        ListaTipoStep->setObjectName(QStringLiteral("ListaTipoStep"));

        horizontalLayout->addWidget(ListaTipoStep);

        ButtonSimula = new QPushButton(widget);
        ButtonSimula->setObjectName(QStringLiteral("ButtonSimula"));

        horizontalLayout->addWidget(ButtonSimula);

        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        horizontalLayout->addWidget(progressBar);


        verticalLayout_5->addLayout(horizontalLayout);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_4 = new QHBoxLayout(widget_2);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        Ciclo = new QLabel(widget_2);
        Ciclo->setObjectName(QStringLiteral("Ciclo"));

        horizontalLayout_2->addWidget(Ciclo);

        StepTime = new QLabel(widget_2);
        StepTime->setObjectName(QStringLiteral("StepTime"));

        horizontalLayout_2->addWidget(StepTime);

        MainTime = new QLabel(widget_2);
        MainTime->setObjectName(QStringLiteral("MainTime"));

        horizontalLayout_2->addWidget(MainTime);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        EventTime = new QLabel(widget_2);
        EventTime->setObjectName(QStringLiteral("EventTime"));

        horizontalLayout_3->addWidget(EventTime);

        RepiStep = new QLabel(widget_2);
        RepiStep->setObjectName(QStringLiteral("RepiStep"));

        horizontalLayout_3->addWidget(RepiStep);

        Time = new QLabel(widget_2);
        Time->setObjectName(QStringLiteral("Time"));

        horizontalLayout_3->addWidget(Time);

        label_8 = new QLabel(widget_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_3->addWidget(label_8);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMaximumSize(QSize(30, 16777215));
        verticalLayout_8 = new QVBoxLayout(widget_3);
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        ButtonErrore = new QToolButton(widget_3);
        ButtonErrore->setObjectName(QStringLiteral("ButtonErrore"));
        ButtonErrore->setMinimumSize(QSize(30, 30));

        verticalLayout_8->addWidget(ButtonErrore);

        ButtonMsgPlc = new QToolButton(widget_3);
        ButtonMsgPlc->setObjectName(QStringLiteral("ButtonMsgPlc"));
        ButtonMsgPlc->setMinimumSize(QSize(30, 30));

        verticalLayout_8->addWidget(ButtonMsgPlc);


        horizontalLayout_4->addWidget(widget_3);

        ListaErrori = new QListWidget(widget_2);
        ListaErrori->setObjectName(QStringLiteral("ListaErrori"));
        ListaErrori->setMinimumSize(QSize(550, 0));
        ListaErrori->setMaximumSize(QSize(550, 16777215));

        horizontalLayout_4->addWidget(ListaErrori);


        verticalLayout_5->addWidget(widget_2);

        tabImpianto = new QTabWidget(widget);
        tabImpianto->setObjectName(QStringLiteral("tabImpianto"));
        tabImpianto->setTabPosition(QTabWidget::South);
        tabVuoto = new QWidget();
        tabVuoto->setObjectName(QStringLiteral("tabVuoto"));
        verticalLayout_4 = new QVBoxLayout(tabVuoto);
        verticalLayout_4->setSpacing(1);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 1, 1, 1);
        VuotoView = new QVuoto(tabVuoto);
        VuotoView->setObjectName(QStringLiteral("VuotoView"));

        verticalLayout_4->addWidget(VuotoView);

        tabImpianto->addTab(tabVuoto, QString());
        tabPotenza = new QWidget();
        tabPotenza->setObjectName(QStringLiteral("tabPotenza"));
        verticalLayout_3 = new QVBoxLayout(tabPotenza);
        verticalLayout_3->setSpacing(1);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(1, 1, 1, 1);
        PotenzaView = new QPotenza(tabPotenza);
        PotenzaView->setObjectName(QStringLiteral("PotenzaView"));

        verticalLayout_3->addWidget(PotenzaView);

        tabImpianto->addTab(tabPotenza, QString());
        tabProcesso = new QWidget();
        tabProcesso->setObjectName(QStringLiteral("tabProcesso"));
        verticalLayout_7 = new QVBoxLayout(tabProcesso);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        ProcessoView = new QProcesso(tabProcesso);
        ProcessoView->setObjectName(QStringLiteral("ProcessoView"));

        verticalLayout_7->addWidget(ProcessoView);

        tabImpianto->addTab(tabProcesso, QString());
        tabMonitorizza = new QWidget();
        tabMonitorizza->setObjectName(QStringLiteral("tabMonitorizza"));
        verticalLayout_2 = new QVBoxLayout(tabMonitorizza);
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        MonitorizzaView = new QMonitorizza(tabMonitorizza);
        MonitorizzaView->setObjectName(QStringLiteral("MonitorizzaView"));

        verticalLayout_2->addWidget(MonitorizzaView);

        tabImpianto->addTab(tabMonitorizza, QString());
        tabLeek = new QWidget();
        tabLeek->setObjectName(QStringLiteral("tabLeek"));
        verticalLayout_6 = new QVBoxLayout(tabLeek);
        verticalLayout_6->setSpacing(1);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(1, 1, 1, 1);
        LeekView = new QLeek(tabLeek);
        LeekView->setObjectName(QStringLiteral("LeekView"));

        verticalLayout_6->addWidget(LeekView);

        tabImpianto->addTab(tabLeek, QString());
        tabModulo = new QWidget();
        tabModulo->setObjectName(QStringLiteral("tabModulo"));
        verticalLayout_9 = new QVBoxLayout(tabModulo);
        verticalLayout_9->setSpacing(1);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(1, 1, 1, 1);
        ModuloView = new QModulo(tabModulo);
        ModuloView->setObjectName(QStringLiteral("ModuloView"));

        verticalLayout_9->addWidget(ModuloView);

        tabImpianto->addTab(tabModulo, QString());

        verticalLayout_5->addWidget(tabImpianto);

        MainWindow->setCentralWidget(widget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabImpianto->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PlcNet", 0));
        ButtonVuoto->setText(QApplication::translate("MainWindow", "Vuoto", 0));
        ButtonStopVuoto->setText(QApplication::translate("MainWindow", "Stop Vuoto", 0));
        ButtonVenting->setText(QApplication::translate("MainWindow", "Venting", 0));
        ButtonStartProcesso->setText(QApplication::translate("MainWindow", "Start Processo", 0));
        ButtonStopProcesso->setText(QApplication::translate("MainWindow", "Stop Processo", 0));
        ButtonManule->setText(QApplication::translate("MainWindow", "Manuale", 0));
        ButtonAutomatico->setText(QApplication::translate("MainWindow", "Automatico", 0));
        ButtonApriCiclo->setText(QApplication::translate("MainWindow", "Apri Ciclo", 0));
        ButtonImpara->setText(QApplication::translate("MainWindow", "Impara", 0));
        ButtonSimula->setText(QApplication::translate("MainWindow", "Simula", 0));
        label->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        Ciclo->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        StepTime->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        MainTime->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        EventTime->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        RepiStep->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        Time->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        label_8->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        ButtonErrore->setText(QApplication::translate("MainWindow", "...", 0));
        ButtonMsgPlc->setText(QApplication::translate("MainWindow", "...", 0));
#ifndef QT_NO_TOOLTIP
        tabImpianto->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        tabImpianto->setTabText(tabImpianto->indexOf(tabVuoto), QApplication::translate("MainWindow", "Vuoto", 0));
        tabImpianto->setTabText(tabImpianto->indexOf(tabPotenza), QApplication::translate("MainWindow", "Potenza", 0));
        tabImpianto->setTabText(tabImpianto->indexOf(tabProcesso), QApplication::translate("MainWindow", "Processo", 0));
        tabImpianto->setTabText(tabImpianto->indexOf(tabMonitorizza), QApplication::translate("MainWindow", "Monitorizza", 0));
        tabImpianto->setTabText(tabImpianto->indexOf(tabLeek), QApplication::translate("MainWindow", "Leek", 0));
        tabImpianto->setTabText(tabImpianto->indexOf(tabModulo), QApplication::translate("MainWindow", "Modulo Plc", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
