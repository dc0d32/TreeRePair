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
#include "SymbolsRanksCounter.h"

#include "TreeRePairMode.h"
#include "Helper.h"
#include "Symbol.h"
#include "TreeRePairConstants.h"
#include "SymbolInfoContainer.h"

#include <iostream>

using namespace std;

void SymbolsRanksCounter::startElement(string & name)
{
	
	unsigned int * childCount = 0;

	if (!childCountsStack.empty())
	{
		childCount = childCountsStack.top();
		(*childCount)++;
	}

	childCount = new unsigned int(0);
	childCountsStack.push(childCount);
}

void SymbolsRanksCounter::endElement(string & name)
{
	unsigned int * childCount = childCountsStack.top();

	Symbol * pSymbol;
	SymbolInfoContainer * pSic = new SymbolInfoContainer(name, *childCount);
	HASH_S_S::iterator result = Symbol::hashMaps.symbolsBySicHashMap.find(pSic);
	if (result == Symbol::hashMaps.symbolsBySicHashMap.end())
	{
		pSymbol = new Symbol(false, false);
		pSymbol->setRank(*childCount);

		// dont use the following because it does not care about childCount
		// pSymbol->addSymbolToHashMaps(name);

		Symbol::hashMaps.symbolsBySicHashMap[pSic] = pSymbol;
		Symbol::hashMaps.idElementTypeHashMap[pSymbol->getId()] = name;
	} else {
		delete pSic;
	}

	childCountsStack.pop();
	delete childCount;
}

void SymbolsRanksCounter::endDocument()
{
}
