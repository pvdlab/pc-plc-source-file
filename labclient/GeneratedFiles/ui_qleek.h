/********************************************************************************
** Form generated from reading UI file 'qleek.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLEEK_H
#define UI_QLEEK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qgraficoriquadro.h"

QT_BEGIN_NAMESPACE

class Ui_QLeek
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *WidSX;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *ButtonAutomatico;
    QPushButton *ButtonStatoVuoto;
    QLabel *label;
    QComboBox *Volume;
    QHBoxLayout *horizontalLayout;
    QLabel *Penning;
    QSpacerItem *horizontalSpacer;
    QLabel *Leek;
    QSpacerItem *horizontalSpacer_2;
    QLabel *Tempo;
    QHBoxLayout *horizontalLayout_2;
    QGraficoRiquadro *Grafico;

    void setupUi(QWidget *QLeek)
    {
        if (QLeek->objectName().isEmpty())
            QLeek->setObjectName(QStringLiteral("QLeek"));
        QLeek->resize(1114, 673);
        verticalLayout_2 = new QVBoxLayout(QLeek);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        WidSX = new QWidget(QLeek);
        WidSX->setObjectName(QStringLiteral("WidSX"));
        verticalLayout = new QVBoxLayout(WidSX);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        ButtonAutomatico = new QPushButton(WidSX);
        ButtonAutomatico->setObjectName(QStringLiteral("ButtonAutomatico"));

        horizontalLayout_3->addWidget(ButtonAutomatico);

        ButtonStatoVuoto = new QPushButton(WidSX);
        ButtonStatoVuoto->setObjectName(QStringLiteral("ButtonStatoVuoto"));

        horizontalLayout_3->addWidget(ButtonStatoVuoto);

        label = new QLabel(WidSX);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        Volume = new QComboBox(WidSX);
        Volume->setObjectName(QStringLiteral("Volume"));

        horizontalLayout_3->addWidget(Volume);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Penning = new QLabel(WidSX);
        Penning->setObjectName(QStringLiteral("Penning"));
        QFont font;
        font.setPointSize(40);
        Penning->setFont(font);

        horizontalLayout->addWidget(Penning);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Leek = new QLabel(WidSX);
        Leek->setObjectName(QStringLiteral("Leek"));
        Leek->setFont(font);

        horizontalLayout->addWidget(Leek);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        Tempo = new QLabel(WidSX);
        Tempo->setObjectName(QStringLiteral("Tempo"));
        QFont font1;
        font1.setPointSize(32);
        Tempo->setFont(font1);

        horizontalLayout->addWidget(Tempo);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        Grafico = new QGraficoRiquadro(WidSX);
        Grafico->setObjectName(QStringLiteral("Grafico"));

        horizontalLayout_2->addWidget(Grafico);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(WidSX);


        retranslateUi(QLeek);

        QMetaObject::connectSlotsByName(QLeek);
    } // setupUi

    void retranslateUi(QWidget *QLeek)
    {
        QLeek->setWindowTitle(QApplication::translate("QLeek", "Form", 0));
        ButtonAutomatico->setText(QApplication::translate("QLeek", "Automatico", 0));
        ButtonStatoVuoto->setText(QApplication::translate("QLeek", "Vuoto", 0));
        label->setText(QApplication::translate("QLeek", "Volume", 0));
        Penning->setText(QApplication::translate("QLeek", "TextLabel", 0));
        Leek->setText(QApplication::translate("QLeek", "TextLabel", 0));
        Tempo->setText(QApplication::translate("QLeek", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class QLeek: public Ui_QLeek {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLEEK_H
