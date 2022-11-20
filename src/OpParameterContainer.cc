#include "OpParameterContainer.hh"

#include "TSystem.h"
#include "TObjString.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
OpParameterContainer::OpParameterContainer(const char* fileName)
//:TObjArray()
{
	ifstream par_file(fileName);
	string line;
	while(getline(par_file,line))
	{
		TString val_name;
		TString val_type;
		istringstream ss(line);
		ss >> val_name >> val_type;

		if(val_name[0] =='#') continue;

		if(val_type.Contains("b") || val_type.Contains("bool"))
		{
			Bool_t val = 0;
			ss >> val;
			par_bool.insert(make_pair(val_name,val));
		}
		if(val_type.Contains("i") || val_type.Contains("int"))
		{
			Int_t val = -1;
			ss >> val;
			par_int.insert(make_pair(val_name,val));
		}
		if(val_type.Contains("d") || val_type.Contains("double"))
		{
			Double_t val = -1;
			ss >> val;
			par_double.insert(make_pair(val_name,val));
		}
		if(val_type.Contains("s") || val_type.Contains("string"))
		{
			string val;
			ss >> val;
			par_string.insert(make_pair(val_name,val));
		}
	}
}

const OpParameterContainer& OpParameterContainer::operator=(const OpParameterContainer &right)
{
	par_bool = right.par_bool;
	par_int = right.par_int;
	par_double = right.par_double;
	par_string = right.par_string;
	return *this;
}

void OpParameterContainer::Print()
{
	std::cout << "yjkim" << std::endl;
}
