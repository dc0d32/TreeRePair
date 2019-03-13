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
#include "EdgeCounter.h"

#include "TreeRePairTypes.h"
#include "Production.h"

EdgeCounter::EdgeCounter(Grammar * pDagGrammarPar, bool countParametersPar)
: TreeTraversal(0), pDagGrammar(pDagGrammarPar), numberOfEdges(0), countParameters(countParametersPar)
{
}

EdgeCounter::~EdgeCounter(void)
{
}

void EdgeCounter::countEdges(TreeNode * pRoot) {
	this->pRoot = pRoot;
	traverseTree();
}

void EdgeCounter::countEdges(Grammar & grammar) {

	for (unsigned int i = 0; i < grammar.size(); i++) {
		Production * pProduction = grammar[i];

		// ref count is not set if we skip the pruning step
//#ifdef DEBUG
//		if (pProduction->getRefCount() <= 0) {
//			cout << "Fatal error: References count of production is <= 0. Exiting ..." << endl;
//			exit(1);
//		}
//#endif

		pRoot = pProduction->getRightHandSide();
		traverseTree();
	}
}

void EdgeCounter::processNode(TreeNode * pNode, unsigned int index, int depth, bool closing) {

	if (index == UINT_MAX || closing == true) {
		return;
	}

	if (pNode->getChild(index)->getSymbol() == Symbol::getPlaceholderSymbol()) {
		return;
	}

	// count edges to parameters
	if (!countParameters
			&& pNode->getChild(index)->getSymbol()->getId() == Symbol::getParameterSymbol()->getId()) {
		return;
	}

	numberOfEdges++;

	TreeNode * pChild = pNode->getChild(index);
	if (pDagGrammar != 0 && pDagGrammar->isNonTerminal(pChild->getSymbol())) {
		Production * pProduction = pDagGrammar->getProductionBySymbol(pChild->getSymbol());
		EdgeCounter edgeCounter(pDagGrammar);
		edgeCounter.countEdges(pProduction->getRightHandSide());
		numberOfEdges += edgeCounter.getNumberOfEdges();
	}
}
