#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <iostream>

#include "RedDotExperiment.h"
#include "StereoCamera.h"
#include "squares.h"

#include "Blob.h"
#include "BlobResult.h"
#include "HelperFunctions.h"


StereoImage stereoImg;

void cBoardTest()
{
    bool quit = false;
    bool grabFrame = true;
    int leftPoint;
    int rightPoint;
    int thresh = 100;
    IplImage* img = 0;
    
    CvPoint lPoint;
    CvPoint rPoint;
    CvSeq* sequence;
    CvMemStorage* store;
    
    CBlobResult res;
    CBlob blob;
    
    store = cvCreateMemStorage(0);
    
    StereoCamera cam;
    
    cvSetMouseCallback("cameraLeft", mouseLeft, &lPoint);
    cvSetMouseCallback("cameraRight", mouseRight, &rPoint);
    
    cam.open();
    cam.enableCalibration(true);
    
    while(!quit)
    {
        if (grabFrame)
        {
            stereoImg = cam.grabFrame();
            
            cvWaitKey(50);
            cvShowImage("cameraLeft", stereoImg.getLeftImage());
            cvShowImage("cameraRight", stereoImg.getRightImage());
        }
        
        grabFrame = false;

        //sequence = findSquares4(stereoImg.getLeftImage(), store);
        //drawSquares(stereoImg.getLeftImage(), sequence);

        
        
           

        int key = cvvWaitKeyEx(0, 0);

        //wait for keypress
        switch ((char) key)
        {
            case 'c':
            case 'p':

                calcDistance(lPoint.x, rPoint.x);
                quit = true;
                
                break;
                
            case 'q':
                
                quit = true;
                break;
                
            case 'b':
                //CvScalar scale;
                int iMaxx, iMinx, iMaxy, iMiny, iMeanx, iMeany;
                
                if (img != 0) cvReleaseImage(&img);
                
                img = convertToBW(stereoImg.getLeftImage());
                
                res = CBlobResult(img, NULL, thresh, true);
                
                //res.Filter( res, B_INCLUDE, CBlobGetArea(), B_LESS, 1000 );// area <150
                //res.Filter( res, B_INCLUDE, CBlobGetArea(), B_GREATER, 0 );// area <150
                //res.Filter( res, B_INCLUDE, CBlobGetArea(), B_GREATER, 500 );
                
                std::cout << res.GetNumBlobs() << std::endl;
                
                for (int i = 0; i < res.GetNumBlobs(); i++)
                {
                    blob = res.GetBlob(i);
                    
                    // get max, and min co-ordinates
                    iMaxx = (int) blob.MaxX();
                    iMinx = (int) blob.MinX();
                    iMaxy = (int) blob.MaxY();
                    iMiny = (int) blob.MinY();
                    
                    // find the average of the blob (i.e. estimate its centre)
                    iMeanx=(iMinx+iMaxx)/2;
                    iMeany=(iMiny+iMaxy)/2;
                    // mark centre
                    cvLine( img, cvPoint(iMeanx, iMeany), cvPoint(iMeanx, iMeany), CV_RGB(255, 0 , 0), 4, 8, 0 );
                    // mark box around blob
                    cvRectangle(img, cvPoint(iMinx , iMiny ), cvPoint ( iMaxx, iMaxy ), CV_RGB(0, 0, 255), 1, 8, 0);

                    //blob.FillBlob(stereoImg.getLeftImage(), CV_RGB(255, i, 0));
                }
                
                cvShowImage("cameraLeft", img);
                
                break;
                
            case 'g':
                grabFrame = true;
                break;

            default:
                //calcDistance(leftPoint, rightPoint);
                break;
        }
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
    
    cvCircle(stereoImg.getLeftImage(), *point, 3, CV_RGB(0, 255, 0));
    
    cvShowImage("cameraLeft", stereoImg.getLeftImage());
}

void mouseRight(int event, int x, int y, int flags, void* param)
{
    CvPoint* point;
    
    if (event != CV_EVENT_LBUTTONDOWN) return;
    
    point = (CvPoint*) param;
    
    point->x = x;
    point->y = y;
    
    cvCircle(stereoImg.getRightImage(), *point, 3, CV_RGB(0, 255, 0));
    
    cvShowImage("cameraRight", stereoImg.getRightImage());
}

void calcDistance(int xLeft, int xRight)
{
    int pixelDiff = xLeft - xRight;
    
    std::cout << "pixel distance = " << pixelDiff << std::endl;
    
    std::cout << "actual distance = " << ((/*551.20*/500 * 10) / (float) pixelDiff) << std::endl;
    
}


void showBlendedVideo()
{
    StereoCamera cam;
    StereoImage img;
    IplImage* blended;
    char c;
    bool quit = false;
    bool enableCal = false;
    
    cvNamedWindow( "blended", CV_WINDOW_AUTOSIZE );
    
    cam.open();
    //cam.enableCalibration(true);
    
    img = cam.grabFrame();
    
    
    blended = cvCreateImage(cvSize(img.getRightImage()->width, img.getRightImage()->height), img.getRightImage()->depth, 3);
    
    while (!quit)
    {   
        //cvAddWeighted(img.getLeftImage(), 0.5, img.getRightImage(), 0.5, 1, blended);
        blendImages(img, blended);

        cvShowImage("blended", blended);
        cvShowImage( "cameraLeft", img.getLeftImage());
        cvShowImage( "cameraRight", img.getRightImage());
        
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
                
            case 'c':
            
            enableCal ^= true;
            cam.enableCalibration(enableCal);
            break;
        }
        
        img = cam.grabFrame();
    }
    
    cvDestroyWindow( "blended" );
    
    
}

