#include "StdAfx.h"
#include "XmlFileParser.h"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

#include "AStr.h"
#include "XStr.h"
#include "NStr.h"

#include "Trace.h"

XERCES_CPP_NAMESPACE_USE
using namespace std;
using namespace core::tools;

namespace test {
namespace tools {

CXmlFileParser::CXmlFileParser(void)
: parser( NULL ), errHandler( NULL)
{

}


CXmlFileParser::~CXmlFileParser(void)
{
	//
	//  Clean up the error handler. The parser does not adopt handlers
	//  since they could be many objects or one object installed for multiple
	//  handlers.
	//
	if (0 != errHandler) {
		delete errHandler;
	}
	
	//
	//  Delete the parser itself.  Must be done prior to calling Terminate.
	// 
	if (0 != parser) {
		delete parser;
	}
}

int CXmlFileParser::fibonacci( int n ) {
	int retVal = 0;
	
	switch (n) {
		case 0:
			retVal = 0;
			break;
		case 1:
		case 2:
			retVal = 1;
			break;
		default:
			retVal = fibonacci(n-1) + fibonacci(n-2);
			break;
	}

	return retVal;
}


// Used to initialize the xml input data from file
bool CXmlFileParser::Initialize(const char * xmlFilePath)
{
	bool retVal = false;

	parser = new XercesDOMParser();

    //memory security check
	if ( 0 != parser ) {
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);    // optional

		errHandler = (ErrorHandler*) new HandlerBase();

		if (0 != errHandler) {
			parser->setErrorHandler(errHandler);

			try {
				WriteTrace( TraceDebug, "Parsing input document %s", xmlFilePath);
				parser->parse(xmlFilePath);

				retVal = true;
			}
			catch (const OutOfMemoryException& toCatch) {
				WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
						A(toCatch.getMessage()) );
			}
			catch (const XMLException& toCatch) {
				WriteTrace( TraceError, "XMLException! Exception message is: %s",
						A(toCatch.getMessage()) );
			}
			catch (const DOMException& toCatch) {
				WriteTrace( TraceError, "DOMException! Exception message is: %s",
						A(toCatch.getMessage()) );
			}
			catch (...) {
				WriteTrace( TraceError, "Unexpected Exception" );
			}

	
			if ( true == retVal ) {
				//no error occured during parsing
				WriteTrace( TraceDebug, "Parse succeded!" );

				retVal = extractFibonacciID( );
			}
		}

	}

 	return retVal;
}

