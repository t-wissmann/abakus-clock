
#ifndef __ABAKUSMAINWINDOW_H_
#define __ABAKUSMAINWINDOW_H_

#include <QWidget>

#define ABAKUS_MW_SUPERCLASS QWidget

// dialogs
class AbakusConfigureDia;
//widgets
class AbakusClock;
class QPushButton;
class QLabel;
class QTextEdit;
// layouts
class QHBoxLayout;
class QVBoxLayout;
// other
class QKeyEvent;

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
    void showHelpDialog();
    
    void toggleHeaderEdit(bool visible);
    void writeHeaderInputToLabel();
protected:
    virtual void keyPressEvent(QKeyEvent* event);
    
private:
    void createGui();
    void initWidgets();
    void connectSlots();
    
    // dialogs
    AbakusConfigureDia* diaConfigure;
    
    // widgets
    AbakusClock* wdgClock;
    QPushButton* btnQuit;
    QPushButton* btnEditHeader;
    QPushButton* btnAbout;
    QPushButton* btnHelp;
    QPushButton* btnConfigure;
    // widgets in header
    QLabel*      lblHeader;
    QTextEdit*   txtHeaderInput;
    QPushButton* btnHeaderConfirm;
    
    // layouts
    QVBoxLayout* layoutParent;
    QHBoxLayout* layoutHeader;
    QHBoxLayout* layoutCenter;
    QVBoxLayout* layoutToolButtons;
    
    
    bool         m_bIsFullscreen;
    
};

#endif
