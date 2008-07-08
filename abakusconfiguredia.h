#ifndef __ABAKUSCONFIGUREDIA_H_
#define __ABAKUSCONFIGUREDIA_H_

#include <QDialog>

class AbakusClock;
// widgets
class QLabel;
class QPushButton;
class QSpinBox;
class QTimeEdit;
class QCheckBox;
class QGroupBox;
class ColorButton;
class QSlider;
class QComboBox;
class QListWidget;
// layouts
class QHBoxLayout;
class QVBoxLayout;
class QStackedLayout;
class ClockAppearance;


class AbakusConfigureDia : public QDialog
{
public:
    Q_OBJECT
public:
    AbakusConfigureDia(QWidget* parent = 0);
    ~AbakusConfigureDia();
    void retranslateUi();
    void setClock(AbakusClock* clock, bool initToDefaults = FALSE);
    void setToSystemColors(ClockAppearance* appear) const;
    void setAppearanceWidgets(ClockAppearance* appear); // loads values from appear to widgets in group box grpColors
public slots:
    void jumpToSystemTime();
    void applyChanges();
    void jumpToTime();
    void applyColorChanges();
    void resetDefaultColors();
    void colorWidgetChanged();
private:
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    // option widgets
    // animation
    QLabel*     lblAnimationTimestep;
    QSpinBox*   spinAnimationTimestep;
    QLabel*     lblAddSecond;
    QSpinBox*   spinAddSecond;
    QLabel*     lblFPS;
    QSpinBox*   spinFPS;
    // clock - time
    QLabel*     lblTime;
    QTimeEdit*  spinTime;
    QPushButton* btnJumpToTime;
    QPushButton* btnToSystemTime;
    QCheckBox*   chkShowSeconds;
    //colors
    QHBoxLayout* layoutColorToolButtons;
    QPushButton* btnApplyColors;
    QCheckBox*   chkColorsAutoApply;
    QPushButton* btnResetDefaultColors;
    QCheckBox*   chkCustomColors;
    QHBoxLayout* layoutBallStyle;
    QLabel*      lblBallStyle;
    QComboBox*   cmbBallStyle;
    QHBoxLayout* layoutColorBall;
    ColorButton* btnColorBallInner;
    ColorButton* btnColorBallOuter;
    QLabel*      lblColorBall;
    QHBoxLayout* layoutBallAlpha;
    QLabel*      lblBallAlpha;
    QSlider*     slidBallAlphaInner;
    QSlider*     slidBallAlphaOuter;
    QHBoxLayout* layoutColorBorder;
    QSlider*     slidColorBorderAlpha;
    ColorButton* btnColorBorder;
    QLabel*      lblColorBorder;
    QHBoxLayout* layoutBorderWidth;
    QLabel*      lblBorderWidth;
    QSlider*     slidBorderWidth;
    QHBoxLayout* layoutAxisFill;
    QLabel*      lblAxisFill;
    QSlider*     slidAxisFillAlpha;
    ColorButton* btnAxisFill;
    QHBoxLayout* layoutGlaze;
    QLabel*      lblGlaze;
    QSlider*     slidGlazeAlpha;
    ColorButton* btnGlaze;
    QHBoxLayout* layoutGlazeMiddle;
    QLabel*      lblGlazeMiddle;
    QSlider*     slidGlazeMiddleAlpha;
    ColorButton* btnGlazeMiddle;
    
    // widgets
    QPushButton* btnApply;
    // containers
    QListWidget*    lstStackControl;
    QStackedLayout* stackMain;
    QGroupBox*  grpBehavior;
    QGroupBox*  grpClockTime;
    QGroupBox*  grpColors;
    // layouts
    QVBoxLayout* layoutBehavior;
    QVBoxLayout* layoutClockTime;
    QVBoxLayout* layoutColors;
    QHBoxLayout* layoutAnimationTimestep;
    QHBoxLayout* layoutFPS;
    QHBoxLayout* layoutAddSecond;
    QHBoxLayout* layoutTime;
    QHBoxLayout* layoutParent;
    
    // members
    AbakusClock* m_pClock;
};


#endif
