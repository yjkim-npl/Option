#include "TROOT.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "TApplication.h"
#include "TObjString.h"
#include "OpParameterContainer.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "TFormula.h"

using namespace std;

ClassImp(OpParameterContainer)

OpParameterContainer::OpParameterContainer(Bool_t debug)
	: TObjArray(), fDebugMode(debug)
{
	fName = "ParameterContainer";
}

OpParameterContainer::OpParameterContainer(const char* parName, Bool_t debug)
	:OpParameterContainer(debug)
{
	AddFile(TString(parName));
}

void OpParameterContainer::SetDebugMode(Bool_t val) {fDebugMode = val;}

void OpParameterContainer::SaveAs(const char* fileName, Option_t *)const
{
	if(TString(fileName).EndsWith(".conf") || TString(fileName).EndsWith(".par"))
		fileName = TString(fileName)+".conf";
	Print(fileName);
}

void OpParameterContainer::ReplaceEnvironmentVariable(TString &val)
{
	if(val[0] = '$')
	{
		TString env = val;
		Ssiz_t nenv = env.First("/");
		env.Resize(nenv);
		env.Remove(0,1);
		TString path = getenv(env);
		val.Replace(0,nenv,path);
	}
}
Int_t OpParameterContainer::AddFile(TString fileName, TString parNameForFile)
{
	TString fileNameFull;

	if(fileName[0] != '/' && fileName[0] != '$' && fileName != '~')
		fileNameFull = TString(gSystem -> Getenv("KEBIPATH")) + "/input/" + fileName;
	else
		fileNameFull = fileName;

	ReplaceEnvironmentVariable(fileNameFull);

	if(TString(gSystem -> Which(".",fileNameFull.Data())).IsNull())
		fileNameFull = TString(gSystem -> Getenv("PWD")) + "/" + fileName;

	if(TString(gSystem -> Which(".",fileNameFull.Data())).IsNull())
	{
		or_error(0) << "Parameter file" << fileNameFull << " does not exist!\n";
		return 0;
	}
	or_info(0) << "Adding parameter file " << fileNameFull << "\n";

	if(parNameForFile.IsNull())
		parNameForFile = Form("INPUT_PARAMETER_FILE%d",fNumInputFiles);
	fNumInputFiles++;
	SetPar(parNameForFile,fileNameFull);

	Int_t countParameters = 0;

	ifstream file(fileNameFull);
	string line;

	while(getline(file,line))
	{
		if(line.find("#") == 0)
			continue;
		countParameters++;
		TString parName;
		TString parType;

		istringstream ss(line);
		ss >> parName >> parType;
		parType.ToLower();

		Bool_t overwrite = false;
		if(parType =="o" || parType == "overwrite"){
			overwrite = true;
			ss >> parType;
			parType.ToLower();
		}
		if(parType =="f" || parType == "file"){
			TString val;
			ss >> val;
			ReplaceEnvironmentVariable(val);
			AddFile(val,parName);
		}
		else if (parType =="b" || parType == "bool" || parType == "bool_t")
		{
			TString valueBoolean;
			ss >> valueBoolean;
			valueBoolean.ToLower();
			Bool_t val = false;
			if(valueBoolean == "true" || valueBoolean == "1" || valueBoolean == "ktrue")
				val = true;
			SetPar(parName, val, overwrite);
		}
		else if (parType == "i" || parType =="int" || parType=="int_t" || parType == "w" || parType == "width" || parType == "width_t")
		{
			Int_t val;
			ss >> val;
			SetPar(parName, val, overwrite);
		}
		else if (parType == "d" || parType == "double" || parType =="Double_t")
		{
			TString valFormula;
			ss >> valFormula;
			Double_t val = TFormula("formula",valFormula).Eval(0);
			SetPar(parName, val, overwrite);
		}
		else if (parType == "s" || parType == "string" || parType == "tstring")
		{
			TString val;
			ss >> val;
			ReplaceEnvironmentVariable(val);
		}
		else countParameters--;
	}
	if(countParameters == 0)
	{
		this -> Remove(FindObject(parNameForFile));
		fNumInputFiles--;
	}
	return countParameters;
}

