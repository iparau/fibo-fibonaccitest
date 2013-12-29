// CTCFibonacciTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <OutputDataTest.h>
#include "incl/OutputData.h"
#include "incl/Trace.h"

namespace test {

CPPUNIT_TEST_SUITE_REGISTRATION( COutputDataTest );

COutputDataTest::COutputDataTest()
: inputXmlFileParser(0), outputXmlFileParser(0)
{
	inputXmlFilePath = "input.xml";
	outputXmlFilePath = "output.xml";
}

COutputDataTest::~COutputDataTest()
{

}

void COutputDataTest::setUp()
{
    WriteTrace( TraceDebug, "Test setUp.");

	inputXmlFileParser = new tools::CXmlFileParser();
	outputXmlFileParser = new tools::CXmlFileParser();
}

void COutputDataTest::tearDown()
{
    WriteTrace( TraceDebug, "Test tearDown.");

	WriteTrace( TraceDebug, "Closing output document %s", outputXmlFilePath);

	//double check for allocation errors
	if ( 0 != inputXmlFileParser ) {
		//finished processing, so we delete inputData in order to release the parser a
		delete inputXmlFileParser;
		inputXmlFileParser = 0;
	}

	//double check for allocation errors
	if ( 0 != outputXmlFileParser ) {
		//finished processing, so we delete inputData in order to release the parser a
		delete outputXmlFileParser;
		outputXmlFileParser = 0;
	}
	
}

void COutputDataTest::testOutput_buildOutputXml()
{
	WriteTrace( TraceDebug, "Test buildOutputXml.");

	bool testInitialise = false;

	//double check for allocation errors
	if ( 0 != inputXmlFileParser ) {
		if ( true == inputXmlFileParser->Initialize( inputXmlFilePath ) ) {
			WriteTrace( TraceDebug, "Input Document succesfully parsed by internal xml parser.");
			core::COutputData outputData( inputXmlFileParser->getFibonacciID() );
			testInitialise = outputData.buildOutputXml( outputXmlFilePath );
		} 
	}

	CPPUNIT_ASSERT( testInitialise );
}

void COutputDataTest::testOutput_getFibonacciID()
{
	WriteTrace( TraceDebug, "Test getFibonacciID.");

	bool testInput = false;

	//double check for allocation errors
	if ( 0 != inputXmlFileParser ) {
		if ( true == inputXmlFileParser->Initialize( inputXmlFilePath ) ) {
			WriteTrace( TraceDebug, "Input Document succesfully parsed by internal xml parser.");
			//double check for allocation errors
			if ( 0 != outputXmlFileParser ) {
				if (true == outputXmlFileParser->Initialize( outputXmlFilePath )) {
					WriteTrace( TraceDebug, "Output Document succesfully parsed by internal xml parser.");
					testInput = ( inputXmlFileParser->getFibonacciID() == outputXmlFileParser->getFibonacciID() );
				}
			}
		}
	}

	CPPUNIT_ASSERT( testInput );
}

void COutputDataTest::testOutput_countChildrenNumbers()
{
	WriteTrace( TraceDebug, "Test countChildrenNumbers.");

	bool testInput = false;

	//double check for allocation errors
	if ( 0 != outputXmlFileParser ) {
		if (true == outputXmlFileParser->Initialize( outputXmlFilePath )) {
			WriteTrace( TraceDebug, "Output Document succesfully parsed by internal xml parser.");
			testInput = ( ( outputXmlFileParser->getFibonacciID() + 1 ) == outputXmlFileParser->countChildrenNumbers() );
		}
	}

	CPPUNIT_ASSERT( testInput );
}

void COutputDataTest::testOutput_validateChildrenNumberNodes()
{
	WriteTrace( TraceDebug, "Test validateChildrenNumberNodes.");

	bool testInput = false;

	//double check for allocation errors
	if ( 0 != outputXmlFileParser ) {
		if (true == outputXmlFileParser->Initialize( outputXmlFilePath )) {
			WriteTrace( TraceDebug, "Output Document succesfully parsed by internal xml parser.");
			testInput = outputXmlFileParser->validateChildrenNumberNodes();
		}
	}

	CPPUNIT_ASSERT( testInput );
}

}; //namespace test
