/*
 * COutput.h
 *
 *  Created on: 31.08.2012
 *      Author: lag
 */

#ifndef COUTPUT_H_
#define COUTPUT_H_

#include <opencv/highgui.h>
#include <opencv/cv.h>

class COutput {
	IplImage * source;
	int Width,Height;
public:
	COutput():source(0),Width(0),Height(0){};
	~COutput(){
		if(source)cvReleaseImage(&source);
	}
	void operator()(const void * i_img,int w,int h,std::string window_name="src")
	{
		char c;
		if(Width!=w || Height!=h)
		{
			if(source)cvReleaseImage(&source);

			Height=h;
			Width=w;
			source=cvCreateImage(cvSize(Width,Height),8,1);
		}
		memcpy(source->imageData,i_img,Width*Height);
		cvShowImage(window_name.data(),source);
	}
};

#endif /* COUTPUT_H_ */
