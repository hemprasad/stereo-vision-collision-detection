#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <iostream>

#include "RedDotExperiment.h"
#include "StereoCamera.h"
#include "StereoImage.h"

StereoImage image;

void cBoardTest()
{
    int leftPoint;
    int rightPoint;
    
    CvPoint lPoint;
    CvPoint rPoint;
    
    StereoCamera cam;
    
    
    
    cvSetMouseCallback("cameraLeft", mouseLeft, &lPoint);
    cvSetMouseCallback("cameraRight", mouseRight, &rPoint);
    
    cam.open();
        
    image = cam.grabFrame();
    
    
    
    
    
    
    cvWaitKey(50);
    cvShowImage("cameraLeft", image.getLeftImage());
    cvShowImage("cameraRight", image.getRightImage());
    
    
    
    
    
    int key = cvvWaitKeyEx(0, 0);
    
    //wait for keypress
    switch ((char) key)
    {
        case 'p':
            
            calcDistance(lPoint.x, rPoint.x);
            
            break;
            
        default:
            calcDistance(leftPoint, rightPoint);
            break;
    }
    
    cam.close();
    
    
    
}


void mouseLeft(int event, int x, int y, int flags, void* param)
{
    CvPoint* point;
    
    if (event != CV_EVENT_LBUTTONDOWN) return;
    
    point = (CvPoint*) param;
    
    point->x = x;
    point->y = y;
    
    cvCircle(image.getLeftImage(), *point, 3, CV_RGB(0, 255, 0));
    
    cvShowImage("cameraLeft", image.getLeftImage());
}

void mouseRight(int event, int x, int y, int flags, void* param)
{
    CvPoint* point;
    
    if (event != CV_EVENT_LBUTTONDOWN) return;
    
    point = (CvPoint*) param;
    
    point->x = x;
    point->y = y;
    
    cvCircle(image.getRightImage(), *point, 3, CV_RGB(0, 255, 0));
    
    cvShowImage("cameraRight", image.getRightImage());
}

void calcDistance(int xLeft, int xRight)
{
    int pixelDiff;
    
    std::cout << "pixel distance = " << (xLeft - xRight) << std::endl;
    
}


void showBlendedVideo()
{
    StereoCamera cam;
    StereoImage img;
    IplImage* blended;
    char c;
    bool quit = false;
    
    cvNamedWindow( "blended", CV_WINDOW_AUTOSIZE );
    
    cam.open();
    //cam.enableCalibration(true);
    
    img = cam.grabFrame();
    
    
    blended = cvCreateImage(cvSize(img.getRightImage()->width, img.getRightImage()->height), img.getRightImage()->depth, 3);
    
    while (!quit)
    {   
        cvAddWeighted(img.getLeftImage(), 0.5, img.getRightImage(), 0.5, 1, blended);

        cvShowImage("blended", blended);
        
        c = (char) cvWaitKey(10);
        
        switch (c)
        {
            case 'q':
                
                quit = true;
                break;
                
            case 'e':
                
                cam.enableCalibration(true);
                break;
                
            case 'd':
                
                cam.enableCalibration(false);
                break;
        }
        
        img = cam.grabFrame();
    }
    
    cvDestroyWindow( "blended" );
    
    
}

