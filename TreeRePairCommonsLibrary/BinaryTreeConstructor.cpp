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
#include "BinaryTreeConstructor.h"

#include "Helper.h"
#include "TreeRePairMode.h"
#include "Production.h"
#include "TreeDeleter.h"
#include "TreeRePairConstants.h"
#include "SymbolInfoContainer.h"

#include <iostream>

using namespace std;

BinaryTreeConstructor::BinaryTreeConstructor(Grammar & dagGrammarPar) :
	TreeConstructor(dagGrammarPar)
{
	// the start symbol is supposed to be the first symbol instantiated
	Symbol::getStartSymbol();
}

BinaryTreeConstructor::~BinaryTreeConstructor()
{
}

Symbol * BinaryTreeConstructor::retrieveSymbol(string name, bool hasLeftChild, bool hasRightChild) {

	SymbolInfoContainer * pSic = new SymbolInfoContainer(name);
	pSic->setHasLeftChild(hasLeftChild);
	pSic->setHasRightChild(hasRightChild);	

	Symbol * pSymbol;
	HASH_S_S::iterator result = Symbol::hashMaps.symbolsBySicHashMap.find(pSic);
	if (result == Symbol::hashMaps.symbolsBySicHashMap.end()) {
		pSymbol = new Symbol(pSic->hasLeftChild(), pSic->hasRightChild());
		 Symbol::setTerminalSymbol(pSymbol->getId());
		pSymbol->addSymbolToHashMaps(name);

		unsigned int childrenCount = 0;
		if (hasLeftChild) {
			childrenCount++;
		}
		if (hasRightChild) {
			childrenCount++;
		}
		pSymbol->setRank(childrenCount);
	}
	else {
		pSymbol = result->second;
	}
	delete pSic;

	return pSymbol;
}

void BinaryTreeConstructor::startElement(string & name)
{

	// cout << "start: " << name << endl;

	nodeCount++;

	TreeNode * pNewNode = 0;
	if (!hierarchyStack.empty())
	{
		unsigned int * pIndex = indexStack.top();
		(*pIndex)++;

		// Symbol * pSymbol = retrieveSymbol(name, true, true);
		// pNewNode = new TreeNode(pSymbol);
		pNewNode = new TreeNode(0, 2);
		TreeNode * pParent = hierarchyStack.top();

		if (*pIndex == 1) {
			pParent->setChild(0, pNewNode);
		} else {
			pParent->setChild(1, pNewNode);
		}
	}
	else {
		Symbol * pSymbol = retrieveSymbol(name, true, false);
		pNewNode = new TreeNode(pSymbol);
		pRoot = pNewNode;
		Production * pProduction = new Production(Symbol::getStartSymbol(), pRoot);
		pProduction->incrementRefCount();
		pProduction->incrementTransitiveRefCount();
		dagGrammar.addProduction(pProduction);
	}

	indexStack.push(new unsigned int(0));
	hierarchyStack.push(pNewNode);
	nameStack.push(name);
}

void BinaryTreeConstructor::endElement(string & name)
{
	// cout << "end: " << name << endl;

	unsigned int * pIndex = indexStack.top();
	for (unsigned int i = 0; i < *pIndex; i++)
	{
		TreeNode * pShareNode = hierarchyStack.top();
		string & symbolName = nameStack.top();

		// cout << "share node: " << Symbol::getNameById(pShareNode->getSymbol()->getId()) << endl;

		bool hasLeftChild = (pShareNode->getChild(0) != 0);
		bool hasRightChild = (pShareNode->getChild(1) != 0);

		Symbol * pNewSymbol = retrieveSymbol(symbolName, hasLeftChild, hasRightChild);
		pShareNode->setSymbol(pNewSymbol);

		hierarchyStack.pop();
		nameStack.pop();

		if (TreeRePairMode::getInstance()->isUseDag()) {
				shareTree(pShareNode);
		}
	}
	delete pIndex;
	indexStack.pop();
}

void BinaryTreeConstructor::endDocument()
{
}
