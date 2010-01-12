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
//     connect(graphicsView, SIGNAL(absoluteViewMoveSignal(int,int)),
//             this, SLOT(moveSceneRectBy(int,int)));

}

void TileBox::moveSceneRectBy(int x, int y) {
    // first let's see if the change affects more than a cellwidth
//     static int xdelta=0;
//     static int ydelta=0;
//
//     xdelta+=x;
//     ydelta+=y;
// //   qDebug() << xdelta << " " << ydelta;
//
//     // now move the view to the new coordinates
// //     QRectF v = graphicsView->sceneRect();
// //     v.moveTo(v.x()+x, v.y()+y);
//     graphicsView->setSceneRect(v);
//
//     if (xdelta > cellsize) {
//         // move east
//         moveTileBoxRelative(1,0);
//         xdelta%=cellsize;
//     }
//
//     if (-xdelta > cellsize) {
//         // move west
//         moveTileBoxRelative(-1,0);
//         xdelta%=cellsize;
//     }
//
//     if (ydelta > cellsize) {
//         // move south
//         moveTileBoxRelative(0,1);
//         ydelta%=cellsize;
//     }
//
//     if (-ydelta > cellsize) {
//         // move north
//         moveTileBoxRelative(0,-1);
//         ydelta%=cellsize;
//     }
}

/*!
the QGraphicsView's width/height is covered by the TileBox.
to be precises: the TileBox is even more. the idea is to compute the tiles in advance.
this helps to get a more interactive feeling...
*/
void TileBox::moveTileBoxRelative(int x, int y) {
//     static int xBoxOffset=0;
//     static int yBoxOffset=0;
//
// //   qDebug("moving by x=%i, y=%i", x, y);
//
//     xBoxOffset+=x;
//     yBoxOffset+=y;
//
//     settings->setValue("xoffset", xBoxOffset);
//     settings->setValue("yoffset", yBoxOffset);
//
// //   s1cene->addText(". 0/0 is here");
//
// //   qDebug("Boxoff by x=%i, y=%i", xBoxOffset, yBoxOffset);
//
//     QRectF sr = graphicsView->sceneRect();
//     int viewWidth =  (int) sr.width();
//     int viewHeight = (int) sr.height();
//
//     int xboxes = (viewWidth+cellsize/2) / cellsize;
//     int yboxes = (viewHeight+cellsize/2) / cellsize;
//
//     if (xboxes%2)
//         xboxes++;
//     if (yboxes%2)
//         yboxes++;
//
// //   qDebug("xboxes: %i yboxes: %i", xboxes, yboxes);
//
//     int rx1 =  xBoxOffset * cellsize + cellsize * (-xboxes/2);
//     int ry1 =  yBoxOffset * cellsize + cellsize * (-yboxes/2);
//     int rx2 =  cellsize * (xboxes);
//     int ry2 =  cellsize * (yboxes);
//
// //   scene->removeItem( viewBox );
// //   viewBox = scene->addRect (QRectF(rx1,ry1,rx2,ry2));
// //   viewBox->setZValue(200);
//
//     int n=0;
//     for (int x=xBoxOffset-xboxes/2+1; x < xBoxOffset+xboxes/2+1; ++x) {
//         for (int y=yBoxOffset-yboxes/2+1; y < yBoxOffset+yboxes/2+1; ++y) {
//             ++n;
// //       qDebug("generating tile %i %i", x,y);
//             generateTile(x,y);
//         }
//     }
}

/*!
this will create a new thread for computing a new tile
*/
void TileBox::generateTile(int x, int y) {
//     int itempos_x = x*cellsize+xoffset-cellsize;
//     int itempos_y = y*cellsize+yoffset-cellsize;
//
// //   qDebug("%i %i",itempos_x,itempos_y);
//     // TODO add check if items already exist, if not then
//     // add them not else
//
//     foreach (QPoint n, tileCoordinatesDB) {
//         if (n.x() == itempos_x && n.y() == itempos_y)
//             return;
//     }
//
//     renderJob job;
//
//     job.id=qrand()%4000;
//     job.x=itempos_x;
//     job.y=itempos_y;
//     job.cellsize=cellsize;
//     job.colorstate=colorstate;
//
//     job.width=cellsize;
//     job.height=cellsize;
//     job.octave=octave;
//     job.frequency=frequency;
//
//     // no speedup, speedup = 1
//     job.speedup=1;
//     tjc->queueJob(job);
//
//     // this dispatches the second renderjob which is much faster done
//     job.speedup=4;
//     tjc->queueJob(job);
//
//     tileCoordinatesDB.push_back(QPoint(itempos_x, itempos_y));
}

/*!
This function is called, when a new tile is rendered and should be displayed in the main gui thread
*/
void TileBox::jobDone(renderJob job) {
//     int speedup = job.speedup;
//     int width = job.width;
//     int height = job.height;
//     QTime tracker;
//     tracker.start();
//
//     QImage qimage(width/speedup, height/speedup, QImage::Format_RGB32);
//     for (int i=0; i < width/speedup; ++i) {
//         for (int j=0; j < height/speedup; ++j) {
//             int o=(j+i*width/speedup)*3;
//             QRgb val = QColor((unsigned char)job.ba[o+0],(unsigned char)job.ba[o+1],(unsigned char)job.ba[o+2]).rgb();
//             qimage.setPixel(i,j,val);
//         }
//     }
//
//     QPixmap pix = QPixmap().fromImage(qimage,Qt::AutoColor).scaled(job.cellsize,job.cellsize);
//     QGraphicsPixmapItem* ni = new QGraphicsPixmapItem(pix);
//     ni->moveBy(job.x-xoffset, job.y-yoffset);
//     ni->setZValue(-job.speedup);
//     scene->addItem(ni);
//   qDebug("Time elapsed to process the rawimage into an image and adding it to the scene: %d ms", tracker.elapsed());
//     emit sceneItemCountSignal(scene->items().size());

}

void TileBox::reset() {
//   tjc->clearJobs();
    tileCoordinatesDB.clear();
}

void TileBox::resetTiles() {
    foreach (QGraphicsItem * z,scene->items()) {
        scene->removeItem(z);
    }

    scene->addLine(QLineF(-300,0,300,0));
    scene->addLine(QLineF(0,-300,0,300));

    //FIXME write new configuration values to class TileBox
//     moveTileBox(0,0);
}
