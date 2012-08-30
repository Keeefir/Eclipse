//============================================================================
// Name        : 4acronis.cpp
// Author      : Lomanov Vasiliy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Filters/CFilter.h"
#include <memory>

using std::auto_ptr;
using std::cout;
using std::endl;

using namespace std;

int main() {

	auto_ptr<CFilter<float> > F(new CAverage<float>(1,0));

	int N=10;
	float a[N],b[N];

	for(int i=0;i<N;i++)
	{
		b[i]=a[i]=i;
	}
	for(int i=0;i<N;i++)
	{
		cout<<a[i]<<" ";
	}cout<<"\n";

	F->filtering(a,b,10,10,1);

	for(int i=0;i<N;i++)
	{
		cout<<b[i]<<" ";
	}cout<<"\n";

	return 0;
}
