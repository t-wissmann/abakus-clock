
#include "abakusclock.h"

// system
#include <QTimer>
#include <QString>
#include <QSizePolicy>

// painter
#include <QPainter>


AbakusClock::AbakusClock()
    : QWidget()
{
    m_nHours = 0;
    m_nMins = 0;
    initGraphicalMembers();
    m_nAddingMinTimestep = -1;
    // init logical ball positions
    for(int i = 0; i < 20; ++i){
        m_rgBalls[i].m_nLogicalCol = (int)(i/5);
        m_rgBalls[i].m_nLogicalRow = i%4;
    }
    
    recomputeLogicalBallPositions(FALSE);
    computeRealFromLogicalBallPosition();
    // create timer for animation refresh
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveBallPositions()));
    timer->start(40);
    
    // set size policy
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    // set minimum size
    setMinimumSize(100, 150);
    
    // create timer for adding minutes every m_nAddingMinTimestep seconds
    m_tmrAddMinute = new QTimer(this);
    connect(m_tmrAddMinute, SIGNAL(timeout()), this, SLOT(addMinute()));
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
    for(int i = 0; i < (20); i++){
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
    
    // draw horizontal col axis
    for(int col = 0; col < 4; ++col){
        // use x coord of top ball of each col
        int x = m_rgBalls[col*5].currentPosition().x();
        // y is y of top ball - radius + margin
        int y = getYByRowNumber(0) - m_nBallDiameter/2 + m_nMargin;
        printAxis(painter, x, y,m_nBallspacing + (int)(m_nBallDiameter-m_nMargin)*2, m_nBallDiameter/10);
        
        // now the lower col axis
        // y is y of top ball - radius + margin
        y = getYByRowNumber(2) - m_nBallDiameter/2 + m_nMargin;
        printAxis(painter, x, y,m_nBallspacing*4 + (int)(m_nBallDiameter)*5-2*m_nMargin,
                  m_nBallDiameter/10);
    }
    
    for(int i = 0; i < 20; i++){
        paintBall(painter, m_rgBalls[i].currentPosition().x(),
            m_rgBalls[i].currentPosition().y(), (int)m_nBallDiameter);
        // draw label if wanted
        /*
        painter.drawText(m_rgBalls[i].currentPosition().x(),
                         m_rgBalls[i].currentPosition().y(),
                        QString::number(i));
        */
    }
    
    // paint middle bar separator
    int y = getYByRowNumber(1) + m_nBallDiameter/2+m_nMiddlebarheight/2;
    int x = m_rgBalls[5].currentPosition().x() + m_nBallDiameter/2 + m_nBallspacing;
    printAxis(painter, x, y, m_nBallDiameter/10,
              m_nBallspacing*3 + (int)(m_nBallDiameter)*4-2*m_nMargin
              );
}



void AbakusClock::paintBall(QPainter& painter, int centerX, int centerY, int height)
{
    QRadialGradient gradient(centerX - height/5, centerY - height/5, height/2);
    QColor color1 = palette().highlight().color().lighter(120);
    QColor color2 = palette().highlight().color();
    color1.setAlpha(200);
    color2.setAlpha(244);
    gradient.setColorAt(0, color1);
    gradient.setColorAt(1, color2);
    QBrush brush(gradient);
    painter.setBrush(brush);
    painter.drawEllipse(centerX-height/2, centerY-height/2, height, height);
}

void AbakusClock::printAxis(QPainter& painter, int x, int y,
                             int height, int width)
{
    painter.setBrush(palette().highlight());
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
                              col[i].currentPosition().x(),
                        getYByRowNumber(newValue)));
            }
            col[i].m_nLogicalRow = newValue;
        }
    }
}


void AbakusClock::computeRealFromLogicalBallPosition()
{
    float ballDiameterWidth = (width() - 2* m_nMargin - m_nBallspacing * 3) / 4;
    float ballDiameterHeight = (height() - m_nMiddlebarheight - 2* m_nMargin - m_nBallspacing * 6) / 7;
    m_nBallDiameter = (ballDiameterHeight < ballDiameterWidth) ?
            ballDiameterHeight : ballDiameterWidth;
    int offsetX = (int)(m_nBallDiameter/2) +(int)
            (width() - m_nBallspacing*3 - m_nBallDiameter*4 ) / 2;
    int offsetY = (int)(m_nBallDiameter/2) + (int)
            (height() - m_nBallspacing*6 - m_nMiddlebarheight - m_nBallDiameter*7 ) / 2;
    
    int x, y;
    for(int i = 0; i < 20; i++){
        
        x = offsetX + (int)(m_rgBalls[i].m_nLogicalCol * (m_nBallspacing + m_nBallDiameter));
        y = offsetY + (int)(m_rgBalls[i].m_nLogicalRow * (m_nBallspacing + m_nBallDiameter));
        if(m_rgBalls[i].m_nLogicalRow >= 2){
            // add middlebarheight for lower cols( rows 2 - 6);
            y += m_nMiddlebarheight;
        }
        
        m_rgBalls[i].setCurrentPosition(QPoint(x, y));
    }
}

void AbakusClock::addMinute()
{
    m_nMins++;
    m_nMins %= 60;
    if(m_nMins == 0)
    {
        m_nMins = 1;
        m_nHours++;
        m_nHours %= 60;
        if(m_nHours == 0){
            m_nHours = 1;
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
    float ballDiameterWidth = (width() - 2* m_nMargin - m_nBallspacing * 3) / 4;
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



void AbakusClock::setTime(char hours, char mins)
{
    m_nHours = hours;
    m_nMins = mins;
    // set time without animation
    recomputeLogicalBallPositions(FALSE);
    computeRealFromLogicalBallPosition();
}


void AbakusClock::setAnimationTimestep(int ms)
{
    for(int i = 0; i < 20; ++i){
        m_rgBalls[i].setAnimationTimestep(ms);
    }
}

int AbakusClock::animationTimestep() const
{
    return m_rgBalls[0].animationTimestep();
}



void AbakusClock::setAddingMinTimestep(int s)
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

int  AbakusClock::addingMinTimestep() const
{
    return m_nAddingMinTimestep;
}

