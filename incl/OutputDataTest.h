#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "XmlFileParser.h"

namespace test {

class COutputDataTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE( COutputDataTest );

	CPPUNIT_TEST( testOutput_buildOutputXml );
	CPPUNIT_TEST( testOutput_getFibonacciID );
	CPPUNIT_TEST( testOutput_countChildrenNumbers );
	CPPUNIT_TEST( testOutput_validateChildrenNumberNodes );

	CPPUNIT_TEST_SUITE_END();

public: 
	COutputDataTest();
	virtual ~COutputDataTest();

public:

	void setUp();
	void tearDown();

	void testOutput_buildOutputXml();
	void testOutput_getFibonacciID();
	void testOutput_countChildrenNumbers();
	void testOutput_validateChildrenNumberNodes();

private:
	tools::CXmlFileParser* inputXmlFileParser;
	tools::CXmlFileParser* outputXmlFileParser;
	const char* inputXmlFilePath;
	const char* outputXmlFilePath;
};

}; //namespace test

