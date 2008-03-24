// 
// File:   main.cc
// Author: nick
//
// Created on 12 January 2008, 13:59
//
// This project is currently test code written to experiment with various libraries.
// It's poorly written at best.

#include <stdlib.h>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <string>
#include <sstream>
#include <cstdlib>

#include <unistd.h>

//#include "wx/wx.h"

//#include "dbxml/DbXml.hpp"

#include "VideoWriter.h"
#include "VideoReader.h"
#include "StereoCamera.h"
#include "DataloggerRecord.h"
#include "Datalogger.h"
#include "RedDotExperiment.h"
#include "HelperFunctions.h"

#include "TelemetryCalculator.h"
#include "TelemetryData.h"
#include "ImpactPredictor.h"

#define SINGLE_CAM
//#define PLAYBACK
#define LEFT_IMAGE_FILENAME "/home/nick/stereoTestImages/leftImage.avi"
#define RIGHT_IMAGE_FILENAME "/home/nick/stereoTestImages/rightImage.avi"
#define PLAYBACK_FILENAME "/home/nick/playbackfile.avi"
#define DISPARITY_FILENAME "/home/nick/disaparity.avi"
#define CAM_CALIBRATION_FILE "/home/nick/cam_calibration.ini"
#define TOTAL_FRAMES 30
#define CALIBRATION_FRAMES 20

using std::string;  
using std::cout;
using std::endl;

//using namespace DbXml;

const int MAX_DISPARITY = 21;

//function prototypes

int test1();
int test2();
int testStereo();

void disparitySlider(int x);
void predictionTest();

void run();
void depthTest();
void singleRun();


void dataLoggerTest();

IplImage* runBirchfield(IplImage* r, IplImage* l);


//IplImage* convertToBW(IplImage* image);
void runCalibration();

void writePGMFile(IplImage* image, string filename);

/**
 *This records left and images from each camera sequentially
 */
void recordStereoVideo(int numFrames, string leftFilename, string rightFilename);

/**
 *This function calculates the disparity map for 2 images
 */
IplImage* calculateDisparity(IplImage* rightImage, IplImage* leftImage, int maxDisp = MAX_DISPARITY);


/*
 *Extracts the requested number of images from the requested filename.
 *Images are extracted from the beginning of the file.
 */
void extractImageFromAVI(string aviName, string imagePrefix, int numOfImages);

//
// 
//
int main(int argc, char** argv) 
{
    //create the test windows
    cvNamedWindow( "cameraLeft", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "cameraRight", CV_WINDOW_AUTOSIZE );
    
    cvMoveWindow("cameraLeft", 0, 0);
     
    //runCalibration();
    //run2();
    //singleRun();
    
    //depthTest();
    
    //dataLoggerTest();
    
    //predictionTest();
    
    //cBoardTest();
    //showBlendedVideo();
    
    camshiftDemo();
    
    cvDestroyWindow( "cameraLeft" );
    cvDestroyWindow( "cameraRight" );
    
    return (EXIT_SUCCESS);
}


void depthTest()
{
    int pos = 21;
    int param1 = 1;
    int param2 = 1;
    int param3 = 1;
    int param4 = 1;
    int param5 = 1;
    //string in = "";
    char input;
    
    bool quit = false;
    bool calc = false;
    bool runBirch = false;
    bool enableCal = false;
    
    cvNamedWindow( "disparity", CV_WINDOW_AUTOSIZE );
    cvCreateTrackbar("disparitySlider", "disparity", &pos, 255, NULL);
    
    IplImage* leftFrame = 0;
    IplImage* rightFrame = 0;
    IplImage* disparityFrame = 0;
    
    StereoCamera cam;
    StereoImage image;
    
    cam.enableCalibration(false);
    
    cam.open();
    
    while (!quit)
    {
        //std::cin >> in;
        input = cvWaitKey(1000);
        
        switch (input)
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
                
            case 'r':
                
                calc = true;
                break;
                
            case 'b':
                runBirch ^= true;
                
                break;
                
            case 'c':
                
                enableCal ^= true;
                cam.enableCalibration(enableCal);
                
                break;
                
            default:
                break;
        }
        
        image = cam.grabFrame();
    
        leftFrame = image.getLeftImage();
        rightFrame = image.getRightImage();
    
        leftFrame = convertToBW(image.getLeftImage());
        rightFrame = convertToBW(image.getRightImage());

        //cvEqualizeHist(leftFrame, leftFrame);
        //cvEqualizeHist(rightFrame, rightFrame);

        //cvSmooth(leftFrame, leftFrame, CV_BLUR, 3, 3);
        //cvSmooth(rightFrame, rightFrame, CV_BLUR, 3, 3);

        //cvSaveImage("left_new.jpg", leftFrame);
        //cvSaveImage("right_new.jpg", rightFrame);

        cvShowImage( "cameraLeft", leftFrame);
        cvShowImage( "cameraRight", rightFrame);

        //cvWaitKey(2000);
        
        //if (calc == false) continue;
        
        if (disparityFrame != 0) cvReleaseImage(&disparityFrame);

        if (runBirch)
        {
            disparityFrame = runBirchfield(leftFrame, rightFrame); //calculateDisparity(rightFrame, leftFrame, pos);
        }
        else
        {
            disparityFrame = calculateDisparity(rightFrame, leftFrame, pos); 
        }
        
        cvConvertScale(disparityFrame, disparityFrame, 255.f / pos);

        //cvSaveImage("disparity.jpg", disparityFrame);*/
        cvShowImage("disparity", disparityFrame);
        //cvShowImage("cameraLeft", leftFrame);
        //cvShowImage("cameraRight", rightFrame);
        
        calc = false;
        
        //cvWaitKey(1000);
        //sleep(1000);
    }
    
    cam.close();
    
}

