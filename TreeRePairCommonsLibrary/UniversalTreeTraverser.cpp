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
#include "UniversalTreeTraverser.h"

#include "TreeRePairTypes.h"
#include "Production.h"

UniversalTreeTraverser::UniversalTreeTraverser(
		Grammar * pGrammarPar, TreeNode * pRootPar) :
	TreeTraversal(pRootPar), pGrammar(pGrammarPar) {

	traverseTree();
}

UniversalTreeTraverser::~UniversalTreeTraverser() {
}

void UniversalTreeTraverser::processNode(TreeNode * pParent, unsigned int index,
		int depth, bool closing) {

	TreeNode * pNode;
	if (index != UINT_MAX) {
		pNode = pParent->getChild(index);
	} else {
		pNode = pParent;
	}

	if (closing) {
		return;
	}

	if (pGrammar != 0 && pGrammar->isNonTerminal(pNode->getSymbol())) {
		Production * pProduction = pGrammar->getProductionBySymbol(pNode->getSymbol());
		UniversalTreeTraverser treeTraverser(pGrammar, pProduction->getRightHandSide());
	} else {
		// TODO use pointer to method to adapt behavior of class
#ifdef DEBUG
		pNode->setVisited(false);
#endif
	}

	return;
}
