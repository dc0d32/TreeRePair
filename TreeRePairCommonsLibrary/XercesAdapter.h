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
#pragma once

#include "XmlContentHandler.h"

#include <xercesc/sax2/DefaultHandler.hpp>

using xercesc::DefaultHandler;
using xercesc::SAXParseException;
using xercesc::Attributes;

/**
 * Adapter used to connect the xercesc SAX parser with an instance
 * of a super class of the XmlContentHandler class.
 */
class XercesAdapter : public DefaultHandler
{
public:
	/**
	 * The constructor.
	 * @param pXmlContentHandler the xml content handler to which the
	 *  events of the SAX parser shall be transported
	 */
	XercesAdapter(XmlContentHandler * pXmlContentHandler);
	virtual ~XercesAdapter();

	/**
	 * Method which is called by the xercesc SAX parser to indicate a
	 * start-tag.
	 */
	void startElement(const XMLCh* const uri, const XMLCh* const localname,
			const XMLCh* const qname, const Attributes& attrs);

	/**
	 * Method which is called by the xercesc SAX parser to indicate a
	 * end-tag.
	 */
	void endElement(const XMLCh * const uri, const XMLCh * const localname,
			const XMLCh * const qname);

	/**
	 * Method which is called by the xercesc SAX parser to indicate that
	 * the end of the XML document is reached.
	 */
	void endDocument();

	/**
	 * Method which is called by the xercesc SAX parser to indicate that
	 * an error occurred.
	 */
	void fatalError(const SAXParseException&);

private:
	/**
	 * The xml content handler to which the
	 * events of the SAX parser shall be transported.
	 */
	XmlContentHandler * pXmlContentHandler;
};
