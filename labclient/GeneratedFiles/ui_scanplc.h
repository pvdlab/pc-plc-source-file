/********************************************************************************
** Form generated from reading UI file 'scanplc.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCANPLC_H
#define UI_SCANPLC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ScanPlc
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QProgressBar *progressBar;
    QListWidget *ListaEventi;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *connetti;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *ScanPlc)
    {
        if (ScanPlc->objectName().isEmpty())
            ScanPlc->setObjectName(QStringLiteral("ScanPlc"));
        ScanPlc->setWindowModality(Qt::WindowModal);
        ScanPlc->resize(860, 597);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/piu.png"), QSize(), QIcon::Normal, QIcon::Off);
        ScanPlc->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ScanPlc);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ScanPlc);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(12);
        font.setItalic(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        progressBar = new QProgressBar(ScanPlc);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        ListaEventi = new QListWidget(ScanPlc);
        ListaEventi->setObjectName(QStringLiteral("ListaEventi"));

        verticalLayout->addWidget(ListaEventi);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        connetti = new QPushButton(ScanPlc);
        connetti->setObjectName(QStringLiteral("connetti"));

        horizontalLayout->addWidget(connetti);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ScanPlc);

        QMetaObject::connectSlotsByName(ScanPlc);
    } // setupUi

    void retranslateUi(QDialog *ScanPlc)
    {
        ScanPlc->setWindowTitle(QApplication::translate("ScanPlc", "Cerco PLC", 0));
        label->setText(QApplication::translate("ScanPlc", "TextLabel", 0));
        connetti->setText(QApplication::translate("ScanPlc", "Nessuna Connessione", 0));
    } // retranslateUi

};

namespace Ui {
    class ScanPlc: public Ui_ScanPlc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCANPLC_H
