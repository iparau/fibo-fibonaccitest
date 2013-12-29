// CTCFibonacciTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <InputDataTest.h>
#include "incl/InputData.h"
#include "incl/Trace.h"

namespace test {

CPPUNIT_TEST_SUITE_REGISTRATION( CInputDataTest );

CInputDataTest::CInputDataTest()
: xmlFileParser(0)
{
	inputXmlFilePath = "input.xml";
}

CInputDataTest::~CInputDataTest()
{

}

void CInputDataTest::setUp()
{
    WriteTrace( TraceDebug, "Test setUp.");

	xmlFileParser = new tools::CXmlFileParser();
}

void CInputDataTest::tearDown()
{
    WriteTrace( TraceDebug, "Test tearDown.");

	WriteTrace( TraceDebug, "Closing input document %s", inputXmlFilePath);

	//double check for allocation errors
	if ( 0 != xmlFileParser ) {
		//finished processing, so we delete inputData in order to release the parser a
		delete xmlFileParser;
		xmlFileParser = 0;
	}
	
}

void CInputDataTest::testInput_Initialise()
{
	WriteTrace( TraceDebug, "Test Initialise.");

	bool testInitialise = false;

	core::CInputData inputData;
	 
	testInitialise = inputData.Initialize( inputXmlFilePath ); 

	CPPUNIT_ASSERT( testInitialise );
}

void CInputDataTest::testInput_getFibonacciID()
{
	WriteTrace( TraceDebug, "Test getFibonacciID.");

	bool testInput = false;

	//double check for allocation errors
	if ( 0 != xmlFileParser ) {
		if ( true == xmlFileParser->Initialize( inputXmlFilePath ) ) {
			WriteTrace( TraceDebug, "Input Document succesfully parsed by internal xml parser.");
			//double check for allocation errors
			core::CInputData inputData;

			if (true == inputData.Initialize( inputXmlFilePath )) {
				WriteTrace( TraceDebug, "Input Document succesfully parsed by CInputData.");
				testInput = ( xmlFileParser->getFibonacciID() == inputData.getFibonacciID() );
			}

		}
	}

	CPPUNIT_ASSERT( testInput );
}

} //namespace test
