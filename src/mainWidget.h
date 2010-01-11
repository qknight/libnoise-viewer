//
// C++ Interface: mainWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDebug>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QSettings>
#include <QByteArray>
#include <QTime>
#include <QPoint>
#include <QVector>
#include <QGraphicsScene>
#include "ui_mainWidget.h"
#include "renderThread.h"
#include "renderJob.h"
#include "threadJobControl.h"
#include "movableGraphicsView.h"

class mainWidget : public QDialog, private Ui::noiseViewWidget
{
  Q_OBJECT

  public:
    mainWidget(QDialog* parent=0);
    ~mainWidget();
  private:
    QSettings* settings;
    void populate();
    QGraphicsScene* scene;
    bool colorstate; // 0 color / 1 bw
    double frequency;
    int octave;
    int xoffset;
    int yoffset;
    threadJobControl* tjc;
    static const int cellsize = 100;
    /*! dispatches renderJobs which generate the tile at position x,y and return them via signal/slot */
    void generateTile(int x, int y);
    QGraphicsRectItem* viewBox;
  private:
    void resetTiles();
    void moveTileBoxRelative(int x, int y);
    QVector<QPoint> tileCoordinatesDB;

  private slots:
    void colorstate_changed(bool);
    void frequency_changed(double);
    void octave_changed(int);
    void jobDone(renderJob job);
    void moveSceneRectBy(int x, int y);
  signals:
    void jobDoneSig(renderJob job);
};

#endif
