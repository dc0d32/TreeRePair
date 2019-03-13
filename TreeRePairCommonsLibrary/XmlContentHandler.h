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

#include <string>

using namespace std;

/**
 * This method is the base class for every class which processes
 * the events emitted by a SAX parser.
 *
 */
class XmlContentHandler
{
public:
	XmlContentHandler();
	virtual ~XmlContentHandler();

	/**
	 * Method which is called by the SAX-like parser to indicate a
	 * start-tag.
	 * @param name the element type of the start-tag
	 */
	virtual void startElement(string & name) = 0;

	/**
	 * Method which is called by the SAX-like parser to indicate a
	 * end-tag.
	 * @param name the element type of the end-tag
	 */
	virtual void endElement(string & name) = 0;

	/**
	 * This method is called by the SAX-like parser to inform the tree
	 * constructor that the end of the input XML document is reached.
	 */
	virtual void endDocument() = 0;

};
