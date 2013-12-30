// CTCFibonacciTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

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

		// informs test-listener about testresults
		CppUnit::TestResult testresult;

		// register listener for collecting the test-results
		CppUnit::TestResultCollector collectedresults;
		testresult.addListener (&collectedresults);

		// register listener for per-test progress output
		CppUnit::BriefTestProgressListener progress;
		testresult.addListener (&progress);

		// insert test-suite at test-runner by registry
		CppUnit::TextUi::TestRunner runner;
		runner.addTest (CppUnit::TestFactoryRegistry::getRegistry().makeTest ());
		runner.run(testresult);

		// output results in compiler-format
		CppUnit::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
		compileroutputter.write ();

		// Output XML for Jenkins CPPunit plugin
		std::ofstream xmlFileOut("FibonacciTestResults.xml");
		CppUnit::XmlOutputter xmlOut(&collectedresults, xmlFileOut);
		xmlOut.write();

		// return 0 if tests were successful
		retVal =  collectedresults.wasSuccessful();

		WriteTrace( TraceDebug, "Test session ended.");
	}
	else {
		WriteTrace( TraceError, "Test session NOT started, library xerces not loaded.");
	}

	return retVal;
}

} //namespace test
