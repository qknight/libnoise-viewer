/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TILEBOX_H
#define TILEBOX_H

#include <QDebug>
#include <QObject>
#include <QImage>
#include <QTime>
#include <QSettings>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include "renderThread.h"
#include "renderJob.h"
#include "threadJobControl.h"

/*!
The idea behind the TileBox is that the background (libnoise graph) is rendered in tiles and
therefore we can have a very interactive QGraphicsView displaying the 'satellite image' of the surface.

The TileBox computes each tile in a single thread which makes best use of multiple CPUs. All the
TileBox needs to work is the current position of the 'QGraphicsView' and with this information
we can render all the tiles needed to fit the view's width/height with tiles.
*/
class TileBox : public QObject {
    Q_OBJECT
    friend class mainWidget;
public:
    TileBox(QGraphicsScene* scene);
    ~TileBox();

private Q_SLOTS:
    void moveTileBox(QRectF sr);
    void resetTiles(QRectF);
    void generateTile(int x, int y);
    void jobDone(renderJob job);
Q_SIGNALS:
    void jobDoneSig(renderJob job);
    void sceneItemCountSignal(int);
private:
    QVector<QPoint> tileCoordinatesDB;
    QSettings* settings;
    QGraphicsRectItem* viewBox;
    QGraphicsScene* scene;
    threadJobControl* tjc;
    int cellsize;
protected:
    bool colorstate;
    double frequency;
    int octave;
};

#endif // TILEBOX_H
