/********************************************************************************
** Form generated from reading UI file 'qprocesso.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QPROCESSO_H
#define UI_QPROCESSO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qgraficoriquadro.h"

QT_BEGIN_NAMESPACE

class Ui_QProcesso
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QComboBox *TabProcesso;
    QToolButton *ButtonNuovoProcesso;
    QToolButton *ButtonDuplicaProcesso;
    QToolButton *ButtonEliminaProcesso;
    QToolButton *ButtonSetTmpProcesso;
    QToolButton *ButtonCaricaProcesso;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTempoProcesso;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QWidget *WidButtonStep;
    QVBoxLayout *verticalLayout;
    QToolButton *ButtonSalva;
    QToolButton *ButtonAddStep;
    QToolButton *ButtonDelStep;
    QToolButton *ButtonDuplicaStep;
    QToolButton *ButtonPrima;
    QToolButton *ButtonDopo;
    QSpacerItem *verticalSpacer;
    QTableView *TabStep;
    QGraficoRiquadro *GraficoView;
    QLabel *label;
    QHBoxLayout *horizontalLayout_4;
    QWidget *WidButtonCompStep;
    QVBoxLayout *verticalLayout_2;
    QToolButton *ButtonInfo;
    QToolButton *ButtonAddCompStep;
    QToolButton *ButtonDelCompStep;
    QToolButton *ButtonCambiaCompStep;
    QToolButton *ButtonSalvaCompCome;
    QSpacerItem *verticalSpacer_2;
    QTableView *TabCompStep;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QToolButton *ButtonAddTempo;
    QToolButton *ButtonDelTempo;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *QProcesso)
    {
        if (QProcesso->objectName().isEmpty())
            QProcesso->setObjectName(QStringLiteral("QProcesso"));
        QProcesso->resize(1005, 602);
        verticalLayout_4 = new QVBoxLayout(QProcesso);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        TabProcesso = new QComboBox(QProcesso);
        TabProcesso->setObjectName(QStringLiteral("TabProcesso"));
        TabProcesso->setMinimumSize(QSize(249, 0));

        horizontalLayout->addWidget(TabProcesso);

        ButtonNuovoProcesso = new QToolButton(QProcesso);
        ButtonNuovoProcesso->setObjectName(QStringLiteral("ButtonNuovoProcesso"));
        ButtonNuovoProcesso->setMinimumSize(QSize(30, 30));

        horizontalLayout->addWidget(ButtonNuovoProcesso);

        ButtonDuplicaProcesso = new QToolButton(QProcesso);
        ButtonDuplicaProcesso->setObjectName(QStringLiteral("ButtonDuplicaProcesso"));
        ButtonDuplicaProcesso->setMinimumSize(QSize(30, 30));

        horizontalLayout->addWidget(ButtonDuplicaProcesso);

        ButtonEliminaProcesso = new QToolButton(QProcesso);
        ButtonEliminaProcesso->setObjectName(QStringLiteral("ButtonEliminaProcesso"));
        ButtonEliminaProcesso->setMinimumSize(QSize(30, 30));

        horizontalLayout->addWidget(ButtonEliminaProcesso);

        ButtonSetTmpProcesso = new QToolButton(QProcesso);
        ButtonSetTmpProcesso->setObjectName(QStringLiteral("ButtonSetTmpProcesso"));
        ButtonSetTmpProcesso->setMinimumSize(QSize(30, 30));

        horizontalLayout->addWidget(ButtonSetTmpProcesso);

        ButtonCaricaProcesso = new QToolButton(QProcesso);
        ButtonCaricaProcesso->setObjectName(QStringLiteral("ButtonCaricaProcesso"));
        ButtonCaricaProcesso->setMinimumSize(QSize(30, 30));

        horizontalLayout->addWidget(ButtonCaricaProcesso);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelTempoProcesso = new QLabel(QProcesso);
        labelTempoProcesso->setObjectName(QStringLiteral("labelTempoProcesso"));

        horizontalLayout->addWidget(labelTempoProcesso);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        WidButtonStep = new QWidget(QProcesso);
        WidButtonStep->setObjectName(QStringLiteral("WidButtonStep"));
        WidButtonStep->setMaximumSize(QSize(30, 16777215));
        verticalLayout = new QVBoxLayout(WidButtonStep);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        ButtonSalva = new QToolButton(WidButtonStep);
        ButtonSalva->setObjectName(QStringLiteral("ButtonSalva"));
        ButtonSalva->setMinimumSize(QSize(30, 30));

        verticalLayout->addWidget(ButtonSalva);

        ButtonAddStep = new QToolButton(WidButtonStep);
        ButtonAddStep->setObjectName(QStringLiteral("ButtonAddStep"));
        ButtonAddStep->setMinimumSize(QSize(30, 30));

        verticalLayout->addWidget(ButtonAddStep);

        ButtonDelStep = new QToolButton(WidButtonStep);
        ButtonDelStep->setObjectName(QStringLiteral("ButtonDelStep"));
        ButtonDelStep->setMinimumSize(QSize(30, 30));

        verticalLayout->addWidget(ButtonDelStep);

        ButtonDuplicaStep = new QToolButton(WidButtonStep);
        ButtonDuplicaStep->setObjectName(QStringLiteral("ButtonDuplicaStep"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ButtonDuplicaStep->sizePolicy().hasHeightForWidth());
        ButtonDuplicaStep->setSizePolicy(sizePolicy);
        ButtonDuplicaStep->setMinimumSize(QSize(30, 30));

        verticalLayout->addWidget(ButtonDuplicaStep);

        ButtonPrima = new QToolButton(WidButtonStep);
        ButtonPrima->setObjectName(QStringLiteral("ButtonPrima"));
        ButtonPrima->setMinimumSize(QSize(30, 30));

        verticalLayout->addWidget(ButtonPrima);

        ButtonDopo = new QToolButton(WidButtonStep);
        ButtonDopo->setObjectName(QStringLiteral("ButtonDopo"));
        ButtonDopo->setMinimumSize(QSize(30, 30));

        verticalLayout->addWidget(ButtonDopo);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(WidButtonStep);

        TabStep = new QTableView(QProcesso);
        TabStep->setObjectName(QStringLiteral("TabStep"));

        horizontalLayout_2->addWidget(TabStep);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        GraficoView = new QGraficoRiquadro(QProcesso);
        GraficoView->setObjectName(QStringLiteral("GraficoView"));

        horizontalLayout_3->addWidget(GraficoView);


        verticalLayout_4->addLayout(horizontalLayout_3);

        label = new QLabel(QProcesso);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_4->addWidget(label);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        WidButtonCompStep = new QWidget(QProcesso);
        WidButtonCompStep->setObjectName(QStringLiteral("WidButtonCompStep"));
        WidButtonCompStep->setMaximumSize(QSize(30, 300));
        verticalLayout_2 = new QVBoxLayout(WidButtonCompStep);
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        ButtonInfo = new QToolButton(WidButtonCompStep);
        ButtonInfo->setObjectName(QStringLiteral("ButtonInfo"));
        ButtonInfo->setMinimumSize(QSize(30, 30));

        verticalLayout_2->addWidget(ButtonInfo);

        ButtonAddCompStep = new QToolButton(WidButtonCompStep);
        ButtonAddCompStep->setObjectName(QStringLiteral("ButtonAddCompStep"));
        ButtonAddCompStep->setMinimumSize(QSize(30, 30));

        verticalLayout_2->addWidget(ButtonAddCompStep);

        ButtonDelCompStep = new QToolButton(WidButtonCompStep);
        ButtonDelCompStep->setObjectName(QStringLiteral("ButtonDelCompStep"));
        ButtonDelCompStep->setMinimumSize(QSize(30, 30));

        verticalLayout_2->addWidget(ButtonDelCompStep);

        ButtonCambiaCompStep = new QToolButton(WidButtonCompStep);
        ButtonCambiaCompStep->setObjectName(QStringLiteral("ButtonCambiaCompStep"));
        ButtonCambiaCompStep->setMinimumSize(QSize(30, 30));

        verticalLayout_2->addWidget(ButtonCambiaCompStep);

        ButtonSalvaCompCome = new QToolButton(WidButtonCompStep);
        ButtonSalvaCompCome->setObjectName(QStringLiteral("ButtonSalvaCompCome"));
        ButtonSalvaCompCome->setMinimumSize(QSize(30, 30));

        verticalLayout_2->addWidget(ButtonSalvaCompCome);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout_4->addWidget(WidButtonCompStep);

        TabCompStep = new QTableView(QProcesso);
        TabCompStep->setObjectName(QStringLiteral("TabCompStep"));
        TabCompStep->setMaximumSize(QSize(16777215, 300));

        horizontalLayout_4->addWidget(TabCompStep);

        widget = new QWidget(QProcesso);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(30, 300));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        ButtonAddTempo = new QToolButton(widget);
        ButtonAddTempo->setObjectName(QStringLiteral("ButtonAddTempo"));
        ButtonAddTempo->setMinimumSize(QSize(30, 30));

        verticalLayout_3->addWidget(ButtonAddTempo);

        ButtonDelTempo = new QToolButton(widget);
        ButtonDelTempo->setObjectName(QStringLiteral("ButtonDelTempo"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(30);
        sizePolicy1.setVerticalStretch(30);
        sizePolicy1.setHeightForWidth(ButtonDelTempo->sizePolicy().hasHeightForWidth());
        ButtonDelTempo->setSizePolicy(sizePolicy1);
        ButtonDelTempo->setMinimumSize(QSize(30, 30));

        verticalLayout_3->addWidget(ButtonDelTempo);

        verticalSpacer_3 = new QSpacerItem(20, 237, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        horizontalLayout_4->addWidget(widget);


        verticalLayout_4->addLayout(horizontalLayout_4);


        retranslateUi(QProcesso);

        QMetaObject::connectSlotsByName(QProcesso);
    } // setupUi

    void retranslateUi(QWidget *QProcesso)
    {
        QProcesso->setWindowTitle(QApplication::translate("QProcesso", "Form", 0));
        ButtonNuovoProcesso->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonDuplicaProcesso->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonEliminaProcesso->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonSetTmpProcesso->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonCaricaProcesso->setText(QApplication::translate("QProcesso", "...", 0));
        labelTempoProcesso->setText(QApplication::translate("QProcesso", "TextLabel", 0));
        ButtonSalva->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonAddStep->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonDelStep->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonDuplicaStep->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonPrima->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonDopo->setText(QApplication::translate("QProcesso", "...", 0));
        label->setText(QString());
        ButtonInfo->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonAddCompStep->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonDelCompStep->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonCambiaCompStep->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonSalvaCompCome->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonAddTempo->setText(QApplication::translate("QProcesso", "...", 0));
        ButtonDelTempo->setText(QApplication::translate("QProcesso", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class QProcesso: public Ui_QProcesso {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QPROCESSO_H
