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
#include "TreeCopier.h"

#include "Production.h"

TreeCopier::TreeCopier(Grammar * grammarPar, TreeNode * pRootPar) :
	TreeTraversal(pRootPar), grammar(grammarPar)
{
	traverseTree();
}

TreeCopier::~TreeCopier(void)
{
}

void TreeCopier::processNode(TreeNode * pParent, unsigned int index,
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
		parents.pop();
		return;
	}

	TreeNode * pNewNode = 0;

	if (grammar != 0 && grammar->isNonTerminal(pNode->getSymbol())) {
		Production * pProduction = grammar->getProductionBySymbol(pNode->getSymbol());
		TreeCopier copier(grammar, pProduction->getRightHandSide());
		pNewNode = copier.getNewRoot();
	} else {
		pNewNode = new TreeNode(pNode->getSymbol());
	}

	if (!parents.empty()) {
		TreeNode * pParent = parents.top();
		pParent->setChild(pNode->getIndex(), pNewNode);
	} else {
		pNewRoot = pNewNode;
	}

	parents.push(pNewNode);
}
