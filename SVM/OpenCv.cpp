// OpenCv.cpp : Defines the entry point for the console application.
//

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/ml/ml.hpp>
#include <stdio.h>


using namespace cv;


int main()
{
    int num_files = 10;
	int img_area = 1280*1024;
	Mat training_mat(num_files,img_area,CV_32FC1);
	
	//first image 00
	Mat img_mat = imread("00.jpg",0); // I used 0 for greyscale
	int ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(0,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//first image 01
	img_mat = imread("01.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(1,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//first image 02
	img_mat = imread("02.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(2,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//first image 03
	img_mat = imread("03.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(3,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//second image 04
	img_mat = imread("04.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(4,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//first image 05
	img_mat = imread("05.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(5,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//first image 06
	img_mat = imread("06.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(6,ii++) = img_mat.at<uchar>(i,j);
    }
	}
	//first image 07
	img_mat = imread("07.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(7,ii++) = img_mat.at<uchar>(i,j);
    }
	}

	//third image 08
	img_mat = imread("08.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(8,ii++) = img_mat.at<uchar>(i,j);
    }
}

		//third image 08
	img_mat = imread("011.jpg",0); // I used 0 for greyscale
	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        training_mat.at<float>(9,ii++) = img_mat.at<uchar>(i,j);
    }
}


	Mat labels(num_files,1,CV_32FC1);


	labels.at<float>(0,0)=0;
	labels.at<float>(1,0)=1;
	labels.at<float>(2,0)=2;
	labels.at<float>(3,0)=3;
	labels.at<float>(4,0)=4;
	labels.at<float>(5,0)=5;
	labels.at<float>(6,0)=6;
	labels.at<float>(7,0)=7;
	labels.at<float>(8,0)=8;
	labels.at<float>(9,0)=1;

	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::POLY;
	params.gamma=3;
	params.degree=2;
	params.term_crit =cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	CvSVM svm;
	svm.train(training_mat, labels, Mat(), Mat(), params);

	//svm.save("firstrial");
	//svm.load("secondtrial");

	img_mat = imread("012.jpg",0); // I used 0 for greyscale
	Mat test(1,img_area,CV_32FC1);


	ii = 0; // Current column in training_mat
	for (int i = 0; i<img_mat.rows; i++) {
    for (int j = 0; j < img_mat.cols; j++) {
        test.at<float>(0,ii++) = img_mat.at<uchar>(i,j);
    }
}

	float testid=svm.predict(test);

	
	printf("test id %4.3f",testid);

    return 0;
}
