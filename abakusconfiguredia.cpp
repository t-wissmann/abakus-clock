#include "abakusconfiguredia.h"
#include "abakusclock.h"

#include <QApplication>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidgetItem>
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
    m_cLastBasicColor = palette().highlight().color();
    m_szThemeExtension = ".clocktheme";
    // create GUI
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
    // hide/close if app gets quit
    setAttribute(Qt::WA_QuitOnClose, FALSE);
    setWindowOptions(parent);
    // reload theme list
    refreshThemeList();
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
    
    lblHmsSeparator = new QLabel;
    spinHmsSeparator = new QSpinBox;
    spinHmsSeparator->setRange(0, 100);
    spinHmsSeparator->setSingleStep(1);
    
    chkFullyAnimated = new QCheckBox;
    
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
    chkColorsAutoApply->setChecked(TRUE);
    btnGeneratePalette = new QPushButton;
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
    
    // window options
    btnApplyWindowOptions = new QPushButton;
    lblBackgroundColor = new QLabel;
    btnBackgroundColor = new ColorButton;
    
    // themes
    lstThemes = new QListWidget;
    btnSaveTheme = new QPushButton;
    btnNewTheme = new QPushButton;
    btnLoadTheme = new QPushButton;
    btnDeleteTheme = new QPushButton;
    chkThemeAutoLoad = new QCheckBox;
    chkThemeAutoLoad->setChecked(TRUE);
    
    // container 
    grpBehavior = new QGroupBox;
    grpBehavior->setFlat(TRUE);
    grpClockTime = new QGroupBox;
    grpClockTime->setFlat(TRUE);
    grpColors = new QGroupBox;
    grpColors->setFlat(TRUE);
    grpWindowOptions = new QGroupBox;
    grpWindowOptions->setFlat(TRUE);
    grpThemes = new QGroupBox;
    grpThemes->setFlat(TRUE);
    lstStackControl = new QListWidget;
    lstStackControl->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
    lstStackControl->setMinimumSize(120, 100);
    
    lstStackControl->addItem("Clock");
    lstStackControl->addItem("Behavior");
    lstStackControl->addItem("Look'n'Feel");
    lstStackControl->addItem("Window options");
    lstStackControl->addItem("Themes");
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
    // HMS Separator
    layoutHmsSeparator = new QHBoxLayout;
    layoutHmsSeparator->setMargin(0);
    layoutHmsSeparator->addWidget(lblHmsSeparator);
    layoutHmsSeparator->addWidget(spinHmsSeparator);
    
    layoutBehavior = new QVBoxLayout;
    layoutBehavior->addLayout(layoutAnimationTimestep);
    layoutBehavior->addLayout(layoutAddSecond);
    layoutBehavior->addLayout(layoutFPS);
    layoutBehavior->addLayout(layoutHmsSeparator);
    layoutBehavior->addWidget(chkFullyAnimated);
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
    layoutColors->addWidget(btnGeneratePalette);
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
    
    // WINDOW OPTIONS
    layoutBackgroundColor = new QHBoxLayout;
    layoutBackgroundColor->setMargin(0);
    layoutBackgroundColor->addWidget(lblBackgroundColor);
    layoutBackgroundColor->addWidget(btnBackgroundColor);
    
    layoutWindowOptions = new QVBoxLayout;
    layoutWindowOptions->addLayout(layoutBackgroundColor);
    layoutWindowOptions->addWidget(btnApplyWindowOptions);
    grpWindowOptions->setLayout(layoutWindowOptions);
    
    // themes
    layoutThemeButtons = new QVBoxLayout;
    layoutThemeButtons->setMargin(0);
    layoutThemeButtons->addStretch();
    layoutThemeButtons->addWidget(btnNewTheme);
    layoutThemeButtons->addWidget(btnLoadTheme);
    layoutThemeButtons->addWidget(btnSaveTheme);
    layoutThemeButtons->addWidget(btnDeleteTheme);
    layoutThemeButtons->addWidget(chkThemeAutoLoad);
    layoutThemeButtons->addStretch();
    
    layoutThemes = new QHBoxLayout;
    layoutThemes->addWidget(lstThemes);
    layoutThemes->addLayout(layoutThemeButtons);
    grpThemes->setLayout(layoutThemes);
    
    stackMain = new QStackedLayout;
    stackMain->addWidget(grpClockTime);
    stackMain->addWidget(grpBehavior);
    stackMain->addWidget(grpColors);
    stackMain->addWidget(grpWindowOptions);
    stackMain->addWidget(grpThemes);
    
    
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
    // write current theme to file
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(writeThemeToDefaultFile()));
    // for gui
    connect(btnJumpToTime, SIGNAL(clicked()), this, SLOT(jumpToTime()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(applyChanges()));
    connect(this,    SIGNAL(accepted()), this, SLOT(applyChanges()));
    connect(btnToSystemTime, SIGNAL(clicked()), this, SLOT(jumpToSystemTime()));
    connect(btnApplyColors, SIGNAL(clicked()), this, SLOT(applyColorChanges()));
    connect(btnResetDefaultColors, SIGNAL(clicked()), this, SLOT(resetDefaultColors()));
    connect(lstStackControl, SIGNAL(currentRowChanged(int)), stackMain, SLOT(setCurrentIndex(int)));
    connect(btnApplyWindowOptions, SIGNAL(clicked()), this, SLOT(applyWindowOptions()));
    connect(btnGeneratePalette, SIGNAL(clicked()), this, SLOT(generatePaletteFromCustomColor()));
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
    // for themes
    connect(btnNewTheme, SIGNAL(clicked()), this, SLOT(createNewTheme()));
    connect(btnSaveTheme, SIGNAL(clicked()), this, SLOT(saveSelectedTheme()));
    connect(btnLoadTheme, SIGNAL(clicked()), this, SLOT(loadSelectedTheme()));
    connect(btnDeleteTheme, SIGNAL(clicked()), this, SLOT(deleteSelectedTheme()));
    connect(chkThemeAutoLoad, SIGNAL(toggled(bool)), this, SLOT(autoLoadThemeIfWanted()));
    connect(lstThemes, SIGNAL(currentRowChanged(int)), this, SLOT(autoLoadThemeIfWanted()));
}


