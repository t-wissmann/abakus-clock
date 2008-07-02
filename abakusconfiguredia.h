#ifndef __ABAKUSCONFIGUREDIA_H_
#define __ABAKUSCONFIGUREDIA_H_

#include <QDialog>

class AbakusClock;
// widgets
class QLabel;
class QPushButton;
class QSpinBox;
class QTimeEdit;
// layouts
class QHBoxLayout;
class QVBoxLayout;


class AbakusConfigureDia : public QDialog
{
public:
    Q_OBJECT
public:
    AbakusConfigureDia(QWidget* parent = 0);
    ~AbakusConfigureDia();
    void retranslateUi();
    void setClock(AbakusClock* clock, bool initToDefaults = FALSE);
public slots:
    void applyChanges();
    void jumpToTime();
    
private:
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    // option widgets
    QLabel*     lblAnimationTimestep;
    QSpinBox*   spinAnimationTimestep;
    QLabel*     lblAddMinute;
    QSpinBox*   spinAddMinute;
    QLabel*     lblTime;
    QTimeEdit*  spinTime;
    QPushButton* btnJumpToTime;
    
    // widgets
    QPushButton* btnApply;
    // layouts
    QHBoxLayout* layoutAnimationTimestep;
    QHBoxLayout* layoutAddMinute;
    QHBoxLayout* layoutTime;
    QVBoxLayout* layoutParent;
    
    // members
    AbakusClock* m_pClock;
};


#endif
