// 
// File:   RedDotExperiment.h
// Author: nick
//
// Created on 24 February 2008, 13:16
//

#ifndef _REDDOTEXPERIMENT_H
#define	_REDDOTEXPERIMENT_H

#include "StereoImage.h"

void cBoardTest();

void showBlendedVideo();

IplImage* blendImages(StereoImage img, IplImage* const blendedImage);

void mouseLeft(int event, int x, int y, int flags, void* param);
void mouseRight(int event, int x, int y, int flags, void* param);

void calcDistance(int xLeft, int xRight);

void camshiftDemo();

#endif	/* _REDDOTEXPERIMENT_H */

