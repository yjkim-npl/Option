#ifndef OpParameterContainer_h
#define OpParameterContainer_h 1

#include "TString.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

class OpParameterContainer// : public TObjArray
{
	public:
		OpParameterContainer(const char* fileName="OpParameter.conf");
		virtual ~OpParameterContainer() {}
		const OpParameterContainer& operator=(const OpParameterContainer &right);
		void Print();// {std::cout << "yjk para" << std::endl;}

		Bool_t GetParBool		(TString name)
			{ return par_bool[name]; }
		Int_t GetParInt			(TString name)
			{ return par_int[name]; }
		Double_t GetParDouble	(TString name)
			{ return par_double[name]; }
		std::string GetParString	(TString name)
			{ return par_string[name]; }

	private:
		std::map<TString,bool>	par_bool;//index of parName, value
		std::map<TString,int> 	par_int;	
		std::map<TString,double> par_double;
		std::map<TString,std::string> par_string;
};
#endif