void run()
{
    VideoReader rightCam;
    VideoReader leftCam;
    VideoWriter disparityRecorder;
    IplImage* leftFrame = 0;
    IplImage* rightFrame = 0;
    IplImage* leftGrey = 0;
    IplImage* rightGrey = 0;
    IplImage* dispImage = 0;
    CvCalibFilter calibFilter;
    
    calibFilter.LoadCameraParams(CAM_CALIBRATION_FILE);
    
    rightCam.open(RIGHT_IMAGE_FILENAME);
    leftCam.open(LEFT_IMAGE_FILENAME);
    
    
    rightFrame = rightCam.readFrame();
    leftFrame = leftCam.readFrame();
    
    if (rightFrame != 0)
    {
        disparityRecorder.create(DISPARITY_FILENAME, cvSize(rightFrame->width,
                                                     rightFrame->height));
    }
    else
    {
        rightCam.close();
        leftCam.close();
        return;
    }
    
    while (rightFrame != 0 && leftFrame != 0)
    {
        rightGrey = convertToBW(rightFrame);
        leftGrey = convertToBW(leftFrame);
        
        dispImage = calculateDisparity(rightGrey, leftGrey);
        disparityRecorder.writeFrame(dispImage);
        
        //clean up greyscale image data
        cvReleaseImage(&leftGrey);
        cvReleaseImage(&rightGrey);
        cvReleaseImage(&dispImage);
        
        rightFrame = rightCam.readFrame();
        leftFrame = leftCam.readFrame();  
    }
    
    rightCam.close();
    leftCam.close();
    disparityRecorder.close();
    
}

IplImage* calculateDisparity(IplImage* rightImage, IplImage* leftImage, int maxDisp)
{
    IplImage* disparityImage;
    
    disparityImage = cvCreateImage(cvSize(rightImage->width, rightImage->height), IPL_DEPTH_8U, 1);
    disparityImage->origin = rightImage->origin;
    
    cvFindStereoCorrespondence(rightImage, leftImage, CV_DISPARITY_BIRCHFIELD, 
                               disparityImage, maxDisp /*, 25, 3, 6, 8, 15*/ );
    
    
    //@TODO this memory may leak
    return disparityImage;
}

/*IplImage* convertToBW(IplImage* image)
{
    IplImage* bwImage = 0;
    
    bwImage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
    bwImage->origin = image->origin;
    
    cvCvtColor(image, bwImage, CV_BGR2GRAY);
    
    return bwImage;
}*/


/**
 *@TODO I need to extract this out into its own class
 */

