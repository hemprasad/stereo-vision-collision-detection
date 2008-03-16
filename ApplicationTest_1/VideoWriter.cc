#include "VideoWriter.h"

VideoWriter::VideoWriter()
{
	m_videoWriter = 0;
}

VideoWriter::~VideoWriter()
{
}

void VideoWriter::writeFrame(IplImage* image)
{
	if (m_videoWriter == 0)
	{
		//throw an exception here
	}
	
	cvWriteFrame( m_videoWriter, image);
}

void VideoWriter::create(string filename, CvSize size)
{
	m_videoWriter = cvCreateVideoWriter( filename.c_str(), CV_FOURCC('D','I','V','X'), //CV_FOURCC('D','I','V','X') WORKS!!!
	                                        30, size);
}

void VideoWriter::close()
{
	cvReleaseVideoWriter( &m_videoWriter );
}
