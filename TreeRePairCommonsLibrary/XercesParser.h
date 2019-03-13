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

#include "TreeRePairMacros.h"
#include "XmlContentHandler.h"

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

using xercesc::SAX2XMLReader;

/**
 * This class implements a SAX parser using the xercesc library.
 */
class XercesParser
{
public:
	/**
	 * The constructor.
	 * @param pFilePath the file to parse
	 * @param pXmlContentHandler the handler which shall receive the
	 *  SAX events
	 */
	XercesParser(char * pFilePath,
			XmlContentHandler * pXmlContentHandler);
	~XercesParser(void);
};

