
#include "abakusclock.h"

// system
#include <QTimer>
#include <QString>
#include <QSizePolicy>

// painter
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QRadialGradient>
#include <QLinearGradient>


AbakusClock::AbakusClock()
    : QWidget()
{
    m_bShowSeconds = TRUE;
    m_nHours = 0;
    m_nMins = 0;
    m_nSecs = 44;
    m_nFPS = 25;
    initGraphicalMembers();
    m_nAddingMinTimestep = -1;
    // init logical ball positions
    for(int i = 0; i < ABAKUS_BALL_COUNT; ++i){
        m_rgBalls[i].m_nLogicalCol = (int)(i/5);
        m_rgBalls[i].m_nLogicalRow = i%ABAKUS_COL_COUNT;
    }
    
    recomputeLogicalBallPositions(FALSE);
    computeRealFromLogicalBallPosition();
    // create timer for animation refresh
    m_tmrRepaint = new QTimer(this);
    connect(m_tmrRepaint, SIGNAL(timeout()), this, SLOT(moveBallPositions()));
    m_tmrRepaint->start(m_nFPS);
    
    // set size policy
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    // set minimum size
    setMinimumSize(100, 150);
    
    // create timer for adding minutes every m_nAddingMinTimestep seconds
    m_tmrAddMinute = new QTimer(this);
    connect(m_tmrAddMinute, SIGNAL(timeout()), this, SLOT(addSecond()));
}

AbakusClock::~AbakusClock()
{
    
}

void AbakusClock::initGraphicalMembers(){
    
    m_nMargin = 3;
    m_nBallspacing = 6;
    m_nMiddlebarheight = 20;
}


void AbakusClock::moveBallPositions()
{
    // here, all balles will get animated
    BallPosition* currentBall = m_rgBalls;
    bool hasToRepaint = FALSE;
    // don't animate hidden balls
    int ballcount = m_bShowSeconds ?
            ABAKUS_BALL_COUNT : (ABAKUS_BALL_COUNT - 10);
    for(int i = 0; i < ballcount; i++){
        currentBall = m_rgBalls + i;
        currentBall->updatePosition();
        if(!currentBall->animation().animationFinished()){
            hasToRepaint = TRUE;
        }
    }
    if(hasToRepaint)
    {
        update();
    }
}


void AbakusClock::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    computeRealFromLogicalBallPosition();
}

void AbakusClock::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen   borderpen(m_cClockAppearance.m_cBorderColor);
    borderpen.setWidth(m_cClockAppearance.m_nBorderWidth);
    painter.setPen(borderpen);
    
    // y is y of top ball - radius + margin
    int yTopCol = getYByRowNumber(0) - m_nBallDiameter/2 + m_nMargin;
    // y is y of top ball - radius + margin
    int yBottomCol = getYByRowNumber(2) - m_nBallDiameter/2 + m_nMargin;
    
    // don't paint hidden balls
    int ballcount = m_bShowSeconds ?
            ABAKUS_BALL_COUNT : (ABAKUS_BALL_COUNT - 10);
    int colCount = m_bShowSeconds ?
            ABAKUS_COL_COUNT : (ABAKUS_COL_COUNT - 2);
    
    // draw horizontal col axis
    for(int col = 0; col < colCount; ++col){
        // use x coord of top ball of each col
        int x = m_rgBalls[col*5].currentPosition().x();
        printAxis(painter, x, yTopCol, m_nBallspacing
            + (int)(m_nBallDiameter-m_nMargin)*2, m_nBallDiameter/10);
        
        // now the lower col axis
        printAxis(painter, x, yBottomCol,m_nBallspacing*4
                + (int)(m_nBallDiameter)*5-2*m_nMargin,
                  m_nBallDiameter/10);
    }
    
    for(int i = 0; i < ballcount; i++){
        paintBall(painter,
                m_rgBalls[i].currentPosition().x(),
                m_rgBalls[i].currentPosition().y(),
                (int)m_nBallDiameter);
    }
    
    // paint middle bar separator
    // reset pen
    painter.setPen(borderpen);
    int y = getYByRowNumber(1) + m_nBallDiameter/2+m_nMiddlebarheight/2;
    int separatorwidth = m_nBallspacing*3 + (int)(m_nBallDiameter)*4-2*m_nMargin;
    if(m_bShowSeconds)
    {
        separatorwidth += m_nBallspacing*2 + m_nBallDiameter*2;
    }
    int x = m_rgBalls[0].currentPosition().x()-m_nBallDiameter/2  + separatorwidth/2;
    printAxis(painter, x, y, m_nBallDiameter/10, separatorwidth);
}



