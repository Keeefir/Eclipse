/*
 * CInput.cpp
 *
 *  Created on: 31.08.2012
 *      Author: lag
 */

#include "CInput.h"


CCapture::CCapture(int i_camind):greyframe(0) {
	// TODO Auto-generated constructor stub

	if ((Cap=cvCreateCameraCapture(i_camind)) < 0)
	{
		throw std::string("Failed to open cam");
	}
	ImageWidth = cvGetCaptureProperty(Cap, CV_CAP_PROP_FRAME_WIDTH);
	ImageHeght = cvGetCaptureProperty(Cap, CV_CAP_PROP_FRAME_HEIGHT);

	greyframe=cvCreateImage(cvSize(ImageWidth,ImageHeght),8,1);

	ThreadLoop=thread(&CInput::Loop,this);
}

CCapture::~CCapture() {
	// TODO Auto-generated destructor stub
	end=true;
	ThreadLoop.join();
	if(Cap)cvReleaseCapture(&Cap);
	if(greyframe)cvReleaseImage(&greyframe);
}
void CCapture::Loop()
{
	struct timeval t1,t2;
	while(!end)
	{
		gettimeofday(&t1,NULL);

		Mutex.lock();
		frame=cvQueryFrame(Cap);
		cvCvtColor(frame , greyframe, CV_BGR2GRAY);
		Mutex.unlock();
		cvWaitKey(1);
		gettimeofday(&t2,NULL);
		printf("%ld\n",(t2.tv_sec-t1.tv_sec)*1000000+t2.tv_usec-t1.tv_usec);
	}
}
void CCapture::GetSize(int &width,int &height)
{
	width=greyframe->width;
	height=greyframe->height;
}

