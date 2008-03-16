// 
// File:   VideoReader.h
// Author: nick
//
// Created on 19 January 2008, 16:27
//

#ifndef _VIDEOREADER_H
#define	_VIDEOREADER_H

#include <cv.h>
#include <highgui.h>
#include <string>

using std::string;

class VideoReader
{
public:
    VideoReader();
    
    void open(string filename);
    IplImage* readFrame();
    void close();
    
private:
    CvCapture* m_capture;
};

#endif	/* _VIDEOREADER_H */

