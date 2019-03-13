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
#include "Digram.h"

#include "TreeRePairMode.h"

#include <sstream>
#include <new>

Digram::Digram(unsigned int childIndexPar, Symbol * pParentSymbolPar,
		Symbol * pChildSymbolPar, TreeNode * pFirstOccurrencePar,
		TreeNode * pLastOccurrencePar) :
	pParentSymbol(pParentSymbolPar), pChildSymbol(pChildSymbolPar), childIndex(
			childIndexPar), numberOfOccurrences(0), pFirstOccurrence(pFirstOccurrencePar),
			pLastOccurrence(pLastOccurrencePar), pPrecedingDigram(0),
			pFollowingDigram(0)
{
	// we use a digram with pParentSymbol == 0 && pChildSymbol == 0
	// for the deleted key of the digram hash map
	if (pParentSymbol != 0 && pChildSymbol != 0) {
		rankOfTreePattern = calcNewRank(pParentSymbol, pChildSymbol);
	}
}

Digram::~Digram(void)
{
}

unsigned int Digram::calcNewRank(Symbol * pParentSymbol, Symbol * pChildSymbol) {

	return pParentSymbol->getRank() + pChildSymbol->getRank() - 1;

}

void Digram::incrementNumberOfOccs(unsigned int addQuantity)
{
	numberOfOccurrences += addQuantity;
}

void Digram::decrementNumberOfOccs(unsigned int subQuantity)
{
#ifdef DEBUG
	if (subQuantity > numberOfOccurrences) {
		cout << "Fatal error: Cannot decrement quantity of digram ";
		Digram::print(cout, this);
		cout << " (quantity to subtract"
				<< " is " << subQuantity << "; current quantity is " << numberOfOccurrences
				<< "). Exiting ..." << endl;
		exit(1);
	}
#endif

	numberOfOccurrences -= subQuantity;
}

void Digram::setFirstOccurrence(TreeNode * pFirstOccurrence) {
	this->pFirstOccurrence = pFirstOccurrence;
}

void Digram::print(ostream& os, const Digram * pDigram)
{
	os << "(";
	os << Symbol::getElementTypeById(pDigram->pParentSymbol->getId());
	os << ":" << pDigram->pParentSymbol->getRank();
	os << ", " << pDigram->childIndex;
	os << ", " << Symbol::getElementTypeById(pDigram->pChildSymbol->getId());
	os << ":" << pDigram->pChildSymbol->getRank();
	os << ")";
}
