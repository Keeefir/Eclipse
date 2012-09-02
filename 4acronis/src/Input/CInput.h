/*
 * CInput.h
 *
 *  Created on: 31.08.2012
 *      Author: lag
 */

#ifndef CINPUT_H_
#define CINPUT_H_

#include <boost/thread.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using boost::thread;
using boost::mutex;

class CInput {
protected:
	bool end;
	int ImageWidth;
	int ImageHeght;
public:
	virtual void operator()(void * dst)=0;
	virtual void Loop()=0;
	virtual void GetSize(int &o_width,int &o_height)=0;

	CInput():end(false),ImageWidth(0),ImageHeght(0){};
	virtual ~CInput(){};
};

class CCapture:public CInput{
	CvCapture * Cap;
	IplImage * frame, *greyframe;
	thread ThreadLoop;
	mutex Mutex;
public:
	explicit CCapture(int i_camind=CV_CAP_ANY);
	~CCapture();

	void operator()(void * dst)
	{
		Mutex.lock();
		memcpy(dst,greyframe->imageData,ImageWidth*ImageHeght);
		Mutex.unlock();
	}
	void Loop();
	void GetSize(int &width,int &height);

};
#endif /* CINPUT_H_ */
