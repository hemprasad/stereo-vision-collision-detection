// 
// File:   VideoWriter.h
// Author: nick
//
// Created on 12 January 2008, 22:01
//

#ifndef _VIDEOWRITER_H
#define	_VIDEOWRITER_H

#include <cv.h>
#include <highgui.h>

#include <string>

using std::string;

class VideoWriter
{
public:
	VideoWriter();
	virtual ~VideoWriter();
	
	void create(string filename, CvSize size);
	void close();
	void writeFrame(IplImage* image);
	
private:
	CvVideoWriter* m_videoWriter;
};

#endif	/* _VIDEOWRITER_H */