//
IplImage* blendImages(StereoImage img, IplImage* const blendedImage)
{
    //IplImage* blended;
    
    //blended = cvCreateImage(cvSize(img.getRightImage()->width, img.getRightImage()->height), img.getRightImage()->depth, 3);
    cvAddWeighted(img.getLeftImage(), 0.5, img.getRightImage(), 0.5, 1, blendedImage);
    
    //return blended; 
}

/*
 *
 *CamshiftDemo
 *
 */

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;
CvPoint origin;
CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int vmin = 10, vmax = 256, smin = 180;

void on_mouse( int event, int x, int y, int flags, void* param )
{
    if( !image )
        return;

    if( image->origin )
        y = image->height - y;

    if( select_object )
    {
        selection.x = MIN(x,origin.x);
        selection.y = MIN(y,origin.y);
        selection.width = selection.x + CV_IABS(x - origin.x);
        selection.height = selection.y + CV_IABS(y - origin.y);
        
        selection.x = MAX( selection.x, 0 );
        selection.y = MAX( selection.y, 0 );
        selection.width = MIN( selection.width, image->width );
        selection.height = MIN( selection.height, image->height );
        selection.width -= selection.x;
        selection.height -= selection.y;
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = cvPoint(x,y);
        selection = cvRect(x,y,0,0);
        select_object = 1;
        break;
    case CV_EVENT_LBUTTONUP:
        select_object = 0;
        if( selection.width > 0 && selection.height > 0 )
            track_object = -1;
        break;
    }
}


CvScalar hsv2rgb( float hue )
{
    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

void camshiftDemo()
{
    StereoCamera cam;
    StereoImage stImage;
    
    cam.open();

    cvNamedWindow( "Histogram", 1 );
    cvNamedWindow( "CamShiftDemo", 1 );
    cvSetMouseCallback( "CamShiftDemo", on_mouse, 0 );
    cvCreateTrackbar( "Vmin", "CamShiftDemo", &vmin, 256, 0 );
    cvCreateTrackbar( "Vmax", "CamShiftDemo", &vmax, 256, 0 );
    cvCreateTrackbar( "Smin", "CamShiftDemo", &smin, 256, 0 );

    for(;;)
    {
        IplImage* frame = 0;
        int i, bin_w, c;

        //frame = cvQueryFrame( capture );
        
        
        //if( !frame )
        //    break;
        
        stImage = cam.grabFrame();
        frame = stImage.getLeftImage();

        if( !image )
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 );
            image->origin = frame->origin;
            hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
            hue = cvCreateImage( cvGetSize(frame), 8, 1 );
            mask = cvCreateImage( cvGetSize(frame), 8, 1 );
            backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
            histimg = cvCreateImage( cvSize(320,200), 8, 3 );
            cvZero( histimg );
        }

        cvCopy( frame, image, 0 );
        cvCvtColor( image, hsv, CV_BGR2HSV );

        if( track_object )
        {
            int _vmin = vmin, _vmax = vmax;

            cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
            cvSplit( hsv, hue, 0, 0, 0 );

            if( track_object < 0 )
            {
                float max_val = 0.f;
                cvSetImageROI( hue, selection );
                cvSetImageROI( mask, selection );
                cvCalcHist( &hue, hist, 0, mask );
                cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
                cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 );
                cvResetImageROI( hue );
                cvResetImageROI( mask );
                track_window = selection;
                track_object = 1;

                cvZero( histimg );
                bin_w = histimg->width / hdims;
                for( i = 0; i < hdims; i++ )
                {
                    int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
                    CvScalar color = hsv2rgb(i*180.f/hdims);
                    cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
                                 cvPoint((i+1)*bin_w,histimg->height - val),
                                 color, -1, 8, 0 );
                }
            }

            cvCalcBackProject( &hue, backproject, hist );
            cvAnd( backproject, mask, backproject, 0 );
            cvCamShift( backproject, track_window,
                        cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
                        &track_comp, &track_box );
            track_window = track_comp.rect;
            
            if( backproject_mode )
                cvCvtColor( backproject, image, CV_GRAY2BGR );
            if( !image->origin )
                track_box.angle = -track_box.angle;
            //cvEllipseBox( image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
            
            /*My Code Here*/
            
            // get max, and min co-ordinates
            int iMaxx = (int) (track_box.center.x + (track_box.size.width / 2));
            int iMinx = (int) (track_box.center.x - (track_box.size.width / 2));
            int iMaxy = (int) (track_box.center.y + (track_box.size.height / 2));
            int iMiny = (int) (track_box.center.y - (track_box.size.height / 2));
            
