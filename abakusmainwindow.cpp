
#include "abakusmainwindow.h"
#include <QApplication>

// dialogs
#include "abakusconfiguredia.h"
#include <QMessageBox>
// widgets
#include "abakusclock.h"
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
// other 
#include <QSizePolicy>
#include <QKeyEvent>

AbakusMainWindow::AbakusMainWindow()
    : ABAKUS_MW_SUPERCLASS ()
{
    bool debugMode = QApplication::arguments().contains("--debug", Qt::CaseInsensitive) ||
            QApplication::arguments().contains("-debug", Qt::CaseInsensitive);
    // init dialogs
    diaConfigure = NULL;
    
    // init members
    m_bIsFullscreen = FALSE;
    
    createGui();
    connectSlots();
    initWidgets();
    retranslateUi();
    
    // load window icon
    setWindowIcon(QIcon(QApplication::applicationDirPath() + "/clock16.png"));
    
    // create dialogs
    diaConfigure  = new AbakusConfigureDia(this);
    diaConfigure->setClock(wdgClock, TRUE);
    wdgClock->setShowFPS(debugMode);
    
    resize(520, 460);
}

AbakusMainWindow::~AbakusMainWindow()
{
}


void AbakusMainWindow::createGui()
{
    // main clock widget
    wdgClock = new AbakusClock;
    
    // header
    txtHeaderInput = new QTextEdit;
    // create new SizePolicy, so that
    // txtHeaderInput (textedit) has the
    // policy of an line edit ( not growing vertically )
    txtHeaderInput->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    txtHeaderInput->setMinimumSize(
        txtHeaderInput->minimumSize().width(), 27*2);
    txtHeaderInput->setBaseSize(txtHeaderInput->minimumSize());
    txtHeaderInput->resize(txtHeaderInput->minimumSize());
    lblHeader = new QLabel;
    lblHeader->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    btnHeaderConfirm = new QPushButton;
    
    //same for footer
    txtFooterInput = new QTextEdit;
    txtFooterInput->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    txtFooterInput->setMinimumSize(
                                   txtFooterInput->minimumSize().width(), 27*2);
    txtFooterInput->setBaseSize(txtFooterInput->minimumSize());
    txtFooterInput->resize(txtFooterInput->minimumSize());
    lblFooter = new QLabel;
    lblFooter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    btnFooterConfirm = new QPushButton;
    
    // tool buttons
    btnQuit = new QPushButton("Quit");
    btnAbout = new QPushButton("About");
    btnHelp = new QPushButton("help");
    btnConfigure = new QPushButton("Configure");
    btnEditHeader = new QPushButton("Edit Title");
    btnEditHeader->setCheckable(TRUE);
    btnEditFooter = new QPushButton("Edit Footer");
    btnEditFooter->setCheckable(TRUE);
    
    // layouts
    // tool buttons
    layoutToolButtons = new QVBoxLayout;
    layoutToolButtons->setMargin(3);
    layoutToolButtons->addStretch();
    layoutToolButtons->addWidget(btnConfigure);
    layoutToolButtons->addWidget(btnEditHeader);
    layoutToolButtons->addWidget(btnEditFooter);
    layoutToolButtons->addWidget(btnHelp);
    layoutToolButtons->addWidget(btnAbout);
    layoutToolButtons->addWidget(btnQuit);
    // layout header
    layoutHeader = new QHBoxLayout;
    layoutHeader->setMargin(0);
    layoutHeader->addWidget(txtHeaderInput);
    layoutHeader->addWidget(lblHeader);
    layoutHeader->addWidget(btnHeaderConfirm);
    // layout footer
    layoutFooter = new QHBoxLayout;
    layoutFooter->setMargin(0);
    layoutFooter->addWidget(txtFooterInput);
    layoutFooter->addWidget(lblFooter);
    layoutFooter->addWidget(btnFooterConfirm);
    // layout center
    layoutCenter = new QHBoxLayout;
    layoutCenter->setMargin(0);
    layoutCenter->addWidget(wdgClock);
    layoutCenter->addLayout(layoutToolButtons);
    //layout parent
    layoutParent = new QVBoxLayout;
    layoutParent->setMargin(3);
    layoutParent->addLayout(layoutHeader);
    layoutParent->addLayout(layoutCenter);
    layoutParent->addLayout(layoutFooter);
    
    setLayout(layoutParent);
}

