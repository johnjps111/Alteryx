// FormulaAddInSample.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "FormulaAddIn.h"
#include <string>

//The resulting dll and accompanying XML file need to be installed into runtimedata/FormulaAddIns to function

void SetString(FormulaAddInData *pReturnValue, const wchar_t *pString)
{
	size_t nLen = wcslen(pString);
	wchar_t *pStringRet = (wchar_t *)GlobalAlloc(GMEM_FIXED, (nLen+1)*sizeof(wchar_t));
	wcscpy(pStringRet, pString);
	pReturnValue->pVal = pStringRet;
	pReturnValue->nVarType = 2;
}

// this sample takes a variable number of numbers and simply sums them together, returning a number
extern "C" long _declspec(dllexport) _stdcall AverageNonNull(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	double dRet = 0.0;
	double dCnt = 0.0;

	for (int x=0; x<nNumArgs; ++x)
	{
		if (pArgs[x].nVarType!=1)
		{
			SetString(pReturnValue, L"Sample_AddNumbers requires only numeric parameters");
			return 0;
		}

		if (pArgs[x].isNull==0) {
			dRet += pArgs[x].dVal;
			dCnt += 1.0;
		}
	}

	if (dCnt > 0.0) {
		pReturnValue->nVarType = 1;
		pReturnValue->dVal = dRet / dCnt;
	} else {
		pReturnValue->nVarType = 2;
		pReturnValue->dVal = NULL;
	}
	return 1;
}
