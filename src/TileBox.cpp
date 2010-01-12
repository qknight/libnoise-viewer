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

#include "TileBox.h"

TileBox::TileBox(QGraphicsScene* scene) {
    cellsize = 100;
    this->scene = scene;
    tjc = new threadJobControl;
    connect(tjc, SIGNAL(jobDoneSig(renderJob)),
            this, SLOT(jobDone(renderJob)));
}

TileBox::~TileBox() {
    qDebug() << "killing jobs";
    tjc->clearJobs();
}
/*!
the QGraphicsView's width/height is covered by the TileBox.
to be precises: the TileBox is even more. the idea is to compute the tiles in advance.
this helps to get a more interactive feeling...

this function uses QGraphicsView.sceneRect(), expands it and creates a grid which is
then iterated through in a 'cell by cell' basis. for each cell the tile is finally generated.

this function does NOT compute any tile, only the coordinates for them
*/
void TileBox::moveTileBox(QRectF sr) {
    for (int x=(sr.x()-(2.5f*cellsize))/cellsize; x < ((sr.x()+sr.width())+(2.5f*cellsize))/cellsize; x++) {
        for (int y=(sr.y()-(2.5f*cellsize))/cellsize; y < ((sr.y()+sr.width())+(2.5f*cellsize))/cellsize; y++) {
//       qDebug() << "generating tile %i %i" << x << " " << y;
            generateTile(x,y);
        }
    }
}

/*!
this will create a new thread for computing a new tile
x and y specify which tile, [x=0,y=0] would be the first tile
ranging from [x1,x2=0 y1,y2=cellsize]
*/
void TileBox::generateTile(int x, int y) {
    int itempos_x = x*cellsize;
    int itempos_y = y*cellsize;

//   qDebug("%i %i",itempos_x,itempos_y);
    // TODO add check if items already exist, if not then
    // add them else don't

    foreach (QPoint n, tileCoordinatesDB) {
        if (n.x() == itempos_x && n.y() == itempos_y) {
// 	  qDebug() << __PRETTY_FUNCTION__ << ": item already exists";
            return;
        }
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
This callback function is called, when a new tile is rendered and should be displayed in the main gui thread
*/
void TileBox::jobDone(renderJob job) {
    int speedup = job.speedup;
    int width = job.width;
    int height = job.height;
    QTime tracker;
    tracker.start();

    QImage qimage(width/speedup, height/speedup, QImage::Format_RGB32);
    for (int i=0; i < width/speedup; ++i) {
        for (int j=0; j < height/speedup; ++j) {
            int o=(j+i*width/speedup)*3;
            QRgb val = QColor((unsigned char)job.ba[o+0],(unsigned char)job.ba[o+1],(unsigned char)job.ba[o+2]).rgb();
            qimage.setPixel(i,j,val);
        }
    }

    QPixmap pix = QPixmap().fromImage(qimage,Qt::AutoColor).scaled(job.cellsize,job.cellsize);
    QGraphicsPixmapItem* ni = new QGraphicsPixmapItem(pix);
    ni->moveBy(job.x, job.y);
    ni->setZValue(-job.speedup);
    scene->addItem(ni);
//     qDebug("Time elapsed to process the rawimage into an image and adding it to the scene: %d ms", tracker.elapsed());
    emit sceneItemCountSignal(scene->items().size());
}

void TileBox::resetTiles(QRectF sr) {
    tjc->clearJobs();
    tileCoordinatesDB.clear();
    foreach (QGraphicsItem * z,scene->items()) {
        scene->removeItem(z);
    }
    
    // coordinate system
    scene->addLine(QLineF(-300,0,300,0));
    scene->addLine(QLineF(0,-300,0,300));

    //FIXME write new configuration values to class TileBox
    moveTileBox(sr);
}