void runCalibration()
{
    StereoCamera cam;
    
    if (cam.calibrateCamera())
    {
        cout << "Camera is calibrated" << endl;
    }
    else
    {
        cout << "Camera did not calibrate" << endl;
    }
    
    
    
//    //const int CALIBRATION_FRAMES = 50;
//    double ETALON_PARAMS[3] = {7, 9, 2}; //horizontal, vertical, size in ???
//    //const string CAM_CALIBRATION_FILE = "/home/nick/cam_calibration.txt";
//    const string RIGHT_CAL_VID = "/home/nick/right_calibration.avi";
//    const string LEFT_CAL_VID = "/home/nick/left_calibration.avi";
//    
//    bool etalonFound = false;
//    
//    CvCalibFilter calibFilter;
//    
//    IplImage* images[2]; //= { image1, image2 };
//    
//    VideoWriter writer;
//    VideoReader leftReader;
//    VideoReader rightReader;
//    
//    calibFilter.SetEtalon(CV_CALIB_ETALON_CHESSBOARD, ETALON_PARAMS);
//    calibFilter.SetCameraCount(2);
//    calibFilter.SetFrames(CALIBRATION_FRAMES);
//    
//    leftReader.open(LEFT_CAL_VID);
//    rightReader.open(RIGHT_CAL_VID);
//    
//    images[0] = leftReader.readFrame();
//    images[1] = rightReader.readFrame();
//    
//    writer.create("/home/nick/etalon_vid.avi", cvSize(images[0]->width, 
//                                                      images[0]->height));
//    int frameCount = 0;
//    while (images[0] != 0 && images[1] != 0 && frameCount < CALIBRATION_FRAMES)
//    {
//        etalonFound = calibFilter.FindEtalon(images);
//        
//        if (etalonFound)
//        {
//            calibFilter.DrawPoints(images);
//            
//            //will need to add an if to check if the push is successfull.
//            //calibration only occurs once total frames have had etalon found
//            calibFilter.Push();
//            frameCount++;
//            
//            //write the left frame
//            writer.writeFrame(images[0]);
//            
//            if (calibFilter.IsCalibrated())
//            {
//                break;
//            }
//        }
//        else
//        {
//            std::cout << "Etalon not found for image set" << std::endl; 
//        }
//        
//        images[0] = leftReader.readFrame();
//        images[1] = rightReader.readFrame();
//        
//    }
//    
//    if (!calibFilter.IsCalibrated())
//    {
//        std::cout << "Cameras are not calibrated" << std::endl;
//    }
//    //calibFilter.SaveCameraParams(CAM_CALIBRATION_FILE);
//    if (!calibFilter.SaveCameraParams("/home/nick/calib.ini"))
//    {
//        std::cout << "Camera Parameters not saved" << std::endl;
//    }
//    writer.close();
//    leftReader.close();
//    rightReader.close();
    
    
}

void recordStereoVideo(int numFrames, string leftFilename, string rightFilename)
{
    VideoWriter writer;
    CvCapture* capture = 0;
    IplImage* image = 0;
    
    capture = cvCaptureFromCAM(0);
    
    
    //wait a couple of seconds for the camera to adjust brightness
    for (int i = 0; i < 90; i++)
    {
        //just waiting so dont bother getting the memory reference
        cvQueryFrame( capture );
        cvWaitKey(20);
    }
    
    for (int i = 0; i < numFrames; i++)
    {
        image = cvQueryFrame( capture );
        
        if (i == 0)
        {
            writer.create(leftFilename, cvSize(image->width,image->height));
        }
        
        writer.writeFrame(image);
        cvShowImage( "cameraLeft", image);
        cvWaitKey(20);
        
    }
    
    cvReleaseCapture(&capture);
    writer.close();
    
    capture = cvCaptureFromCAM(1);
    
    //wait a couple of seconds for the camera to adjust brightness
    for (int i = 0; i < 90; i++)
    {
        //just waiting so dont bother getting the memory reference
        cvQueryFrame( capture );
        cvWaitKey(20);
    }
    
    for (int i = 0; i < numFrames; i++)
    {
        image = cvQueryFrame( capture );
        
        if (i == 0)
        {
            writer.create(rightFilename, cvSize(image->width,image->height));
        }
        
        writer.writeFrame(image);
        cvShowImage( "cameraRight", image);
        cvWaitKey(20);
        
    }
    
    
    cvReleaseCapture(&capture);
    writer.close();
    
}

/*
 *TODO I've added some functionality here that also uses external programs to convert
 *images. Eventually this will need to be internalised.
 */
