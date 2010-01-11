//
// C++ Implementation: mainWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
#include "mainWidget.h"

mainWidget::mainWidget(QDialog* parent) : QDialog(parent) {
  setupUi(this);
  tjc = new threadJobControl;
  scene = new QGraphicsScene;
  graphicsView->setScene(scene);
  graphicsView->setSceneRect(-200, -200, 200, 200);
// //   graphicsView->setResizeAnchor (QGraphicsView::AnchorViewCenter);
// //   graphicsView->setAlignment ( Qt::AlignCenter);
  graphicsView->centerOn(QPoint(0,0));
  settings = new QSettings("libnoise-view", "qknight");

  frequency = settings->value("frequency", 0.002).toDouble();;
  octave = settings->value("octave", 4).toInt();
  colorstate = settings->value("drawcoloredOrBlackWhite", true).toBool();

  static const int cellsize = 100;

  connect(tjc,SIGNAL(jobDoneSig(renderJob)), this,
        SLOT(jobDone(renderJob)));

  if (colorstate)
    r1->setChecked(true);
  else
    r2->setChecked(true);
  frequencyleft->setValue(frequency);
  octavesleft->setValue(octave);

  connect(octavesleft,SIGNAL(valueChanged ( int )),this,
        SLOT(octave_changed(int)));
  connect(frequencyleft,SIGNAL(valueChanged ( double )),this,
        SLOT(frequency_changed(double)));

  connect(r1,SIGNAL( toggled ( bool )),
      this,SLOT( colorstate_changed(bool)));
  connect(graphicsView, SIGNAL(onMoveSig(int,int)),
      this, SLOT(moveSceneRectBy(int,int)));

  resetTiles();
}

mainWidget::~mainWidget(){ }

void mainWidget::moveSceneRectBy(int x, int y) {
  // first let's see if the change affects more than a cellwidth
  static int xdelta=0;
  static int ydelta=0;
  
  xdelta+=x;
  ydelta+=y;
//   qDebug() << xdelta << " " << ydelta;
  
  // now move the view to the new coordinates
  QRectF v = graphicsView->sceneRect();
  v.moveTo(v.x()+x, v.y()+y);
  graphicsView->setSceneRect(v);

  if (xdelta > cellsize) {
    // move east
    moveTileBoxRelative(1,0);
    xdelta%=cellsize;
  }

  if (-xdelta > cellsize) {
    // move west
    moveTileBoxRelative(-1,0);
    xdelta%=cellsize;
  }

  if (ydelta > cellsize) {
    // move south
    moveTileBoxRelative(0,1);
    ydelta%=cellsize;
  }

  if (-ydelta > cellsize) {
    // move north
    moveTileBoxRelative(0,-1);
    ydelta%=cellsize;
  }
}

/*!
the QGraphicsView's width/height is covered by the TileBox.
to be precises: the TileBox is even more. the idea is to compute the tiles in advance.
this helps to get a more interactive feeling...
*/
void mainWidget::moveTileBoxRelative(int x, int y) {
  static int xBoxOffset=0;
  static int yBoxOffset=0;

//   qDebug("moving by x=%i, y=%i", x, y);

  xBoxOffset+=x;
  yBoxOffset+=y;

  settings->setValue("xoffset", xBoxOffset);
  settings->setValue("yoffset", yBoxOffset);


//   s1cene->addText(". 0/0 is here");

//   qDebug("Boxoff by x=%i, y=%i", xBoxOffset, yBoxOffset);

  QRectF sr = graphicsView->sceneRect();
  int viewWidth =  (int) sr.width();
  int viewHeight = (int) sr.height();

  int xboxes = (viewWidth+cellsize/2) / cellsize;
  int yboxes = (viewHeight+cellsize/2) / cellsize;

  if (xboxes%2)
    xboxes++;
  if (yboxes%2)
    yboxes++;

//   qDebug("xboxes: %i yboxes: %i", xboxes, yboxes);

  int rx1 =  xBoxOffset * cellsize + cellsize * (-xboxes/2);
  int ry1 =  yBoxOffset * cellsize + cellsize * (-yboxes/2);
  int rx2 =  cellsize * (xboxes);
  int ry2 =  cellsize * (yboxes);

//   scene->removeItem( viewBox );
//   viewBox = scene->addRect (QRectF(rx1,ry1,rx2,ry2));
//   viewBox->setZValue(200);

  int n=0;
  for(int x=xBoxOffset-xboxes/2+1; x < xBoxOffset+xboxes/2+1; ++x) {
    for(int y=yBoxOffset-yboxes/2+1; y < yBoxOffset+yboxes/2+1; ++y) {
      ++n;
//       qDebug("generating tile %i %i", x,y);
      generateTile(x,y);
    }
  }
//   qDebug("generated %i tiles", n);
}

