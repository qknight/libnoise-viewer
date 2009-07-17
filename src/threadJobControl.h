//
// C++ Interface: threadJobControl
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef THREADJOBCONTROL_H
#define THREADJOBCONTROL_H

#include <QVector>
#include <QObject>

#include "renderThread.h"
#include "renderJob.h"

class threadJobControl : public QObject {
Q_OBJECT

  public:
    threadJobControl();
    ~threadJobControl();
    void queueJob(renderJob job);
    void clearJobs();
  private:
    void processJob(renderJob);
    QVector<renderThread*> renderThreads;
    QVector<renderJob> jobs;
    int runningJobs;

  private slots:
    void renderingDone(renderJob);
    void jobStatusChanged();
  signals:
     void jobDoneSig(renderJob job);
     void jobStatusChangedSig();

};

#endif
