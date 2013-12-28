#pragma once

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

XERCES_CPP_NAMESPACE_USE

namespace test {
namespace tools {

	class CXmlFileParser
	{
	public:
		CXmlFileParser(void);
		virtual ~CXmlFileParser(void);
		// Used to initialize the xml input data from file
		bool Initialize(const char * xmlFilePath);

		int getFibonacciID() { return fibonacciID; };
		int countChildrenNumbers();
		bool validateChildrenNumberNodes();

	private:
		bool extractFibonacciID();
		static int fibonacci( int n );

	protected:
		int              fibonacciID;
		XercesDOMParser* parser;
		ErrorHandler*    errHandler;
	};

}; //namespace tools

}; //namespace test



