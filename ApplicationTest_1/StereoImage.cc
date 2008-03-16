#include "StereoImage.h"


StereoImage::StereoImage() {
    
}

StereoImage::~StereoImage() {
    
}

IplImage* StereoImage::getRightImage()
{
    return m_rightImage;
}

void StereoImage::setRightImage(IplImage* rightImage)
{
    m_rightImage = rightImage;
}

IplImage* StereoImage::getLeftImage()
{
    return m_leftImage;
}

void StereoImage::setLeftImage(IplImage* leftImage)
{
    m_leftImage = leftImage;
}


void StereoImage::getFrames(IplImage* rightImage, IplImage* leftImage)
{
    leftImage = m_leftImage;
    rightImage = m_rightImage;
}

void StereoImage::setFrames(IplImage* rightImage, IplImage* leftImage)
{
    m_leftImage = leftImage;
    m_rightImage = rightImage;
}

IplImage* StereoImage::getFrames()
{
    IplImage* images[2] = { m_leftImage, m_rightImage };
    IplImage* image;
    
    return image;
}

