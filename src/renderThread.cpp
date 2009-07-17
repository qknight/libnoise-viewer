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

//     qDebug("x %i",job.x);
//     qDebug("y %i",job.y);
//     qDebug("cellsize %i",job.cellsize);
//     qDebug("colorstate %i",(unsigned int)job.colorstate);
//     qDebug("speedup %i",job.speedup);
//     qDebug("width %i",job.width);
//     qDebug("height %i",job.height);
//     qDebug("octave %i",job.octave);
//     qDebug("frequency %f",job.frequency);

//     module::Voronoi myModule;
  module::RidgedMulti myModule;
//    module::Perlin myModule;
//   module::Billow myModule;
//  module::Spheres myModule;

//     mod.register("Frequency", GetFrequency(), SetFrequency(), <double>);

//     std::cout << "GetFrequency() = " << myModule.GetFrequency   () << "\n";
//     std::cout << "GetLacunarity() = " << myModule.GetLacunarity   ()<< "\n";
//     std::cout << "GetNoiseQuality() = " << myModule.GetNoiseQuality   () << "\n";
//     std::cout << "GetOctaveCount() = " << myModule.GetOctaveCount() << "\n";
//     std::cout << "GetPersistence() = " << myModule.GetPersistence() << "\n";
//     std::cout << "GetSeed() = " << myModule.GetSeed   () << "\n";
//     std::cout << "GetSourceModuleCount() = " << myModule.GetSourceModuleCount   () << "\n";
//
//     module::Invert myModule;
//     myModule.SetSourceModule(0,zModule);

  myModule.SetOctaveCount (job.octave);
  myModule.SetFrequency (job.frequency);

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
    renderer.AddGradientPoint ( 0.940000, utils::Color (0, 129, 0, 255));
    renderer.AddGradientPoint ( 0.900000, utils::Color (0, 190, 0, 255));
    renderer.AddGradientPoint ( 0.850000, utils::Color (0, 250, 0, 255));
    renderer.AddGradientPoint ( 0.800000, utils::Color (120, 250, 250, 255));
    renderer.AddGradientPoint ( 0.590000, utils::Color (20, 120, 200, 255));
    renderer.AddGradientPoint ( 0.540000, utils::Color (20, 50, 250, 255));
    renderer.AddGradientPoint ( 0.400000, utils::Color (0, 0, 120, 255));

    renderer.EnableLight ();
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
