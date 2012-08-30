/*
 * CFilter.h
 *
 *  Created on: 31.08.2012
 *      Author: Lag
 */

#ifndef CFILTER_H_
#define CFILTER_H_

#include <cstdlib>
#include <cstddef>
#include <iostream>

template<typename _T>
class CFilter {
public:
	CFilter(){};
	virtual ~CFilter(){};
	virtual void filtering(const _T * i_source,_T * o_dst,int i_length,int i_width,int i_heght)=0;
};
template<typename _T>
class CAverage:public CFilter<_T>{
	int XHalfStrobSize;
	int YHalfStrobSize;
public:
	explicit CAverage(int i_xhalfsize=1,int i_yhalfsize=1):XHalfStrobSize(i_xhalfsize),YHalfStrobSize(i_yhalfsize){};
	void filtering(const _T * i_source,_T * o_dst,int i_length,int i_width,int i_heght)
	{
		int x,y,i,j;
		double sum,count;



		for(x=0;x<i_width;++x)
			for(y=0;y<i_heght;++y)
			{
				sum=0;
				count=0;

				for( i=std::max(0,x-XHalfStrobSize) ; i<=std::min(i_width-1,x+XHalfStrobSize) ; i++ )
					for( j=std::max(0,y-YHalfStrobSize) ; j<=std::min(i_heght-1,y+YHalfStrobSize) ; j++ )
					{
						//std::cout<<i_source[(y+j)*i_length+x+i]<<std::endl;
						sum+=i_source[(y+j)*i_length+x+i];
						++count;
					}

				o_dst[y*i_length+x]=static_cast<_T>(sum/count);
			}
	}
};
template<typename _T>
class CGauss:public CFilter<_T>{
public:
	void filtering(const _T * i_source,_T * o_dst,int i_length,int i_width,int i_heght)
	{

	}
};
template<typename _T>
class CEqualHist:public CFilter<_T>{
public:
	void filtering(const _T * i_source,_T * o_dst,int i_length,int i_width,int i_heght)
	{

	}
};

#endif /* CFILTER_H_ */
