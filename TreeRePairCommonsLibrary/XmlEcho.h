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
#include "XmlContentHandler.h"

#include <string>

/**
 * This class is used for testing purposes only. It echos the
 * events emitted by the SAX parser.
 */
class XmlEcho : public XmlContentHandler
{
public:
	XmlEcho();
	virtual ~XmlEcho();

	void startElement(string & name);
	void endElement(string & name);
	void endDocument();
};
