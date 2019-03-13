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
#include "XercesAdapter.h"

#include <iostream>

using xercesc::XMLString;

XercesAdapter::XercesAdapter(XmlContentHandler * pXmlContentHandlerPar) :
	pXmlContentHandler(pXmlContentHandlerPar)
{
}

XercesAdapter::~XercesAdapter()
{
}

void XercesAdapter::startElement(const XMLCh* const uri,
		const XMLCh* const localname, const XMLCh* const qname,
		const Attributes& attrs)
{
	char* pNameArray = XMLString::transcode(localname);
	string name(pNameArray);
	pXmlContentHandler->startElement(name);
	XMLString::release(&pNameArray);
}

void XercesAdapter::endElement(const XMLCh * const uri,
		const XMLCh * const localname, const XMLCh * const qname)
{
	char* pNameArray = XMLString::transcode(localname);
	string name(pNameArray);
	pXmlContentHandler->endElement(name);
	XMLString::release(&pNameArray);
}

void XercesAdapter::endDocument()
{
	pXmlContentHandler->endDocument();
}

void XercesAdapter::fatalError(const SAXParseException& exception)
{
	char* pMessage = XMLString::transcode(exception.getMessage());
	cout << "Fatal Error: " << pMessage << " at line: "
			<< exception.getLineNumber() << endl;
	XMLString::release(&pMessage);
	exit(0);
}