Int_t OpParameterContainer::AddPar(OpParameterContainer *parc, TString parNameForFile)
{
	or_info(0) << "Adding parameter container " << parc->GetName() << "\n";

	if(parNameForFile.IsNull())
		parNameForFile = Form("INPUT_PAR_CONTAINER%d",fNumInputFiles);
	fNumInputFiles++;

	Int_t countParameters = 0;
	Int_t countSameParameters = 0;
	TIter iterator(parc);
	TObject *obj;
	while((obj = dynamic_cast<TObject*>(iterator())))
	{
		TString name = obj -> GetName();

		TObject* found = FindObject(name);
		if(found != nullptr){
			if(name.Index("INPUT_PARAMETER_FILE")==0)
				((TNamed*)obj) -> SetName(name+"_");
			else{
				or_error(0) << "Parameter with name " << name << " already exist!\n";
				++countSameParameters;
				continue;
			}
		}
		Add(obj);
		++countParameters;
	}
	return countParameters;
}

Int_t OpParameterContainer::GetNumInputFiles() {return fNumInputFiles;}

void OpParameterContainer::Print(Option_t * option) const
{
	if(TString(option) == "raw")
	{
		TObjArray::Print();
		return;
	}

	bool printout = true;
	ofstream fileOut;

	TString fileName(option);
	if(fileName.EndsWith(".conf") || fileName.EndsWith(".par"))
		printout = false;
	if(printout)
	{
		if(fDebugMode) or_info(0) << "KBPar::336\n";
	}
	if(!printout)
	{
		or_info(0) << "OpPar::356\n";
	}

	TString arrayTitleIsSet;
	Int_t countDownArrayIdx = 0;
	TIter iterator(this);
	TObject* obj;
	while((obj = dynamic_cast<TObject*>(iterator())))
	{
		bool skipPrint = false;
		TString className = obj -> ClassName();
		TString key = obj -> GetName();
		TString parType;
		TString valueString;
		TString comment;
		Bool_t parameterIsNew = false;
		if(key.Index("NEWPAR") ==0) {key.Remove(0,6); parameterIsNew = true;}
		if(className == "TNamed")
		{
			TNamed* par = (TNamed*) obj;
			valueString = par -> GetTitle();
			if(valueString.Index("AXIS_PARAMETER_")==0)
			{
				valueString.ReplaceAll("AXIS_PARAMETER_","");
				parType = "";
			}else parType = "string";
		}
		else if(className == "TParameter<int>")
		{
			TParameter<Int_t> *par = (TParameter<Int_t>*) obj;
			valueString += par -> GetVal();
			parType = "int";
			if(key.Index("NUM_VALUES_") ==0){
				countDownArrayIdx = valueString.Atoi();
				arrayTitleIsSet = "[]";
				skipPrint = true;
			}
		}
		else if(className == "TParameter<double>")
		{
			TParameter<Double_t> *par = (TParameter<Double_t>*) obj;
			valueString += par -> GetVal();
			parType = "double";
		}
		else if(className == "TParameter<bool>")
		{
			TParameter<Bool_t> *par = (TParameter<Bool_t>*) obj;
			valueString += par -> GetVal();
			parType = "bool";
		}else continue;

		if(parameterIsNew)
			comment += "You must modify this parameter value";
		if(!comment.IsNull())
			comment = TString("#  ") + comment;
		if(!skipPrint)
		{
			ostringstream ssLine;
			bool thisIsNewLine = false;
			if(countDownArrayIdx !=0)
			{
				--countDownArrayIdx;
				key = TString(key(0,key.First("[")));
				if(!arrayTitleIsSet.IsNull())
				{
					if(arrayTitleIsSet=="[]")
						arrayTitleIsSet = parType + arrayTitleIsSet;
					thisIsNewLine = true;
					ssLine << left << setw(28) << key << setw(10) << arrayTitleIsSet << valueString;
					arrayTitleIsSet="";
					if(countDownArrayIdx==0)
						ssLine << comment << "\n";
				}
				else if (countDownArrayIdx==0) ssLine << ",  " << valueString << comment << "\n";
				else ssLine << ",  " << valueString;
			}
			else
			{
				thisIsNewLine = true;
				ssLine << left << setw(28) << key << setw(10) << parType << valueString << comment << "\n";
			}
			if(printout)  cout << "printout\n";
				if(thisIsNewLine) or_info(0) << ssLine.str();
			else
			fileOut << ssLine.str();
		}
	}
}