void AbakusClock::paintBall(QPainter& painter, int centerX, int centerY, int height)
{
    int left = centerX-height/2;
    int top = centerY-height/2;
    int glazeMargin = m_cClockAppearance.m_nBorderWidth/2;
    // init brushes
    QPen   borderpen(m_cClockAppearance.m_cBorderColor);
    borderpen.setWidth(m_cClockAppearance.m_nBorderWidth);
    painter.setPen(borderpen);
    switch(m_cClockAppearance.m_eStyle){
        case ClockAppearance::RadialGradient: {
            // init gradient and brush
            QRadialGradient gradient(centerX - height/5, centerY - height/5, height/2);
            gradient.setColorAt(0, m_cClockAppearance.m_cBallInnerColor);
            gradient.setColorAt(1, m_cClockAppearance.m_cBallOuterColor);
            painter.setBrush(QBrush(gradient));
            // now paint
            painter.drawEllipse(left, top, height, height);
            break;
        }
        case ClockAppearance::Tango: {
            // set margin to bigger margin
            glazeMargin = m_cClockAppearance.m_nBorderWidth + 1;
            // init brush for outer color
            painter.setBrush(QBrush(m_cClockAppearance.m_cBallOuterColor));
            painter.drawEllipse(left, top, height, height);
            // init gradient for inner color
            QLinearGradient gradient(left, top, left+height, top+height );
            gradient.setColorAt(0, m_cClockAppearance.m_cBallInnerColor);
            gradient.setColorAt(1, m_cClockAppearance.m_cBallInnerColor.darker(150));
            painter.setPen(Qt::NoPen); // disable pen
            painter.setBrush(QBrush(gradient));
            int diameter = height - glazeMargin*2;
            painter.drawEllipse(centerX-diameter/2, centerY-diameter/2,
                                diameter, diameter);
            
            break;
        }
    }
    
    
    // paint glaze
    if(m_cClockAppearance.m_cGlazeTop.alpha()
       || m_cClockAppearance.m_cGlazeMiddle.alpha())
        // if glaze.alpha() != 0 -> if glaze is visible
    {
        QLinearGradient gradient(centerX, top, centerX, centerY);
        gradient.setColorAt(0, m_cClockAppearance.m_cGlazeTop);
        gradient.setColorAt(1, m_cClockAppearance.m_cGlazeMiddle);
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        QRect rect(left+glazeMargin,
                   top+glazeMargin,
                   height-glazeMargin*2, height-glazeMargin*2);
        
        painter.drawChord(rect, 0*16, 180 *16);
    }
}

void AbakusClock::printAxis(QPainter& painter, int x, int y,
                             int height, int width)
{
    painter.setBrush(QBrush(m_cClockAppearance.m_cAxisFillColor));
    painter.drawRoundRect(x-width/2, y, width, height, 44, 44);
}


