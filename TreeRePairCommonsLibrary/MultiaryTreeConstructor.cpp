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
#include "MultiaryTreeConstructor.h"

#include "TreeRePairMode.h"
#include "Helper.h"
#include "TreeRePairConstants.h"
#include "SymbolInfoContainer.h"

#include <iostream>

using namespace std;

MultiaryTreeConstructor::MultiaryTreeConstructor(
		Grammar & grammarPar) :
		TreeConstructor(grammarPar)
{
	binary = false;
}

MultiaryTreeConstructor::~MultiaryTreeConstructor()
{
}

void MultiaryTreeConstructor::startElement(string & name)
{
	nodeCount++;

	vector<TreeNode*> * childrenVector = new vector<TreeNode*> ();
	childrenStack.push(childrenVector);
}

void MultiaryTreeConstructor::endElement(string & name)
{
	vector<TreeNode*> * childrenVector = childrenStack.top();

	unsigned int size = childrenVector->size();
	SymbolInfoContainer * pSic = new SymbolInfoContainer(name, size);
	Symbol * pSymbol = Symbol::hashMaps.symbolsBySicHashMap[pSic];
	delete pSic;
	if (pSymbol == 0)
	{
		cout << "Error: Can't find symbol in hash map. Exiting ..." << endl;
		exit(1);
	}

	TreeNode * pNewNode = new TreeNode(pSymbol);

	for (unsigned int i = 0; i < childrenVector->size(); i++)
	{
		TreeNode * pChild = childrenVector->at(i);
		pNewNode->setChild(i, pChild);
		shareTree(pChild);
	}

	childrenStack.pop();
	delete childrenVector;
	childrenVector = 0;

	if (!childrenStack.empty())
	{
		childrenVector = childrenStack.top(); // parents vector
		childrenVector->push_back(pNewNode);
	}
	else
	{
		Production * pProduction = new Production(Symbol::getStartSymbol(), pNewNode);
		pProduction->setRefCount(1);
		pProduction->setTransitiveRefCount(1);
		dagGrammar.addProduction(pProduction);
	}
}



void MultiaryTreeConstructor::endDocument()
{
	// do nothing
}