// Used to initialize the xml input data from xml DOM doc
bool  CXmlFileParser::extractFibonacciID()
{
	bool retVal = false;

	DOMDocument* inputDoc = parser->getDocument();

	if ( 0 != inputDoc ) {
		WriteTrace( TraceDebug, "Extracting fibonacci id from DOM document" );

		try {
			DOMElement* rootNode = inputDoc->getDocumentElement();
				
			if (0 != rootNode) {
				const XMLCh *rootTagName = rootNode->getTagName();
					 
				WriteTrace( TraceDebug, "Root node Tag name is %s", 
					A(rootTagName) );

				if ( rootNode ->hasAttributes() ) {
					const char* attrIDTagName = "id";
						 
					if ( rootNode->hasAttribute( X( attrIDTagName ) ) ) {
						DOMAttr* attrID = rootNode->getAttributeNode( X( attrIDTagName ) );

						if (0 != attrID) {
							const XMLCh* attrIDValue = attrID->getValue();
								 
							WriteTrace( TraceInfo, "Extract succceded, id attribute value is %s", 
								A( attrIDValue ) );

							fibonacciID = XMLString::parseInt( attrIDValue );

							retVal = true;
						}
						else {
							//detected internal document error
							retVal = false;

							WriteTrace( TraceError, "Document root node invalid, atribute 'id' not detected!" );
						}
					}
					else {
						//detected internal document error
						retVal = false;

						WriteTrace( TraceError, "Document root node invalid, atribute 'id' not identified!" );
					}
				}
				else {
					//detected internal document error
					retVal = false;

					WriteTrace( TraceError, "Document root node invalid, no attributes detected!" );
				}
			}
			else {
				//detected internal document error
				retVal = false;

				WriteTrace( TraceError, "Document root node invalid, not identified!" );
			}
		}
		catch (const OutOfMemoryException& toCatch) {
			WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const XMLException& toCatch) {
			WriteTrace( TraceError, "XMLException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const DOMException& toCatch) {
			WriteTrace( TraceError, "DOMException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (...) {
			WriteTrace( TraceError, "Unexpected Exception" );
		}
	}
	else {
		//detected internal document error
		retVal = false;

		WriteTrace( TraceError, "Document content invalid!" );
	}

	return retVal;
}

int  CXmlFileParser::countChildrenNumbers()
{
	int retVal = -1;

	DOMDocument* inputDoc = parser->getDocument();

	if ( 0 != inputDoc ) {
		WriteTrace( TraceDebug, "Counting fibonacci childrens \"number\" list from DOM document" );

		try {
			DOMElement* rootNode = inputDoc->getDocumentElement();
			
			//double check for allocation errors	
			if (0 != rootNode) {
				const XMLCh *rootTagName = rootNode->getTagName();
					 
				WriteTrace( TraceDebug, "Root node Tag name is %s", 
					A(rootTagName) );

				if ( rootNode->hasChildNodes() ) {
					
					DOMNodeList* numberNodesList = rootNode->getChildNodes();

					//double check for allocation errors
					if (0 != numberNodesList) {
						XMLSize_t numberNodesListLength  = numberNodesList->getLength();

						WriteTrace( TraceDebug, "Root node children list length is %s", 
							N(numberNodesListLength) );

						//initialize counter of "number" nodes
						retVal = 0;

						for ( XMLSize_t index = 0; index < numberNodesListLength; index++ ) {
							DOMNode* node = numberNodesList->item( index );

							//double check for allocation errors
							if ( 0 != node) {
								//count only element type nodes
								if ( DOMNode::ELEMENT_NODE == node->getNodeType() ) {
									const XMLCh *nodeTagName = node->getNodeName();

									if  ( 0 == XMLString::compareString( X ( "number" ) , nodeTagName ) ) {
										retVal++;
									}
								}
							}
						}
					}
					else {
					    WriteTrace( TraceError, "Document root node children list invalid, internal error retrieving child nodes list!" );
					}
				}
				else {
					//detected internal document error

					WriteTrace( TraceError, "Document root node children list invalid, no childrens detected!" );
				}
			}
			else {
				//detected internal document error

				WriteTrace( TraceError, "Document root node invalid, not identified!" );
			}
		}
		catch (const OutOfMemoryException& toCatch) {
			WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const XMLException& toCatch) {
			WriteTrace( TraceError, "XMLException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const DOMException& toCatch) {
			WriteTrace( TraceError, "DOMException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (...) {
			WriteTrace( TraceError, "Unexpected Exception" );
		}
	}
	else {
		//detected internal document error
		retVal = false;

		WriteTrace( TraceError, "Document content invalid!" );
	}

	return retVal;
}

bool CXmlFileParser::validateChildrenNumberNodes()
{
	bool retVal = false;

	DOMDocument* inputDoc = parser->getDocument();

	if ( 0 != inputDoc ) {
		WriteTrace( TraceDebug, "Validating fibonacci childrens \"number\" list from DOM document" );

		try {
			DOMElement* rootNode = inputDoc->getDocumentElement();
			
			//double check for allocation errors	
			if (0 != rootNode) {
				const XMLCh *rootTagName = rootNode->getTagName();
					 
				WriteTrace( TraceDebug, "Root node Tag name is %s", 
					A(rootTagName) );

				bool bAttrValid = true;

				////////////////////////////////////////////////////////////////////////////
				if ( rootNode ->hasAttributes() ) {
					const char* attrIDTagName = "id";
						 
					if ( rootNode->hasAttribute( X( attrIDTagName ) ) ) {
						DOMAttr* attrID = rootNode->getAttributeNode( X( attrIDTagName ) );

						if (0 != attrID) {
							const XMLCh* attrIDValue = attrID->getValue();
								 
							WriteTrace( TraceInfo, "Extract id succceded, id attribute value is %s", 
								A( attrIDValue ) );

							int fibonacciNumberID = XMLString::parseInt( attrIDValue );

							if ( getFibonacciID() != fibonacciNumberID ) {
								WriteTrace( TraceError, "Root node invalid, atribute 'id' is %s and should be %s!",
									N ( fibonacciNumberID ),N ( getFibonacciID() ) );

								bAttrValid = false;
							}
							else {
								WriteTrace( TraceDebug, "Root node atribute 'id' value is valid!");
							}
						}
						else {
							//detected internal document error
							bAttrValid = false;

							WriteTrace( TraceError, "Root node invalid, atribute 'id' not detected!" );
						}
					}
					else {
						//detected internal document error
						bAttrValid = false;;

						WriteTrace( TraceError, "Root node invalid, atribute 'id' not identified!" );
					}

					/////////////////////////////////////////////////////////
					const char* attrValueTagName = "value";
						 
					if ( rootNode->hasAttribute( X( attrValueTagName ) ) ) {
						DOMAttr* attrValue = rootNode->getAttributeNode( X( attrValueTagName ) );

						if (0 != attrValue) {
							const XMLCh* attrValueValue = attrValue->getValue();
								 
							WriteTrace( TraceInfo, "Extract value succceded, value attribute value is %s", 
								A( attrValueValue ) );

							int fibonacciNumberValue = XMLString::parseInt( attrValueValue );
							int fibonacciIndexValue = this->fibonacci( getFibonacciID() );
																	
							if ( fibonacciNumberValue != fibonacciIndexValue ) {
								WriteTrace( TraceError, "Root node invalid, atribute 'value' is %s and should be %s!",
									N ( fibonacciNumberValue ),N ( fibonacciIndexValue ) );

								bAttrValid = false;;
							}
							else {
								WriteTrace( TraceDebug, "Root node atribute 'value' value is valid!");
							}
						}
						else {
							//detected internal document error
							bAttrValid = false;;

							WriteTrace( TraceError, "Root node invalid, atribute 'value' not detected!" );
						}
					}
					else {
						//detected internal document error
						bAttrValid = false;;

						WriteTrace( TraceError, "Root node invalid, atribute 'value' not identified!" );
					}
					/////////////////////////////////////////////////////////
				}
				else {
					//detected internal document error
					bAttrValid = false;

					WriteTrace( TraceError, "Root node invalid, no attributes detected!" );
				}
				////////////////////////////////////////////////////////////////////////////

				//continue if root node attributes are valid
				if ( bAttrValid ) {

					if ( rootNode->hasChildNodes() ) {
					
						DOMNodeList* numberNodesList = rootNode->getChildNodes();

						//double check for allocation errors
						if (0 != numberNodesList) {
							XMLSize_t numberNodesListLength  = numberNodesList->getLength();

							WriteTrace( TraceDebug, "Root node children list length is %s", 
								N(numberNodesListLength) );

							//initialize counter of "number" nodes
							int fibonacciIndex = 0;

							for ( XMLSize_t index = 0; index < numberNodesListLength; index++ ) {
								DOMNode* node = numberNodesList->item( index );

								//double check for allocation errors
								if ( 0 != node) {
									//count only element type nodes
									if ( DOMNode::ELEMENT_NODE == node->getNodeType() ) {
									
										const XMLCh *nodeTagName = node->getNodeName();

										if  ( 0 == XMLString::compareString( X ( "number" ) , nodeTagName ) ) {
											///////////////////////////////////////////////////////////////////////////
											DOMElement* elem = dynamic_cast<DOMElement*>(node);

											//double check for allocation errors
											if ( 0 != elem ) {
																																																																													if ( elem ->hasAttributes() ) {
												const char* attrIDTagName = "id";
						 
												if ( elem->hasAttribute( X( attrIDTagName ) ) ) {
													DOMAttr* attrID = elem->getAttributeNode( X( attrIDTagName ) );

													if (0 != attrID) {
														const XMLCh* attrIDValue = attrID->getValue();
								 
														WriteTrace( TraceInfo, "Extract number id succceded, id attribute value is %s", 
															A( attrIDValue ) );

														int fibonacciNumberID = XMLString::parseInt( attrIDValue );

														if ( fibonacciIndex != fibonacciNumberID ) {
															WriteTrace( TraceError, "Number node invalid, atribute 'id' is %s and should be %s!",
																N ( fibonacciNumberID ),N ( fibonacciIndex ) );

															break;
														}
														else {
															WriteTrace( TraceDebug, "Number node atribute 'id' value is valid!");
														}
													}
													else {
														//detected internal document error
														break;

														WriteTrace( TraceError, "Number node invalid, atribute 'id' not detected!" );
													}
												}
												else {
													//detected internal document error
													break;

													WriteTrace( TraceError, "Number node invalid, atribute 'id' not identified!" );
												}

												/////////////////////////////////////////////////////////
												const char* attrValueTagName = "value";
						 
												if ( elem->hasAttribute( X( attrValueTagName ) ) ) {
													DOMAttr* attrValue = elem->getAttributeNode( X( attrValueTagName ) );

													if (0 != attrValue) {
														const XMLCh* attrValueValue = attrValue->getValue();
								 
														WriteTrace( TraceInfo, "Extract number value succceded, value attribute value is %s", 
															A( attrValueValue ) );

														int fibonacciNumberValue = XMLString::parseInt( attrValueValue );
														int fibonacciIndexValue = this->fibonacci( fibonacciIndex );
																	
														if ( fibonacciNumberValue != fibonacciIndexValue ) {
															WriteTrace( TraceError, "Number node invalid, atribute 'value' is %s and should be %s!",
																N ( fibonacciNumberValue ),N ( fibonacciIndexValue ) );

															break;
														}
														else {
															WriteTrace( TraceDebug, "Number node atribute 'value' value is valid!");
														}
													}
													else {
														//detected internal document error
														break;

														WriteTrace( TraceError, "Number node invalid, atribute 'value' not detected!" );
													}
												}
												else {
													//detected internal document error
													break;

													WriteTrace( TraceError, "Number node invalid, atribute 'value' not identified!" );
												}
												/////////////////////////////////////////////////////////
											}
												else {
													//detected internal document error
													break;

													WriteTrace( TraceError, "Number node invalid, no attributes detected!" );
												}
											}
											///////////////////////////////////////////////////////////////////////////
										
											fibonacciIndex++;
										}
									}
								}
							}
					
							retVal = ( fibonacciIndex == ( getFibonacciID() + 1 ) );
						}
						else {
							WriteTrace( TraceError, "Document root node children list invalid, internal error retrieving child nodes list!" );
						}
					}
					else {
						//detected internal document error

						WriteTrace( TraceError, "Document root node children list invalid, no childrens detected!" );
					}
				}
				else {
					//detected internal document error

					WriteTrace( TraceError, "Document root node invalid, attributes detected error!" );
				}
			}
			else {
				//detected internal document error

				WriteTrace( TraceError, "Document root node invalid, not identified!" );
			}
		}
		catch (const OutOfMemoryException& toCatch) {
			WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const XMLException& toCatch) {
			WriteTrace( TraceError, "XMLException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const DOMException& toCatch) {
			WriteTrace( TraceError, "DOMException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (...) {
			WriteTrace( TraceError, "Unexpected Exception" );
		}
	}
	else {
		//detected internal document error
		retVal = false;

		WriteTrace( TraceError, "Document content invalid!" );
	}

	return retVal;
}

} //namespace tools {
} //namespace test