//            std::cout << "max x:" << iMaxx << std::endl;
//            std::cout << "min x:" << iMinx << std::endl;
//            std::cout << "max y:" << iMaxy << std::endl;
//            std::cout << "min y:" << iMiny << std::endl;
            
            CvRect rect;
            
            
            rect.width = (int) track_box.size.width;
            rect.height = (int) track_box.size.height;
            rect.x = iMinx;
            rect.y = iMiny;
            
            
            cvRectangle(image, cvPoint(iMinx , iMiny ), cvPoint ( iMaxx, iMaxy ), CV_RGB(0, 0, 255), 1, 8, 0);
            
            
            
            /*CBlobResult res;
            CBlob blob;
            IplImage* blobImage = 0;
            
            if (blobImage != 0) cvReleaseImage(&blobImage);
                
                blobImage = convertToBW(image);
                
                cvSetImageROI(blobImage, rect);
                
                res = CBlobResult(blobImage, NULL, 50, true);
                
                cvResetImageROI(blobImage);
                
                //res.Filter( res, B_INCLUDE, CBlobGetArea(), B_LESS, 1000 );// area <150
                //res.Filter( res, B_INCLUDE, CBlobGetArea(), B_GREATER, 0 );// area <150
                //res.Filter( res, B_INCLUDE, CBlobGetArea(), B_GREATER, 500 );
                
                std::cout << "blob count:" << res.GetNumBlobs() << std::endl;
                
                for (int i = 0; i < res.GetNumBlobs(); i++)
                {
                    blob = res.GetBlob(i);
                    
                    // get max, and min co-ordinates
                    int blobiMaxx = (int) blob.MaxX();
                    int blobiMinx = (int) blob.MinX();
                    int blobiMaxy = (int) blob.MaxY();
                    int blobiMiny = (int) blob.MinY();
                    
                    // find the average of the blob (i.e. estimate its centre)
                    int blobiMeanx=(iMinx+iMaxx)/2;
                    int blobiMeany=(iMiny+iMaxy)/2;
                    // mark centre
                    cvLine( blobImage, cvPoint(blobiMeanx, blobiMeany), cvPoint(blobiMeanx, blobiMeany), CV_RGB(255, 0 , 0), 4, 8, 0 );
                    // mark box around blob
                    cvRectangle(blobImage, cvPoint(blobiMinx , blobiMiny ), cvPoint ( blobiMaxx, blobiMaxy ), CV_RGB(0, 0, 255), 1, 8, 0);

                    //blob.FillBlob(stereoImg.getLeftImage(), CV_RGB(255, i, 0));
                }
                
                cvShowImage("cameraLeft", blobImage);
             *
             */
            
            
            /*End My Code*/
            
        }
        
        if( select_object && selection.width > 0 && selection.height > 0 )
        {
            cvSetImageROI( image, selection );
            cvXorS( image, cvScalarAll(255), image, 0 );
            cvResetImageROI( image );
        }

        cvShowImage( "CamShiftDemo", image );
        cvShowImage( "Histogram", histimg );

        c = cvWaitKey(10);
        if( (char) c == 27 )
            break;
        switch( (char) c )
        {
        case 'b':
            backproject_mode ^= 1;
            break;
        case 'c':
            track_object = 0;
            cvZero( histimg );
            break;
        case 'h':
            show_hist ^= 1;
            if( !show_hist )
                cvDestroyWindow( "Histogram" );
            else
                cvNamedWindow( "Histogram", 1 );
            break;
        default:
            ;
        }
    }

    //cvReleaseCapture( &capture );
    
    cam.close();
    cvDestroyWindow("CamShiftDemo");

    //return 0;
}