/*!
this will create a new thread for computing a new tile
*/
void mainWidget::generateTile(int x, int y) {
  int itempos_x = x*cellsize+xoffset-cellsize;
  int itempos_y = y*cellsize+yoffset-cellsize;

//   qDebug("%i %i",itempos_x,itempos_y);
  // TODO add check if items already exist, if not then
  // add them not else

  foreach (QPoint n, tileCoordinatesDB) {
    if (n.x() == itempos_x && n.y() == itempos_y)
      return;
  }

  renderJob job;

  job.id=qrand()%4000;
  job.x=itempos_x;
  job.y=itempos_y;
  job.cellsize=cellsize;
  job.colorstate=colorstate;

  job.width=cellsize;
  job.height=cellsize;
  job.octave=octave;
  job.frequency=frequency;
  
  // no speedup, speedup = 1
  job.speedup=1;
  tjc->queueJob(job);

  // this dispatches the second renderjob which is much faster done
  job.speedup=4;
  tjc->queueJob(job);
  
  tileCoordinatesDB.push_back(QPoint(itempos_x, itempos_y));
}

/*!
This function is called, when a new tile is rendered and should be displayed in the main gui thread
*/
void mainWidget::jobDone(renderJob job){
  int speedup = job.speedup;
  int width = job.width;
  int height = job.height;
  QTime tracker;
  tracker.start();

  QImage qimage(width/speedup,height/speedup,QImage::Format_RGB32);
  for(int i=0; i < width/speedup; ++i) {
    for(int j=0; j < height/speedup; ++j) {
      int o=(j+i*width/speedup)*3;
      QRgb val = QColor((unsigned char)job.ba[o+0],(unsigned char)job.ba[o+1],(unsigned char)job.ba[o+2]).rgb();
      qimage.setPixel(i,j,val);
    }
  }

  QPixmap pix = QPixmap().fromImage(qimage,Qt::AutoColor).scaled(job.cellsize,job.cellsize);
  QGraphicsPixmapItem* ni = new QGraphicsPixmapItem(pix);
  ni->moveBy(job.x-xoffset, job.y-yoffset);
  ni->setZValue(-job.speedup);
  scene->addItem(ni);
//   qDebug("Time elapsed to process the rawimage into an image and adding it to the scene: %d ms", tracker.elapsed());
  itemLabel->setText(QString("%1").arg(scene->items().size()));
}

void mainWidget::colorstate_changed( bool state){
  settings->setValue("drawcoloredOrBlackWhite", state);
  colorstate = state;
  resetTiles();
}

void mainWidget::frequency_changed(double i){
  settings->setValue("frequency", i);
  frequency = i;
  resetTiles();
}

void mainWidget::octave_changed(int i){
  settings->setValue("octave",i);
  octave = i;
  resetTiles();
}

void mainWidget::resetTiles() {
  tjc->clearJobs();
  tileCoordinatesDB.clear();
  foreach (QGraphicsItem * z,scene->items()) {
    scene->removeItem(z);
  }

  scene->addLine(QLineF(-600,0,600,0));
  scene->addLine(QLineF(0,-600,0,600));

  moveTileBoxRelative(0,0);
}
