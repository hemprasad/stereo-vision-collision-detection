#include "VideoReader.h"

VideoReader::VideoReader()
{
    m_capture = 0;
}

/**
 *TODO: Add exception handling
 */
void VideoReader::open(string filename)
{
    m_capture = cvCaptureFromFile(filename.c_str());
}


IplImage* VideoReader::readFrame()
{
    IplImage* img = 0;
    
    if (m_capture == 0)
    {
        //TODO throw exception 
    }
    
    img = cvQueryFrame(m_capture);
    
    if (img == 0)
    {
        //TODO not sure if I should automatically release the capture
    }
    
    //I might need to make a clone of the Image
    
    //newImage = cvCloneImage(frameR);
    
    return img;
}


void VideoReader::close()
{
    cvReleaseCapture(&m_capture);
    m_capture = 0;
}
