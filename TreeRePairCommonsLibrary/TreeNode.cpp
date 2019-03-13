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
#include "TreeNode.h"

#include "TreeRePairMode.h"
#include "Production.h"
#include "Digram.h"
#include "Helper.h"

unsigned int TreeNode::idCounter = 1;

TreeNode * TreeNode::getChild(Grammar & grammar, TreeNode * pNode, unsigned int index) {

	TreeNode * pChild = pNode->getChild(index);
	if (pChild != 0 && grammar.isNonTerminal(pChild->getSymbol())) {
		Production * pProduction = grammar.getProductionBySymbol(pChild->getSymbol());
		pChild = pProduction->getRightHandSide();
	}
	return pChild;
}

TreeNode::TreeNode(Symbol* pSymbolPar, unsigned int rankPar) :
	id(idCounter++), pSymbol(pSymbolPar), rank(rankPar), currentChildrenCount(0),
	pParent(0), index(UINT_MAX), transitiveRefCount(0), offset(0)
{
#ifdef DEBUG
	visited = false;
#endif

	if (pSymbol != 0) {
		rank = pSymbol->getRank();
	}

	if (rank > 0) {
		pChildren = new TreeNode*[rank];
	} else {
		pChildren = 0;
	}

	if (rank > 0 && TreeRePairMode::getInstance()->getAlgMode()
			== TreeRePairMode::COMPRESS) {
		pNextDigramOccurrences = new TreeNode*[rank];
		pPreviousDigramOccurrences = new TreeNode*[rank];
	} else {
		pNextDigramOccurrences = 0;
		pPreviousDigramOccurrences = 0;
	}

	for (unsigned int i = 0; i < rank; i++) {
		pChildren[i] = 0;

		if (TreeRePairMode::getInstance()->getAlgMode()
				== TreeRePairMode::COMPRESS) {
			pNextDigramOccurrences[i] = 0;
			pPreviousDigramOccurrences[i] = 0;
		}
	}
}

TreeNode::~TreeNode(void)
{
	if (this->rank > 0)
	{
		delete[] pChildren;
		delete[] pNextDigramOccurrences;
		delete[] pPreviousDigramOccurrences;
//		delete[] pIsOccurrence;
	}
}

bool TreeNode::isRegisteredOccurrence(HASH_D_D * pDigramHashMap, Grammar & dagGrammar, int index) {

	if (getNextDigramOccurrence(index) != 0 || getPreviousDigramOccurrence(index) != 0) {
		return true;
	} else {
		// if the current node is the only occurrence of the
		// corresponding digram the getNextDigramOccurrence- and getPreviousDigramOccurrence-
		// pointers are null pointers
		Digram * pDigram = Helper::getDigram(pDigramHashMap, dagGrammar, this, index);
		if (pDigram != 0 && pDigram->getFirstOccurrence() == this) {
			return true;
		} else {
			return false;
		}
	}
}

void TreeNode::setSymbol(Symbol * pSymbolPar) {
	pSymbol = pSymbolPar;
	if (!pSymbol->hasLeftChild() && !pSymbol->hasRightChild()) {
		currentChildrenCount = 0;
		rank = 0;
		delete[] pChildren;
		delete[] pNextDigramOccurrences;
		delete[] pPreviousDigramOccurrences;
//		delete[] pIsOccurrence;
	} else if (!pSymbol->hasLeftChild()) {
		for (unsigned int i = 1; i < currentChildrenCount; i++) {
			pChildren[i]->setIndex(i - 1);
		}
		offset = 1;
	}
}

unsigned int TreeNode::getTransitiveRefCount() {

	return transitiveRefCount;
}
;

TreeNode * TreeNode::getChild(unsigned int i) const
{
#ifdef DEBUG
	if (i >= 0 && i < currentChildrenCount - offset)
	{
#endif
		return pChildren[i + offset];
#ifdef DEBUG
	}
	else
	{
		cout << "Fatal error: Can't get child for child index " << i
				<< " if the current children count is " << currentChildrenCount
				<< endl;
		exit(1);
	}
#endif
}

void TreeNode::setChild(unsigned int index, TreeNode * pChild)
{
#ifdef DEBUG
	if (index >= 0 && index < this->rank - offset)
	{
#endif

		if (currentChildrenCount - offset <= index)
		{
			currentChildrenCount = index + 1 + offset;
		}

		pChildren[index + offset] = pChild;
		if (pChild != 0)
		{
			pChild->setIndex(index);
			pChild->setParent(this);
		}

#ifdef DEBUG
	}
	else
	{
		cout << "Fatal error: Can't set child with index " << index << " labeled by "
				<< Symbol::getElementTypeById(pChild->getSymbol()->getId())
				<< " for node labeled by "
				<< Symbol::getElementTypeById(getSymbol()->getId())
				<< ". The maximum children count is " << rank << "."
				<< endl;
		exit(1);
	}
#endif
}

void TreeNode::addChild(TreeNode * pNode)
{
	setChild(currentChildrenCount, pNode);
}

void TreeNode::setNextDigramOccurrence(unsigned int childIndex, TreeNode* pNextDigramOccurrence)
{

#ifdef DEBUG
	if (childIndex >= 0 && childIndex < this->rank - offset && pNextDigramOccurrence != this)
	{
#endif
		pNextDigramOccurrences[childIndex + offset] = pNextDigramOccurrence;
#ifdef DEBUG
	}
	else
	{
		cout << "Fatal error: Can't set next digram occurrence." << endl;
		exit(1);
	}
#endif
}

void TreeNode::setPreviousDigramOccurrence(unsigned int childIndex, TreeNode* pPreviousDigramOccurrence)
{

#ifdef DEBUG
	if (childIndex >= 0 && childIndex < this->rank - offset
			&& pPreviousDigramOccurrence != this)
	{
#endif
		pPreviousDigramOccurrences[childIndex + offset] = pPreviousDigramOccurrence;
#ifdef DEBUG
	}
	else
	{
		cout << "Fatal error: Can't set previous digram occurrence." << endl;
		exit(1);
	}
#endif
}

TreeNode * TreeNode::getNextDigramOccurrence(unsigned int childIndex)
{

#ifdef DEBUG
	if (childIndex >= 0 && childIndex < this->rank - offset)
	{
#endif
		return pNextDigramOccurrences[childIndex + offset];
#ifdef DEBUG
	}
	else
	{
		cout << "Fatal error: Can't get next digram occurrence for child index "
				<< childIndex << " if the maximum children count is "
				<< rank - offset << endl;
		exit(1);
	}
#endif
}

TreeNode * TreeNode::getPreviousDigramOccurrence(unsigned int childIndex)
{

#ifdef DEBUG
	if (childIndex >= 0 && childIndex < this->rank - offset)
	{
#endif
		return pPreviousDigramOccurrences[childIndex + offset];
#ifdef DEBUG
	}
	else
	{
		cout << "Fatal error: Can't get previous digram occurrence for child index "
				<< childIndex << " if the maximum children count is "
				<< rank - offset << endl;
		exit(1);
	}
#endif
}

#ifdef DEBUG
void TreeNode::print(ostream& os, const TreeNode* pNode, bool recursive)
{
	os << "-Node \"";
	Symbol::print(os, pNode->pSymbol);
	os << "\"";

	if (recursive)
	{

		os << " (";

		for (unsigned int i = pNode->offset; i < pNode->currentChildrenCount; i++)
		{
			TreeNode::print(os, pNode->getChild(i));
			if (i < pNode->currentChildrenCount - 1)
			{
				os << ", ";
			}
		}

		os << ")";
	}
}
#endif
