
#ifndef __ABAKUSCLOCK_H_
#define __ABAKUSCLOCK_H_

#include <QWidget>
#include <QList>

#include "ballposition.h"

#define ABAKUS_COL_COUNT 6
#define ABAKUS_BALL_COUNT 30

#include <QTime>
#include <QPixmap>

class QTimer;
class QPaintEvent;
class QResizeEvent;
#include <stdio.h>

class ClockAppearance{
public:
    enum BallStyle{
        RadialGradient,
        Tango
    };
    
    ClockAppearance(){
        m_eStyle = RadialGradient;
        m_nBorderWidth = 1;
    }
    BallStyle m_eStyle;
    QColor m_cBallInnerColor;
    QColor m_cBallOuterColor;
    QColor m_cBorderColor;
    int    m_nBorderWidth;
    QColor m_cAxisFillColor;
    QColor m_cGlazeTop;
    QColor m_cGlazeMiddle;
    QColor m_cGlazeShadow1;
    QColor m_cGlazeShadow2;
    void writeTo(FILE* buf);
    void initFrom(FILE* pFile);
};

class AbakusClock : public QWidget{
    Q_OBJECT
public:
    AbakusClock();
    virtual ~AbakusClock();
    
    void recomputeLogicalBallPositions(bool animated = TRUE);
    void setLogicalBallPositionsInCol(BallPosition* col,
                            int colheight, int value, bool animated = TRUE);
    void computeRealFromLogicalBallPosition();
    int  getYByRowNumber(int row);
    void setTime(char hours, char mins, char secs = 0);
    void setAnimationTimestep(int ms);
    int animationTimestep() const;
    
    void setAddingTimestep(int s);
    int  addingTimestep() const;
    
    bool areSecondsVisible() const;
    
    void setClockAppearance(const ClockAppearance& appear);
    ClockAppearance clockAppearance() const;
    int FPS() const;
    void setFPS(int fps);
    void repaintBallTemplate();
    void repaintVAxisTemplate();
    void setShowFPS(bool on);
    bool fullyAnimated() const;
    int hmsSeparator() const;
    void swapBalls(int index1, int index2);
public slots:
    void moveBallPositions();
    void addMinute();
    void addSecond();
    void setSecondsVisible(bool visible);
    void repaintAllGuiTemplates();
    void setFullyAnimated(bool on);
    void setHmsSeparator(int value);
    void randomizeBalls(int ballsToSwap = ABAKUS_BALL_COUNT);
protected:
    virtual void resizeEvent(QResizeEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    virtual void paintBall(QPainter& painter, int centerX, int centerY, int height);
    virtual void printAxis(QPainter& painter, int x, int y, int height, int width = 4);
private:
    void initGraphicalMembers();
    
    int computeBallDiameterFromWidth();
    int computeOffsetX(int balldiameter);
    
    bool         m_bFullyAnimated;
    QTimer*      m_tmrAddMinute;
    bool         m_bShowSeconds;
    BallPosition m_rgBalls[ABAKUS_BALL_COUNT];
    char         m_nHours;
    char         m_nMins;
    char         m_nSecs;
    float        m_nBallDiameter;
    int          m_nHmsSeparator; // separator between hours and mins and between mins and secs
    int          m_nMargin;  // margin in px
    int          m_nBallspacing;
    int          m_nMiddlebarheight;
    // every m_nAddingMinTimestep milli secondes will be added 1 minute to current time
    // if m_nAddingMinTimestep <= 0, nothing will happen
    int          m_nAddingMinTimestep; 
    QTimer*      m_tmrRepaint;
    int          m_nFPS; // frequency for m_tmrRepaint
    ClockAppearance m_cClockAppearance;
    QTime       m_cPaintTime;
    int         m_nCounter;
    /// double buffer: pixmaps, needed in paintEvent();
    QPixmap     m_cCurrentFrame;
    QPixmap     m_cBallPic;
    QPixmap     m_cVAxisTemplate;
    bool        m_bShowFPS;
};

#endif
