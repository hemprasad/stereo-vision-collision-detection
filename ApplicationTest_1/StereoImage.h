// 
// File:   StereoImage.h
// Author: nick
//
// Created on 5 February 2008, 21:36
//

#ifndef _STEREOIMAGE_H
#define	_STEREOIMAGE_H

#include <highgui.h>

class StereoImage
{
    
private:
    
    IplImage* m_rightImage;
    IplImage* m_leftImage;
    
public:
    
    StereoImage();
    ~StereoImage();
    
    IplImage* getRightImage();
    void setRightImage(IplImage* rightImage);
    IplImage* getLeftImage(); 
    void setLeftImage(IplImage* leftImage);
    
    void getFrames(IplImage* rightImage, IplImage* leftImage);
    IplImage* getFrames();
    void setFrames(IplImage* rightImage, IplImage* leftImage);
    
    
};


#endif	/* _STEREOIMAGE_H */

