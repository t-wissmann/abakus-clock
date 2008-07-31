
#include "abakusclock.h"

// stdlib
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// system
#include <QTimer>
#include <QString>
#include <QSizePolicy>

// painter
#include <QPainter>
#include <QPen>
#include <QPicture>
#include <QImage>
#include <QRect>
#include <QRadialGradient>
#include <QLinearGradient>


AbakusClock::AbakusClock()
    : QWidget()
{
    m_nCounter = 0;
    m_bShowSeconds = TRUE;
    m_nHours = 0;
    m_nMins = 0;
    m_nSecs = 44;
    m_nFPS = 25;
    m_nHmsSeparator = 0;
    m_bFullyAnimated = TRUE;
    m_bShowFPS = FALSE; // don't show current fps in the top left corner per default
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
    m_tmrRepaint->start(1000/m_nFPS);
    
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
    repaintAllGuiTemplates();
}

#define __PrintPaintTimer if(0 && !(m_nCounter % 5)) qDebug("Time needed for paint: %d ms in line %d", m_cPaintTime.elapsed(), __LINE__);

void AbakusClock::paintEvent(QPaintEvent*)
{
    // paint
    //QPixmap image(width(), height());
    QPainter imagePainter(this);
    imagePainter.setRenderHint(QPainter::Antialiasing, TRUE);
    
    QPen   borderpen(m_cClockAppearance.m_cBorderColor);
    borderpen.setWidth(m_cClockAppearance.m_nBorderWidth);
    imagePainter.setPen(borderpen);
    
    // don't paint hidden balls
    int ballcount = m_bShowSeconds ?
            ABAKUS_BALL_COUNT : (ABAKUS_BALL_COUNT - 10);
    int colCount = m_bShowSeconds ?
            ABAKUS_COL_COUNT : (ABAKUS_COL_COUNT - 2);
    
    
    
    // paint middle bar separator
    {
        // reset pen and brush
        imagePainter.setPen(borderpen);
        imagePainter.setBrush(QBrush(m_cClockAppearance.m_cAxisFillColor));
        int y = getYByRowNumber(1) + m_nBallDiameter/2+m_nMiddlebarheight/2;
        int separatorwidth = m_nBallspacing*3 + (int)(m_nBallDiameter)*4-2*m_nMargin + m_nHmsSeparator;
        if(m_bShowSeconds)
        {
            separatorwidth += m_nBallspacing*2 + m_nBallDiameter*2 + m_nHmsSeparator;
        }
        int x = m_rgBalls[0].currentPosition().x()-m_nBallDiameter/2  + separatorwidth/2;
        printAxis(imagePainter, x, y, m_nBallDiameter/10, separatorwidth);
    }
    
    
    imagePainter.setPen(Qt::NoPen);
    imagePainter.setBrush(Qt::NoBrush);
    // draw vertical col axis
    {
        int axisPicWidth = m_cVAxisTemplate.width();
        int x;
        // set brush to axis background
        for(int col = 0; col < colCount; ++col){
            // use x coord of top ball of each col
            x = m_rgBalls[col*5].currentPosition().x() - axisPicWidth/2;
            // print axis from axis template pixmap
            imagePainter.drawPixmap(x, 0, m_cVAxisTemplate);
        }
    }
    
    // print all balls
    {
        int picwidth = m_cBallPic.width();
        QRectF source(0, 0, picwidth, picwidth);
        QRectF target(0, 0, picwidth, picwidth);
        int offset = picwidth/2;
        for(int i = 0; i < ballcount; i++){
            // now draw this picture for each ball
            target.moveTo(
                    m_rgBalls[i].currentPosition().x()-offset,
                    m_rgBalls[i].currentPosition().y()-offset);
            imagePainter.drawPixmap(target, m_cBallPic, source);
        }
    }
    
    // draw FPS if wanted
    if(m_bShowFPS)
    {
        QColor textColor = m_cClockAppearance.m_cBorderColor;
        int currentFps = 999;
        int elapsedTime = m_cPaintTime.elapsed();
        if(elapsedTime != 0)
        {
            currentFps = 1000 / elapsedTime;
        }
        textColor.setAlpha(255);
        
        QString text;
        text.sprintf("%4d FPS", currentFps);
        // set to fixed font
        QFont font("mono");
        imagePainter.setFont(font);
        imagePainter.setPen(QPen(textColor));
        imagePainter.drawText(3, 11, text);
    }
    // now all elements have been painted to image
    imagePainter.end();
    
    // paint image to widget
    /*
    {
        QPainter widgetPainter(this);
        QRectF rect(0, 0, width(), height());
        widgetPainter.drawPixmap(rect, image, rect);
    }*/
    
    // print debug output
    /// currently disabled
    /*
    m_nCounter = (m_nCounter+1) % 5; // reset counter
    if(!(m_nCounter % 5))
    {// only print every fivth item
        qDebug("Last repaint: %d ms ago in line %d", m_cPaintTime.elapsed(), __LINE__);
    }
    // restart timer 
    */
    m_cPaintTime.start();
}


