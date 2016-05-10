///////////////////////////////////////////////////////////////////////////////
//
// (C) 1997-2006 SRC, LLC  -   All rights reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// Module: FORMULAADDIN.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef FORMULAADDIN_H__
#define FORMULAADDIN_H__

// an XML file is required to addin in functions for the formula library.
// they can either be added via specifying a formula expression that 
// will execute in place of the new function or the function can be defined 
// in an external dll.  Any number of new functions can be defined in a single
// XML file.
//
// If defining a new function in terms of an expression, the parameters will
// be P1, P2, etc.
// if there are more than 9 parameters, the parameter names will be P01, ... P15, ...
/*
<FormulaAddIn>
	<Function>
		<Name>CountWords</Name>

		<!--  variable="true" is only valid for a Dll function plugin and then the value
			represents the minimum # of parameters -->
		<NumParams variable="false">1</NumParams>
		<Category>String</Category>
		<InsertText>CountWords(s)</InsertText>
		<Description>Counts the words in a string</Description>
		
		<!-- Either the Dll or the Formula tag is required, but not both -->
		<Formula>RegEx_CountMatches(P1, "\<")</Formula>
		<!--
		<Dll>
			<!-- The Dll path is relative to the application path that will be running it -->
			<Name>abc.dll</Name>
			<EntryPoint>CountWords</EntryPoint>
		</Dll> -->
	</Function>
</FormulaAddIn>
*/

//  API definition for Formula functions plugins
struct FormulaAddInData
{
	int nVarType;  // 1 for double, 2 for wchar_t
	int isNull; // 1 if NULL, 0 if valid

	double dVal; // valid if nVarType==1 && isNull==0
	const wchar_t * pVal; // valid if nVarType==2 && isNull==0

	inline FormulaAddInData()
	{
		memset(this, 0, sizeof(*this));
	}
};

// Formula function signature.  The function exposed in the <Dll><EntryPoint> of the 
// FormulaAddIn XML MUST have exactly this signature.
// this MUST be thread safe
//
// return 1 for success
//	If 1, place return value in pReturnValue.  If returning a string, it MUST be allocated with
//		GlobalAlloc and will be free'd with GlobalFree
// or 0 for failure.  
//	If 0, place a string in the pReturnValue that represents the error message
//		Again, the string MUST be allocated with GlobalAlloc
typedef long ( _stdcall * FormulaAddInPlugin)(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue);


#endif //FORMULAADDIN_H__