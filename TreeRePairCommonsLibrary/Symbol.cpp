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
#include "Symbol.h"

#include "Helper.h"
#include "TreeRePairConstants.h"
#include "SymbolInfoContainer.h"

SymbolsHashMaps Symbol::hashMaps;
unsigned int Symbol::counter = 0;
Symbol * Symbol::pParameterSymbol = 0;
Symbol * Symbol::pPlaceholderSymbol = 0;
Symbol * Symbol::pStartSymbol = 0;

Symbol * Symbol::getStartSymbol() {

	if (pStartSymbol == 0) {
		pStartSymbol = new Symbol();
		pStartSymbol->setRank(0);
		pStartSymbol->addSymbolToHashMaps(START_TAG);
	}

	return pStartSymbol;
}

Symbol * Symbol::getParameterSymbol() {

	if (pParameterSymbol == 0) {
		pParameterSymbol = new Symbol();
		pParameterSymbol->setRank(0);
	}

	return pParameterSymbol;
}

Symbol * Symbol::getPlaceholderSymbol() {

	if (pPlaceholderSymbol == 0) {
		pPlaceholderSymbol = new Symbol();
		pPlaceholderSymbol->setRank(0);
	}

	return pPlaceholderSymbol;
}

string Symbol::getElementTypeById(unsigned int id)
{
	HASH_I_S::iterator result = hashMaps.idElementTypeHashMap.find(id);
	if (result != hashMaps.idElementTypeHashMap.end()) {
		return result->second;
	} else {
		Symbol * pSymbol = hashMaps.symbolByIdHashMap[id];
		return "id:" + Helper::stringify((size_t) pSymbol->getName());
	}
}
;

void Symbol::addSymbolToHashMaps(string elementType)
{

	// cout << "element type: " << elementType << endl;

	SymbolInfoContainer * pSic = new SymbolInfoContainer(elementType);
	pSic->setHasLeftChild(leftChild);
	pSic->setHasRightChild(rightChild);
	hashMaps.symbolsBySicHashMap[pSic] = this;

	hashMaps.idElementTypeHashMap[getId()] = elementType;
}

// default arity is 2; the arity information for symbols with arity 2 isn't
// written to the compressed file
Symbol::Symbol(bool leftChildPar, bool rightChildPar) :
	leftChild(leftChildPar), rightChild(rightChildPar), id(counter), name(counter++), rank(
			DEFAULT_ARITY)
{
	hashMaps.symbolByIdHashMap[id] = this;
}

#ifdef DEBUG
void Symbol::print(ostream& os, const Symbol * pSymbol)
{
	if (os != cout) {
		os << Symbol::getElementTypeById(pSymbol->id);
	} else {
		os << Symbol::getElementTypeById(pSymbol->id) << ":"
				<< pSymbol->getRank() << " [id:"
				<< Helper::stringify((size_t)pSymbol->getName()) << "]";
	}
}
#endif
