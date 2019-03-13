/*
 * TreeRePair is an implementation of the Re-pair for Trees algorithm.
 *
 * Copyright (C) 2010  Roy Mennicke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License Version 3 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not,
 * see <http://www.gnu.org/licenses/gpl-3.0.txt>.
 */
#include "XercesParser.h"

#include "XercesAdapter.h"

#include <iostream>

using namespace std;

using xercesc::XMLPlatformUtils;
using xercesc::XMLException;
using xercesc::XMLReaderFactory;
using xercesc::XMLUni;
using xercesc::XMLString;


XercesParser::XercesParser(char * pFilePath,
		XmlContentHandler * pXmlContentHandler)

{
	try
	{
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& toCatch)
	{
		char* pMessage = XMLString::transcode(toCatch.getMessage());
		cout << "Error during initialization! :\n";
		cout << "Exception message is: \n" << pMessage << "\n";
		XMLString::release(&pMessage);
		return;
	}

	char * currentChar = pFilePath;
	while (*currentChar != '\0')
	{
		if (*currentChar == '\\')
		{
			*currentChar = '/';
		}
		currentChar++;
	}

	// http://xerces.apache.org/xerces-c/program-sax2.html#UsingSAX2API
	SAX2XMLReader * pParser = XMLReaderFactory::createXMLReader();
	pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, false); // optional

	XercesAdapter * pXercesAdapter = 0;
	try
	{
		pXercesAdapter = new XercesAdapter(pXmlContentHandler);

		pParser->setContentHandler(pXercesAdapter);
		pParser->setErrorHandler(pXercesAdapter);

		pParser->parse(pFilePath);

	} catch (const XMLException& toCatch)
	{
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n" << message << "\n";
		XMLString::release(&message);
	} catch (const SAXParseException& toCatch)
	{
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n" << message << "\n";
		XMLString::release(&message);
	} catch (...)
	{
		cout << "Unexpected Exception \n";
	}

	delete pXercesAdapter;
	delete pParser;
}

XercesParser::~XercesParser()
{
}

