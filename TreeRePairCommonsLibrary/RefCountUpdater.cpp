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
#include "RefCountUpdater.h"

#include "Production.h"

RefCountUpdater::RefCountUpdater(Grammar & mainGrammarPar, TreeNode * pNodePar, int addPar)
: TreeTraversal(pNodePar), mainGrammar(mainGrammarPar), add(addPar)
{
	traverseTree();
}

RefCountUpdater::~RefCountUpdater() {

}

void RefCountUpdater::processNode(TreeNode * pNode, unsigned int index, int depth, bool closing) {

	if (closing) {
		return;
	}

	if (index != UINT_MAX) {
		pNode = pNode->getChild(index);
	}

	if (mainGrammar.isNonTerminal(pNode->getSymbol())) {
		Production * pOtherProduction = mainGrammar.getProductionBySymbol(pNode->getSymbol());
		if (add >= 0) {
			pOtherProduction->setRefCount(pOtherProduction->getRefCount() + add);
			pOtherProduction->removeRefNode(pNode);
		} else {
			pOtherProduction->addRefNode(pNode);
		}
	}
}
