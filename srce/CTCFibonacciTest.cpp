// CTCFibonacciTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <xercesc/util/PlatformUtils.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

#include "CTCFibonacciTest.h"

#include "AStr.h"
#include "Trace.h"

XERCES_CPP_NAMESPACE_USE
using namespace core::tools;

namespace test {

CTCFibonacciTest::CTCFibonacciTest()
: bInitialized( true )
{
	try {
		XMLPlatformUtils::Initialize();
	}
	catch ( const XMLException& toCatch ) {
		// Do your failure processing here
        WriteTrace( TraceError, "Error Message: %s", 
			A(toCatch.getMessage()) );

		bInitialized = false;
	}
	catch ( ... ) {
		// Do your failure processing here
        WriteTrace( TraceError, "Error Message: Unknown Exception");

		bInitialized = false;
	}
}

CTCFibonacciTest::~CTCFibonacciTest()
{
	// Other terminations and cleanup.
	XMLPlatformUtils::Terminate();
}

bool CTCFibonacciTest::runTests()
{
	// Do your actual work with Xerces-C++ here.
	bool retVal = false;

	if ( true == isInitialized() ) {
		WriteTrace( TraceDebug, "Test session started.");

		CppUnit::TextUi::TestRunner runner;
		CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
		runner.addTest( registry.makeTest() );

		retVal = runner.run( "", false );

		WriteTrace( TraceDebug, "Test session ended.");
	}
	else {
		WriteTrace( TraceError, "Test session NOT started, library xerces not loaded.");
	}

	return retVal;
}

} //namespace test
