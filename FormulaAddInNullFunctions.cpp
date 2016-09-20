// FormulaAddInSample.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "FormulaAddIn.h"
#include <string>

//The resulting dll and accompanying XML file need to be installed into RunTimeData/FormulaAddIns to function

void SetString(FormulaAddInData *pReturnValue, const wchar_t *pString)
{
    size_t nLen = wcslen(pString);
    wchar_t *pStringRet = (wchar_t *)GlobalAlloc(GMEM_FIXED, (nLen+1)*sizeof(wchar_t));
    wcscpy(pStringRet, pString);
    pReturnValue->pVal = pStringRet;
    pReturnValue->nVarType = 2;
}

// this takes a variable number of numbers and produces an average of the non-null numbers
// if there are no non-null numbers, NULL will be returned.
extern "C" long _declspec(dllexport) _stdcall AvgNonNull(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    double dRet = 0.0;
    double dCountNonNull = 0.0;
    for (int x=0; x<nNumArgs; ++x) {
        if (pArgs[x].nVarType!=1) {
            SetString(pReturnValue, L"AvgNonNull requires only numeric parameters");
            return 0;
        }
        if (pArgs[x].isNull==0) {
            dCountNonNull += 1.0;
            dRet += pArgs[x].dVal;
        }
    }
    if (dCountNonNull > 0.0) {
        pReturnValue->nVarType = 1;
        pReturnValue->dVal = (dRet / dCountNonNull);
    } else {
        pReturnValue->nVarType = 2;
        pReturnValue->dVal = NULL;
    }
    return 1;
}

// this takes a variable number of objects and determines how many are not null
extern "C" long _declspec(dllexport) _stdcall CountNonNull(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    int dCountNonNull = 0;
    for (int x = 0; x<nNumArgs; ++x) {
        if (pArgs[x].isNull == 0) {
            dCountNonNull += 1;
        }
    }
    pReturnValue->nVarType = 1;
    pReturnValue->dVal = dCountNonNull;
    return 1;
}


// this takes a variable number of objects returns the first non-null value in the list
// if there are no non-null items, NULL will be returned.
extern "C" long _declspec(dllexport) _stdcall Coalesce(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    for (int x = 0; x<nNumArgs; ++x) {
        if (pArgs[x].isNull == 0) {
            pReturnValue->nVarType = pArgs[x].nVarType;
            pReturnValue->dVal = pArgs[x].dVal;
            pReturnValue->pVal = pArgs[x].pVal;
            return 1;
        }
    }
    pReturnValue->nVarType = 2;
    pReturnValue->dVal = 0;
    pReturnValue->pVal = NULL;
    return 1;
}
