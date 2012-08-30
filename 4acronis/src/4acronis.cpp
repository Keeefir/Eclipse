//============================================================================
// Name        : 4acronis.cpp
// Author      : Lomanov Vasiliy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

int main() {

	IplImage * source=cvLoadImage("meduza.jpg");
	//IplImage * result=cvCreateImage(cvGetSize(source),source->depth,source->nChannels);


	cvShowImage("source",source);

	//cvShowImage("result",result);

	cvWaitKey();

	cvReleaseImage(&source);
	//cvReleaseImage(&result);

	return 0;
}