void extractImageFromAVI(string aviName, string imagePrefix, int numOfImages)
{
    IplImage* image;
    VideoReader reader;
    std::stringstream imageName;
    std::stringstream exe;
    
    reader.open(aviName);
    
    for (int i = 0; i < numOfImages; i++)
    {
        imageName.str();
        exe.str();
        
        image = reader.readFrame();
        
        //save image as jpeg
        imageName << "/home/nick/stereoTestImages/" << imagePrefix << "TestImage" << i << ".jpg";
        cvSaveImage(imageName.str().c_str(), image);
        
        //convert to png
        exe << "jpegtopnm " << imageName.str() << " | ppmtopgm > /home/nick/stereoTestImages/new_" << imagePrefix << i << ".pgm";
        std::system(exe.str().c_str());
    }
    
    reader.close();
}

//need to remove the window create/destroy calls
int test1()
{
    VideoWriter writer;
    
    CvCapture* captureL = cvCaptureFromCAM(0);

    if( !captureL ) 
    {
        fprintf( stderr, "ERROR: capture is NULL \n" );
	    //getchar();
        return -1;
    }
	  
    #ifndef SINGLE_CAM
    CvCapture* captureR = cvCaptureFromCAM(1);

    if( !captureR ) 
    {
          cvReleaseCapture( &captureL );
          fprintf( stderr, "ERROR: capture is NULL \n" );
          //getchar();
          return -1;
    }
    #endif
	
    // Create a window in which the captured images will be presented
    cvNamedWindow( "cameraLeft", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "cameraRight", CV_WINDOW_AUTOSIZE );

    cvMoveWindow("cameraRight", 400, 50);
	 
	  
    // Show the image captured from the camera in the window and repeat
    for (int i = 0; i <  300; i++) //30 frames * 10 seconds 
    {
        // Get one frame from camera 1
        IplImage* frameL = cvQueryFrame( captureL );
        if( !frameL ) 
        {
          fprintf( stderr, "ERROR: frame is null...\n" );
          getchar();
          break;
        }

        cvShowImage( "cameraLeft", frameL);
        // Do not release the frame!

        if (i == 0)
        {
            writer.create(LEFT_IMAGE_FILENAME, cvSize(frameL->width,frameL->height));
        }

        writer.writeFrame(frameL);
		
        #ifndef SINGLE_CAM
        // Get one frame from camera 2
        IplImage* frameR = cvQueryFrame( captureR );
        if( !frameR ) 
        {
          fprintf( stderr, "ERROR: frame is null...\n" );
          getchar();
          break;
        }

        cvShowImage( "camera2", frameR);
        // Do not release the frame!
        #endif
        
        /*IplImage* frameL = 0;
        IplImage* frameR = 0;
        
        cvGrabFrame(captureL);
        cvGrabFrame(captureR);
        
        frameL = cvRetrieveFrame(captureL);
        frameR = cvRetrieveFrame(captureR);
        
        cvShowImage( "cameraLeft", frameL);
        cvShowImage( "cameraRight", frameR);*/
        
        //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
        //remove higher bits using AND operator
		
	if( (cvWaitKey(10) & 255) == 27 ) break;
        
    }
	  
    //writer.close();

    // Release the capture device housekeeping
    cvReleaseCapture( &captureL );
    cvDestroyWindow( "cameraLeft" );

    #ifndef SINGLE_CAM
    cvReleaseCapture( &captureR );
    #endif
    
    //playback the captured image
    #ifdef PLAYBACK
    CvCapture* playback = 0;
    IplImage* img = 0;
    
    playback = cvCaptureFromFile(LEFT_IMAGE_FILENAME);
    
    //img = cvQueryFrame( playback );
    
    while (cvGrabFrame(playback))
    {
        img = cvRetrieveFrame(playback);
        cvShowImage( "cameraRight", img);
        cvWaitKey(20);
    }
    
    //while (img != 0)
    //{
    //    cvShowImage( "cameraRight", img);
    //    img = cvQueryFrame(playback);
    //}
    
    
    cvReleaseCapture(&playback);
    #endif
    
    cvDestroyWindow( "cameraRight" );
    
    return 1;
}


