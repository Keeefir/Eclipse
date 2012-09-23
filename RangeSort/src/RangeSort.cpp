//============================================================================
// Name        : RangeSort.cpp
// Author      : Lomanov Vasiliy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include "stdio.h"
using namespace std;

using std::vector;

typedef vector<int> v_t;

template<class con_t>
ostream & operator<<(ostream & out,con_t c)
{
	std::copy(c.begin(),c.end(),std::ostream_iterator<typename con_t::value_type >(out," "));
	return out;
}
template<class _T>
_T SetRand(_T arg)
{
	return rand()%256;
}

void Sort(v_t::iterator begin,v_t::iterator end,int bit)
{
	printf("sorting %d\n",bit);
	if( begin==end-1 || bit<0 )return;
	v_t buff(begin,end);

	v_t::iterator	buff_begin=buff.begin(),
					buff_end=buff.end(),
					ind=begin;


	while(buff_begin!=buff_end)
	{
		if((*buff_begin) & (1<<bit))
		{
			--end;
			(*end)=(*buff_begin);

		}
		else{
			(*begin)=(*buff_begin);
			++begin;
		}
		++ind;
		++buff_begin;
	}

	Sort(begin,ind,bit-1);
	Sort(ind,end,bit-1);

}

int main() {

	v_t a(10);
	std::transform(a.begin(),a.end(),a.begin(),SetRand<int>);

	cout<<a<<endl;

	Sort(a.begin(),a.end(),7);

	cout<<a<<endl;

	return 0;
}
