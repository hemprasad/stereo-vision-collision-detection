// 
// File:   StereoCamera.h
// Author: nick
//
// Created on 5 February 2008, 21:39
//

#ifndef _STEREOCAMERA_H
#define	_STEREOCAMERA_H

#include <highgui.h>
#include <cvaux.h>

#include "StereoImage.h"

const int LEFT_CAMERA_ID = 0;
const int RIGHT_CAMERA_ID = 1;
const int CAMERA_ADJUSTMENT_TIME = 90;

class StereoCamera
{
    
private:

    CvCapture* m_leftCam; //= cvCaptureFromCAM(0);
    CvCapture* m_rightCam;
    CvCalibFilter m_calibFilter;
    
    bool m_calibIsEnabled;
    
    
public:
    
    StereoCamera();
    ~StereoCamera();
    
    //plug in the left camera first. left = video0
    void open();
    void close();
    StereoImage grabFrame();
    
    bool calibrateCamera();
    void enableCalibration(bool enable);
    
};

#endif	/* _STEREOCAMERA_H */

