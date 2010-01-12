//
// C++ Implementation: renderThread
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

// http://doc.trolltech.com/4.3/threads.html#threads-and-qobjects
// http://labs.trolltech.com/page/Projects/Threads/QtConcurrent

#include "renderThread.h"
#include <QTime>

renderThread::renderThread(QObject *parent) : QThread(parent) {
  qRegisterMetaType<renderJob>("renderJob");
  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

renderThread::~renderThread() { }

void renderThread::render(renderJob job) {
    QMutexLocker locker(&mutex);
    this->job=job;
    start(QThread::IdlePriority);
}

void renderThread::run() {
  renderJob j = generateRawImage(this->job);

  emit renderingDoneSig(j);
}

renderJob renderThread::generateRawImage (renderJob job) {
  using namespace noise;
  QTime tracker;
  tracker.start();

  module::RidgedMulti myModule;
  myModule.SetOctaveCount (job.octave);
  myModule.SetFrequency (job.frequency);

//   module::Perlin myModule;
//   myModule.SetOctaveCount (job.octave);
//   myModule.SetFrequency (job.frequency);

//     module::Voronoi myModule;
//     myModule.SetFrequency (job.frequency);

//   module::Billow myModule;
//   myModule.SetOctaveCount (job.octave);
//   myModule.SetFrequency (job.frequency);
  
//   module::Spheres myModule;
//   myModule.SetFrequency (job.frequency);

  utils::NoiseMap heightMap;
  utils::NoiseMapBuilderPlane heightMapBuilder;

  heightMapBuilder.SetSourceModule (myModule);
  heightMapBuilder.SetDestNoiseMap (heightMap);
  heightMapBuilder.SetDestSize (job.width/job.speedup, job.height/job.speedup);
  heightMapBuilder.SetBounds (job.x,job.x+job.width,job.y,job.y+job.height);
  heightMapBuilder.Build ();

  utils::RendererImage renderer;
  utils::Image image;
  renderer.SetSourceNoiseMap (heightMap);
  renderer.SetDestImage (image);

  if (job.colorstate) {
    renderer.ClearGradient ();

    renderer.AddGradientPoint ( 1.000000, utils::Color (0, 90, 0, 255));
    renderer.AddGradientPoint ( 0.740000, utils::Color (0, 129, 0, 255));
    renderer.AddGradientPoint ( 0.700000, utils::Color (0, 190, 0, 255));
    renderer.AddGradientPoint ( 0.650000, utils::Color (0, 250, 0, 255));
    renderer.AddGradientPoint ( 0.600000, utils::Color (120, 250, 250, 255));
    renderer.AddGradientPoint ( 0.390000, utils::Color (20, 120, 200, 255));
    renderer.AddGradientPoint ( 0.340000, utils::Color (20, 50, 250, 255));
    renderer.AddGradientPoint ( 0.200000, utils::Color (0, 0, 120, 255));

    renderer.EnableLight();
    renderer.SetLightContrast (3.0); // Triple the contrast
    renderer.SetLightBrightness (2.0); // Double the brightness
  }

  renderer.Render();

  // now the data is in image
  for(int x=0; x < job.width/job.speedup; ++x) {
    for(int y=0; y < job.height/job.speedup; ++y) {
      utils::Color a = image.GetValue (x, y);
      job.ba.push_back(a.red);
      job.ba.push_back(a.green);
      job.ba.push_back(a.blue);
    }
  }
//   qDebug("Time elapsed: %d ms", tracker.elapsed());

  return job;
}
