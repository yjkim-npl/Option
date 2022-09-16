#ifndef OpParameterContainer_h
#define OpParameterContainer_h 1

#include "OpGlobal.hh"
#include "TObjArray.h"
#include "TNamed.h"
#include "TParameter.h"
#include <vector>

using std::vector;

class OpParameterContainer : public TObjArray
{
	public:
		OpParameterContainer(Bool_t debug = false);
		OpParameterContainer(const char *parName, Bool_t debug = false);
		virtual ~OpParameterContainer() {}

		void SaveAs(const char *filename, Option_t *option = "") const;

		void SetDebugMode(Bool_t val = true);

		virtual void Print(Option_t *option = "") const;

		void ReplaceEnvironmentVariable(TString &val);
		virtual Int_t AddFile(TString fileName, TString parNameForFile = "");
		virtual Int_t AddPar(OpParameterContainer *parc, TString parNameForFile = "");
		Int_t GetNumInputFiles();

		Bool_t SetPar(TString name, Bool_t       val, Bool_t overwrite = false);
		Bool_t SetPar(TString name, Int_t        val, Bool_t overwrite = false);
		Bool_t SetPar(TString name, Double_t     val, Bool_t overwrite = false);
		Bool_t SetPar(TString name, TString      val, Bool_t overwrite = false);
		Bool_t SetPar(TString name, const char*  val, Bool_t overwrite = false);
		Bool_t SetParColor(TString name, TString valColor, Bool_t overwrite=false);

		Int_t 		GetParN(TString name);
		Bool_t 		GetParBool(TString name, Int_t idx = -1);
		Int_t 		GetParInt(TString name, Int_t idx = -1);
		Double_t 	GetParDouble(TString name, Int_t idx = -1);
		TString 	GetParString(TString name, Int_t idx = -1);

		vector<Bool_t>  GetParVBool	 (TString name);
		vector<Int_t>	GetParVInt	 (TString name);
		vector<Double_t>GetParVDouble(TString name);
		vector<TString> GetParVString(TString name);

		Bool_t CheckPar(TString name);

		bool IsEmpty() const;

	private:
		Bool_t fDebugMode = false;
		Int_t fNumInputFiles=0;
		ClassDef(OpParameterContainer,1)
};
#endif
