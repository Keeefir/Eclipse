//============================================================================
// Name        : Example.cpp
// Author      : Lomanov Vasiliy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <memory>

#include "Filters/CFilter.h"
#include "Input/CInput.h"
#include "Output/COutput.h"
#include "Event/CEvent.h"

#include "sys/time.h"
#include "stdio.h"
using namespace std;

using std::cerr;
using std::endl;

int main(int argc,char * argv[]) {

	if(argc!=2)
	{
		cerr<<"Wrong args number"<<endl;
		return -1;
	}

	///dgsrg

	std::auto_ptr<CFilter<unsigned char> > Filter (FilterFabric<unsigned char,double>(argv[1]));
	std::auto_ptr<CInput> Camera (new CCapture);
	std::auto_ptr<COutput> Show (new COutput);
	std::auto_ptr<CEvent> Event (new CCVEvent);

	int Width,Height;

	Camera->GetSize(Width,Height);
	unsigned char * source=new unsigned char[Width*Height];
	unsigned char * result=new unsigned char[Width*Height];
try
{
	while((*Event)())
	{
		(*Camera)(source);
		(*Filter)(source,result,Width,Width,Height,1);
		(*Show)(result,Width,Height);
	}
}
catch(...)
{
	delete [] source;
	delete [] result;
}
	delete [] source;
	delete [] result;

	return 0;
}
