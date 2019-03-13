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
#include "TreeConstructor.h"

#include "Production.h"
#include "TreeNode.h"
#include "ProductionTreeContainer.h"

TreeConstructor::TreeConstructor(Grammar & dagGrammarPar) :
	XmlContentHandler(), dagGrammar(dagGrammarPar), nodeCount(0), binary(true)
{
}

TreeConstructor::~TreeConstructor()
{
	HASH_T_T::iterator it;
	for (it = treesHashMap.begin(); it != treesHashMap.end(); ++it)
	{
		delete it->second;
		it->second = 0;
	}
	treesHashMap.clear();
}

bool TreeConstructor::isShareable(TreeNode * pNode) {

	// check necessary in multiary mode
	if (pNode->getParent() == 0) {
		return false;
	}

	for (unsigned int i = 0; i < pNode->getCurrentChildrenCount(); i++) {

//		if (pNode->getChild(i) == 0) {
//			return false;
//		}

		Symbol * pSymbol = pNode->getChild(i)->getSymbol();

		if (pSymbol == 0) {
			return false;
		}

		if (!dagGrammar.isNonTerminal(pSymbol)) {
			return false;
		}
	}

	return true;
}

void TreeConstructor::shareTree(TreeNode * pNode)
{
	if (isShareable(pNode)) {

		ProductionTreeContainer * pContainer = treesHashMap[pNode];
		if (pContainer == 0)
		{
			treesHashMap[pNode] = new ProductionTreeContainer(pNode);
		}
		else
		{
			Production * pProduction = pContainer->getProduction();
			Symbol * pSymbol = 0;
			if (pProduction == 0)
			{
				pSymbol = new Symbol();
				pSymbol->setRank(0);
				TreeNode * pParentNodeShared = pContainer->getFirstOccurrenceOfTree()->getParent();
				unsigned int childIndex = pContainer->getFirstOccurrenceOfTree()->getIndex();

				pProduction = new Production(pSymbol, pContainer->getFirstOccurrenceOfTree());
				pProduction->incrementRefCount();
				pProduction->incrementTransitiveRefCount();
				dagGrammar.addProduction(pProduction);
				pContainer->setProduction(pProduction);

				TreeNode * pReferenceNode = new TreeNode(pSymbol);
				pParentNodeShared->setChild(childIndex, pReferenceNode);
				pProduction->addRefNode(pReferenceNode);

				pProduction->getRightHandSide()->setParent(0);
				pContainer->setFirstOccurrencePointerToNull();

				// ...
				if (isShareable(pParentNodeShared)) {
					treesHashMap[pParentNodeShared] = new ProductionTreeContainer(pParentNodeShared);
				}
			}
			else
			{
				pSymbol = pProduction->getLeftHandSide();
			}

			TreeNode * pParent = pNode->getParent();
			TreeNode * pRefNode = new TreeNode(pSymbol);
			pParent->setChild(pNode->getIndex(), pRefNode);

			pProduction->addRefNode(pRefNode);
			pProduction->incrementRefCount();
			pProduction->incrementTransitiveRefCount();
			pNode->setParent(0);

			for (unsigned int i = 0; i < pNode->getCurrentChildrenCount(); i++) {
				if (dagGrammar.isNonTerminal(pNode->getChild(i)->getSymbol())) {
					Production * pTempProduction =
							dagGrammar.getProductionBySymbol(pNode->getChild(i)->getSymbol());
					pTempProduction->decrementRefCount();
					pTempProduction->removeRefNode(pNode->getChild(i));
				}
			}

			treeDeleter.deleteTree(pNode);
		}
	}
}
