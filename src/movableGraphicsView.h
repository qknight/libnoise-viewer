//
// C++ Interface: movableGraphicsView
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MOVABLEGRAPHICSVIEW_H
#define MOVABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
/**
	@author Joachim Schiele <js@lastlog.de>
*/
class movableGraphicsView : public QGraphicsView
{
  Q_OBJECT

  public:
    movableGraphicsView(QWidget * parent = 0);
    ~movableGraphicsView();
  private:
    void resizeEvent ( QResizeEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    int x;
    int y;
    bool onmove;
  public:
  signals:
    void onMoveSig(int,int);

};

#endif
