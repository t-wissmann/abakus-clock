#include "abakusconfiguredia.h"
#include "abakusclock.h"

// widgets
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QTimeEdit>
#include "colorbutton.h"
#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QSplitter>

// dialogs
#include <QColorDialog>

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
    spinAnimationTimestep->setValue(1000);
    
    lblAddSecond = new QLabel;
    spinAddSecond = new QSpinBox;
    spinAddSecond->setSingleStep(1000);
    spinAddSecond->setMinimum(0);
    spinAddSecond->setMaximum(60000);
    spinAddSecond->setValue(60);
    
    lblFPS = new QLabel;
    spinFPS = new QSpinBox;
    spinFPS->setSingleStep(1);
    spinFPS->setRange(0, 500);
    
    // clock
    btnJumpToTime = new QPushButton;
    lblTime   = new QLabel;
    spinTime  = new QTimeEdit(QTime::currentTime());
    btnToSystemTime = new QPushButton;
    
    chkShowSeconds = new QCheckBox;
    // only show hours and mins
    spinTime->setDisplayFormat("hh:mm:ss");
    // bottom bar
    btnApply = new QPushButton;
    btnApply->setDefault(TRUE);
    
    // colors
    chkCustomColors = new QCheckBox;
    chkCustomColors->setChecked(TRUE);
    btnApplyColors = new QPushButton;
    btnResetDefaultColors = new QPushButton;
    chkColorsAutoApply = new QCheckBox;
    // style
    lblBallStyle = new QLabel;
    cmbBallStyle = new QComboBox;
    // border
    lblColorBorder = new QLabel;
    slidColorBorderAlpha = new QSlider(Qt::Horizontal);
    slidColorBorderAlpha->setRange(0, 255);
    btnColorBorder = new ColorButton;
    lblBorderWidth = new QLabel;
    slidBorderWidth = new QSlider(Qt::Horizontal);
    slidBorderWidth ->setRange(0, 30);
    // ball inner
    lblColorBall = new QLabel;
    btnColorBallInner = new ColorButton;
    btnColorBallOuter = new ColorButton;
    lblBallAlpha = new QLabel;
    slidBallAlphaInner = new QSlider(Qt::Horizontal);
    slidBallAlphaInner->setRange(0, 255); // alpha range
    slidBallAlphaInner->setValue(200);
    slidBallAlphaOuter = new QSlider(Qt::Horizontal);
    slidBallAlphaOuter->setRange(0, 255); // alpha range
    slidBallAlphaOuter->setValue(244);
    // axis
    lblAxisFill = new QLabel;
    slidAxisFillAlpha = new QSlider(Qt::Horizontal);
    slidAxisFillAlpha->setRange(0, 255); // alpha range
    slidAxisFillAlpha->setValue(255);
    btnAxisFill = new ColorButton;
    // glaze (top)
    lblGlaze = new QLabel;
    slidGlazeAlpha = new QSlider(Qt::Horizontal);
    slidGlazeAlpha->setRange(0, 255); // alpha range
    btnGlaze = new ColorButton;
    // glaze (middle)
    lblGlazeMiddle = new QLabel;
    slidGlazeMiddleAlpha = new QSlider(Qt::Horizontal);
    slidGlazeMiddleAlpha->setRange(0, 255); // alpha range
    btnGlazeMiddle = new ColorButton;
    // glaze shadow 1
    lblGlazeShadow1 = new QLabel;
    slidGlazeShadow1Alpha = new QSlider(Qt::Horizontal);
    slidGlazeShadow1Alpha->setRange(0, 255); // alpha range
    btnGlazeShadow1 = new ColorButton;
    // glaze shadow 2
    lblGlazeShadow2 = new QLabel;
    slidGlazeShadow2Alpha = new QSlider(Qt::Horizontal);
    slidGlazeShadow2Alpha->setRange(0, 255); // alpha range
    btnGlazeShadow2 = new ColorButton;
    
    // container 
    grpBehavior = new QGroupBox;
    grpClockTime = new QGroupBox;
    grpColors = new QGroupBox;
    lstStackControl = new QListWidget;
    lstStackControl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
    lstStackControl->setMinimumSize(120, 100);
    
    lstStackControl->addItem("Clock");
    lstStackControl->addItem("Behavior");
    lstStackControl->addItem("Look'n'Feel");
}

