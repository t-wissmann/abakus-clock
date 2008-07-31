#ifndef __ABAKUS_VERSION_H_
#define __ABAKUS_VERSION_H_


#define __ABAKUS_VERSION 1.0
#include <QtCore>

#ifdef Q_WS_WIN
#define __ABAKUS_CURRENTTHEMEPATH QApplication::applicationDirPath() + "/currentthemerc"
// simply in directory, where .exe is
#define __ABAKUS_USERTHEMEDIR QApplication::applicationDirPath()
void abakus_create_userthemedir() {
 // nothing to do on win
}
#endif

#ifdef Q_WS_X11
#include <QApplication>
#include <QDir>
#include <QStringList>
#include <QFile>
#define __ABAKUS_PREFIX "/usr"
#define __ABAKUS_USERTHEMEDIR QDir::homePath() + "/.abakus"
#define __ABAKUS_CURRENTTHEMEPATH __ABAKUS_USERTHEMEDIR + "/currentthemerc"
void abakus_create_userthemedir() {
    if(!QDir().exists(__ABAKUS_USERTHEMEDIR)) {
        QDir().mkpath(__ABAKUS_USERTHEMEDIR);
        QDir cGlobalThemeDir (QApplication::applicationDirPath());
        cGlobalThemeDir.cdUp(); 
        cGlobalThemeDir.cd("share");
        cGlobalThemeDir.cd("abakus_qt4");
        //qDebug("dir: %s", cGlobalThemeDir.path().toAscii().data());
        QString szDir = cGlobalThemeDir.path();
        QStringList rgEntries = cGlobalThemeDir.entryList(QDir::Files);
        for(int i = 0; i < rgEntries.size(); ++i) {
            qDebug("copiing %s to %s",
                (szDir + QDir::separator() + rgEntries[i]).toAscii().data(),
                (__ABAKUS_USERTHEMEDIR).toAscii().data());
            QFile::copy(szDir + QDir::separator() + rgEntries[i],
                       __ABAKUS_USERTHEMEDIR + QDir::separator() + rgEntries[i]);
        }
    }
}
#endif


#endif

