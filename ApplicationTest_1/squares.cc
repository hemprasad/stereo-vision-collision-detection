//
// The full "Square Detector" program.
// It loads several images subsequentally and tries to find squares in
// each image
//
//#ifdef _CH_
//#pragma package <opencv>
//#endif

//#include "cv/include/cv.h"

#include <highgui.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "squares.h"


int thresh = 50;
IplImage* img = 0;
IplImage* img0 = 0;
//CvMemStorage* storage = 0;
//const char* wndname = "Square Detection Demo";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2 
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage )
{
    CvSeq* contours;
    int i, c, l, N = 5;
    CvSize sz = cvSize( img->width & -2, img->height & -2 );
    IplImage* timg = cvCloneImage( img ); // make a copy of input image
    IplImage* gray = cvCreateImage( sz, 8, 1 ); 
    IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
    IplImage* tgray;
    CvSeq* result;
    double s, t;
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );
    
    // select the maximum ROI in the image
    // with the width and height divisible by 2
    cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));
    
    //NICK: move in from the edge of the image to remove calibration artifacts
    //cvSetImageROI( timg, cvRect( 50, 50, sz.width - 100, sz.height - 100 ));
    
    // down-scale and upscale the image to filter out the noise
    cvPyrDown( timg, pyr, 7 );
    cvPyrUp( pyr, timg, 7 );
    
    tgray = cvCreateImage( sz, 8, 1 );
    
    // find squares in every color plane of the image
    for( c = 0; c < 3; c++ )
    {
        // extract the c-th color plane
        cvSetImageCOI( timg, c+1 );
        cvCopy( timg, tgray, 0 );
        
        // try several threshold levels
        for( l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading   
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging) 
                cvCanny( tgray, gray, 0, thresh, 5 );
                // dilate canny output to remove potential
                // holes between edge segments 
                cvDilate( gray, gray, 0, 1 );
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
            }
            
            // find contours and store them all as a list
            cvFindContours( gray, storage, &contours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
            
            // test each contour
            while( contours )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                result = cvApproxPoly( contours, sizeof(CvContour), storage,
                    CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( result->total == 4 &&
                    fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
                    cvCheckContourConvexity(result) )
                {
                    s = 0;
                    
                    for( i = 0; i < 5; i++ )
                    {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 )
                        {
                            t = fabs(angle(
                            (CvPoint*)cvGetSeqElem( result, i ),
                            (CvPoint*)cvGetSeqElem( result, i-2 ),
                            (CvPoint*)cvGetSeqElem( result, i-1 )));
                            s = s > t ? s : t;
                        }
                    }
                    
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence 
                    if( s < 0.3 )
                        for( i = 0; i < 4; i++ )
                            cvSeqPush( squares,
                                (CvPoint*)cvGetSeqElem( result, i ));
                }
                
                // take the next contour
                contours = contours->h_next;
            }
        }
    }
    
    // release all the temporary images
    cvReleaseImage( &gray );
    cvReleaseImage( &pyr );
    cvReleaseImage( &tgray );
    cvReleaseImage( &timg );
    
    return squares;
}


// the function draws all the squares in the image
void drawSquares( IplImage* img, CvSeq* squares )
{
    CvSeqReader reader;
    //IplImage* cpy = cvCloneImage( img );
    int i;
    double dist;

	//int distX, distY, sqX, sqY;
    
    // initialize reader of the sequence
    cvStartReadSeq( squares, &reader, 0 );
    
    // read 4 sequence elements at a time (all vertices of a square)
    for( i = 0; i < squares->total; i += 4 )
    {
        CvPoint pt[4], *rect = pt;
        int count = 4;
        
        // read 4 vertices
        CV_READ_SEQ_ELEM( pt[0], reader );
		printf("point1 = (%d,%d)\n", pt[0].x, pt[0].y);
        CV_READ_SEQ_ELEM( pt[1], reader );
		printf("point2 = (%d,%d)\n", pt[1].x, pt[1].y);
        CV_READ_SEQ_ELEM( pt[2], reader );
		printf("point3 = (%d,%d)\n", pt[2].x, pt[2].y);
        CV_READ_SEQ_ELEM( pt[3], reader );
		printf("point4 = (%d,%d)\n", pt[3].x, pt[3].y);
		
        //distX = (pt[0].x-pt[1].x)*(pt[0].x-pt[1].x);
        //distY = (pt[0].y-pt[1].y)*(pt[0].y-pt[1].y);
        //sqX = distX*distX;
        //sqY = distY*distY;

        dist=sqrt( ((pt[0].x-pt[1].x)*(pt[0].x-pt[1].x)) + ((pt[0].y-pt[1].y)*(pt[0].y-pt[1].y)) );
        printf("dist=%lf\n", dist);
        
        // draw the square as a closed polyline 
        //if(dist>145 && dist<165)
        {
            cvPolyLine( img, &rect, &count, 1, 1, CV_RGB(0,255,0), 1, CV_AA, 0 );
        }
    }
    
    // show the resultant image
    //cvShowImage( wndname, cpy );
    //cvReleaseImage( &cpy );
}


//char* names[] = { "tray.jpg", 0 }; //"tray2.jpg", "tray3.jpg", "tray4.jpg", 0 };

//int main(int argc, char** argv)
//{
//	int i, c;
//
//	CvCapture* capture = 0;
//    
//    if( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
//        capture = cvCaptureFromCAM( argc == 2 ? argv[1][0] - '0' : 0 );
//    else if( argc == 2 )
//        capture = cvCaptureFromAVI( argv[1] );
//
//    if( !capture )
//    {
//        fprintf(stderr,"Could not initialize capturing...\n");
//        return -1;
//    }
//
//	// create memory storage that will contain all the dynamic data
//    storage = cvCreateMemStorage(0);
//	cvNamedWindow( wndname, 0 );
//	for(;;){
//    //for( i = 0; names[i] != 0; i++ )
//    //{
//    //    // load i-th image
//    //    img0 = cvLoadImage( names[i], 1 );
//    //    if( !img0 )
//    //    {
//    //        printf("couldn't load %s\n", names[i] );
//    //        continue;
//    //    }
//    //    img = cvCloneImage( img0 );
//
//		IplImage* frame = 0;
//        
//        frame = cvQueryFrame( capture );
//        if( !frame )
//            break;
//		img = cvCloneImage( frame );
//
//        // create window and a trackbar (slider) with parent "image" and set callback
//        // (the slider regulates upper threshold, passed to Canny edge detector) 
//        //cvNamedWindow( wndname, 1 );
//        
//        // find and draw the squares
//        drawSquares( img, findSquares4( img, storage ) );
//        		
//
//        // wait for key.
//        // Also the function cvWaitKey takes care of event processing
//        c = cvWaitKey(0);
//        // release both images
//        //cvReleaseImage( &img );
//        //cvReleaseImage( &img0 );
//        // clear memory storage - reset free space position
//        cvClearMemStorage( storage );
//
//        if( (char)c == 27 )
//            break;
//    }
//	cvReleaseCapture( &capture );
//    cvDestroyWindow( wndname );
//    return 0;
//}
