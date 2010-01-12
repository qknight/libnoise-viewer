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

#include "ui_mainWidget.h"
#include "movableGraphicsView.h"
#include "TileBox.h"

class mainWidget : public QDialog, private Ui::noiseViewWidget {
    Q_OBJECT
public:
    mainWidget(QDialog* parent=0);
private:
    QSettings* settings;
    QGraphicsScene* scene;
    TileBox* tileBox;
    bool colorstate;
    double frequency;
    int octave;
private Q_SLOTS:
    void colorstate_changed(bool);
    void frequency_changed(double);
    void octave_changed(int);
public Q_SLOTS:
    void updateSceneItemLabel(int size);
Q_SIGNALS:
  void changeColorstate(bool);
  void changeOctave(int);
  void changeFrequency(double);
  void resetTilesSignal();
};

#endif
