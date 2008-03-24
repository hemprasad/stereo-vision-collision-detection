// 
// File:   squares.h
// Author: nick
//
// Created on 23 March 2008, 19:40
//

#ifndef _SQUARES_H
#define	_SQUARES_H

#include <cv.h>

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2 
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 );


// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage );
        
// the function draws all the squares in the image
void drawSquares( IplImage* img, CvSeq* squares );

#endif	/* _SQUARES_H */

