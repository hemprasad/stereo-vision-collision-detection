#include "StereoCamera.h"
#include <iostream>


StereoCamera::StereoCamera()
{
    m_rightCam = 0;
    m_leftCam = 0;
    
    m_calibIsEnabled = false;
    
}


StereoCamera::~StereoCamera()
{
    
}
    
void StereoCamera::open()
{
    m_leftCam = cvCaptureFromCAM(LEFT_CAMERA_ID);
    
    if (m_leftCam == 0)
    {
        //TODO throw exception
    }
    
    m_rightCam = cvCaptureFromCAM(RIGHT_CAMERA_ID);
    
    if (m_rightCam == 0)
    {
        cvReleaseCapture(&m_leftCam);
        //TODO throw exception
    }
    
    //wait a couple of seconds for the cameras brightness to adjust
    
    /*for(int i = 0; i < CAMERA_ADJUSTMENT_TIME; i++)
    {
        grabFrame();
    }*/
}


void StereoCamera::close()
{
    cvReleaseCapture(&m_leftCam);
    cvReleaseCapture(&m_rightCam);
}


StereoImage StereoCamera::grabFrame()
{
    StereoImage image;
    
    if (m_calibIsEnabled)
    {
        IplImage* images[2];
        
        images[0] = cvQueryFrame(m_leftCam);
        images[1] = cvQueryFrame(m_rightCam);
        
        m_calibFilter.Undistort(images, images);
        m_calibFilter.Rectify(images, images);
        
        image.setLeftImage(images[0]);
        image.setRightImage(images[1]);
        
    }
    else
    {
    
        image.setLeftImage(cvQueryFrame(m_leftCam));
        image.setRightImage(cvQueryFrame(m_rightCam));
    }
    
    return image;
}

bool StereoCamera::calibrateCamera()
{
    double ETALON_PARAMS[3] = {7, 9, 2}; //horizontal, vertical, size in ???
    StereoImage frame;
    bool etalonFound = false;
    //CvCalibFilter m_m_calibFilter;
    int successFrameCount = 0;
    IplImage* images[2];
    
    m_calibFilter.SetEtalon(CV_CALIB_ETALON_CHESSBOARD, ETALON_PARAMS);
    m_calibFilter.SetCameraCount(2);
    
    //TODO need to create const int for this
    m_calibFilter.SetFrames(15);
    
    //open the cameras
    open();
    
    while (successFrameCount < 16)
    {
        frame = grabFrame();
        images[0] = frame.getLeftImage();
        images[1] = frame.getRightImage();
        
        if (m_calibFilter.FindEtalon(images))
        {
            m_calibFilter.DrawPoints(images);
            
            //will need to add an if to check if the push is successfull.
            //calibration only occurs once total frames have had etalon found
            m_calibFilter.Push();
            successFrameCount++;
            
            //write the left frame
            //writer.writeFrame(images[0]);
            
            
            
            if (m_calibFilter.IsCalibrated())
            {
                m_calibFilter.SaveCameraParams("/home/nick/calib.ini");
                return true;
            }
        }
        else
        {
            std::cout << "Etalon not found for image set" << std::endl; 
        }
        
        cvShowImage( "cameraRight", images[1]);
            
        //wait so the pattern can move
        cvWaitKey(3000);
        
    }
    
    if (m_calibFilter.IsCalibrated())
    {
        std::cout << "happened here" << std::endl;
        m_calibFilter.SaveCameraParams("/home/nick/calib.ini");
        return true;
    }
    
    close();
    
    return false;
}

void StereoCamera::enableCalibration(bool enable)
{
    //TODO need to fix this up
    
    if (enable)
    {
        m_calibFilter.LoadCameraParams("/home/nick/calib.ini");
        m_calibIsEnabled = true;
    }
    else
    {
        m_calibIsEnabled = false;
    }
}

//void runCalibration()
//{
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
//    
//    
//}


