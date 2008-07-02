
#include "abakusmainwindow.h"
#include <QApplication>

// dialogs
#include "abakusconfiguredia.h"
#include <QMessageBox>
// widgets
#include "abakusclock.h"
#include <QPushButton>
// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>

AbakusMainWindow::AbakusMainWindow()
    : ABAKUS_MW_SUPERCLASS ()
{
    
    // init dialogs
    diaConfigure = NULL;
    
    createGui();
    connectSlots();
    retranslateUi();
    
    // create dialogs
    diaConfigure  = new AbakusConfigureDia(this);
    diaConfigure->setClock(wdgClock, TRUE);
}

AbakusMainWindow::~AbakusMainWindow(){
    
    
}


void AbakusMainWindow::createGui()
{
    // main clock widget
    wdgClock = new AbakusClock;
    
    // tool buttons
    btnQuit = new QPushButton("Quit");
    btnAbout = new QPushButton("About");
    btnConfigure = new QPushButton("Configure");
    
    // layouts
    // tool buttons
    layoutToolButtons = new QVBoxLayout;
    layoutToolButtons->setMargin(3);
    layoutToolButtons->addStretch();
    layoutToolButtons->addWidget(btnConfigure);
    layoutToolButtons->addWidget(btnAbout);
    layoutToolButtons->addWidget(btnQuit);
    
    // layout parent
    layoutParent = new QHBoxLayout;
    layoutParent->setMargin(1);
    layoutParent->addWidget(wdgClock);
    layoutParent->addLayout(layoutToolButtons);
    
    setLayout(layoutParent);
}

void AbakusMainWindow::connectSlots()
{
    connect(btnQuit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(btnConfigure, SIGNAL(clicked()), this, SLOT(showConfigureDialog()));
    connect(btnAbout, SIGNAL(clicked()), this, SLOT(showAboutDialog()));
}


void AbakusMainWindow::retranslateUi()
{
    btnQuit->setText(tr("Beenden"));
    btnAbout->setText(tr("Info"));
    btnConfigure->setText(tr("Einrichten"));
    setWindowTitle(tr("Abakus Uhr - Animiert"));
}



void AbakusMainWindow::showConfigureDialog()
{
    if(!diaConfigure)
    {
        diaConfigure  = new AbakusConfigureDia(this);
        diaConfigure->setClock(wdgClock);
    }
    diaConfigure->show();
}

void AbakusMainWindow::showAboutDialog()
{
    QString msg = "Abakus Clock - animated\n";
    msg += "It was written in C++ using Qt4\n";
    msg += "License: GPL\n";
    msg += "Author: Thorsten Wissmann edu@thorsten-wissmann.de";
    QMessageBox::information(this, "About Abakus Clock - animated", msg);
}
