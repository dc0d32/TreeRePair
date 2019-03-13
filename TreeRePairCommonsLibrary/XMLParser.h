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

/**
 * This class implements a light-weight SAX-like XML document parser which
 * only takes care of start-tags, end-tags and empty element tags.
 */
class XMLParser
{
public:
	/**
	 * The constructor of the class.
	 * @param pFilePath the XML document to parse
	 * @param xmlContentHandler the handler of the events emitted by the
	 *  parser
	 */
	XMLParser(char * pFilePath, XmlContentHandler & xmlContentHandler);
	~XMLParser(void);

private:
	/**
	 * The handler of the events emitted by the parser.
	 */
	XmlContentHandler & xmlContentHandler;

	/**
	 * The input stream used by the parser.
	 */
	ifstream * pIfs;
};