void AbakusClock::repaintAllGuiTemplates()
{
    repaintBallTemplate();
    repaintVAxisTemplate();
}

void AbakusClock::repaintBallTemplate()
{
    // reallocate new memory for pixmap
    int picwidth = m_nBallDiameter+m_cClockAppearance.m_nBorderWidth*2 +2; // 2 px more for antialiasing space^^ ;D
    m_cBallPic = QPixmap(picwidth,picwidth);
    // clear picture
    m_cBallPic.fill(QColor(0, 0, 0, 0));
    // init painter
    QPainter ballpainter;
    ballpainter.begin(&m_cBallPic);
    ballpainter.setRenderHint(QPainter::Antialiasing);
    // draw general ball image
    paintBall(ballpainter, picwidth/2, picwidth/2,(int)m_nBallDiameter);
    // done..
    ballpainter.end();
}

void AbakusClock::repaintVAxisTemplate()
{
    // yTopCol is y of top ball - radius + margin
    int yTopCol = getYByRowNumber(0) - m_nBallDiameter/2 + m_nMargin;
    // yBottomCol is y of top ball - radius + margin
    int yBottomCol = getYByRowNumber(2) - m_nBallDiameter/2 + m_nMargin;
    int additionalHeightMargin = m_nBallDiameter/10;
    // init margins,,, coordinates etc..
    int upperRowHeight = m_nBallspacing
            + (int)(m_nBallDiameter-m_nMargin)*2;
    int lowerRowHeight = m_nBallspacing*4
            + (int)(m_nBallDiameter)*5-2*m_nMargin;
    upperRowHeight -= additionalHeightMargin*2;
    lowerRowHeight -= additionalHeightMargin*2;
    int axisWidth = m_nBallDiameter/10;
    int picWidth = axisWidth + m_cClockAppearance.m_nBorderWidth*2;
    int x = picWidth/2;
    
    // reallocate new momery for pixmap
    m_cVAxisTemplate = QPixmap(picWidth, height());
    // clear picture
    m_cVAxisTemplate.fill(QColor(0, 0, 0, 0));
    // init painter
    QPainter axisPainter;
    // set brush to axis background
    axisPainter.begin(&m_cVAxisTemplate);
    axisPainter.setRenderHint(QPainter::Antialiasing, TRUE);
    QPen   borderpen(m_cClockAppearance.m_cBorderColor);
    borderpen.setWidth(m_cClockAppearance.m_nBorderWidth);
    
    axisPainter.setPen(borderpen);
    axisPainter.setBrush(QBrush(m_cClockAppearance.m_cAxisFillColor));
    
    // paint both axis: lower and upper
    // at first upper
    printAxis(axisPainter, x, yTopCol+additionalHeightMargin, upperRowHeight, axisWidth);
    // now the lower col axis
    printAxis(axisPainter, x, yBottomCol+additionalHeightMargin, lowerRowHeight, axisWidth);
    // done..
    axisPainter.end();
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
            // set margin to bigger margin -> tango like ;D
            glazeMargin = m_cClockAppearance.m_nBorderWidth+1;
            
            int diameter = height - glazeMargin*2;
            QRadialGradient gradient(centerX, centerY, (int)height/2);
            double skipPosition = 0.8;
            if(height > 0 && diameter > 0)
            {
                skipPosition = ((double)diameter) / ((double)height);
            }
            
            gradient.setColorAt(0, m_cClockAppearance.m_cBallInnerColor);
            gradient.setColorAt(skipPosition-0.01, m_cClockAppearance.m_cBallInnerColor);
            gradient.setColorAt(skipPosition+0.01, m_cClockAppearance.m_cBallOuterColor);
            gradient.setColorAt(1, m_cClockAppearance.m_cBallOuterColor);
            painter.setBrush(QBrush(gradient));
            painter.drawEllipse(left, top, height, height);
            
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
    // paint glaze shadow
    if(m_cClockAppearance.m_cGlazeShadow1.alpha()
       || m_cClockAppearance.m_cGlazeShadow2.alpha())
        // if glaze.alpha() != 0 -> if glaze is visible
    {
        QLinearGradient gradient(centerX, centerY, centerX, centerY+height/2);
        gradient.setColorAt(0, m_cClockAppearance.m_cGlazeShadow1);
        gradient.setColorAt(1, m_cClockAppearance.m_cGlazeShadow2);
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        QRect rect(left+glazeMargin,
                   top+glazeMargin,
                   height-glazeMargin*2, height-glazeMargin*2);
        
        painter.drawChord(rect, 180*16, 180 *16);
    }
}

