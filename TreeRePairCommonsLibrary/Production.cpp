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
#include "Production.h"

#include "Symbol.h"
#include "TreeNode.h"
#include "XmlTreePrinter.h"
#include "TreeDeleter.h"
#include "Digram.h"
#include "TreeCopier.h"

#include <algorithm>

Production * Production::createProduction(Symbol * pNewSymbol, Digram * pDigramReplaced) {

	Symbol * pParentSymbol = pDigramReplaced->getParentSymbol();
	Symbol * pChildSymbol = pDigramReplaced->getChildSymbol();

	unsigned int childIndex = pDigramReplaced->getChildIndex();

	TreeNode * pParentNode = new TreeNode(pParentSymbol);
	TreeNode * pChildNode = new TreeNode(pChildSymbol);

	Production * pProduction = new Production(pNewSymbol, pParentNode);
	unsigned int parameterCounter = 0;

	for (unsigned int i = 0; i < childIndex; i++) {
		TreeNode * pParameterNode = new TreeNode(Symbol::getParameterSymbol());
		pParentNode->setChild(i, pParameterNode);
		pProduction->setParameter(parameterCounter++, pParameterNode);
	}

	pParentNode->setChild(childIndex, pChildNode);

	for (unsigned int i = 0; i < pChildSymbol->getRank(); i++) {
		TreeNode * pParameterNode = new TreeNode(Symbol::getParameterSymbol());
		pChildNode->setChild(i, pParameterNode);
		pProduction->setParameter(parameterCounter++, pParameterNode);
	}

	for (unsigned int i = childIndex + 1; i < pParentSymbol->getRank(); i++) {
		TreeNode * pParameterNode = new TreeNode(Symbol::getParameterSymbol());
		pParentNode->setChild(i, pParameterNode);
		pProduction->setParameter(parameterCounter++, pParameterNode);
	}

	pProduction->setTransitiveRefCount(pDigramReplaced->getNumberOfOccs());

	return pProduction;

}

void Production::deleteProduction(Production * pProduction) {

//#ifdef DEBUG
//	TreeNode * pNode = pProduction->getRightHandSide();
//	for (unsigned int i = 0; i < pNode->getCurrentChildrenCount(); i++) {
//		if (pNode->getOccurrence(i)) {
//			cout << "Fatal error when deleting production. Exiting ..." << TreeNode::idCounter << endl;
//			exit(1);
//		}
//	}
//#endif

	TreeDeleter treeDeleter;
	treeDeleter.deleteTree(pProduction->getRightHandSide());

	delete pProduction;
}

Production::Production(Symbol * pSymbolPar, TreeNode * pNodePar) :
	pLeftHandSide(pSymbolPar), pRightHandSide(pNodePar), refCount(0),
			transitiveRefCount(0) {

	if (pRightHandSide != 0) {
		pRightHandSide->setParent(0);
	}

	if (pLeftHandSide->getRank() > 0) {
		pParameters = new TreeNode*[pLeftHandSide->getRank()];
	} else {
		pParameters = 0;
	}
}

Production::~Production(void) {
	if (pLeftHandSide->getRank() > 0) {
		delete [] pParameters;
	}
}

Production * Production::copy() {
	TreeCopier copier(0, getRightHandSide());
	TreeNode * pNewRightHandSide = copier.getNewRoot();
	Production * pCopy = new Production(getLeftHandSide(), pNewRightHandSide);

	TreeNode * pCurrentNode = pNewRightHandSide;
	unsigned int curIndex = 0;

	for (unsigned int i = 0; i < getLeftHandSide()->getRank(); i++) {

		while (curIndex >= pCurrentNode->getSymbol()->getRank()
				|| pCurrentNode->getChild(curIndex)->getSymbol()
					!= Symbol::getParameterSymbol()) {

			if (curIndex < pCurrentNode->getSymbol()->getRank()) {
				pCurrentNode = pCurrentNode->getChild(curIndex);
				curIndex = 0;
			} else {
				curIndex = pCurrentNode->getIndex() + 1;
				pCurrentNode = pCurrentNode->getParent();
			}
		}

		TreeNode * pParameterNode = pCurrentNode->getChild(curIndex);
		pCopy->setParameter(i, pParameterNode);
		curIndex++;
	}

	return pCopy;
}

void Production::decrementRefCount(unsigned int quantity) {

#ifdef DEBUG
	if (refCount < quantity) {
		cout << "Fatal error: Variable refCount is < 0. Exiting ..." << endl;
		exit(1);
	}
#endif

	refCount -= quantity;
}
;

void Production::decrementTransitiveRefCount(unsigned int quantity) {

#ifdef DEBUG
	if (transitiveRefCount < quantity) {
		cout << "Fatal error: Variable transitiveRefCount is < 0. Exiting ..."
				<< endl;
		exit(1);
	}
#endif

	transitiveRefCount -= quantity;
}
;

void Production::setRightHandSide(TreeNode * pNodePar) {
	pRightHandSide = pNodePar;
	if (pRightHandSide != 0) {
		pRightHandSide->setParent(0);
	}
}

void Production::addRefNode(TreeNode * pNode) {
	referencingNodes.insert(pNode);
}

void Production::removeRefNode(TreeNode * pNode) {
	referencingNodes.erase(pNode);
}

#ifdef DEBUG
void Production::print(ostream& os, const Production * pProduction) {

	os << "production: left-hand side " << Symbol::getElementTypeById(
			pProduction->pLeftHandSide->getId()) << ", right-hand side: " << endl;

	XmlTreePrinter treePrinter;
	treePrinter.printTree(pProduction->pRightHandSide, true, false);
}
#endif