Bool_t OpParameterContainer::SetPar(TString name, Bool_t val, Bool_t overwrite)
{
	if(FindObject(name) != nullptr)
	{
		if(overwrite)
			this -> Remove(FindObject(name));
		else{
			or_error(0) << "Parameter with name " << name << " already exist!\n" ;
			return false;
		}
	}

	Add(new TParameter<Bool_t>(name,val));
	return true;
}

Bool_t OpParameterContainer::SetPar(TString name, Int_t val, Bool_t overwrite)
{
	if(FindObject(name) != nullptr)
	{
		if(overwrite)
			this -> Remove(FindObject(name));
		else{
			or_error(0) << "Parameter with name " << name << " already exist!\n";
			return false;
		}
	}

	Add(new TParameter<Int_t>(name,val));
	return true;
}

Bool_t OpParameterContainer::SetPar(TString name, Double_t val, Bool_t overwrite)
{
	if(FindObject(name) != nullptr)
	{
		if(overwrite)
			this -> Remove(FindObject(name));
		else{
			or_error(0) << "Parameter with name " << name << " already exist!\n";
			return false;
		}
	}

	Add(new TParameter<Double_t>(name,val));
	return true;
}

Bool_t OpParameterContainer::SetPar(TString name, TString val, Bool_t overwrite)
{
	if(FindObject(name) != nullptr)
	{
		if(overwrite)
			this -> Remove(FindObject(name));
		else{
			or_error(0) << "Parameter with name " << name << " already exist!\n";
			return false;
		}
	}

	Add(new TNamed(name,val));
	return true;
}

Bool_t OpParameterContainer::SetParColor(TString name, TString valcolor, Bool_t overwrite)
{
	if(FindObject(name) != nullptr)
	{
		if(overwrite)
			this -> Remove(FindObject(name));
		else{
			or_error(0) << "Parameter with name " << name << " already exist!\n";
			return false;
		}
	}
	Int_t val = 0;
	if(valcolor.IsDec()){
		val = valcolor.Atoi();
	}else if (valcolor.Index("k")==0){
		valcolor.ReplaceAll("kWhite","0");
		valcolor.ReplaceAll("kBlack","1");
		valcolor.ReplaceAll("kGray","920");
		valcolor.ReplaceAll("ored","632");
		valcolor.ReplaceAll("kGreen","416");
		valcolor.ReplaceAll("kBlue","600");
		valcolor.ReplaceAll("kYellow","400");
		valcolor.ReplaceAll("kMagenta","616");
		valcolor.ReplaceAll("kCyan","432");
		valcolor.ReplaceAll("kOrange","800");
		valcolor.ReplaceAll("kSpring","820");
		valcolor.ReplaceAll("kTeal","840");
		valcolor.ReplaceAll("kAzure","860");
		valcolor.ReplaceAll("kViolet","880");
		valcolor.ReplaceAll("kPink","900");

		auto colorCombi = valcolor.Tokenize("+");
		int val1 = (((TObjString *) colorCombi -> At(0)) -> GetString()).Atoi();
		int val2 = 0;
		if(colorCombi -> GetEntriesFast() > 1)
			val2 = (((TObjString*) colorCombi -> At(1)) -> GetString()).Atoi();
		val = val1 + val2;
	}
	else
		return false;
	Add(new TParameter<Int_t>(name,val));
	return true;
}

Bool_t OpParameterContainer::SetPar(TString name, const char* val, Bool_t overwrite)
{
	return SetPar(name, TString(val), overwrite);
}

Int_t OpParameterContainer::GetParN(TString name)
{
	name = TString("NUM_VALUES_")+name;

	TObject* obj = FindObject(name);
	if(obj == nullptr)
		return 0;

	TString className = obj -> ClassName();
	if(className != "TParameter<int>")
		or_error(0) << name << " parameter typee is " << className << ", not int!\n";

	return ((TParameter<Int_t>*) obj) -> GetVal();
}

