
#include <QApplication>
#include "abakusmainwindow.h"

// not needed anymore (with an own main window)
/*
#include <QString>
#include <QTimer>
#include <QTime>
#include "abakusclock.h"
*/

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    AbakusMainWindow mw;
    
    mw.show();
    
    return app.exec();
}

// old main:
/*
int mainOLD(int argc, char* argv[]){
    QApplication app(argc, argv);
    AbakusClock wdg;
    
    if(argc > 1 && QString(argv[1]) == "-example")
    {
        int h = 19, m = 54;
        int timestep = 2000;
        if(argc > 4){
            h = abs(atoi(argv[2]));
            m = abs(atoi(argv[3]));
            timestep = abs(atoi(argv[4]))*100;
        }
        wdg.setTime(h, m);
        // tick tack
        QTimer *ticktack = new QTimer(&wdg);
        wdg.connect(ticktack, SIGNAL(timeout()), &wdg, SLOT(addMinute()));
        ticktack->start(timestep);
    }
    else
    {
        wdg.setTime(QTime::currentTime().hour(), QTime::currentTime().minute());
        // tick tack
        QTimer *ticktack = new QTimer(&wdg);
        wdg.connect(ticktack, SIGNAL(timeout()), &wdg, SLOT(addMinute()));
        ticktack->start(600000);
    }
    wdg.show();
    return app.exec();
}
*/



