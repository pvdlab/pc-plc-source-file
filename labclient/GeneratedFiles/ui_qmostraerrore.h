/********************************************************************************
** Form generated from reading UI file 'qmostraerrore.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMOSTRAERRORE_H
#define UI_QMOSTRAERRORE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QMostraErrore
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ButtonOk;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *QMostraErrore)
    {
        if (QMostraErrore->objectName().isEmpty())
            QMostraErrore->setObjectName(QStringLiteral("QMostraErrore"));
        QMostraErrore->setWindowModality(Qt::WindowModal);
        QMostraErrore->resize(690, 234);
        verticalLayout = new QVBoxLayout(QMostraErrore);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(QMostraErrore);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        QFont font;
        font.setPointSize(14);
        font.setItalic(true);
        listWidget->setFont(font);

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ButtonOk = new QPushButton(QMostraErrore);
        ButtonOk->setObjectName(QStringLiteral("ButtonOk"));

        horizontalLayout->addWidget(ButtonOk);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QMostraErrore);

        QMetaObject::connectSlotsByName(QMostraErrore);
    } // setupUi

    void retranslateUi(QDialog *QMostraErrore)
    {
        QMostraErrore->setWindowTitle(QApplication::translate("QMostraErrore", "Dialog", 0));
        ButtonOk->setText(QApplication::translate("QMostraErrore", "Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class QMostraErrore: public Ui_QMostraErrore {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMOSTRAERRORE_H