void AbakusConfigureDia::createLayouts()
{
    // option layouts
    // BEHAVIOR    
    layoutAnimationTimestep = new QHBoxLayout;
    layoutAnimationTimestep->setMargin(0);
    layoutAnimationTimestep->addWidget(lblAnimationTimestep);
    layoutAnimationTimestep->addWidget(spinAnimationTimestep);
    layoutAddSecond = new QHBoxLayout;
    layoutAddSecond->setMargin(0);
    layoutAddSecond->addWidget(lblAddSecond);
    layoutAddSecond->addWidget(spinAddSecond);
    // fps.
    layoutFPS = new QHBoxLayout;
    layoutFPS->setMargin(0);
    layoutFPS->addWidget(lblFPS);
    layoutFPS->addWidget(spinFPS);
    
    layoutBehavior = new QVBoxLayout;
    layoutBehavior->addLayout(layoutAnimationTimestep);
    layoutBehavior->addLayout(layoutAddSecond);
    layoutBehavior->addLayout(layoutFPS);
    layoutBehavior->addWidget(btnApply);
    
    grpBehavior->setLayout(layoutBehavior);
    
    // TIME
    layoutTime = new QHBoxLayout;
    layoutTime->setMargin(0);
    layoutTime->addWidget(lblTime);
    layoutTime->addWidget(spinTime);
    layoutTime->addWidget(btnJumpToTime);
    
    layoutClockTime = new QVBoxLayout;
    layoutClockTime->addWidget(chkShowSeconds);
    layoutClockTime->addLayout(layoutTime);
    layoutClockTime->addWidget(btnToSystemTime);
    grpClockTime->setLayout(layoutClockTime);
    
    // COLORS
    layoutColorBall = new QHBoxLayout;
    layoutColorBall->setMargin(0);
    layoutColorBall->addWidget(lblColorBall);
    layoutColorBall->addWidget(btnColorBallInner);
    layoutColorBall->addWidget(btnColorBallOuter);
    // ball style
    layoutBallStyle = new QHBoxLayout;
    layoutBallStyle->setMargin(0);
    layoutBallStyle->addWidget(lblBallStyle);
    layoutBallStyle->addWidget(cmbBallStyle);
    // border
    layoutColorBorder = new QHBoxLayout;
    layoutColorBorder->setMargin(0);
    layoutColorBorder->addWidget(lblColorBorder);
    layoutColorBorder->addWidget(slidColorBorderAlpha);
    layoutColorBorder->addWidget(btnColorBorder);
    layoutBorderWidth = new QHBoxLayout;
    layoutBorderWidth->setMargin(0);
    layoutBorderWidth->addWidget(lblBorderWidth);
    layoutBorderWidth->addWidget(slidBorderWidth);
    // alpha
    layoutBallAlpha = new QHBoxLayout;
    layoutBallAlpha->setMargin(0);
    layoutBallAlpha->addWidget(lblBallAlpha);
    layoutBallAlpha->addWidget(slidBallAlphaInner);
    layoutBallAlpha->addWidget(slidBallAlphaOuter);
    // axis fill
    layoutAxisFill = new QHBoxLayout;
    layoutAxisFill->setMargin(0);
    layoutAxisFill->addWidget(lblAxisFill);
    layoutAxisFill->addWidget(slidAxisFillAlpha);
    layoutAxisFill->addWidget(btnAxisFill);
    // glaze
    layoutGlaze = new QHBoxLayout;
    layoutGlaze->setMargin(0);
    layoutGlaze->addWidget(lblGlaze);
    layoutGlaze->addWidget(slidGlazeAlpha);
    layoutGlaze->addWidget(btnGlaze);
    layoutGlazeMiddle = new QHBoxLayout;
    layoutGlazeMiddle->setMargin(0);
    layoutGlazeMiddle->addWidget(lblGlazeMiddle);
    layoutGlazeMiddle->addWidget(slidGlazeMiddleAlpha);
    layoutGlazeMiddle->addWidget(btnGlazeMiddle);
    // glaze shadow
    layoutGlazeShadow1 = new QHBoxLayout;
    layoutGlazeShadow1->setMargin(0);
    layoutGlazeShadow1->addWidget(lblGlazeShadow1);
    layoutGlazeShadow1->addWidget(slidGlazeShadow1Alpha);
    layoutGlazeShadow1->addWidget(btnGlazeShadow1);
    layoutGlazeShadow2 = new QHBoxLayout;
    layoutGlazeShadow2->setMargin(0);
    layoutGlazeShadow2->addWidget(lblGlazeShadow2);
    layoutGlazeShadow2->addWidget(slidGlazeShadow2Alpha);
    layoutGlazeShadow2->addWidget(btnGlazeShadow2);
    
    // "tool" buttons // buttons on the bottom - like Apply and ResetDefaults
    layoutColorToolButtons = new QHBoxLayout;
    layoutColorToolButtons->setMargin(0);
    layoutColorToolButtons->addWidget(btnResetDefaultColors);
    layoutColorToolButtons->addWidget(btnApplyColors);
    
    layoutColors = new QVBoxLayout;
    layoutColors->addWidget(chkCustomColors);
    layoutColors->addLayout(layoutBallStyle);
    layoutColors->addLayout(layoutColorBall);
    layoutColors->addLayout(layoutBallAlpha);
    layoutColors->addLayout(layoutColorBorder);
    layoutColors->addLayout(layoutBorderWidth);
    layoutColors->addLayout(layoutAxisFill);
    layoutColors->addLayout(layoutGlaze);
    layoutColors->addLayout(layoutGlazeMiddle);
    layoutColors->addLayout(layoutGlazeShadow1);
    layoutColors->addLayout(layoutGlazeShadow2);
    layoutColors->addWidget(chkColorsAutoApply);
    layoutColors->addLayout(layoutColorToolButtons);
    
    grpColors->setLayout(layoutColors);
    
    stackMain = new QStackedLayout;
    stackMain->addWidget(grpClockTime);
    stackMain->addWidget(grpBehavior);
    stackMain->addWidget(grpColors);
    
    // main splitter
    splitterMain = new QSplitter;
    splitterMain->addWidget(lstStackControl);
    QWidget* wdgFoo = new QWidget;// layouts can't be added to splitters
    wdgFoo->setLayout(stackMain); // so we need to cheat: add layout to widget
    splitterMain->addWidget(wdgFoo); // and then add this widget
    
    // parent layout
    layoutParent = new QHBoxLayout;
    layoutParent->setMargin(4);
    layoutParent->addWidget(splitterMain);
    setLayout(layoutParent);
    // resize to minimum -> lstStackControl gets resized smaller
    resize(22, 22);
}

