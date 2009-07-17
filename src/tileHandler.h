//
// C++ Interface: tileHandler
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TILEHANDLER_H
#define TILEHANDLER_H

#include <QObject>
#include "movableGraphicsScene.h"
#include "movableGraphicsView.h"


// class movableGraphicsScene;

/**
	@author Joachim Schiele <js@lastlog.de>
*/
class tileHandler : public QObject {
  Q_OBJECT

  public:
    tileHandler(movableGraphicsScene* scene, movableGraphicsView* graphicsView,
        int x, int y, int width, int height, int cellsize);
    ~tileHandler();


  private:
    int x; // lower left coordinates of the virtual bounding box
    int y; //    containing the tiles which need to be processed
           //    and keep track of
    int width;
    int height;
    int cellsize;
    movableGraphicsScene* scene;
    movableGraphicsView* graphicsView;

};

#endif
