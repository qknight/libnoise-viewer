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

#include <QDebug>
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

private Q_SLOTS:
    void renderingDone(renderJob);
    void jobStatusChanged();
Q_SIGNALS:
    void jobDoneSig(renderJob job);
    void jobStatusChangedSig();
};

#endif
