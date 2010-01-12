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
        int xdiff = x - event->x();
        int ydiff = y - event->y();
	// x and y store the old values so we can compute the difference
        x = event->x();
        y = event->y();
	
        QRectF v = sceneRect();
        v.moveTo(v.x()+xdiff, v.y()+ydiff);
        setSceneRect(v);

// 	qDebug() << "you are looking at x/y = " << v.x()+ width()/2 << " " << v.y()+ height()/2;
        emit absoluteViewMoveSignal(v.x()+ width()/2,v.y()+ height()/2);
    }
}

/*!
normally the view would be resized to match the scene but we have to force it NOT
to change. we also suppress the scrollbars -> they are hidden by intention since
the planar surface does NOT have any end.
*/
void movableGraphicsView::resizeEvent ( QResizeEvent * event ) {
//   qDebug() << __PRETTY_FUNCTION__ << width() << " " << height();
//FIXME on resize the position is resetted to 0/0 which is bad if the resize happens at
//      different coordinates (which is very likely)
    //FIXME why -6 pixels?
    setSceneRect(QRect((-width()/2)+3, (-height()/2)+3, width()-3, height()-3));
}