void AbakusConfigureDia::retranslateUi()
{
    setWindowTitle(tr("Abakus Uhr einrichten"));
    grpBehavior->setTitle(tr("Verhalten"));
    grpClockTime->setTitle(tr("Uhrzeit"));
    grpColors->setTitle(tr("Erscheinungsbild"));
    grpWindowOptions->setTitle(tr("Fensteroptionen"));
    grpThemes->setTitle(tr("Themes"));
    // behavior
    lblAnimationTimestep->setText(tr("Animationsdauer:"));
    lblAddSecond->setText(tr("Sekunde addieren:"));
    btnApply->setText(tr("Anwenden"));
    spinAddSecond->setSuffix(tr(" ms"));
    spinAddSecond->setSpecialValueText(tr("niemals"));
    spinAnimationTimestep->setSuffix(tr(" ms"));
    chkFullyAnimated->setText(tr("Vollst%auml;ndig animiert").replace("%auml;", QChar(0x00E4))); // = unicode for &auml;
    // fps
    lblFPS->setText(tr("Aktualisierungsrate:"));
    spinFPS->setSuffix(tr(" FPS"));
    spinFPS->setSpecialValueText(tr("ausgeschaltet"));
    // hms separator
    lblHmsSeparator->setText(tr("Trennlinie zwischen\nH-M bzw M-S:"));
    spinHmsSeparator->setSuffix(tr(" Pixel"));
    // time
    lblTime->setText(tr("Zur Uhrzeit"));
    btnJumpToTime->setText(tr("springen"));
    chkShowSeconds->setText(tr("Sekunden anzeigen"));
    btnToSystemTime->setText(tr("Zur Systemzeit springen"));
    // colors
    chkCustomColors->setText(tr("Benutzerdefinierte Farben"));
    chkColorsAutoApply->setText(tr("Automatisches Anwenden"));
    btnResetDefaultColors->setText(tr("Voreinstellungen"));
    btnGeneratePalette->setText(tr("Palette aus Farbe generieren..."));
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
    // Window options
    btnApplyWindowOptions->setText(tr("Anwenden"));
    lblBackgroundColor->setText(tr("Hintergrundfarbe:"));
    // themes
    btnNewTheme->setText(tr("Neu"));
    btnSaveTheme->setText(tr("Speichern"));
    btnDeleteTheme->setText(tr("Entfernen"));
    btnLoadTheme->setText(tr("Laden"));
    chkThemeAutoLoad->setText(tr("Automatisch\nLaden"));
    
    // list on the left
    QListWidgetItem* item; int i = 0;
    item = lstStackControl->item(i++);
    if(item) item->setText(tr("Uhrzeit"));
    item = lstStackControl->item(i++); // get next item
    if(item) item->setText(tr("Verhalten"));
    item = lstStackControl->item(i++); // get next item
    if(item) item->setText(tr("Erscheinungsbild"));
    item = lstStackControl->item(i++); // get next item
    if(item) item->setText(tr("Fensteroptionen"));
    item = lstStackControl->item(i++); // get next item
    if(item) item->setText(tr("Themes"));
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
    m_pClock->setFullyAnimated(chkFullyAnimated->isChecked());
    m_pClock->setAddingTimestep(spinAddSecond->value());
    m_pClock->setFPS(spinFPS->value());
    m_pClock->setHmsSeparator(spinHmsSeparator->value());
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
    chkFullyAnimated->setChecked(m_pClock->fullyAnimated());
    spinHmsSeparator->setValue(m_pClock->hmsSeparator());
    
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
    // load current theme
    loadDefaultFileToTheme();
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
    m_pClock->repaintAllGuiTemplates();
    m_pClock->update();
}


