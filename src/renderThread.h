//
// C++ Interface: renderThread
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QByteArray>
#include "renderJob.h"
#include "noiseutils.h"
#include <QMetaType>
#include "module/invert.h"

class renderThread : public QThread {
  Q_OBJECT

  private:
    QMutex mutex;
    QWaitCondition condition;
    bool restart;
    bool abort;
    renderJob job;

  protected:
    void run();

  public:
    renderThread(QObject *parent = 0);
    void render(renderJob job);
    ~renderThread();
    renderJob generateRawImage (renderJob job);

  signals:
    void renderingDoneSig(renderJob job);

};

#endif