void AbakusClock::recomputeLogicalBallPositions(bool animated)
{
    BallPosition* currentCol = m_rgBalls;
    // hours
    // 7 rows -> 2 spaces -> 5 balls
    setLogicalBallPositionsInCol(currentCol,    1, (int)((m_nHours/10)/5), animated);
    setLogicalBallPositionsInCol(currentCol+=1, 4, (m_nHours/10)%5, animated);
    setLogicalBallPositionsInCol(currentCol+=4, 1, (int)((m_nHours%10)/5), animated);
    // actually it would be (m_nHours % 10)% 5 instead of (m_nHours)%5
    // but %10 ist reduntant because of % 5
    setLogicalBallPositionsInCol(currentCol+=1, 4, (m_nHours)%5, animated);
    
    // mins
    setLogicalBallPositionsInCol(currentCol+=4, 1, (int)((m_nMins/10)/5), animated);
    setLogicalBallPositionsInCol(currentCol+=1, 4, (m_nMins/10)%5, animated);
    setLogicalBallPositionsInCol(currentCol+=4, 1, (int)((m_nMins%10)/5), animated);
    setLogicalBallPositionsInCol(currentCol+=1, 4, (m_nMins)%5, animated);
    
    // secs
    setLogicalBallPositionsInCol(currentCol+=4, 1, (int)((m_nSecs/10)/5), animated);
    setLogicalBallPositionsInCol(currentCol+=1, 4, (m_nSecs/10)%5, animated);
    setLogicalBallPositionsInCol(currentCol+=4, 1, (int)((m_nSecs%10)/5), animated);
    setLogicalBallPositionsInCol(currentCol+=1, 4, (m_nSecs)%5, animated);
    
}


void AbakusClock::setLogicalBallPositionsInCol(BallPosition* col, int colheight, int value,
                                               bool animated)
{
    if(colheight == 1)
    {
        int newValue;
        if(value == 1)
        {
            newValue = 1;
        }
        else // value == 0
        {
            newValue = 0;
        }
        if(col[0].m_nLogicalRow != newValue){
            col[0].m_nLogicalRow = newValue;
            if(animated)
            {
                col[0].moveTo(QPoint(
                    col[0].currentPosition().x(),
                    getYByRowNumber(newValue)));
            }
        }
        
    }else{
        for(int i = 0; i < colheight; ++i){
            int newValue;
            // 2: offset for upper cols
            // +1 wether ball is below or above space
            if(i < value)
            {
                newValue = 2 + i;
            }else
            {
                newValue = 2 + i + 1;
            }
            if(animated && (col[i].m_nLogicalRow != newValue))
            {
                col[i].moveTo(QPoint(
                        col[i].currentPosition().x(), // x value
                        getYByRowNumber(newValue)));  // y value
            }
            col[i].m_nLogicalRow = newValue;
        }
    }
}


void AbakusClock::computeRealFromLogicalBallPosition()
{
    // if seconds are shown, we have 6 cols,,,, else only 4 cols
    float ballDiameterWidth = (width() - 2* m_nMargin - m_nBallspacing * 3
            - (m_bShowSeconds ? (m_nBallspacing * 2) : 0)) / (m_bShowSeconds ? 6:4);
    float ballDiameterHeight = (height() - m_nMiddlebarheight - 2* m_nMargin - m_nBallspacing * 6) / 7;
    m_nBallDiameter = (ballDiameterHeight < ballDiameterWidth) ?
            ballDiameterHeight : ballDiameterWidth;
    int offsetX = computeOffsetX(m_nBallDiameter);
    int offsetY = (int)(m_nBallDiameter/2) + (int)
            (height() - m_nBallspacing*6 - m_nMiddlebarheight - m_nBallDiameter*7 ) / 2;
    
    int x, y;
    for(int i = 0; i < ABAKUS_BALL_COUNT; i++){
        
        x = offsetX + (int)(m_rgBalls[i].m_nLogicalCol * (m_nBallspacing + m_nBallDiameter));
        y = offsetY + (int)(m_rgBalls[i].m_nLogicalRow * (m_nBallspacing + m_nBallDiameter));
        if(m_rgBalls[i].m_nLogicalRow >= 2){
            // add middlebarheight for lower cols( rows 2 - 6);
            y += m_nMiddlebarheight;
        }
        
        m_rgBalls[i].setCurrentPosition(QPoint(x, y));
    }
}


int AbakusClock::computeBallDiameterFromWidth()
{
    // TODO
    return -1;
}

int AbakusClock::computeOffsetX(int balldiameter)
{
    int offsetX = width() / 2;
    if(m_bShowSeconds)
    {
        offsetX -= (m_nBallspacing*5 + balldiameter*6 ) / 2;
    }
    else
    {
        offsetX -= (m_nBallspacing*3 + balldiameter*4 ) / 2;
    }
    offsetX += (int)(balldiameter/2);
    return offsetX;
}


