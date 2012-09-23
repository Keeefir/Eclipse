/*
 * CEvent.h
 *
 *  Created on: 02.09.2012
 *      Author: Lag
 */

#ifndef CEVENT_H_
#define CEVENT_H_

class CEvent {
public:
	CEvent(){};
	virtual ~CEvent(){};

	virtual int operator()()=0;
};
class CCVEvent:public CEvent{
	int key;
public:
	CCVEvent():key(0){};
	~CCVEvent(){};

	int operator()()
	{
		key=cvWaitKey(1);
		switch(key)
		{
		case 27:
			return 0;
			break;
		default : return key;
		}
	}
};
#endif /* CEVENT_H_ */
