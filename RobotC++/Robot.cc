#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "Create.h"
#include <unistd.h>
#include <libplayerc/playerc.h>

float velocity = 0.1;
IplImage* GetThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
    cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThreshed);
    cvReleaseImage(&imgHSV);
    return imgThreshed;
}

int main()
{
    //Initialize IRobot Create
    Create create;
    create.lights(0, 0, 0);
    create.motor_raw(0.1, 0.1);
    usleep(100000);
    // Initialize capturing live feed from the camera
    CvCapture* capture = 0;
    
    capture = cvCaptureFromCAM(1);
    
    // Couldn't get a device? Throw an error and quit
    if(!capture)
    {
        printf("Could not initialize capturing...");
        return -1;
    }
    // The window we'll be using
    cvNamedWindow("thresh");
    // This image holds the "scribble" data...
    // the tracked positions of the ball
    //IplImage* imgScribble = NULL;
    // An infinite loop
    while(true)
    {
        // Will hold a frame captured from the camera
        IplImage* frame = 0;
        frame = cvQueryFrame(capture);
        // If we couldn't grab a frame... quit
        if(!frame)
            break;
        // If this is the first frame, we need to initialize it
        //if(imgScribble == NULL)
        //{
        //    imgScribble = cvCreateImage(cvGetSize(frame), 8, 3);
        //}
        // Holds the yellow thresholded image (yellow = white, rest = black)
        IplImage* imgYellowThresh = GetThresholdedImage(frame);
        // Calculate the moments to estimate the position of the ball
        CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(imgYellowThresh, moments, 1);
        
        // The actual moment values
        static double moment10 = 0;
        static double moment01 = 0;
        moment10 = cvGetSpatialMoment(moments, 1, 0);
        moment01 = cvGetSpatialMoment(moments, 0, 1);
        static double area = 0;
        area = cvGetCentralMoment(moments, 0, 0);

        if(area > 5000){
            // Holding the last and current ball positions
            static int posX = 0;
        
            static int posY = 0;
        
            posX = moment10/area;
            posY = moment01/area;

            static float angularSpeed = 0;
            angularSpeed = (float)(320 - posX) / (float)(1600);

            create.motor_raw(0,0);
            usleep(10);
            create.motor_raw(velocity, angularSpeed);
            usleep(10);
        }
        else{
            create.motor_raw(0,0.38);
            usleep(10);
            //create.motor_raw(0, 0);
            //usleep(100000);
        }

        cvShowImage("thresh", imgYellowThresh);

        // Wait for a keypress
        int c = cvWaitKey(10);
        if(c!=-1)
        {
            // If pressed, break out of the loop
            break;
        }
        // Release the thresholded image+moments... we need no memory leaks.. please
        cvReleaseImage(&imgYellowThresh);
    }
    // We're done using the camera. Other applications can now use it
    cvReleaseCapture(&capture);
    return 0;
}
