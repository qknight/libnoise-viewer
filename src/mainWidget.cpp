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

    scene = new QGraphicsScene;
    graphicsView->setScene(scene);

    tileBox = new TileBox(scene);
    graphicsView->centerOn(QPoint(0,0));
    settings = new QSettings("libnoise-view", "qknight");

    frequency = settings->value("frequency", 0.002).toDouble();;
    octave = settings->value("octave", 4).toInt();
    colorstate = settings->value("drawcoloredOrBlackWhite", true).toBool();

    tileBox->colorstate=colorstate;
    tileBox->octave=octave;
    tileBox->frequency=frequency;

    if (colorstate)
        r1->setChecked(true);
    else
        r2->setChecked(true);
    frequencyleft->setValue(frequency);
    octavesleft->setValue(octave);

    connect(octavesleft,SIGNAL(valueChanged ( int )),
            this, SLOT(octave_changed(int)));
    connect(frequencyleft,SIGNAL(valueChanged ( double )),
            this, SLOT(frequency_changed(double)));
    connect(r1, SIGNAL( toggled ( bool )),
            this, SLOT( colorstate_changed(bool)));

    connect(graphicsView, SIGNAL(absoluteViewMoveSignal(QRectF)),
            tileBox, SLOT(moveTileBox(QRectF)));

    connect(this, SIGNAL(resetTilesSignal(QRectF)),
            tileBox, SLOT(resetTiles(QRectF)));

    connect(tileBox, SIGNAL(sceneItemCountSignal(int)),
            this, SLOT(updateSceneItemLabel(int)));
	    
    emit resetTilesSignal(graphicsView->sceneRect());
}

mainWidget::~mainWidget() {
    delete tileBox;
    delete scene;
}

void mainWidget::updateSceneItemLabel(int size) {
    itemLabel->setText(QString("%1").arg(size));
}

void mainWidget::colorstate_changed( bool state) {
    settings->setValue("drawcoloredOrBlackWhite", state);
    tileBox->colorstate=state;
    emit resetTilesSignal(graphicsView->sceneRect());
}

void mainWidget::frequency_changed(double i) {
    settings->setValue("frequency", i);
    tileBox->frequency=i;
    emit resetTilesSignal(graphicsView->sceneRect());
}

void mainWidget::octave_changed(int i) {
    settings->setValue("octave",i);
    tileBox->octave=i;
    emit resetTilesSignal(graphicsView->sceneRect());
}