void AbakusClock::printAxis(QPainter& painter, int x, int y,
                             int height, int width)
{
    painter.drawRoundRect(x-width/2, y, width, height, 44, 44);
    //painter.drawRect(x-width/2, y, width, height);
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
                    col[0].targetPosition().x(),
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
                    col[i].targetPosition().x(), // dont change target of x value
                    getYByRowNumber(newValue)));  // y value
                
            }
            col[i].m_nLogicalRow = newValue;
        }
    }
}


void AbakusClock::computeRealFromLogicalBallPosition()
{
    // if seconds are shown, we have 6 cols,,,, else only 4 cols
    float ballDiameterWidth = (width() - 2* m_nMargin - m_nBallspacing * 3 - m_nHmsSeparator
            - (m_bShowSeconds ? (m_nBallspacing * 2+ m_nHmsSeparator) : 0)) / (m_bShowSeconds ? 6:4);
    float ballDiameterHeight = (height() - m_nMiddlebarheight - 2* m_nMargin - m_nBallspacing * 6) / 7;
    m_nBallDiameter = (ballDiameterHeight < ballDiameterWidth) ?
            ballDiameterHeight : ballDiameterWidth;
    int offsetX = computeOffsetX(m_nBallDiameter);
    int offsetY = (int)(m_nBallDiameter/2) + (int)
            (height() - m_nBallspacing*6 - m_nMiddlebarheight - m_nBallDiameter*7 ) / 2;
    
    int x, y;
    for(int i = 0; i < ABAKUS_BALL_COUNT; i++){
        if(i == 10)
        {
            // after all hour balls
            // add spacing between hours and mins
            offsetX += m_nHmsSeparator;
        }
        if(i == 20)
        {
            // after all hours and min balls
            // add spacing between mins and secs
            offsetX += m_nHmsSeparator;
        }
        x = offsetX + (int)(m_rgBalls[i].m_nLogicalCol * (m_nBallspacing + m_nBallDiameter));
        y = offsetY + (int)(m_rgBalls[i].m_nLogicalRow * (m_nBallspacing + m_nBallDiameter));
        if(m_rgBalls[i].m_nLogicalRow >= 2){
            // add middlebarheight for lower cols( rows 2 - 6);
            y += m_nMiddlebarheight;
        }
        
        if(m_bFullyAnimated)
        {
            m_rgBalls[i].moveTo(QPoint(x, y));
        }
        else
        {
            m_rgBalls[i].setCurrentPosition(QPoint(x, y));
        }
    }
}


