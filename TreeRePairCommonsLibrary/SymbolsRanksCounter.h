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
#include "TreeRePairTypes.h"

#include <vector>
#include <stack>

/**
 * This class parses the input XML document and creates for every
 * unique combination of an element type and its number of children
 * an instance of the Symbol class. It is used together with the
 * MultiaryTreeConstructor class to generate a multiary ranked
 * version of the input XML document tree.
 */
class SymbolsRanksCounter : public XmlContentHandler {
public:
	SymbolsRanksCounter() {};
	~SymbolsRanksCounter(void) {};

    void startElement(string & name);
	void endElement(string & name);
	void endDocument();

private:
	/**
	 * Keeps track of the number of children of the current node
	 * and all its (recursive) parent nodes.
	 */
	stack <unsigned int*> childCountsStack;
};
