/********************************************************************************
** Form generated from reading UI file 'qnuovocompstep.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QNUOVOCOMPSTEP_H
#define UI_QNUOVOCOMPSTEP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QNuovoCompStep
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *ListaCompStep;
    QHBoxLayout *horizontalLayout;
    QPushButton *ButtonOk;
    QSpacerItem *horizontalSpacer;
    QPushButton *ButtonAnnulla;

    void setupUi(QDialog *QNuovoCompStep)
    {
        if (QNuovoCompStep->objectName().isEmpty())
            QNuovoCompStep->setObjectName(QStringLiteral("QNuovoCompStep"));
        QNuovoCompStep->resize(630, 90);
        QNuovoCompStep->setMinimumSize(QSize(630, 90));
        QNuovoCompStep->setMaximumSize(QSize(630, 90));
        verticalLayout = new QVBoxLayout(QNuovoCompStep);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ListaCompStep = new QComboBox(QNuovoCompStep);
        ListaCompStep->setObjectName(QStringLiteral("ListaCompStep"));

        verticalLayout->addWidget(ListaCompStep);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ButtonOk = new QPushButton(QNuovoCompStep);
        ButtonOk->setObjectName(QStringLiteral("ButtonOk"));

        horizontalLayout->addWidget(ButtonOk);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ButtonAnnulla = new QPushButton(QNuovoCompStep);
        ButtonAnnulla->setObjectName(QStringLiteral("ButtonAnnulla"));

        horizontalLayout->addWidget(ButtonAnnulla);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QNuovoCompStep);

        QMetaObject::connectSlotsByName(QNuovoCompStep);
    } // setupUi

    void retranslateUi(QDialog *QNuovoCompStep)
    {
        QNuovoCompStep->setWindowTitle(QApplication::translate("QNuovoCompStep", "Dialog", 0));
        ButtonOk->setText(QApplication::translate("QNuovoCompStep", "Ok", 0));
        ButtonAnnulla->setText(QApplication::translate("QNuovoCompStep", "Annulla", 0));
    } // retranslateUi

};

namespace Ui {
    class QNuovoCompStep: public Ui_QNuovoCompStep {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QNUOVOCOMPSTEP_H
