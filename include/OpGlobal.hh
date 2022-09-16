#ifndef OPGLOBAL_HH
#define OPGLOBAL_HH 1

#include <iostream>
#include <fstream>
#include "TString.h"

#define op_out 		OPLog(0,0)
#define op_debug 	OPLog(__FILE__,__LINE__)

#define op_print 	OPLog(fName,__FUNCTION__,fRank,1)
#define op_info		OPLog(fName,__FUNCTION__,fRank,2)
#define op_warning	OPLog(fName,__FUNCTION__,fRank,3)
#define op_error	OPLog(fName,__FUNCTION__,fRank,4)

#define or_print(LRANK)		OPLog(this->ClassName(),__FUNCTION__,LRANK,1)
#define or_info(LRANK)		OPLog(this->ClassName(),__FUNCTION__,LRANK,2)
#define or_warning(LRANK)	OPLog(this->ClassName(),__FUNCTION__,LRANK,3)
#define or_error(LRANK)		OPLog(this->ClassName(),__FUNCTION__,LRANK,4)

#define cout_info		OPLog(0,2)
#define cout_warning	OPLog(0,3)
#define cout_error		OPLog(0,4)

class OPLog
{
	public:
		OPLog(TString name, const std::string &title, int rank, int option)
		{
			if(option ==0)
				return;
			for(auto i=0; i<rank; ++i)
				std::cout << "  ";
			TString header = Form("[%s::%s] ",name.Data(),title.c_str());

			switch(option)
			{
				case 1: std::cout << header; break;
				case 2: std::cout << header << "\33[0;32m"<<"info  "<< "\33[0m"; break;
				case 3: std::cout << header << "\33[0;33m"<<"warning  "<< "\33[0m"; break;
				case 4: std::cout << header << "\33[0;31m"<<"error  "<< "\33[0m"; break;
				default: ;
			}
		}
		OPLog(int rank = 0, int option = 1)
		{
			if(option ==0)
				return;
			for(auto i=0; i<rank; ++i)
				std::cout << "   ";
			switch(option)
			{
				case 1: break;
				case 2: std::cout << "\33[0;32m"<<"info  "<< "\33[0m"; break;
				case 3: std::cout << "\33[0;33m"<<"warning  "<< "\33[0m"; break;
				case 4: std::cout << "\33[0;31m"<<"error  "<< "\33[0m"; break;
				default: ;
			}
		}

		OPLog(const std::string &title, int line)
		{
			std::cout<<"+\033[0;36m"<<Form("%d ",line)<<"\033[0m "<<Form("%s \033[0;36m#\033[0m ",title.c_str());
		}

		template <class T> OPLog &operator<<(const T &v)
		{
			std::cout << v;
			return *this;
		}
		OPLog &operator <<(std::ostream(*f)(std::ostream&))
		{
			std:: cout << *f;
			return *this;
		}
};

#endif
