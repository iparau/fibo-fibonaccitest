#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "XmlFileParser.h"

namespace test {

class CInputDataTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE( CInputDataTest );

	CPPUNIT_TEST( testInput_Initialise );
	CPPUNIT_TEST( testInput_getFibonacciID );

	CPPUNIT_TEST_SUITE_END();

public: 
	CInputDataTest();
	virtual ~CInputDataTest();

public:

	void setUp();
	void tearDown();

	void testInput_Initialise();
	void testInput_getFibonacciID();

private:
	tools::CXmlFileParser* xmlFileParser;
	const char* inputXmlFilePath;
};

}; //namespace test

