/********************************************************************************
** Form generated from reading UI file 'qstartprocesso.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSTARTPROCESSO_H
#define UI_QSTARTPROCESSO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QStartProcesso
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widGas;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *widCatodiSx;
    QWidget *widCatodiDx;
    QWidget *widBias;

    void setupUi(QDialog *QStartProcesso)
    {
        if (QStartProcesso->objectName().isEmpty())
            QStartProcesso->setObjectName(QStringLiteral("QStartProcesso"));
        QStartProcesso->setWindowModality(Qt::WindowModal);
        QStartProcesso->resize(888, 728);
        verticalLayout_2 = new QVBoxLayout(QStartProcesso);
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        label = new QLabel(QStartProcesso);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(16777215, 30));

        verticalLayout_2->addWidget(label);

        progressBar = new QProgressBar(QStartProcesso);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout_2->addWidget(progressBar);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widGas = new QWidget(QStartProcesso);
        widGas->setObjectName(QStringLiteral("widGas"));
        widGas->setMaximumSize(QSize(250, 16777215));

        horizontalLayout_2->addWidget(widGas);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widCatodiSx = new QWidget(QStartProcesso);
        widCatodiSx->setObjectName(QStringLiteral("widCatodiSx"));

        horizontalLayout->addWidget(widCatodiSx);

        widCatodiDx = new QWidget(QStartProcesso);
        widCatodiDx->setObjectName(QStringLiteral("widCatodiDx"));

        horizontalLayout->addWidget(widCatodiDx);


        verticalLayout->addLayout(horizontalLayout);

        widBias = new QWidget(QStartProcesso);
        widBias->setObjectName(QStringLiteral("widBias"));
        widBias->setMaximumSize(QSize(16777215, 230));

        verticalLayout->addWidget(widBias);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(QStartProcesso);

        QMetaObject::connectSlotsByName(QStartProcesso);
    } // setupUi

    void retranslateUi(QDialog *QStartProcesso)
    {
        QStartProcesso->setWindowTitle(QApplication::translate("QStartProcesso", "Dialog", 0));
        label->setText(QApplication::translate("QStartProcesso", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class QStartProcesso: public Ui_QStartProcesso {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSTARTPROCESSO_H