void AbakusMainWindow::connectSlots()
{
    connect(btnQuit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(btnConfigure, SIGNAL(clicked()), this, SLOT(showConfigureDialog()));
    connect(btnAbout, SIGNAL(clicked()), this, SLOT(showAboutDialog()));
    connect(btnHelp, SIGNAL(clicked()), this, SLOT(showHelpDialog()));
    connect(btnEditHeader, SIGNAL(toggled(bool)), this, SLOT(toggleHeaderEdit(bool)));
    connect(btnHeaderConfirm, SIGNAL(clicked()), this, SLOT(writeHeaderInputToLabel()));
    connect(btnEditFooter, SIGNAL(toggled(bool)), this, SLOT(toggleFooterEdit(bool)));
    connect(btnFooterConfirm, SIGNAL(clicked()), this, SLOT(writeFooterInputToLabel()));
}


void AbakusMainWindow::initWidgets()
{
    txtHeaderInput->setVisible(FALSE);
    lblHeader->setVisible(FALSE);
    btnHeaderConfirm->setVisible(FALSE);
    
    txtFooterInput->setVisible(FALSE);
    lblFooter->setVisible(FALSE);
    btnFooterConfirm->setVisible(FALSE);
}

void AbakusMainWindow::retranslateUi()
{
    setWindowTitle(tr("Abakus Uhr - Animiert"));
    btnConfigure->setText(tr("Einrichten"));
    btnHelp->setText(tr("Hilfe"));
    btnAbout->setText(tr("Info"));
    btnQuit->setText(tr("Beenden"));
    btnEditHeader->setText(tr("Kopfzeile"));
     // 0x00DF = unicode for &szlig; -> ß
    btnEditFooter->setText(tr("Fu&szlig;zeile").replace("&szlig;", QChar(0x00DF)));
    btnHeaderConfirm->setText(tr("OK"));
    btnFooterConfirm->setText(tr("OK"));
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
    msg += QString("Compiled at ") + __TIME__ + " o' clock on " + __DATE__ + "\n";
    msg += "Author: Thorsten Wissmann edu@thorsten-wissmann.de";
    QMessageBox::information(this, "About Abakus Clock - animated", msg);
}


void AbakusMainWindow::keyPressEvent(QKeyEvent* event)
{
    if(!event){
        return;
    }
    int key = event->key();
    switch(key)
    {
        case Qt::Key_H:{
            bool visible = !btnConfigure->isVisible();
            btnConfigure->setVisible(visible);
            btnQuit->setVisible(visible);
            btnAbout->setVisible(visible);
            btnHelp->setVisible(visible);
            btnEditHeader->setVisible(visible);
            btnEditFooter->setVisible(visible);
            
            break;
        }
        case Qt::Key_Q:{
            QApplication::quit();
            break;
        }
        case Qt::Key_F:{
            m_bIsFullscreen = !m_bIsFullscreen;
            if(m_bIsFullscreen){
                showFullScreen();
            }else{
                showNormal();
            }
            break;
        }
        case Qt::Key_E:{
            // toggle button -> toggle header edit field
            btnEditHeader->setChecked(!btnEditHeader->isChecked());
            break;
        }
        case Qt::Key_R:{
            // toggle button -> toggle footer edit field
            btnEditFooter->setChecked(!btnEditFooter->isChecked());
            break;
        }
        case Qt::Key_C:{
            showConfigureDialog();
            break;
        }
    }
}

void AbakusMainWindow::showHelpDialog()
{
    QString msg = "Shortcuts:\n";
    msg += "F - Toggle Fullscreen\n";
    msg += "H - Toggle Toolbuttons\n";
    msg += "E - Edit Header\n";
    msg += "R - Edit Footer\n";
    msg += "C - Configurate\n";
    msg += "Q - Quit\n";
    QMessageBox::information(this, "Abakus Clock - animated", msg);
}


void AbakusMainWindow::toggleHeaderEdit(bool visible)
{
    if(visible){
        // if edit was shown, then write
        // label text to textedit txtHeaderInput
        txtHeaderInput->setPlainText(lblHeader->text());
    }else
    {
        // if edit was hidden, and label was shown,
        // then write text to label
        lblHeader->setText(txtHeaderInput->toPlainText());
    }
    // toggle visibility of objects
    btnHeaderConfirm->setVisible(visible);
    txtHeaderInput->setVisible(visible);
    lblHeader->setVisible((!visible) && (!lblHeader->text().isEmpty()));
    // focus this, to provide keyboard shortcuts
    this->setFocus();
}

void AbakusMainWindow::writeHeaderInputToLabel()
{
    btnEditHeader->setChecked(FALSE);
}

void AbakusMainWindow::toggleFooterEdit(bool visible)
{
    if(visible){
        // if edit was shown, then write
        // label text to textedit txtFooterInput
        txtFooterInput->setPlainText(lblFooter->text());
    }else
    {
        // if edit was hidden, and label was shown,
        // then write text to label
        lblFooter->setText(txtFooterInput->toPlainText());
    }
    // toggle visibility of objects
    btnFooterConfirm->setVisible(visible);
    txtFooterInput->setVisible(visible);
    lblFooter->setVisible((!visible) && (!lblFooter->text().isEmpty()));
    // focus this, to provide keyboard shortcuts
    this->setFocus();
}

void AbakusMainWindow::writeFooterInputToLabel()
{
    btnEditFooter->setChecked(FALSE);
}