int AbakusClock::computeBallDiameterFromWidth()
{
    // TODO
    return -1;
}

int AbakusClock::computeOffsetX(int balldiameter)
{
    int offsetX = width() / 2 - m_nHmsSeparator/2;
    if(m_bShowSeconds)
    {
        offsetX -= m_nHmsSeparator/2;
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
    float ballDiameterWidth = (width() - 2* m_nMargin - m_nBallspacing * 3 - m_nHmsSeparator
            - (m_bShowSeconds ? (m_nBallspacing * 2 + m_nHmsSeparator) : 0)) / (m_bShowSeconds ? 6:4);
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
    repaintAllGuiTemplates();
    update();
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
        int interval = 1000 / m_nFPS;
        m_tmrRepaint->start(interval);
    }
}


void AbakusClock::setShowFPS(bool on)
{
    m_bShowFPS = on;
}


void AbakusClock::setFullyAnimated(bool on)
{
    m_bFullyAnimated = on;
}

bool AbakusClock::fullyAnimated() const
{
    return m_bFullyAnimated;
}


int AbakusClock::hmsSeparator() const
{
    return m_nHmsSeparator;
}
void AbakusClock::setHmsSeparator(int value)
{
    if(m_nHmsSeparator == value)
    {
        // if nothing would be changed
        return;
    }
    m_nHmsSeparator = value;
    computeRealFromLogicalBallPosition();
    repaintAllGuiTemplates();
}


void AbakusClock::swapBalls(int index1, int index2)
{
    if(index1 < 0 || index1 > ABAKUS_BALL_COUNT)
    {
        return;
    }
    if(index2 < 0 || index2 > ABAKUS_BALL_COUNT)
    {
        return;
    }
    char tmpbuf[sizeof(BallPosition)];
    int size = sizeof(BallPosition);
    // copy to tmp buf
    memcpy(tmpbuf, m_rgBalls+index1, size);
    // write from index2 to index1
    memcpy(m_rgBalls+index1, m_rgBalls+index2, size);
    // restore index1 to mem of index2
    memcpy(m_rgBalls+index2, tmpbuf, size);
    // and now swap logial positions
    int logicalRow = m_rgBalls[index1].m_nLogicalRow;
    int logicalCol = m_rgBalls[index1].m_nLogicalCol;
    m_rgBalls[index1].m_nLogicalRow = m_rgBalls[index2].m_nLogicalRow;
    m_rgBalls[index1].m_nLogicalCol = m_rgBalls[index2].m_nLogicalCol;
    m_rgBalls[index2].m_nLogicalRow = logicalRow;
    m_rgBalls[index2].m_nLogicalCol = logicalCol;
    //qDebug("swapped %d with %d", index1, index2);
}

#define RAND_INDEX ((int)((double)ABAKUS_BALL_COUNT*rand()/(RAND_MAX+1.0)))

void AbakusClock::randomizeBalls(int ballsToSwap)
{
    for(int i = 0; i < ballsToSwap; ++i)
    {
        swapBalls(i%ABAKUS_BALL_COUNT, RAND_INDEX);
    }
    // now compute (new)real positions
    computeRealFromLogicalBallPosition();
    // and update
    update();
}


