#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/ml/ml.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "Create.h"
#include <unistd.h>
#include <libplayerc/playerc.h>
#include <string.h>

using namespace cv;

float velocity = 0.1;
float old_area = 0;
// number of ir readings in a row to test for ramp
int maxRampCounter = 20;

IplImage* GetThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
    cvInRangeS(imgHSV, cvScalar(20, 90, 90), cvScalar(30, 255, 255), imgThreshed);
    cvReleaseImage(&imgHSV);
    return imgThreshed;
}

void get_to_mid_from_left(){
    
}

void get_to_mid_from_left_mid(){
    
}

void get_to_mid_from_right(){
    
}

void get_to_mid_from_right_mid(){
    
}

void run_over_ramp(Create create, Create::ir_values ir, int rampCounter){
    // driving over the ramp
    rampCounter = 0;
    while(true){
        ir = create.read_ir();
        std::cout << "right: " << ir.right << " left: " << ir.left << std::endl;
        if (ir.fleft < 1150 || ir.fright < 1150){
            rampCounter++;
        } else {
            rampCounter = 0;
        }
        
        if (rampCounter > maxRampCounter){
            create.motor_raw(0,0);
            usleep(10);
            break;
        }
        create.move(-velocity);
        usleep(10);
        if (ir.left < 600){
            create.motor_raw(velocity, 0.5);
            usleep(100000);
        }
        else if (ir.right < 600){
            create.motor_raw(velocity, -0.5);
            usleep(100000);
        }
    }
}

int main()
{
    //Initialize learning machine
    int img_area = 1280*960;
    Mat img_mat;
    int ii = 0;
    CvSVM svm;
    //Load learned library
    svm.load("learned_lib");
    
    //Initialize IRobot Create
    Create create;
    create.lights(0, 0, 0);

    create.calibrate_ir();

    Create::ir_values ir = create.read_ir();

    std::cout << "ir left: " << ir.left << " right: " << ir.right << " fleft: " << ir.fleft << " fright: " << ir.fright << std::endl;

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
    int rampCounter = 0;

    // An infinite loop
    while(true)
    {
        // update ir reading values
        ir = create.read_ir();
        usleep(10);
        
        std::cout << "ir left: " << ir.left << " right: " << ir.right << " fleft: " << ir.fleft << " fright: " << ir.fright << std::endl;
        // check if we are on the ramp
        if (ir.fleft > 1280 && ir.fright > 1280)
            rampCounter++;
        else
            rampCounter = 0;

        // check if we have been on ramp long enough
        if (rampCounter > maxRampCounter){
            create.motor_raw(0,0);
            usleep(100000);
            break;
        }
        
        // Will hold a frame captured from the camera
        IplImage* frame = 0;
        frame = cvQueryFrame(capture);
        // If we couldn't grab a frame... quit
        if(!frame)
            break;

        // Holds the yellow thresholded image (yellow = white, rest = black)
        IplImage* imgYellowThresh = GetThresholdedImage(frame);
        
        //Smooth the binary image using Gaussian kernel
        cvSmooth(imgYellowThresh, imgYellowThresh, CV_GAUSSIAN,3,3);
        
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
        
        if(area > 5000 && area < 150000){
//            if(old_area != 0 && area > old_area)
//                velocity = velocity * (area/old_area);
//            old_area = area;
            
            // Holding the current position of the object
            static int posX = 0;
        
            static int posY = 0;
        
            posX = moment10/area;
            posY = moment01/area;

            static float angularSpeed = 0;
            angularSpeed = (float)(320 - posX) / (float)(1600);

            create.motor_raw(-velocity, angularSpeed);
            usleep(10);
        }
        else if(area >= 150000){
//            create.motor_raw(0,0);
//            usleep(10);
            img_mat = Mat(frame, true);
            Mat test(1,img_area,CV_32FC1);
            
            ii = 0; // Current column in training_mat
            for (int i = 0; i<img_mat.rows; i++) {
                for (int j = 0; j < img_mat.cols; j++) {
                    test.at<float>(0,ii++) = img_mat.at<uchar>(i,j);
                }
            }
            int testid = svm.predict(test);
            if (testid==2) {
                break;
                //run_over_ramp(create, ir, rampCounter);
            }
        }
        else{
            create.motor_raw(0,0.38);
            usleep(10);
        }

//        cvShowImage("thresh", imgYellowThresh);

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

    // driving over the ramp
    run_over_ramp(create, ir, rampCounter);

    // We're done using the camera. Other applications can now use it
    cvReleaseCapture(&capture);
    create.shutdown();
    return 0;
}
