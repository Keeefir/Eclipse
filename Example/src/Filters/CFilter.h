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
#include <string.h>
#include <iostream>
#include <vector>
#include <boost/shared_array.hpp>

using std::vector;
using boost::shared_array;
using std::cerr;
using std::endl;


template<typename _T>
class CFilter {
protected:
	_T * Source;
	_T * Dst;
	int Length;
	int Width;
	int Height;
	int Arg;
public:
	virtual ~CFilter(){};

	void SetParams(_T * i_source,_T * o_dst,int i_length,int i_width,int i_heght,int i_arg)
	{
		if(i_source)	Source=i_source;
		else throw std::string("Null ptr");
		if(o_dst)		Dst=o_dst;
		else throw std::string("Null ptr");

		if( (i_length>=i_width) && (i_width>0) && (i_heght>0) )
		{
			Length=i_length;
			Width=i_width;
			Height=i_heght;
			Arg=i_arg;
		}
		else throw std::string("Bad size");
	}
	virtual void operator()()=0;
	void operator()(_T * i_source,_T * o_dst,int i_length,int i_width,int i_heght,int i_arg)
	{
		SetParams(i_source,o_dst,i_length,i_width,i_heght,i_arg);
		operator()();
	}
};
template<typename _T>
class CAverage:public CFilter<_T>{
public:
	virtual ~CAverage(){};

	virtual void operator()()=0;
};
template<typename _T,typename prec_type=double>
class CUsualAverage:public CAverage<_T>{
public:

	void operator()()
	{
		int x,y,i,j;
		prec_type sum,count;

		for(x=0;x<CFilter<_T>::Width;x++)
			for(y=0;y<CFilter<_T>::Height;y++)
			{
				sum=0;
				count=0;
				for( i=std::max(0,x-CFilter<_T>::Arg) ; i<std::min(CFilter<_T>::Width-1,x+CFilter<_T>::Arg) ; i++ )
					for( j=std::max(0,y-CFilter<_T>::Arg) ; j<std::min(CFilter<_T>::Height-1,y+CFilter<_T>::Arg) ; j++ )
					{
						sum+=CFilter<_T>::Source[j*(CFilter<_T>::Length)+i];
						count++;
					}
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>(sum/count);
			}
	}
};
template<typename _T,typename prec_type=double>
class CIntegralAverage:public CAverage<_T>{

	shared_array<prec_type> IntegralImage;