Bool_t OpParameterContainer::GetParBool(TString name, Int_t idx)
{
	if(idx >=0) return GetParBool(name+"["+idx+"]");
	TObject* obj = FindObject(name);
	if(obj == nullptr){
		or_error(0) << "parameter with name " << name << " does not exist!\n";
		if(!fDebugMode) gApplication -> Terminate();
		SetPar(TString("NEWPAR")+name,false);
		return false;
	}
	TString className = obj -> ClassName();
	if(className != "TParameter<bool>")
	{
		or_error(0) << name << " parameter type is " << className << ", not bool!\n";;
	}
	return ((TParameter<Bool_t>*)obj) -> GetVal();

}

Int_t OpParameterContainer::GetParInt(TString name, Int_t idx)
{
	if(idx >=0) return GetParInt(name+"["+idx+"]");
	TObject* obj = FindObject(name);
	if(obj == nullptr){
		or_error(0) << "parameter with name " << name << " does not exist!\n";;
		if(!fDebugMode) gApplication -> Terminate();
		SetPar(TString("NEWPAR")+name,-999);
		return -999;
	}
	TString className = obj -> ClassName();
	if(className != "TParameter<int>"){
		or_error(0) << name << " parameter type is " << className << ", not int!\n";
	}
	return((TParameter<Int_t> *) obj) -> GetVal();
}

Double_t OpParameterContainer::GetParDouble(TString name, Int_t idx)
{
	if(idx >=0) return GetParDouble(name+"["+idx+"]");
	TObject* obj = FindObject(name);
	if(obj == nullptr){
		or_error(0) << "parameter with name " << name << " does not exist!\n";
		if(!fDebugMode) gApplication -> Terminate();
		SetPar(TString("NEWPAR")+name,-999);
		return -999;
	}
	TString className = obj -> ClassName();
	if(className != "TParameter<double>"){
		or_error(0) << name << " parameter type is " << className << ", not double!\n";
	}
	return((TParameter<Double_t> *) obj) -> GetVal();
}

TString OpParameterContainer::GetParString(TString name, Int_t idx)
{
	if(idx >=0) return GetParString(name+"["+idx+"]");
	TObject* obj = FindObject(name);
	if(obj == nullptr){
		or_error(0) << "parameter with name " << name << " does not exist!\n";
		if(!fDebugMode) gApplication -> Terminate();
		SetPar(TString("NEWPAR")+name,-999);
		return -999;
	}
	TString className = obj -> ClassName();
	if(className != "TNamed"){
		or_error(0) << name << " parameter type is " << className << ", not String!\n";
	}
	return((TNamed *) obj) -> GetTitle();
}

vector<Bool_t> OpParameterContainer::GetParVBool(TString name)
{
	vector<Bool_t> array;
	auto npar = GetParN(name);
	for(auto i=0; i<npar; ++i)
		array.push_back(GetParBool(name,i));
	return array;
}
vector<Int_t> OpParameterContainer::GetParVInt(TString name)
{
	vector<Int_t> array;
	auto npar = GetParN(name);
	for(auto i=0; i<npar; ++i)
		array.push_back(GetParInt(name,i));
	return array;
}
vector<Double_t> OpParameterContainer::GetParVDouble(TString name)
{
	vector<Double_t> array;
	auto npar = GetParN(name);
	for(auto i=0; i<npar; ++i)
		array.push_back(GetParDouble(name,i));
	return array;
}
vector<TString> OpParameterContainer::GetParVString(TString name)
{
	vector<TString> array;
	auto npar = GetParN(name);
	for(auto i=0; i<npar; ++i)
		array.push_back(GetParString(name,i));
	return array;
}

Bool_t OpParameterContainer::CheckPar(TString name)
{
	if (FindObject(name) != nullptr) return true;
	if (FindObject(TString("VECTOR3_")+name) != nullptr) return true;
	if (FindObject(TString("NUM_VALUES_")+name) != nullptr) return true;
	return false;
}

bool OpParameterContainer::IsEmpty() const
{
	if( GetEntriesFast() > 0)
		return false;
	return true;
}
