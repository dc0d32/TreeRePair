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
#include "DigramCounter.h"

#include "Helper.h"
#include "Digram.h"
#include "TreeRePairMode.h"
#include "TreeRePairTypes.h"
#include "DigramReplacer.h"
#include "OccurrenceFinder.h"

DigramCounter::DigramCounter(Grammar & dagGrammarPar,
		HASH_D_D * pDigramHashMapPar, DigramPriorityQueue * pPriorityQueuePar) :
	TreeTraversal(0), pDigramHashMap(pDigramHashMapPar), pPriorityQueue(
			pPriorityQueuePar), dagGrammar(dagGrammarPar) {

	for (unsigned int i = 0; i < dagGrammar.size(); i++) {
		pProduction = dagGrammar[i];
		pRoot = pProduction->getRightHandSide();
		traverseTree();
	}
}

DigramCounter::~DigramCounter(void) {
}

void DigramCounter::processNode(TreeNode * pNode, unsigned int index, int depth,
		bool closing) {

	unsigned int transitiveRefCount = pProduction->getTransitiveRefCount();

#ifdef DEBUG
	if (transitiveRefCount == 0) {
		cout << "Fatal error: Transitive reference count is 0. Exiting ..." << endl;
		exit(1);
	}
#endif

	// we traverse the tree in postorder
	if (!closing) {
		return;
	}

	pNode->setTransitiveRefCount(transitiveRefCount);

	if (index == UINT_MAX) {
		return;
	}

	registerOccurrence(pNode, index);

	TreeNode * pChildNode = pNode->getChild(index);
	pChildNode->setTransitiveRefCount(transitiveRefCount);

//	if (dagGrammar.isNonTerminal(pChildNode->getSymbol())) {
//		Production * pProduction = dagGrammar.getProductionBySymbol(pChildNode->getSymbol());
//		if (!pProduction->isCounted()) {
//			DigramCounter counter(dagGrammar, pProduction, pDigramHashMap, pPriorityQueue);
//			pProduction->setCounted(true);
//		}
//	}
}

void DigramCounter::registerOccurrences(TreeNode * pNode) {
	unsigned int childrenCount = pNode->getSymbol()->getRank();

	for (unsigned int i = 0; i < childrenCount; i++) {
		registerOccurrence(pNode, i);
	}
}

void DigramCounter::registerOccurrence(TreeNode * pNode, unsigned int childIndex) {

	if (pNode == 0) {
		return;
	}

#ifdef DEBUG
	if (pNode->isRegisteredOccurrence(pDigramHashMap, dagGrammar, childIndex)) {
		cout << "Fatal error when counting pairs." << endl;
		exit(1);
	}
#endif

	TreeNode * pChild = TreeNode::getChild(dagGrammar, pNode, childIndex);

	if (dagGrammar.isNonTerminal(pNode->getChild(childIndex)->getSymbol())
			&& pNode->getSymbol() == pChild->getSymbol()) {

		return;
	}

	if (pChild == 0) {
		pNode->setNextDigramOccurrence(childIndex, 0);
		pNode->setPreviousDigramOccurrence(childIndex, 0);
		return;
	}

	if (Digram::calcNewRank(pNode->getSymbol(), pChild->getSymbol())
		> TreeRePairMode::getInstance()->getMaxRank()) {

		return;
	}

	unsigned int refCount = pNode->getTransitiveRefCount();

#ifdef DEBUG
	if (refCount == 0) {
		cout << "Fatal error: Reference count is 0. Exiting ..." << endl;
		exit(1);
	}
#endif

	if (!isNonOverlapping(pNode, childIndex)) {
		pNode->setNextDigramOccurrence(childIndex, 0);
		pNode->setPreviousDigramOccurrence(childIndex, 0);
		return;
	}

	Digram * pDigram = new Digram(childIndex, pNode->getSymbol(), pChild->getSymbol(), pNode, pNode);

	HASH_D_D::iterator it = pDigramHashMap->find(pDigram);
	if (it != pDigramHashMap->end()) {
		// found
		delete pDigram;
		pDigram = it->second;

		TreeNode * pLastOccurrence = pDigram->getLastOccurrence();
		if (pLastOccurrence != 0) {
			pLastOccurrence->setNextDigramOccurrence(childIndex, pNode);
			pNode->setPreviousDigramOccurrence(childIndex, pLastOccurrence);
			pNode->setNextDigramOccurrence(childIndex, 0);
		}

		pDigram->setLastOccurrence(pNode);

		if (pDigram->getFirstOccurrence() == 0) {
			pDigram->setFirstOccurrence(pNode);
		}
	} else { // not found
		(*pDigramHashMap)[pDigram] = pDigram;
	}

	pPriorityQueue->incrementQuantity(pDigram, refCount);
}

bool DigramCounter::isNonOverlapping(TreeNode * pNode, unsigned int index) {

	TreeNode * pChild = pNode->getChild(index);
	if (pChild == 0) {
		return false;
	}

	if (pNode->getSymbol() != pChild->getSymbol()) {
		return true;
	}

	TreeNode * pParent = pNode->getParent();
	if (pParent != 0
			&& pParent->getSymbol() == pNode->getSymbol()
			&& pNode->getIndex() == index
			&& pParent->isRegisteredOccurrence(pDigramHashMap, dagGrammar, index)) {

		return false;
	}

	TreeNode * pChildOfChild = pChild->getChild(index);
	if (pChildOfChild == 0) {
		return true;
	}

	if (pNode->getSymbol() == pChildOfChild->getSymbol()
			&& pChild->isRegisteredOccurrence(pDigramHashMap, dagGrammar, index)) {
		return false;
	}

	return true;
}


void DigramCounter::deregisterOccurrence(TreeNode * pNode, unsigned int index,
		Digram * pSkipDigram) {

	if (pNode->isRegisteredOccurrence(pDigramHashMap, dagGrammar, index)) {

		Digram * pDigram = Helper::getDigram(pDigramHashMap, dagGrammar, pNode, index);

		if (pDigram != pSkipDigram) {
			pPriorityQueue->decrementQuantity(pDigram, pNode->getTransitiveRefCount());
		}

		updateOccurrencesList(pDigram, pNode);

		pNode->setNextDigramOccurrence(index, 0);
		pNode->setPreviousDigramOccurrence(index, 0);
	} else {
#ifdef DEBUG
		if (pNode->getPreviousDigramOccurrence(index) != 0 || pNode->getNextDigramOccurrence(index)
				!= 0) {
			cout << "Fatal error when removing occurrences. Digram ("
					<< pNode->getSymbol()->getId() << ", "
					<< pNode->getChild(index)->getSymbol()->getId()
					<< ") has pointers to previous or next digram occurrence even it is not marked as an occurrence. Exiting ...";
			exit(1);
		}
#endif
	}
}

void DigramCounter::updateOccurrencesList(Digram * pDigram, TreeNode * pNode) {

	unsigned index = pDigram->getChildIndex();
	TreeNode * pPreviousOccurrence = pNode->getPreviousDigramOccurrence(index);
	TreeNode * pNextOccurrence = pNode->getNextDigramOccurrence(index);

	if (pPreviousOccurrence != 0) {
		pPreviousOccurrence->setNextDigramOccurrence(index, pNextOccurrence);
	} else {
		pDigram->setFirstOccurrence(pNextOccurrence);
	}

	if (pNextOccurrence != 0) {
		pNextOccurrence->setPreviousDigramOccurrence(index, pPreviousOccurrence);
	} else {
		pDigram->setLastOccurrence(pPreviousOccurrence);
	}
}