	template<typename _I>
	_I * IntImg(const _T * source,int length,int xapp,int yapp,_I *IntegralImage)
	{
		int x,y;


		IntegralImage[0]=static_cast<_I>(source[0]);

		for(x=1;x<xapp;x++)
		{
			IntegralImage[x]=IntegralImage[x-1]+static_cast<_I>(source[x]);
		}

		for(y=1;y<yapp;y++)
		{
			IntegralImage[y*xapp]=IntegralImage[(y-1)*length]+static_cast<_I>(source[y*length]);
		}
		for(x=1;x<xapp;x++)
		{
			for(y=1;y<yapp;y++)
			{
				IntegralImage[y*xapp+x]=static_cast<_I>(source[x+length*(y)])+IntegralImage[y*xapp+(x-1)]+IntegralImage[(y-1)*xapp+(x)]-IntegralImage[(y-1)*xapp+(x-1)];
			}
		}

		return IntegralImage;
	}
public:
	CIntegralAverage():IntegralImage(0){};
	~CIntegralAverage(){};
	void operator()()
	{
		int x,y,i,j;
		double sum,count;


		static int local_width=0,local_heght=0;

		//if(!IntegralImage)IntegralImage=shared_array<int>(CFilter<_T>::Width*CFilter<_T>::Height);
		if((CFilter<_T>::Width!=local_width)||(CFilter<_T>::Height!=local_heght))
		{
			local_width=CFilter<_T>::Width;
			local_heght=CFilter<_T>::Height;
			IntegralImage.reset();
			IntegralImage=shared_array<prec_type>(new prec_type[local_width*local_width]);
		}

		IntImg(CFilter<_T>::Source,(CFilter<_T>::Length),CFilter<_T>::Width,CFilter<_T>::Height,IntegralImage.get());
		sum=((2*CFilter<_T>::Arg+1))*((2*CFilter<_T>::Arg+1));

		CFilter<_T>::Dst[CFilter<_T>::Arg*(CFilter<_T>::Length)+CFilter<_T>::Arg]=static_cast<_T>(IntegralImage[(2*CFilter<_T>::Arg)*CFilter<_T>::Width+(2*CFilter<_T>::Arg)]/sum);
		y=CFilter<_T>::Arg;
		for(x=CFilter<_T>::Arg+1;x<CFilter<_T>::Width-CFilter<_T>::Arg;x++)
		{
			CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>((IntegralImage[(y+CFilter<_T>::Arg)*CFilter<_T>::Width+(x+CFilter<_T>::Arg)]-IntegralImage[(y+CFilter<_T>::Arg)*CFilter<_T>::Width+(x-CFilter<_T>::Arg)-1])/sum);
		}
		x=CFilter<_T>::Arg;
		for(y=CFilter<_T>::Arg+1;y<CFilter<_T>::Height-CFilter<_T>::Arg;y++)
		{
			CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>((IntegralImage[(y+CFilter<_T>::Arg)*CFilter<_T>::Width+(x+CFilter<_T>::Arg)]-IntegralImage[(y-CFilter<_T>::Arg-1)*CFilter<_T>::Width+x+CFilter<_T>::Arg])/sum);
		}
		//printf("%d\n",__LINE__);
		for(x=CFilter<_T>::Arg+1;x<CFilter<_T>::Width-CFilter<_T>::Arg;x++)
			for(y=CFilter<_T>::Arg+1;y<CFilter<_T>::Height-CFilter<_T>::Arg;y++)
			{
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>((IntegralImage[(y+CFilter<_T>::Arg)*(CFilter<_T>::Width)+(x+CFilter<_T>::Arg)]+IntegralImage[(y-CFilter<_T>::Arg-1)*(CFilter<_T>::Width)+(x-CFilter<_T>::Arg-1)]-IntegralImage[(y-CFilter<_T>::Arg-1)*(CFilter<_T>::Width)+x+CFilter<_T>::Arg]-IntegralImage[(y+CFilter<_T>::Arg)*(CFilter<_T>::Width)+x-CFilter<_T>::Arg-1])/sum);
			}
		for(y=0;y<CFilter<_T>::Height;y++)
		{
			for(x=0;x<CFilter<_T>::Arg+1;x++)
			{
				sum=0;
				count=0;
				for( i=std::max(0,x-CFilter<_T>::Arg) ; i<std::min(CFilter<_T>::Width-1,x+CFilter<_T>::Arg) ; i++ )
					for( j=std::max(0,y-CFilter<_T>::Arg) ; j<std::min(CFilter<_T>::Height-1,y+CFilter<_T>::Arg) ; j++ )
					{
						sum+=CFilter<_T>::Source[j*(CFilter<_T>::Length)+i];
						count++;
					}
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>(sum/count);
			}
			for(x=CFilter<_T>::Width-CFilter<_T>::Arg;x<CFilter<_T>::Width;x++)
			{
				sum=0;
				count=0;
				for( i=std::max(0,x-CFilter<_T>::Arg) ; i<std::min(CFilter<_T>::Width-1,x+CFilter<_T>::Arg) ; i++ )
					for( j=std::max(0,y-CFilter<_T>::Arg) ; j<std::min(CFilter<_T>::Height-1,y+CFilter<_T>::Arg) ; j++ )
					{
						sum+=CFilter<_T>::Source[j*(CFilter<_T>::Length)+i];
						count++;
					}
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>(sum/count);
			}
		}
		for(x=CFilter<_T>::Arg;x<CFilter<_T>::Width-CFilter<_T>::Arg;x++)
		{
			for(y=0;y<CFilter<_T>::Arg+1;y++)
			{
				sum=0;
				count=0;
				for( i=std::max(0,x-CFilter<_T>::Arg) ; i<std::min(CFilter<_T>::Width-1,x+CFilter<_T>::Arg) ; i++ )
					for( j=std::max(0,y-CFilter<_T>::Arg) ; j<std::min(CFilter<_T>::Height-1,y+CFilter<_T>::Arg) ; j++ )
					{
						sum+=CFilter<_T>::Source[j*(CFilter<_T>::Length)+i];
						count++;
					}
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>(sum/count);
			}
			for(y=CFilter<_T>::Height-CFilter<_T>::Arg;y<CFilter<_T>::Height;y++)
			{
				sum=0;
				count=0;
				for( i=std::max(0,x-CFilter<_T>::Arg) ; i<std::min(CFilter<_T>::Width-1,x+CFilter<_T>::Arg) ; i++ )
					for( j=std::max(0,y-CFilter<_T>::Arg) ; j<std::min(CFilter<_T>::Height-1,y+CFilter<_T>::Arg) ; j++ )
					{
						sum+=CFilter<_T>::Source[j*(CFilter<_T>::Length)+i];
						count++;
					}
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=static_cast<_T>(sum/count);
			}
		}

	}
};
template<typename _T,typename prec_type=double>
class CMedian:public CFilter<_T>{
	vector<_T> SortingBuff;
public:
	void operator()()
	{
		int count;
		int full_size=(2*CFilter<_T>::Arg+1)*(2*CFilter<_T>::Arg+1);
		if(SortingBuff.size()!=full_size)
			SortingBuff.resize(full_size);

		int x,y,i,j;
		for(x=0;x<CFilter<_T>::Width;x++)
			for(y=0;y<CFilter<_T>::Height;y++)
			{
				count=0;
				for( i=std::max(0,x-CFilter<_T>::Arg) ; i<std::min(CFilter<_T>::Width-1,x+CFilter<_T>::Arg) ; i++ )
					for( j=std::max(0,y-CFilter<_T>::Arg) ; j<std::min(CFilter<_T>::Height-1,y+CFilter<_T>::Arg) ; j++ )
					{
						SortingBuff[count]=CFilter<_T>::Source[(j)*(CFilter<_T>::Length)+i];
						count++;
					}
				std::sort(&SortingBuff[0],&SortingBuff[count]);
				CFilter<_T>::Dst[y*(CFilter<_T>::Length)+x]=SortingBuff.at(count/2);
			}

	}
};
template<typename _T,typename prec_type=double>
/*
	шаблон не бдет использоваться в функции,любые данные будут обрабатываться как unsigned char
	обрабатывать другие типы не имеет смысла
	сделано для единообразия вызова конструктора
*/
class CEqualHist:public CFilter<_T>{
	typedef CFilter<_T> _Base ;
public:
	void operator()()
	{

		int x,y;
		double	hist[256]={0},
				S=0,
				temp;

		for(x=0;x<_Base::Width;x++)
			for(y=0;y<_Base::Height;y++)
			{
				hist[_Base::Source[y*(_Base::Length)+x]]++;
				S++;
			}
		for(x=0;x<256;x++)
		{
			hist[x]/=S;
		}
		for(x=1;x<256;x++)
		{
			hist[x]+=hist[x-1];
		}
		for(x=0;x<_Base::Width;x++)
			for(y=0;y<_Base::Height;y++)
			{
				temp=hist[_Base::Source[y*(_Base::Length)+x]]*255.0;
				_Base::Dst[y*(_Base::Length)+x]=temp;
			}
	}
};

template<typename _T,typename prec_type>
CFilter<_T> * FilterFabric(std::string choose)
{
	if(choose=="IntAver")
		return new CIntegralAverage<_T,prec_type>;
	if(choose=="UsAver")
		return new CUsualAverage<_T,prec_type>;
	if(choose=="Median")
		return new CMedian<_T,prec_type>;
	if(choose=="return new CEqualHist<_T,prec_type>;")
		return new CEqualHist<_T,prec_type>;

	cerr<<"Wrong filter name.Using default IntegralAverage filter"<<endl;
	return new CIntegralAverage<_T,prec_type>;
}

#endif /* CFILTER_H_ */