void AbakusConfigureDia::connectSlots()
{
    connect(btnJumpToTime, SIGNAL(clicked()), this, SLOT(jumpToTime()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(applyChanges()));
    connect(this,    SIGNAL(accepted()), this, SLOT(applyChanges()));
    connect(btnToSystemTime, SIGNAL(clicked()), this, SLOT(jumpToSystemTime()));
    connect(btnApplyColors, SIGNAL(clicked()), this, SLOT(applyColorChanges()));
    connect(btnResetDefaultColors, SIGNAL(clicked()), this, SLOT(resetDefaultColors()));
    connect(lstStackControl, SIGNAL(currentRowChanged(int)), stackMain, SLOT(setCurrentIndex(int)));
    // connections for auto-color changes
    connect(chkColorsAutoApply, SIGNAL(toggled(bool)), this, SLOT(colorWidgetChanged()));
    connect(cmbBallStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(chkCustomColors, SIGNAL(toggled(bool)), this, SLOT(colorWidgetChanged()));
    connect(btnColorBallInner, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(btnColorBallOuter, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidBallAlphaInner, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(slidBallAlphaOuter, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(slidColorBorderAlpha, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(btnColorBorder, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidBorderWidth, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(btnAxisFill, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidAxisFillAlpha, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(btnGlaze, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidGlazeAlpha, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(btnGlazeMiddle, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidGlazeMiddleAlpha, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(btnGlazeShadow1, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidGlazeShadow1Alpha, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
    connect(btnGlazeShadow2, SIGNAL(colorChanged(QColor)), this, SLOT(colorWidgetChanged()));
    connect(slidGlazeShadow2Alpha, SIGNAL(valueChanged(int)), this, SLOT(colorWidgetChanged()));
}


void AbakusConfigureDia::retranslateUi()
{
    setWindowTitle(tr("Abakus Uhr einrichten"));
    grpBehavior->setTitle(tr("Verhalten"));
    grpClockTime->setTitle(tr("Uhrzeit"));
    grpColors->setTitle(tr("Erscheinungsbild"));
    // behavior
    lblAnimationTimestep->setText(tr("Animationsdauer:"));
    lblAddSecond->setText(tr("Sekunde addieren:"));
    btnApply->setText(tr("Anwenden"));
    spinAddSecond->setSuffix(tr(" ms"));
    spinAddSecond->setSpecialValueText(tr("niemals"));
    spinAnimationTimestep->setSuffix(tr(" ms"));
    // fps
    lblFPS->setText(tr("Aktualisierungsrate:"));
    spinFPS->setSuffix(tr(" FPS"));
    spinFPS->setSpecialValueText(tr("ausgeschaltet"));
    // time
    lblTime->setText(tr("Zur Uhrzeit"));
    btnJumpToTime->setText(tr("springen"));
    chkShowSeconds->setText(tr("Sekunden anzeigen"));
    btnToSystemTime->setText(tr("Zur Systemzeit springen"));
    // colors
    chkCustomColors->setText(tr("Benutzerdefinierte Farben"));
    chkColorsAutoApply->setText(tr("Automatisches Anwenden"));
    btnResetDefaultColors->setText(tr("Voreinstellungen"));
    btnApplyColors->setText(tr("Anwenden"));
    lblBallStyle->setText(tr("Aussehen:"));
    cmbBallStyle->clear();
    cmbBallStyle->insertItem(ClockAppearance::RadialGradient, "Radialer Farbverlauf");
    cmbBallStyle->insertItem(ClockAppearance::Tango,          "Tango");
    lblColorBall->setText(tr("Ballfarbe Mitte/Rand:"));
    lblColorBorder->setText(tr("Rahmenfarbe:"));
    lblBorderWidth->setText(tr("Rahmendicke:"));
    lblBallAlpha->setText(tr("Alpha Mitte/Rand:"));
    lblAxisFill->setText(tr("Achsenfarbe:"));
    lblGlaze->setText(tr("Glanz der Kugeln oben:"));
    lblGlazeMiddle->setText(tr("Glanz der Kugeln mittig:"));
    lblGlazeShadow1->setText(tr("Schatten des Glanzes mittig:"));
    lblGlazeShadow2->setText(tr("Schatten des Glanzes unten:"));
    QListWidgetItem* item;
    item = lstStackControl->item(0);
    if(item) item->setText(tr("Uhrzeit"));
    item = lstStackControl->item(1);
    if(item) item->setText(tr("Verhalten"));
    item = lstStackControl->item(2);
    if(item) item->setText(tr("Erscheinungsbild"));
}

void AbakusConfigureDia::applyChanges()
{
    if(!m_pClock){
        return;
    }
    m_pClock->setAnimationTimestep(spinAnimationTimestep->value());
    
    int addmin = spinAddSecond->value();
    if(addmin > 0 && addmin < 40)
    {
        spinAddSecond->setValue(40);
    }
    m_pClock->setAddingTimestep(spinAddSecond->value());
    m_pClock->setFPS(spinFPS->value());
}


void AbakusConfigureDia::setClock(AbakusClock* clock, bool initToDefaults)
{
    if(m_pClock)
    {
        // disconnect old connection
        disconnect(chkShowSeconds, SIGNAL(toggled(bool)),
                   m_pClock, SLOT(setSecondsVisible(bool)));
    }
    m_pClock = clock;
    if(!m_pClock){
        return;
    }
    spinAnimationTimestep->setValue(m_pClock->animationTimestep());
    spinAddSecond->setValue(m_pClock->addingTimestep());
    spinFPS->setValue(m_pClock->FPS());
    chkShowSeconds->setChecked(m_pClock->areSecondsVisible());
    connect(chkShowSeconds, SIGNAL(toggled(bool)),
            m_pClock, SLOT(setSecondsVisible(bool)));
    
    
    if(initToDefaults){
        m_pClock->setTime(QTime::currentTime().hour(),
                          QTime::currentTime().minute(),
                          QTime::currentTime().second());
        spinAddSecond->setValue(1000);
        m_pClock->setAddingTimestep(spinAddSecond->value());
        ClockAppearance appear;
        setToSystemColors(&appear);
        m_pClock->setClockAppearance(appear);
    }
    // load colors
    ClockAppearance app = m_pClock->clockAppearance();
    // reset widget values
    setAppearanceWidgets(&app);
}

void AbakusConfigureDia::setAppearanceWidgets(ClockAppearance* appear)
{
    btnColorBallInner->setColor(appear->m_cBallInnerColor);
    btnColorBallOuter->setColor(appear->m_cBallOuterColor);
    slidBallAlphaInner->setValue(appear->m_cBallInnerColor.alpha());
    slidBallAlphaOuter->setValue(appear->m_cBallOuterColor.alpha());
    btnColorBorder->setColor(appear->m_cBorderColor);
    slidBorderWidth->setValue(appear->m_nBorderWidth);
    slidColorBorderAlpha->setValue(appear->m_cBorderColor.alpha());
    btnAxisFill->setColor(appear->m_cAxisFillColor);
    slidAxisFillAlpha->setValue(appear->m_cAxisFillColor.alpha());
    btnGlaze->setColor(appear->m_cGlazeTop);
    slidGlazeAlpha->setValue(appear->m_cGlazeTop.alpha());
    btnGlazeMiddle->setColor(appear->m_cGlazeMiddle);
    slidGlazeMiddleAlpha->setValue(appear->m_cGlazeMiddle.alpha());
    btnGlazeShadow1->setColor(appear->m_cGlazeShadow1);
    slidGlazeShadow1Alpha->setValue(appear->m_cGlazeShadow1.alpha());
    btnGlazeShadow2->setColor(appear->m_cGlazeShadow2);
    slidGlazeShadow2Alpha->setValue(appear->m_cGlazeShadow2.alpha());
    cmbBallStyle->setCurrentIndex(appear->m_eStyle);
}

void AbakusConfigureDia::jumpToTime()
{
    if(!m_pClock)
    {
        return;
    }
    int h, m, s;
    h = spinTime->time().hour();
    m = spinTime->time().minute();
    s = spinTime->time().second();
    m_pClock->setTime(h, m, s);
}
void AbakusConfigureDia::jumpToSystemTime()
{
    spinTime->setTime(QTime::currentTime());
    jumpToTime();
}

void AbakusConfigureDia::applyColorChanges()
{
    if(!m_pClock)
    {
        return;
    }
    ClockAppearance app;
    if(chkCustomColors->isChecked())
    {
        app.m_cBallOuterColor = btnColorBallOuter->color();
        app.m_cBallInnerColor = btnColorBallInner->color();
        app.m_cBorderColor    = btnColorBorder->color();
        app.m_cBorderColor.setAlpha(slidColorBorderAlpha->value());
        app.m_nBorderWidth    = slidBorderWidth->value();
        app.m_cBallOuterColor.setAlpha(slidBallAlphaOuter->value());
        app.m_cBallInnerColor.setAlpha(slidBallAlphaInner->value());
        app.m_cAxisFillColor  = btnAxisFill->color();
        app.m_cAxisFillColor.setAlpha(slidAxisFillAlpha->value());
        app.m_cGlazeTop  = btnGlaze->color();
        app.m_cGlazeTop.setAlpha(slidGlazeAlpha->value());
        app.m_cGlazeMiddle  = btnGlazeMiddle->color();
        app.m_cGlazeMiddle.setAlpha(slidGlazeMiddleAlpha->value());
        app.m_cGlazeShadow1  = btnGlazeShadow1->color();
        app.m_cGlazeShadow1.setAlpha(slidGlazeShadow1Alpha->value());
        app.m_cGlazeShadow2  = btnGlazeShadow2->color();
        app.m_cGlazeShadow2.setAlpha(slidGlazeShadow2Alpha->value());
        app.m_eStyle = (ClockAppearance::BallStyle)cmbBallStyle->currentIndex();
    }
    else
    {
        setToSystemColors(&app);
    }
    m_pClock->setClockAppearance(app);
    m_pClock->update();
}


void AbakusConfigureDia::setToSystemColors(ClockAppearance* appear) const
{
    if(!appear)
    {
        return;
    }
    QPalette pal = palette();
    pal.setCurrentColorGroup(QPalette::Active);
    appear->m_eStyle        = ClockAppearance::RadialGradient;
    appear->m_cBorderColor = pal.color(QPalette::WindowText);
    appear->m_cBorderColor.setAlpha(50);
    appear->m_nBorderWidth = 2;
    appear->m_cBallOuterColor = pal.highlight().color();
    appear->m_cBallOuterColor.setAlpha(244);
    appear->m_cBallInnerColor = pal.highlight().color().lighter(140);
    appear->m_cBallInnerColor.setAlpha(200);
    appear->m_cAxisFillColor = pal.highlight().color();
    appear->m_cAxisFillColor.setAlpha(180);
    appear->m_cGlazeTop    = QColor( 255, 255, 255, 20);
    appear->m_cGlazeMiddle = QColor( 255, 255, 255, 80);
    appear->m_cGlazeShadow1 = QColor( 255, 255, 255, 20);
    appear->m_cGlazeShadow2 = pal.highlight().color().darker(190);
    appear->m_cGlazeShadow2.setAlpha(200);
}



void AbakusConfigureDia::resetDefaultColors()
{
    if(!m_pClock)
    {
        return;
    }
    ClockAppearance appear;
    setToSystemColors(&appear);
    // reset widget values
    setAppearanceWidgets(&appear);
}


void AbakusConfigureDia::colorWidgetChanged()
{
    if(chkColorsAutoApply->isChecked())
    {
        applyColorChanges();
    }
}



