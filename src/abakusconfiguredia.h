#ifndef __ABAKUSCONFIGUREDIA_H_
#define __ABAKUSCONFIGUREDIA_H_

#include <QDialog>

class AbakusClock;
class ClockAppearance;
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
class QSplitter;



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
    static void setToColorsFromPalette(ClockAppearance* appear, const QPalette& pal);
    void setAppearanceWidgets(ClockAppearance* appear); // loads values from appear to widgets in group box grpColors
    void setWindowOptions(QWidget* window);
public slots:
    void jumpToSystemTime();
    void applyChanges();
    void jumpToTime();
    void applyColorChanges();
    void resetDefaultColors();
    void colorWidgetChanged();
    void applyWindowOptions();
    void generatePaletteFromCustomColor();
    void writeThemeToDefaultFile();
    void writeThemeToFile(QString filepath);
    void loadDefaultFileToTheme();
    void loadFileToTheme(QString filepath, ClockAppearance* app, bool printErrors = TRUE);
    void refreshThemeList();
    void createNewTheme();
    void saveSelectedTheme();
    void loadSelectedTheme();
    void deleteSelectedTheme();
    void autoLoadThemeIfWanted();
    void randomizeBalls();
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
    QPushButton* btnApply;
    QCheckBox*  chkFullyAnimated;
    QLabel*     lblHmsSeparator;
    QSpinBox*   spinHmsSeparator;
    QPushButton* btnRandomizeBalls;
    
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
    QPushButton* btnGeneratePalette;
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
    QHBoxLayout* layoutGlazeShadow1;
    QLabel*      lblGlazeShadow1;
    QSlider*     slidGlazeShadow1Alpha;
    ColorButton* btnGlazeShadow1;
    QHBoxLayout* layoutGlazeShadow2;
    QLabel*      lblGlazeShadow2;
    QSlider*     slidGlazeShadow2Alpha;
    ColorButton* btnGlazeShadow2;
    // Window options
    QPushButton* btnApplyWindowOptions;
    QVBoxLayout* layoutWindowOptions;
    QHBoxLayout* layoutBackgroundColor;
    QLabel*      lblBackgroundColor;
    ColorButton* btnBackgroundColor;
    // themes
    QListWidget* lstThemes;
    QHBoxLayout* layoutThemes;
    QVBoxLayout* layoutThemeButtons;
    QPushButton* btnSaveTheme;
    QPushButton* btnNewTheme;
    QPushButton* btnLoadTheme;
    QPushButton* btnDeleteTheme;
    QCheckBox*   chkThemeAutoLoad;
    // containers
    QListWidget*    lstStackControl;
    QStackedLayout* stackMain;
    QGroupBox*  grpBehavior;
    QGroupBox*  grpClockTime;
    QGroupBox*  grpColors;
    QGroupBox*  grpWindowOptions;
    QGroupBox*  grpThemes;
    // layouts
    QVBoxLayout* layoutBehavior;
    QVBoxLayout* layoutClockTime;
    QVBoxLayout* layoutColors;
    QHBoxLayout* layoutAnimationTimestep;
    QHBoxLayout* layoutFPS;
    QHBoxLayout* layoutAddSecond;
    QHBoxLayout* layoutTime;
    QHBoxLayout* layoutHmsSeparator;
    QSplitter*   splitterMain;
    QHBoxLayout* layoutParent;
    
    // members
    QString      m_szThemeExtension;
    AbakusClock* m_pClock;
    QWidget*     m_pCurrentWindow;
    QColor       m_cLastBasicColor; // last color, that was selected, when
                                    // btnGeneratePalette was clicked to regenerate palette
};


#endif
