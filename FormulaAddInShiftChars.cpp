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

// this takes a string and a "bit shift count"
// every character in the string, it will be shifted up by bitshiftcount amount
extern "C" long _declspec(dllexport) _stdcall ShiftChars(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    uint64_t shiftCount = (uint64_t)(pArgs[1].dVal);
    std::wstring strRet;
    for (int x = 0; x < wcslen(pArgs[0].pVal); ++x) {
        strRet += (wchar_t)((uint64_t)(pArgs[0].pVal[x]) + shiftCount);
    }
    SetString(pReturnValue, strRet.c_str());
    return 1;
}

// this takes a string and a "bit shift count"
// for every numeric character in the string:
//     it will be shifted up by bitshiftcount amount (mod 10: wrapping from 9 to 0)
// for all non-numeric, the chracter will be left unchanged
extern "C" long _declspec(dllexport) _stdcall ShiftCharsNumeric(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    uint64_t shiftCount = (uint64_t)(pArgs[1].dVal);
    uint64_t oldChar, newChar;
    std::wstring strRet;
    for (int x = 0; x < wcslen(pArgs[0].pVal); ++x) {
        oldChar = (uint64_t)(pArgs[0].pVal[x]);
        if (oldChar >= (uint64_t)(((wchar_t *)L"0")[0]) && 
            oldChar <= (uint64_t)(((wchar_t *)L"9")[0])) {
            // determine potential shift amount
            newChar = oldChar + shiftCount;
            // adjust shift if it will put us off the end
            while (newChar >(uint64_t)(((wchar_t *)L"9")[0])) {
                newChar -= 10;
            }
            while (newChar < (uint64_t)(((wchar_t *)L"0")[0])) {
                newChar += 10;
            }
            // perform the shift and add to output string
            strRet += (wchar_t)newChar;
        } else {
            // grab next character unchanged
            strRet += pArgs[0].pVal[x];
        }
    }
    SetString(pReturnValue, strRet.c_str());
    return 1;
}

// this takes a string and a "bit shift count"
// for every alpha character in the string:
//     it will be shifted up by bitshiftcount amount (mod 26: wrapping from Z to A or z to a)
// for all non alpha, the chracter will be left unchanged
extern "C" long _declspec(dllexport) _stdcall ShiftCharsAlpha(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    uint64_t shiftCount = (uint64_t)(pArgs[1].dVal);
    uint64_t oldChar, newChar;
    std::wstring strRet;
    for (int x = 0; x < wcslen(pArgs[0].pVal); ++x) {
        oldChar = (uint64_t)(pArgs[0].pVal[x]);
        if (oldChar >= (uint64_t)(((wchar_t *)L"A")[0]) &&
            oldChar <= (uint64_t)(((wchar_t *)L"Z")[0])) {
            // determine potential shift amount
            newChar = oldChar + shiftCount;
            // adjust shift if it will put us off the end
            while (newChar >(uint64_t)(((wchar_t *)L"Z")[0])) {
                newChar -= 26;
            }
            while (newChar < (uint64_t)(((wchar_t *)L"A")[0])) {
                newChar += 26;
            }
            // perform the shift and add to output string
            strRet += (wchar_t)newChar;
        } else if (oldChar >= (uint64_t)(((wchar_t *)L"a")[0]) &&
                   oldChar <= (uint64_t)(((wchar_t *)L"z")[0])) {
            // determine potential shift amount
            newChar = oldChar + shiftCount;
            // adjust shift if it will put us off the end
            while (newChar >(uint64_t)(((wchar_t *)L"z")[0])) {
                newChar -= 26;
            }
            while (newChar < (uint64_t)(((wchar_t *)L"a")[0])) {
                newChar += 26;
            }
            // perform the shift and add to output string
            strRet += (wchar_t)newChar;
        } else {
            // grab next character unchanged
            strRet += pArgs[0].pVal[x];
        }
    }
    SetString(pReturnValue, strRet.c_str());
    return 1;
}

// this takes a string and a "bit shift count"
// for every numeric character in the string:
//     it will be shifted up by bitshiftcount amount (mod 10: wrapping from 9 to 0)
// for every alpha character in the string:
//     it will be shifted up by bitshiftcount amount (mod 26: wrapping from Z to A or z to a)
// for all non alpha, non-numeric, the chracter will be left unchanged
extern "C" long _declspec(dllexport) _stdcall ShiftCharsAlphaNum(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
    uint64_t shiftCount = (uint64_t)(pArgs[1].dVal);
    uint64_t oldChar, newChar;
    std::wstring strRet;
    for (int x = 0; x < wcslen(pArgs[0].pVal); ++x) {
        oldChar = (uint64_t)(pArgs[0].pVal[x]);
        if (oldChar >= (uint64_t)(((wchar_t *)L"A")[0]) && oldChar <= (uint64_t)(((wchar_t *)L"Z")[0])) {
            // determine potential shift amount
            newChar = oldChar + shiftCount;
            // adjust shift if it will put us off the end
            while (newChar >(uint64_t)(((wchar_t *)L"Z")[0])) {
                newChar -= 26;
            } 
            while (newChar < (uint64_t)(((wchar_t *)L"A")[0])) {
                newChar += 26; // in case they shifted a negative amount off the front...
            }
            // add shifted char to output string
            strRet += (wchar_t)newChar;
        } else if (oldChar >= (uint64_t)(((wchar_t *)L"a")[0]) && oldChar <= (uint64_t)(((wchar_t *)L"z")[0])) {
            newChar = oldChar + shiftCount;
            while (newChar >(uint64_t)(((wchar_t *)L"z")[0])) {
                newChar -= 26;
            } 
            while (newChar < (uint64_t)(((wchar_t *)L"a")[0])) {
                newChar += 26;
            }
            strRet += (wchar_t)newChar;
        } else if (oldChar >= (uint64_t)(((wchar_t *)L"0")[0]) && oldChar <= (uint64_t)(((wchar_t *)L"9")[0])) {
            newChar = oldChar + shiftCount;
            while (newChar > (uint64_t)(((wchar_t *)L"9")[0])) {
                newChar -= 10;
            } 
            while (newChar < (uint64_t)(((wchar_t *)L"0")[0])) {
                newChar += 10;
            }
            strRet += (wchar_t)newChar;
        } else {
            // grab next character unchanged
            strRet += pArgs[0].pVal[x];
        }
    }
    SetString(pReturnValue, strRet.c_str());
    return 1;
}
