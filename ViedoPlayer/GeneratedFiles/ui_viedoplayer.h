/********************************************************************************
** Form generated from reading UI file 'viedoplayer.ui'
**
** Created: Mon Oct 7 14:59:07 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEDOPLAYER_H
#define UI_VIEDOPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViedoPlayerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ViedoPlayerClass)
    {
        if (ViedoPlayerClass->objectName().isEmpty())
            ViedoPlayerClass->setObjectName(QString::fromUtf8("ViedoPlayerClass"));
        ViedoPlayerClass->resize(600, 400);
        menuBar = new QMenuBar(ViedoPlayerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        ViedoPlayerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ViedoPlayerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ViedoPlayerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ViedoPlayerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ViedoPlayerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ViedoPlayerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ViedoPlayerClass->setStatusBar(statusBar);

        retranslateUi(ViedoPlayerClass);

        QMetaObject::connectSlotsByName(ViedoPlayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ViedoPlayerClass)
    {
        ViedoPlayerClass->setWindowTitle(QApplication::translate("ViedoPlayerClass", "ViedoPlayer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ViedoPlayerClass: public Ui_ViedoPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEDOPLAYER_H
