//============================================================================
// Name        : ShowTextNumber.cpp
// Author      : Lomanov Vasiliy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <fstream>
#include "math.h"
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::map;
using std::vector;

class CReadPair
{
	std::ifstream in;
public:
	CReadPair(){};
	explicit CReadPair(string i_file){
		in.open(i_file.c_str());
		if(!in)
			throw string("Wrong dictionary file");
	};
	~CReadPair(){};
	bool GetPair(string & i_key,string i_translate){
		if(!in.eof())
		{
			if(!getline(in,i_translate,'\n'))
				throw std::string("Bad dictionaty file");

			i_key.erase( i_key.end()-1 );
			i_translate.erase( i_translate.end()-1 );

			return true;
		}
		return false;
	};
	CReadPair(CReadPair &C)
	{

	};
	void operator=(CReadPair C)
	{
		//in=C.in;
	}
};
/*
 * класс словарь
 * при создании читает из файла ключ и в следующей строке - его "перевод"
 * из-за особенностей русского языка необходимо указать все 10 форм склонения единиц
 * склонения (одна тысячА , две тысячИ , и т.д.) , а также десятки , сотни
 * и названия каждой третьей единицы в родительном падеже (тысяч , миллионов)
 * такая система может показаться излишней , но она потребует на прибавление каждых трех
 * разрядов необходимо внести в словарь 10 новых значений + 30 изначальных названий десятков и сотен
 */
class CDictionary{
	typedef map<string,string> dict_t;
	dict_t Lang;

	CReadPair Reader;

public:
	CDictionary(){};
	CDictionary(const string &i_dictfile){


		//Reader=CReadPair(i_dictfile.c_str());
		string key,translate;
		while(Reader.GetPair(key,translate))
		{
			Lang.insert(std::make_pair(key,translate));
		}
	};
	~CDictionary(){};

	string GetTranslate(string Key)
	{
		dict_t::iterator it=Lang.find(Key);
		if(it==Lang.end())
			return string(" Unknown_word ");
		else
			return it->second;
	}
	explicit CDictionary(CDictionary & C)
	{
		Reader=C.Reader;
		Lang=C.Lang;
	};
	void operator=(CDictionary C)
	{
		Reader=C.Reader;
		Lang=C.Lang;
	}

};

class CNumSpectr{
	typedef vector<int> digits_t;
public:
	template<class _T>
	digits_t operator()(_T num,int power)
	{
		digits_t digits;
		_T temp;
		while(num)
		{
			temp=num/static_cast<_T>(power);
			digits.push_back( static_cast<int>( num-temp*static_cast<_T>(power) ) );
			num=temp;
		}
		return digits_t(digits.rbegin(),digits.rend());
	}
};

class CDigit2Word{
	CDictionary D;
	CDictionary Comments;
	CNumSpectr Num2Digits;

	/*
	 * логика внутри цикла метода может показаться запутанной и тяжеловатой
	 * для сопровождения. Собственно, это было сделано в пользу малого размера словаря
	 * при желании его можно сделать более пригодным для сопровождения
	 */
	string Translate(int num,int power)
	{
		string result;
		int temp;
		vector<int> digits;
		char buff[100];

		digits=Num2Digits(num,power);


		vector<int>::iterator it=digits.begin();
		vector<int>::iterator end=digits.end();

		int dig=digits.size()-1;
		while(it!=end)
		{
				switch(dig%3)
				{
					case 0:{
						sprintf(buff,"d%d%d", dig ,*it );
						result+=D.GetTranslate(buff) + " ";
						break;
					}
					case 1:{
						if(*it)
						{
							if(*it==1)
							{
								temp=10*(*it);
								++it;
								--dig;
								temp+=(*it);
								sprintf(buff,"%d", temp );
								result+=D.GetTranslate(buff) + " ";
								sprintf(buff,"d%d0", dig );
								result+=D.GetTranslate(buff) + " ";
							}
							else
							{
								sprintf(buff,"%d", *it*10 );
								result+=D.GetTranslate(buff) + " ";
							}
						}
						break;
					}
					case 2:{
						if(*it)
						{
							sprintf(buff,"%d", *it*100 );
							result+=D.GetTranslate(buff) + " ";
						}
						break;
					}
				}
			--dig;
			++it;
		}
		return result;
	}
public:
	CDigit2Word(string lang,string comments){

		D=CDictionary(lang);
		//Comments=CDictionary(comments);
	};
	~CDigit2Word(){};


	string TranslateIn10Power(int Num)
	{
		return Translate(Num,10)+Comments.GetTranslate("Ru comment about dec system");
	}
	string TranslateIn8Power(int Num)
	{
		return Translate(Num,8)+Comments.GetTranslate("Ru comment about hex system");
	}
};

int main() {

	CDigit2Word T("RUnums.txt","RUcomm.txt");

	int Num;
	cin>>Num;

	cout<<T.TranslateIn10Power(Num)<<endl;
	cout<<T.TranslateIn8Power(Num)<<endl;

	return 0;
}