int test2()
{
    CvCapture* captureL = 0; //cvCaptureFromCAM(0);
    CvCapture* captureR = 0;//cvCaptureFromCAM(1);
    IplImage* frameL = 0;
    IplImage* frameR = 0;
    IplImage* leftFrame;
    IplImage* rightFrame;
    
    // Create a window in which the captured images will be presented
    cvNamedWindow( "cameraLeft", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "cameraRight", CV_WINDOW_AUTOSIZE );

    cvMoveWindow("cameraRight", 400, 50);
    
    
    for (int i = 0; i <  (30 * 10); i++) //30 frames * 10 seconds
    {
        
        captureL = cvCaptureFromCAM(0);
        
        if( !captureL ) 
        {
            fprintf( stderr, "ERROR: capture is NULL \n" );
            return -1;
        }

        cvGrabFrame(captureL);
        frameL = cvRetrieveFrame(captureL);
        leftFrame = cvCloneImage(frameL);
        
        cvReleaseCapture(&captureL);
        
        //cature the right frame
        captureR = cvCaptureFromCAM(1);

        if( !captureR ) 
        {
              //cvReleaseCapture( &captureL );
              fprintf( stderr, "ERROR: capture is NULL \n" );
              return -1;
        }
        
        cvGrabFrame(captureR);
        frameR = cvRetrieveFrame(captureR);
        rightFrame = cvCloneImage(frameR);
        
        cvReleaseCapture(&captureR);
        
        cvShowImage( "cameraLeft", leftFrame);
        cvShowImage( "cameraRight", rightFrame);
        
        cvWaitKey(20);
        
        cvReleaseImage(&leftFrame);
        cvReleaseImage(&rightFrame);
    }
    
}

int testStereo()
{
    CvCapture* captureL = 0; //cvCaptureFromCAM(0);
    CvCapture* captureR = 0;//cvCaptureFromCAM(1);
    IplImage* frameL = 0;
    IplImage* frameR = 0;
    
    cvSetCaptureProperty(captureL, CV_CAP_PROP_FRAME_WIDTH, 320);
    cvSetCaptureProperty(captureL, CV_CAP_PROP_FRAME_HEIGHT, 240);

    
    captureL = cvCaptureFromCAM(0);
    
        
    if( !captureL ) 
    {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        return -1;
    }
    
    //cout << cvGetCaptureProperty(captureL, CV_CAP_PROP_FPS) << endl;
    //cout << cvGetCaptureProperty(captureL, CV_CAP_PROP_FRAME_WIDTH) << endl;
    //cout << cvGetCaptureProperty(captureL, CV_CAP_PROP_FRAME_HEIGHT) << endl;
    //cout << cvGetCaptureProperty(captureL, CV_CAP_PROP_FOURCC) << endl; 
    
    
    
    captureR = cvCaptureFromCAM(1);

    if( !captureR ) 
    {
          //cvReleaseCapture( &captureL );
          fprintf( stderr, "ERROR: capture is NULL \n" );
          cvReleaseCapture(&captureL);
          return -1;
    }
    
    
    for (int i = 0; i <  (30 * 10); i++) //30 frames * 10 seconds
    {
        
        frameL = cvQueryFrame( captureL );
        frameR = cvQueryFrame( captureR );        
        
        cvShowImage( "cameraLeft", frameL);
        cvShowImage( "cameraRight", frameR);
        
        cvWaitKey(20);
    }
    
    cvReleaseCapture(&captureL);
    //cvReleaseCapture(&captureR);
    
    return 1;
}

void writePGMFile(IplImage* image, string filename)
{
    std::stringstream imageName;
    std::stringstream exe;
    
    
    imageName.str();
    exe.str();

    //save image as jpeg
    //imageName << "/home/nick/stereoTestImages/" << filename << ".jpg";
    imageName << "/home/nick/NetBeansProjects/ApplicationTest_1/pictures/" << filename << ".jpg";
    //imageName << filename << ".jpg";
    cvSaveImage(imageName.str().c_str(), image);

    //convert to png
    //exe << "jpegtopnm " << imageName.str() << " | ppmtopgm > /home/nick/stereoTestImages/" << filename << ".pgm";
    exe << "jpegtopnm " << imageName.str() << " | ppmtopgm > " << filename << ".pgm";
    std::system(exe.str().c_str());
}


void predictionTest()
{
    int distance = 0;
    //int distance2 = 0;
    char input;
    
    TelemetryCalculator calc;
    TelemetryData data;
    ImpactPredictor predictor;
    
    bool quit = false;
    bool pause = false;
    
    cvNamedWindow( "TelemetryTest", CV_WINDOW_AUTOSIZE );
    cvCreateTrackbar("distance", "TelemetryTest", &distance, 300, NULL);
    
    while (!quit)
    {
        //distance++;
        //std::cin >> in;
        input = cvWaitKey(5000);
        
        switch (input)
        {
            case 'q':
                
                quit = true;
                break;
                
            case 'p':
                pause ^= true;
                break;
                
            case 'c':
                pause = false;
                break;
                
            default:
                break;
        }
        
        if (pause) continue;
        
        calc.calculate(5, (double) distance);
        
        data = calc.getTelemetry();
        
        cout << "Distance:" << data.getDistance() << " Velocity:" << data.getVelocity() << " Acceleration:" << data.getAcceleration() << endl;
        cout << "Time to impact:" << predictor.getTimeToImpact(data) << endl;
        cout << endl;
        //predictor.getTimeToImpact(data);
        
    }
}

