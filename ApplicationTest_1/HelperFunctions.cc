#include "HelperFunctions.h"

#include <string>

IplImage* convertToBW(IplImage* image)
{
    IplImage* bwImage = 0;
    
    bwImage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
    bwImage->origin = image->origin;
    
    std::string model = image->colorModel;
    
    /*if (model == "RGB")
    {
        cvCvtColor(image, bwImage, CV_RGB2GRAY);
    }
    else*/
    {
        cvCvtColor(image, bwImage, CV_BGR2GRAY);
    }
    
    return bwImage;
}

