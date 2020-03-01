/********************************************************************************
** Form generated from reading UI file 'initdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INITDIALOG_H
#define UI_INITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_InitDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_10;
    QLabel *Ciclo;
    QComboBox *Nome;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_2;
    QLineEdit *Impianto;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_11;
    QSpinBox *PlcPorta;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *Host;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *StepTime;
    QLineEdit *DatiUtente;
    QHBoxLayout *horizontalLayout_3;
    QLabel *TempoOn;
    QLineEdit *DatiPass;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QSpinBox *DatiPorta;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *Connetti;
    QSpacerItem *horizontalSpacer;
    QPushButton *Annulla;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Salva;

    void setupUi(QDialog *InitDialog)
    {
        if (InitDialog->objectName().isEmpty())
            InitDialog->setObjectName(QStringLiteral("InitDialog"));
        InitDialog->resize(667, 313);
        verticalLayout = new QVBoxLayout(InitDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        Ciclo = new QLabel(InitDialog);
        Ciclo->setObjectName(QStringLiteral("Ciclo"));
        Ciclo->setMinimumSize(QSize(120, 0));
        Ciclo->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_10->addWidget(Ciclo);

        Nome = new QComboBox(InitDialog);
        Nome->setObjectName(QStringLiteral("Nome"));

        horizontalLayout_10->addWidget(Nome);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_2 = new QLabel(InitDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_11->addWidget(label_2);

        Impianto = new QLineEdit(InitDialog);
        Impianto->setObjectName(QStringLiteral("Impianto"));

        horizontalLayout_11->addWidget(Impianto);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_11 = new QLabel(InitDialog);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setMinimumSize(QSize(120, 0));
        label_11->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_7->addWidget(label_11);

        PlcPorta = new QSpinBox(InitDialog);
        PlcPorta->setObjectName(QStringLiteral("PlcPorta"));
        PlcPorta->setMaximum(999999);

        horizontalLayout_7->addWidget(PlcPorta);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(InitDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));

        horizontalLayout->addWidget(label);

        Host = new QLineEdit(InitDialog);
        Host->setObjectName(QStringLiteral("Host"));

        horizontalLayout->addWidget(Host);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(18, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_3 = new QLabel(InitDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        StepTime = new QLabel(InitDialog);
        StepTime->setObjectName(QStringLiteral("StepTime"));
        StepTime->setMinimumSize(QSize(120, 0));
        StepTime->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_2->addWidget(StepTime);

        DatiUtente = new QLineEdit(InitDialog);
        DatiUtente->setObjectName(QStringLiteral("DatiUtente"));

        horizontalLayout_2->addWidget(DatiUtente);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        TempoOn = new QLabel(InitDialog);
        TempoOn->setObjectName(QStringLiteral("TempoOn"));
        TempoOn->setMinimumSize(QSize(120, 0));
        TempoOn->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_3->addWidget(TempoOn);

        DatiPass = new QLineEdit(InitDialog);
        DatiPass->setObjectName(QStringLiteral("DatiPass"));

        horizontalLayout_3->addWidget(DatiPass);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(InitDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(120, 0));
        label_4->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_4->addWidget(label_4);

        DatiPorta = new QSpinBox(InitDialog);
        DatiPorta->setObjectName(QStringLiteral("DatiPorta"));
        DatiPorta->setMaximum(99999);

        horizontalLayout_4->addWidget(DatiPorta);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        Connetti = new QPushButton(InitDialog);
        Connetti->setObjectName(QStringLiteral("Connetti"));

        horizontalLayout_12->addWidget(Connetti);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer);

        Annulla = new QPushButton(InitDialog);
        Annulla->setObjectName(QStringLiteral("Annulla"));

        horizontalLayout_12->addWidget(Annulla);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_2);

        Salva = new QPushButton(InitDialog);
        Salva->setObjectName(QStringLiteral("Salva"));

        horizontalLayout_12->addWidget(Salva);


        verticalLayout->addLayout(horizontalLayout_12);


        retranslateUi(InitDialog);

        QMetaObject::connectSlotsByName(InitDialog);
    } // setupUi

    void retranslateUi(QDialog *InitDialog)
    {
        InitDialog->setWindowTitle(QApplication::translate("InitDialog", "Dialog", 0));
        Ciclo->setText(QApplication::translate("InitDialog", "Nome", 0));
        label_2->setText(QApplication::translate("InitDialog", "Impianto", 0));
        label_11->setText(QApplication::translate("InitDialog", "Porta Plc", 0));
        label->setText(QApplication::translate("InitDialog", "Host", 0));
        label_3->setText(QApplication::translate("InitDialog", "DataBase Dati", 0));
        StepTime->setText(QApplication::translate("InitDialog", "Utente ", 0));
        TempoOn->setText(QApplication::translate("InitDialog", "Password ", 0));
        label_4->setText(QApplication::translate("InitDialog", "Porta", 0));
        Connetti->setText(QApplication::translate("InitDialog", "Connetti", 0));
        Annulla->setText(QApplication::translate("InitDialog", "Annulla", 0));
        Salva->setText(QApplication::translate("InitDialog", "Salva", 0));
    } // retranslateUi

};

namespace Ui {
    class InitDialog: public Ui_InitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITDIALOG_H