void disparitySlider(int x)
{
    
    
    
    
}

void singleRun()
{
    //recordStereoVideo(TOTAL_FRAMES, LEFT_IMAGE_FILENAME, RIGHT_IMAGE_FILENAME);
    //recordStereoVideo(CALIBRATION_FRAMES, "/home/nick/left_calibration.avi", "/home/nick/right_calibration.avi");
    
    //runCalibration();
    
    //run();
    
    //testStereo();
    
    
//    cvWaitKey(5000);
    
    StereoCamera cam;
    StereoImage image;
    
//    if (cam.calibrateCamera())
//    {
//        cout << "Camera is calibrated" << endl;
//    }
//    else
//    {
//        cout << "Camera did not calibrate" << endl;
//    }
    
    //cam.enableCalibration(true);
    
    cam.open();
    
    image = cam.grabFrame();
    
    IplImage* leftFrame = 0;
    IplImage* rightFrame = 0;
    IplImage* disparityFrame = 0;
    
    //leftFrame = cvLoadImage("b1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    //rightFrame = cvLoadImage("b2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    
    cvSaveImage("/home/nick/openVis/left.png", image.getLeftImage());
    cvSaveImage("/home/nick/openVis/right.png", image.getRightImage());
    
    leftFrame = convertToBW(image.getLeftImage());
    rightFrame = convertToBW(image.getRightImage());
    
    //cvEqualizeHist(leftFrame, leftFrame);
    //cvEqualizeHist(rightFrame, rightFrame);
    
    //cvSmooth(leftFrame, leftFrame, CV_BLUR, 3, 3);
    //cvSmooth(rightFrame, rightFrame, CV_BLUR, 3, 3);
    
    
    writePGMFile(leftFrame, "left");
    writePGMFile(rightFrame, "right");
    
    //cvSaveImage("left_new.jpg", leftFrame);
    //cvSaveImage("right_new.jpg", rightFrame);
    
    //cvShowImage( "cameraLeft", leftFrame);
    //cvShowImage( "cameraRight", rightFrame);
    
    //cvWaitKey(2000);
    
    disparityFrame = calculateDisparity(rightFrame, leftFrame);
    
    cvConvertScale(disparityFrame, disparityFrame, 255.f / MAX_DISPARITY);
    
    cvSaveImage("disparity.jpg", disparityFrame);
    
    
    //for running the external birchfield algorithm
    //extractImageFromAVI(LEFT_IMAGE_FILENAME, "left", 1);
    //extractImageFromAVI(RIGHT_IMAGE_FILENAME, "right", 1);
    
}

IplImage* runBirchfield(IplImage* r, IplImage* l)
{
    //IplImage* leftFrame;
    //IplImage* rightFrame;
    IplImage* disparityImage;
    
    //leftFrame = convertToBW(l);
    //rightFrame = convertToBW(r);
    
    writePGMFile(l, "birchLeft");
    writePGMFile(r, "birchRight");
    
    cvWaitKey(100);
    
    //std::system("/home/nick/stereoTestImages/p2p4webcam -b birchLeft.pgm birchRight.pgm");
    std::system("./p2p4webcam -b birchLeft.pgm birchRight.pgm");
    
    disparityImage = cvLoadImage("dm.pgm");
    
    //cvConvertScale(disparityImage, disparityImage, 255.f / MAX_DISPARITY);
    
    //cvReleaseImage(&leftFrame);
    //cvReleaseImage(&rightFrame);
    
    //cvWaitKey(5000);
    
    return disparityImage;
}


void dataLoggerTest()
{
    Datalogger logger;
    DataloggerRecord rec;
    
    logger.openLog();
    
    rec.setValues(21, 22, 23, 24, 25);
    
    //logger.eraseLogfile();
    
    //logger.createNewLog("test2log.xml");
    
    logger.addRecord(rec);
    
    cout << logger.retrieveLog() << endl;

    logger.closeLog();
}

/*class MyFrame : public wxFrame
{
    
};*/








