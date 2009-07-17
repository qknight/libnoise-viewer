//
// C++ Interface: renderJob
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENDERJOB_H
#define RENDERJOB_H

#include <QByteArray>

/**
	@author Joachim Schiele <js@lastlog.de>
*/
class renderJob {
  public:
    QByteArray ba;
    int x;
    int y;
    int cellsize;
    int speedup;
    bool colorstate;
    int width;
    int height;
    int octave;
    double frequency;
    int id;
  public:
    renderJob();
    ~renderJob();

};

#endif
