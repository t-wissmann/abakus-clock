#include "abakusconfiguredia.h"
#include "abakusclock.h"

// widgets
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTimeEdit>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>

AbakusConfigureDia::AbakusConfigureDia(QWidget* parent)
    : QDialog(parent)
{
    // init members
    m_pClock = NULL;
    // create GUI
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
    // hide/close if app gets quit
    setAttribute(Qt::WA_QuitOnClose, FALSE);
}

AbakusConfigureDia::~AbakusConfigureDia()
{
    
}


void AbakusConfigureDia::allocateWidgets()
{
    // option widgets
    lblAnimationTimestep = new QLabel;
    spinAnimationTimestep = new QSpinBox;
    spinAnimationTimestep->setMinimum(10);
    spinAnimationTimestep->setSingleStep(10);
    spinAnimationTimestep->setMaximum(30000);
    
    lblAddMinute = new QLabel;
    spinAddMinute = new QSpinBox;
    spinAddMinute->setSingleStep(1000);
    spinAddMinute->setMinimum(0);
    spinAddMinute->setMaximum(60000);
    spinAddMinute->setValue(60);
    
    btnJumpToTime = new QPushButton;
    lblTime   = new QLabel;
    spinTime  = new QTimeEdit(QTime::currentTime());
    // only show hours and mins
    spinTime->setDisplayFormat("hh:mm");
    // bottom bar
    btnApply = new QPushButton;
    btnApply->setDefault(TRUE);
}

void AbakusConfigureDia::createLayouts()
{
    // option layouts
    layoutAnimationTimestep = new QHBoxLayout;
    layoutAnimationTimestep->setMargin(0);
    layoutAnimationTimestep->addWidget(lblAnimationTimestep);
    layoutAnimationTimestep->addWidget(spinAnimationTimestep);
    layoutAddMinute = new QHBoxLayout;
    layoutAddMinute->setMargin(0);
    layoutAddMinute->addWidget(lblAddMinute);
    layoutAddMinute->addWidget(spinAddMinute);
    layoutTime = new QHBoxLayout;
    layoutTime->setMargin(0);
    layoutTime->addWidget(lblTime);
    layoutTime->addWidget(spinTime);
    layoutTime->addWidget(btnJumpToTime);
    // parent layout
    layoutParent = new QVBoxLayout;
    layoutParent->setMargin(1);
    layoutParent->addLayout(layoutTime);
    layoutParent->addLayout(layoutAnimationTimestep);
    layoutParent->addLayout(layoutAddMinute);
    layoutParent->addWidget(btnApply);
    setLayout(layoutParent);
}

void AbakusConfigureDia::connectSlots()
{
    connect(btnJumpToTime, SIGNAL(clicked()), this, SLOT(jumpToTime()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(applyChanges()));
    connect(this,    SIGNAL(accepted()), this, SLOT(applyChanges()));
}


void AbakusConfigureDia::retranslateUi()
{
    lblAnimationTimestep->setText(tr("Animationsdauer:"));
    lblAddMinute->setText(tr("Minute addieren:"));
    btnApply->setText(tr("Anwenden"));
    spinAddMinute->setSuffix(tr(" ms"));
    spinAddMinute->setSpecialValueText(tr("niemals"));
    spinAnimationTimestep->setSuffix(tr(" ms"));
    lblTime->setText(tr("Uhrzeit:"));
    btnJumpToTime->setText(tr("hinspringen"));
    setWindowTitle(tr("Abakus Uhr einrichten"));
}

void AbakusConfigureDia::applyChanges()
{
    if(!m_pClock){
        return;
    }
    m_pClock->setAnimationTimestep(spinAnimationTimestep->value());
    
    int addmin = spinAddMinute->value();
    if(addmin > 0 && addmin < 40)
    {
        spinAddMinute->setValue(40);
    }
    m_pClock->setAddingMinTimestep(spinAddMinute->value());
}


void AbakusConfigureDia::setClock(AbakusClock* clock, bool initToDefaults)
{
    m_pClock = clock;
    if(!m_pClock){
        return;
    }
    spinAnimationTimestep->setValue(m_pClock->animationTimestep());
    spinAddMinute->setValue(m_pClock->addingMinTimestep());
    
    if(initToDefaults){
        m_pClock->setTime(QTime::currentTime().hour(), QTime::currentTime().minute());
    }
}

void AbakusConfigureDia::jumpToTime()
{
    if(!m_pClock)
    {
        return;
    }
    int h, m;
    h = spinTime->time().hour();
    m = spinTime->time().minute();
    m_pClock->setTime(h, m);
}

