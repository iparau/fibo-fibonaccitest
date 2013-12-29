#include "stdafx.h"

#include "CTCFibonacciTest.h"

#include "incl/Trace.h"

int _tmain(int argc, _TCHAR* argv[])
{
	SetTraceLevel( TraceDetail );

	bool wasSuccessful = false;

	test::CTCFibonacciTest* testData = new test::CTCFibonacciTest();

	//double check for allocation errors
	if ( 0 != testData ) {
		wasSuccessful = testData->runTests();
	
		//finished processing, so we delete inputData in order to release the parser a
		//and call Teminate, below
		delete testData;
	}

	return wasSuccessful ? 0 : 1;

}