void ClockAppearance::writeTo(FILE* buf)
{
    fprintf(buf, "#clock appearance: DONT EDIT OR CHANGE ORDER OF LINES !\n");
    fprintf(buf, "#style: 0 RadialGradient, 1 Tango\n");
    fprintf(buf, "%d\n", (int)m_eStyle);
    fprintf(buf, "#ball color: inner: alpha, rgb\n");
    fprintf(buf, "%d, %s\n",
            m_cBallInnerColor.alpha(),
            m_cBallInnerColor.name().remove("#").toAscii().data());
    fprintf(buf, "#ball color: outer: alpha, rgb\n");
    fprintf(buf, "%d, %s\n",
            m_cBallOuterColor.alpha(),
            m_cBallOuterColor.name().remove("#").toAscii().data());
    fprintf(buf, "#border: width, alpha, rgb\n");
    fprintf(buf, "%d, %d, %s\n", m_nBorderWidth, m_cBorderColor.alpha(), m_cBorderColor.name().remove("#").toAscii().data());
    fprintf(buf, "#axis fill color: alpha, rgb\n");
    fprintf(buf, "%d, %s\n", m_cAxisFillColor.alpha(), m_cAxisFillColor.name().remove("#").toAscii().data());
    fprintf(buf, "#glaze on top: alpha, rgb\n");
    fprintf(buf, "%d, %s\n", m_cGlazeTop.alpha(), m_cGlazeTop.name().remove("#").toAscii().data());
    fprintf(buf, "#glaze in the middle: alpha, rgb\n");
    fprintf(buf, "%d, %s\n", m_cGlazeMiddle.alpha(), m_cGlazeMiddle.name().remove("#").toAscii().data());
    fprintf(buf, "#glaze shadow 1 (middle): alpha, rgb\n");
    fprintf(buf, "%d, %s\n", m_cGlazeShadow1.alpha(), m_cGlazeShadow1.name().remove("#").toAscii().data());
    fprintf(buf, "#glaze shadow 2 (bottom): alpha, rgb\n");
    fprintf(buf, "%d, %s\n", m_cGlazeShadow2.alpha(), m_cGlazeShadow2.name().remove("#").toAscii().data());
    
}

#define GET_NEXT_LINE(file,line) { \
    char*  readElements; \
    while(1) \
    { \
        readElements = fgets(line, 80, file); \
        if(readElements <= 0) \
        { \
            return; \
        } \
        /* get first char != ' ' */ \
        int pos = -1; \
        while(line[++pos] == ' '); \
        if(line[pos] != '#') \
        { \
            break; \
        } \
    }\
    /* remove \n at end of line */ \
    if(line[strlen(line)-1] == '\n') \
    { \
        line[strlen(line)-1] = '\0'; \
    }\
}

void ClockAppearance::initFrom(FILE* pFile)
{
    char szLine[80];
    char rgb[80];
    int  alpha;
    
    // stye
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d", (int*)&m_eStyle);
    // inner ball color
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cBallInnerColor.setNamedColor(QString('#') + QString(rgb));
    m_cBallInnerColor.setAlpha(alpha);
    // outer ball color
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cBallOuterColor.setNamedColor(QString('#') + QString(rgb));
    m_cBallOuterColor.setAlpha(alpha);
    
    // border color
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %d, %s\n",&m_nBorderWidth, &alpha, rgb);
    m_cBorderColor.setNamedColor(QString("#") + rgb);
    m_cBorderColor.setAlpha(alpha);
    // axis fill color
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cAxisFillColor.setNamedColor(QString("#") + rgb);
    m_cAxisFillColor.setAlpha(alpha);
    // glaze top
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cGlazeTop.setNamedColor(QString("#") + rgb);
    m_cGlazeTop.setAlpha(alpha);
    // glaze middle
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cGlazeMiddle.setNamedColor(QString("#") + rgb);
    m_cGlazeMiddle.setAlpha(alpha);
    // glaze shadow 1 (middle)
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cGlazeShadow1.setNamedColor(QString("#") + rgb);
    m_cGlazeShadow1.setAlpha(alpha);
    // glaze shadow 2 (bottom)
    GET_NEXT_LINE(pFile, szLine);
    sscanf(szLine, "%d, %s", &alpha, rgb);
    m_cGlazeShadow2.setNamedColor(QString("#") + rgb);
    m_cGlazeShadow2.setAlpha(alpha);
}