void AbakusConfigureDia::setToSystemColors(ClockAppearance* appear) const
{
    QPalette currentPalette = palette();
    // set palette to active palette
    currentPalette.setCurrentColorGroup(QPalette::Active);
    setToColorsFromPalette(appear, currentPalette);
}

void AbakusConfigureDia::setToColorsFromPalette(ClockAppearance* appear, const QPalette& pal)
{
    if(!appear)
    {
        return;
    }
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


void AbakusConfigureDia::generatePaletteFromCustomColor()
{
    QColor basiccolor = QColorDialog::getColor(m_cLastBasicColor, this);
    if(!basiccolor.isValid())
    {
        return;
    }
    m_cLastBasicColor = basiccolor; // set just selected color to new m_cLastBasicColor
    QPalette pal = palette();
    // generate new palette from basiccolor
    pal.setColor(QPalette::Highlight, basiccolor);
    if(basiccolor.value() < 60) // if basiccolor ist very dark
    {// then set to light border
        pal.setColor(QPalette::WindowText, QColor(255, 255, 255));
    }
    else // else set to dark border
    {
        pal.setColor(QPalette::WindowText, QColor(33, 33, 33));
    }
    // apply colors to clock appearance
    ClockAppearance app;
    setToColorsFromPalette(&app, pal);
    // backup value, if chkColorsAutoApply is checked
    bool autoApply = chkColorsAutoApply->isChecked();
    // uncheck, so that there isn't autoupdate for each value
    chkColorsAutoApply->setChecked(FALSE);
    // apply new colors to widgets
    setAppearanceWidgets(&app);
    // restore, if chkColorsAutoApply was checked
    chkColorsAutoApply->setChecked(autoApply);
    // done ;-)
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


void AbakusConfigureDia::applyWindowOptions()
{
    if(!m_pCurrentWindow){
        return;
    }
    QPalette pal = palette();
    pal.setColor(QPalette::Window, btnBackgroundColor->color());
    m_pCurrentWindow->setPalette(pal);
}

void AbakusConfigureDia::setWindowOptions(QWidget* window)
{
    m_pCurrentWindow = window;
    if(!m_pCurrentWindow){
        return;
    }
    QPalette pal = window->palette();
    btnBackgroundColor->setColor(pal.window().color());
}


void AbakusConfigureDia::writeThemeToDefaultFile()
{
    writeThemeToFile(QApplication::applicationDirPath() + "/currentthemerc");
}

void AbakusConfigureDia::writeThemeToFile(QString filepath)
{
    //qDebug("saving to %s", filepath.toLocal8Bit().data());
    FILE* pFile = fopen(filepath.toLocal8Bit().data(), "w");
    if(!pFile)
    {
         QMessageBox::critical(this, "Fehler beim speichern einer Datei",
                               tr("Fehler beim speichern der datei %file").replace("%file", filepath));
         return;
    }
    if(m_pClock)
    {
        ClockAppearance app = m_pClock->clockAppearance();
        app.writeTo(pFile);
    }
    fclose(pFile);
}


void AbakusConfigureDia::loadDefaultFileToTheme()
{
    if(m_pClock)
    {
        ClockAppearance app = m_pClock->clockAppearance();
        loadFileToTheme(QApplication::applicationDirPath() + "/currentthemerc", &app, FALSE);
        m_pClock->setClockAppearance(app);
        setAppearanceWidgets(&app);
    }
}

void AbakusConfigureDia::loadFileToTheme(QString filepath, ClockAppearance* app, bool printErrors)
{
    //qDebug("saving to %s", filepath.toLocal8Bit().data());
    FILE* pFile = fopen(filepath.toLocal8Bit().data(), "r");
    if(!pFile)
    {
        if(printErrors)
        {
            QMessageBox::critical(this, "Fehler beim laden einer Datei",
                              tr("Fehler beim laden der datei %file").replace("%file", filepath));
        }
        return;
    }
    if(app)
    {
        app->initFrom(pFile);
    }
    fclose(pFile);
}


void AbakusConfigureDia::refreshThemeList()
{
    QDir dir(QApplication::applicationDirPath());
    int extensionlength = m_szThemeExtension.length();
    QStringList themes = dir.entryList(QStringList() << ("*" + m_szThemeExtension),
                                       QDir::Readable | QDir::Files, QDir::Name);
    for(int i = 0; i < themes.size(); ++i)
    {
        //remove extensions
        themes[i] = themes[i].left(themes[i].length() - extensionlength);
    }
    lstThemes->clear();
    lstThemes->addItems(themes);
}

void AbakusConfigureDia::createNewTheme()
{
    QString filename = QInputDialog::getText(this, "Neues Theme erstellen", "Geben sie den Namen des neuen Themes ein:");
    filename.remove(m_szThemeExtension);
    if(filename.isEmpty())
    {
        // if CANCEL was clicked
        return;
    }
    writeThemeToFile(filename + m_szThemeExtension);
    refreshThemeList();
}

void AbakusConfigureDia::saveSelectedTheme()
{
    QListWidgetItem* selectedItem = lstThemes->currentItem();
    if(selectedItem)
    {
        // if a theme is selected
        writeThemeToFile(selectedItem->text() + m_szThemeExtension);
    }
}


void AbakusConfigureDia::autoLoadThemeIfWanted()
{
    if(chkThemeAutoLoad->isChecked())
    {
        loadSelectedTheme();
    }
}

void AbakusConfigureDia::loadSelectedTheme()
{
    QListWidgetItem* selectedItem = lstThemes->currentItem();
    if(selectedItem && m_pClock)
    {
        ClockAppearance app = m_pClock->clockAppearance();
        // if a theme is selected
        loadFileToTheme(selectedItem->text() + m_szThemeExtension, &app);
        m_pClock->setClockAppearance(app);
        setAppearanceWidgets(&app);
    }
}

void AbakusConfigureDia::deleteSelectedTheme()
{
    QDir dir(QApplication::applicationDirPath());
    QListWidgetItem* selectedItem = lstThemes->currentItem();
    if(selectedItem)
    {
        // if a theme is selected
        dir.remove(selectedItem->text() + m_szThemeExtension);
    }
    refreshThemeList();
}