void AbakusClock::addMinute()
{
    m_nMins++;
    m_nMins %= 60;
    if(m_nMins == 0)
    {
        m_nHours++;
        m_nHours %= 24;
        if(m_nHours < 0){
            m_nHours = 0;
        }
    }
    bool animated = TRUE;
    recomputeLogicalBallPositions(animated);
    if(!animated){
        computeRealFromLogicalBallPosition();
    }
    update();
}
void AbakusClock::addSecond()
{
    m_nSecs++;
    m_nSecs %= 60;
    if(m_nSecs == 0)
    {
        m_nMins++;
        m_nMins %= 60;
        if(m_nMins == 0)
        {
            m_nHours++;
            m_nHours %= 24;
            if(m_nHours < 0){
                m_nHours = 0;
            }
        }
    }
    bool animated = TRUE;
    recomputeLogicalBallPositions(animated);
    if(!animated){
        computeRealFromLogicalBallPosition();
    }
    update();
}

int  AbakusClock::getYByRowNumber(int row)
{
    // if seconds are shown, we have 6 cols,,,, else only 4 cols
    float ballDiameterWidth = (width() - 2* m_nMargin - m_nBallspacing * 3
            - (m_bShowSeconds ? (m_nBallspacing * 2) : 0)) / (m_bShowSeconds ? 6:4);
    float ballDiameterHeight = (height() - m_nMiddlebarheight - 2* m_nMargin - m_nBallspacing * 6) / 7;
    m_nBallDiameter = (ballDiameterHeight < ballDiameterWidth) ?
            ballDiameterHeight : ballDiameterWidth;
    int offsetY = (int)(m_nBallDiameter/2) + (int)
            (height() - m_nBallspacing*6 - m_nMiddlebarheight - m_nBallDiameter*7 ) / 2;
    int y = offsetY+(int)((m_nBallspacing+m_nBallDiameter)*row);
    if(row >= 2){
        // add middlebarheight for lower cols( rows 2 - 6);
        y += m_nMiddlebarheight;
    }
    return y;
}



void AbakusClock::setTime(char hours, char mins, char secs)
{
    m_nHours = hours;
    m_nMins = mins;
    m_nSecs = secs;
    // set time without animation
    recomputeLogicalBallPositions(FALSE);
    computeRealFromLogicalBallPosition();
}


void AbakusClock::setAnimationTimestep(int ms)
{
    for(int i = 0; i < ABAKUS_BALL_COUNT; ++i){
        m_rgBalls[i].setAnimationTimestep(ms);
    }
}

int AbakusClock::animationTimestep() const
{
    return m_rgBalls[0].animationTimestep();
}



void AbakusClock::setAddingTimestep(int s)
{
    if(m_nAddingMinTimestep == s)
    {
        // if nothing would change,
        // there is no need to reset the timer
        return;
    }
    m_nAddingMinTimestep = s;
    m_tmrAddMinute->stop();
    if(m_nAddingMinTimestep > 0)
    {
        m_tmrAddMinute->start(m_nAddingMinTimestep);
    }
}

int  AbakusClock::addingTimestep() const
{
    return m_nAddingMinTimestep;
}


void AbakusClock::setSecondsVisible(bool visible)
{
    if(m_bShowSeconds == visible)
    {
        return;
    }
    m_bShowSeconds = visible;
    // recompute positions
    computeRealFromLogicalBallPosition();
    update();
}

bool AbakusClock::areSecondsVisible() const
{
    return m_bShowSeconds;
}


void AbakusClock::setClockAppearance(const ClockAppearance& appear)
{
    m_cClockAppearance = appear;
}

ClockAppearance AbakusClock::clockAppearance() const
{
    return m_cClockAppearance;
}

int AbakusClock::FPS() const
{
    return m_nFPS;
}

void AbakusClock::setFPS(int fps)
{
    
    if(fps == m_nFPS){
        // do nothing, if nothing would change
        return;
    }
    m_nFPS = fps;
    m_tmrRepaint->stop();
    if(m_nFPS > 0)
    { // only (re-)start on valid m_nFPS
        m_tmrRepaint->start(1000 / m_nFPS);
    }
}

