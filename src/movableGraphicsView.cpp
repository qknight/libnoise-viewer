//
// C++ Implementation: movableGraphicsView
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "movableGraphicsView.h"

movableGraphicsView::movableGraphicsView(QWidget * parent) : QGraphicsView(parent) {
  setDragMode(QGraphicsView::ScrollHandDrag);
  onmove=false;
}

movableGraphicsView::~movableGraphicsView()
{
}

void movableGraphicsView::mousePressEvent ( QMouseEvent * event ) {
  if (event->button() == Qt::LeftButton) {
    x=event->x();
    y=event->y();
    onmove=true;
  }
}

void movableGraphicsView::mouseReleaseEvent ( QMouseEvent * event ) {
    onmove=false;
}

void movableGraphicsView::mouseMoveEvent ( QMouseEvent * event ) {
  if (onmove == true) {
    int xdiff=x-event->x();
    int ydiff=y-event->y();
    x=event->x();
    y=event->y();
    emit onMoveSig(xdiff,ydiff);
  }
}

void movableGraphicsView::resizeEvent ( QResizeEvent * event ) {
//   qDebug() << __PRETTY_FUNCTION__ << width() << " " << height();
  //FIXME why -6 pixels?
  setSceneRect(QRect(0, 0, width()-6, height()-6));
}
