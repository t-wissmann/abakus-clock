
#ifndef __ABAKUSMAINWINDOW_H_
#define __ABAKUSMAINWINDOW_H_

#include <QWidget>

#define ABAKUS_MW_SUPERCLASS QWidget

// dialogs
class AbakusConfigureDia;
//widgets
class AbakusClock;
class QPushButton;
// layouts
class QHBoxLayout;
class QVBoxLayout;

class AbakusMainWindow : public QWidget
{
public:
    Q_OBJECT
public:
    AbakusMainWindow();
    virtual ~AbakusMainWindow();
    
    void retranslateUi();
public slots:
    void showConfigureDialog();
    void showAboutDialog();
    
private:
    void createGui();
    void connectSlots();
    
    // dialogs
    AbakusConfigureDia* diaConfigure;
    
    // widgets
    AbakusClock* wdgClock;
    QPushButton* btnQuit;
    QPushButton* btnAbout;
    QPushButton* btnConfigure;
    
    // layouts
    QHBoxLayout* layoutParent;
    QVBoxLayout* layoutToolButtons;
    
};

#endif
