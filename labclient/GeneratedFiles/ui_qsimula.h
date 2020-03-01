/********************************************************************************
** Form generated from reading UI file 'qsimula.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSIMULA_H
#define UI_QSIMULA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QSimula
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpinBox *BiasLowTempo;
    QPushButton *BiasLow;
    QSpacerItem *verticalSpacer;
    QPushButton *chiudi;

    void setupUi(QDialog *QSimula)
    {
        if (QSimula->objectName().isEmpty())
            QSimula->setObjectName(QStringLiteral("QSimula"));
        QSimula->setWindowModality(Qt::WindowModal);
        QSimula->resize(418, 235);
        verticalLayout = new QVBoxLayout(QSimula);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        BiasLowTempo = new QSpinBox(QSimula);
        BiasLowTempo->setObjectName(QStringLiteral("BiasLowTempo"));
        BiasLowTempo->setMaximumSize(QSize(60, 16777215));
        BiasLowTempo->setMaximum(999);
        BiasLowTempo->setValue(10);

        horizontalLayout->addWidget(BiasLowTempo);

        BiasLow = new QPushButton(QSimula);
        BiasLow->setObjectName(QStringLiteral("BiasLow"));

        horizontalLayout->addWidget(BiasLow);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 216, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        chiudi = new QPushButton(QSimula);
        chiudi->setObjectName(QStringLiteral("chiudi"));

        verticalLayout->addWidget(chiudi);


        retranslateUi(QSimula);

        QMetaObject::connectSlotsByName(QSimula);
    } // setupUi

    void retranslateUi(QDialog *QSimula)
    {
        QSimula->setWindowTitle(QApplication::translate("QSimula", "Dialog", 0));
        BiasLow->setText(QApplication::translate("QSimula", "BiasLow", 0));
        chiudi->setText(QApplication::translate("QSimula", "Chiudi", 0));
    } // retranslateUi

};

namespace Ui {
    class QSimula: public Ui_QSimula {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSIMULA_H
