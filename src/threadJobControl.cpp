//
// C++ Implementation: threadJobControl
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "threadJobControl.h"


threadJobControl::threadJobControl() {
  runningJobs=0;
  connect(this,SIGNAL(jobStatusChangedSig()),this,SLOT(jobStatusChanged()));
}
threadJobControl::~threadJobControl() { }

void threadJobControl::clearJobs() {
  jobs.clear();
  //TODO kill pending threads
}

void threadJobControl::queueJob(renderJob job) {
  jobs.push_back(job);
  emit jobStatusChangedSig();
}

void threadJobControl::jobStatusChanged() {
  if (runningJobs < 2) {
    if (jobs.size() > 0) {
      for (int i=0; i < jobs.size(); ++i) {
//         qDebug("%i",jobs[i].speedup);
        if (jobs[i].speedup > 1) {
          processJob(jobs[i]);
          jobs.remove(i);
          ++runningJobs;
          return;
        }
      }
      int r=qrand()%jobs.size();
      processJob(jobs[r]);
      jobs.remove(r);
      ++runningJobs;
    }
  }
}

void threadJobControl::processJob(renderJob job) {
  renderThread* rt = new renderThread;
  // TODO, empty rt out of renderThreads if done
  renderThreads.push_back(rt);

  connect(rt,SIGNAL(renderingDoneSig(renderJob)),
      this,SLOT(renderingDone(renderJob)));

  rt->render(job);
}

void threadJobControl::renderingDone(renderJob job){
  emit jobDoneSig(job);
  --runningJobs;
  emit jobStatusChangedSig();
}
