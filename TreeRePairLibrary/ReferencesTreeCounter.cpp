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
#include "ReferencesTreeCounter.h"

#include "TreeRePairTypes.h"
#include "HuffmanSuccinctWriter.h"
#include "Production.h"

ReferencesTreeCounter::ReferencesTreeCounter(TreeNode * pRootPar,
		Grammar * dagGrammarPar, Grammar & mainGrammarPar) :
	TreeTraversal(pRootPar), dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar)
{
	traverseTree();
}

ReferencesTreeCounter::~ReferencesTreeCounter(void)
{
}

void ReferencesTreeCounter::processNode(TreeNode * pParent, unsigned int index,
		int depth, bool closing)
{

	TreeNode * pNode;
	if (index != UINT_MAX)
	{
		pNode = pParent->getChild(index);
	}
	else
	{
		pNode = pParent;
	}

	if (closing)
	{
		return;
	}

	if (dagGrammar != 0 && dagGrammar->isNonTerminal(pNode->getSymbol())) {
		Production * pProduction = dagGrammar->getProductionBySymbol(pNode->getSymbol());
		ReferencesTreeCounter treeCounter(pProduction->getRightHandSide(), dagGrammar, mainGrammar);
	} else {
		countReference(pNode);
	}
}

void ReferencesTreeCounter::countReference(TreeNode * pNode) {

	Symbol * pSymbol = pNode->getSymbol();
	if (mainGrammar.isNonTerminal(pSymbol)) {
		Production * pProduction = mainGrammar.getProductionBySymbol(pSymbol);
		pProduction->incrementRefCount();
		pProduction->addRefNode(pNode);
	}

}
