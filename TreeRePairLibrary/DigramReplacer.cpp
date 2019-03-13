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
#include "DigramReplacer.h"

#include "TreeRePairMode.h"
#include "TextualGrammarWriter.h"
#include "TablePrinter.h"
#include "TreeDeleter.h"
#include "UniversalTreeTraverser.h"
#include "Production.h"

#include <sstream>
#include <iostream>

using namespace std;

DigramReplacer::DigramReplacer(DigramPriorityQueue * pPriorityQueue,
		DigramCounter * pDigramCounter, HASH_D_D * pDigramHashMap,
		Grammar & dagGrammar, Grammar & mainGrammar) :
	pPriorityQueue(pPriorityQueue), pDigramCounter(pDigramCounter),
					pDigramHashMap(pDigramHashMap), dagGrammar(dagGrammar),
					mainGrammar(mainGrammar), counter(1) {

	replaceDigrams();
}

DigramReplacer::~DigramReplacer(void) {
}

void DigramReplacer::replaceDigrams()
{
	Symbol * pSymbol = 0;
	Production * pProduction = 0;

	Digram * pCurrentDigram = pPriorityQueue->top();

	unsigned int lastQuantity = 0;
	if (pCurrentDigram != 0)
	{
		lastQuantity = pCurrentDigram->getNumberOfOccs();
	}

	TablePrinter tablePrinter;
	unsigned int * widths = 0;
	if (TreeRePairMode::getInstance()->isShowProgress())
	{
		widths = new unsigned int[3]; // { 7, 40, 20 };
		widths[0] = 10;
		widths[1] = 55;
		widths[2] = 11;

		tablePrinter.init(3, widths);
		tablePrinter.addColumnData("Step");
		tablePrinter.addColumnData("Introduced production");
		tablePrinter.addColumnData("Occurrences");
		tablePrinter.printRowAsHeader();
	}

	while (pCurrentDigram != 0) {

		lastQuantity = pCurrentDigram->getNumberOfOccs();

		if (pSymbol == 0) {
			pSymbol = createNewNonterminal(pCurrentDigram);
		}
		else {
			pSymbol->setRank(pCurrentDigram->getRankOfTreePattern());
		}

#ifdef DEBUG
		// checkOccurrences(pDigramHashMap, counter > 3);
		checkOccurrences(pDigramHashMap, false);
		// checkProductions();
		Helper::parseForInvalidSymbols();
#endif

		if (TreeRePairMode::getInstance()->isShowProgress())
		{
			tablePrinter.addColumnData(counter);
			ostringstream sstreamDigram;
			Digram::print(sstreamDigram, pCurrentDigram);
			string strProduction = Symbol::getElementTypeById(pSymbol->getId()) + ":" + Helper::stringify((size_t) pSymbol->getRank()) + " -> "
					+ sstreamDigram.str();
			tablePrinter.addColumnData(strProduction);
			tablePrinter.addColumnData(pCurrentDigram->getNumberOfOccs());
			tablePrinter.printRow();
		}

		pProduction = replaceDigramOccurrences(pCurrentDigram, pSymbol, counter);

		if (pProduction != 0)
		{
			mainGrammar.addProduction(pProduction);

			pPriorityQueue->setDigramQuantityToZero(pCurrentDigram);

			counter++;
			pSymbol = 0;
		}

		pCurrentDigram = pPriorityQueue->top();
	}
}

Production* DigramReplacer::replaceDigramOccurrences(Digram * pDigram, Symbol * pNonterminal,
		int digramCounter)
{
	TreeNode * pTemp = 0;

	OccurrenceFinder * pFinder = new OccurrenceFinder(pDigram);
	TreeNode * pOccurrence = pFinder->getNextOccurrence(0);

	Production * pProduction = Production::createProduction(pNonterminal, pDigram);

	unsigned int counter = 0;

	while (pOccurrence != 0)
	{
		counter += pOccurrence->getTransitiveRefCount();

#ifdef DEBUG
		Helper::isValidOccurrence(dagGrammar, pOccurrence, pDigram, counter);
#endif

		if (TreeRePairMode::getInstance()->isShowProgress())
		{
#ifndef DEBUG
			if (counter % 50 == 0 || counter
					== pDigram->getNumberOfOccs())
#else
			if (counter % 1 == 0 || counter
					== pDigram->getNumberOfOccs())
#endif
			{
				Helper::printProgress(counter, pDigram->getNumberOfOccs());
			}
		}

		pTemp = pOccurrence;
		TreeNode * pNodeToDelete = pTemp;
		TreeNode * pSecondNodeToDelete = 0;

		pSecondNodeToDelete = pOccurrence->getChild(pDigram->getChildIndex());
		// if (dagGrammar.isNonTerminal(pSecondNodeToDelete->getSymbol())) {
			// pSecondNodeToDelete = 0;
		// }

		pOccurrence = pFinder->getNextOccurrence(pOccurrence);

		replaceOccurrencesAux(pTemp, pDigram, pNonterminal);

		// delete replaced nodes
		delete pNodeToDelete;
		if (pSecondNodeToDelete != 0) {
			delete pSecondNodeToDelete;
		}
	}

	if (TreeRePairMode::getInstance()->isShowProgress())
	{
		cout << endl;
	}

	return pProduction;
}

Symbol * DigramReplacer::createNewNonterminal(Digram * pDigram) {

	Symbol * pNonterminal = new Symbol();
	pNonterminal->setRank(pDigram->getRankOfTreePattern());
	return pNonterminal;
}

TreeNode * DigramReplacer::replaceOccurrencesAux(TreeNode * pOccurrence,
		Digram * pDigram, Symbol * pNonTerminal) {

	// update existing digram occurrence queues
	deregisterAbsorbedOccurrences(pDigram, pOccurrence);

	// replace occurrence by node labeled by nonterminal
	TreeNode * pNewNode = updateTree(pNonTerminal, pDigram, pOccurrence);

	// create new digrams
	registerNewOccurrences(pDigram, pNewNode);

	return pNewNode;
}

Production * DigramReplacer::createDagProduction(Grammar & dagGrammar,
		TreeNode * pNode, unsigned int transitiveRefCount) {

	Symbol * pNonTerminalSymbol = new Symbol();
	pNonTerminalSymbol->setRank(0);

	Production * pProduction = new Production(pNonTerminalSymbol, pNode);
	pProduction->incrementRefCount();
	pProduction->setTransitiveRefCount(transitiveRefCount);

	dagGrammar.addProduction(pProduction);

	return pProduction;
}

TreeNode * DigramReplacer::updateTree(Symbol * pNonterminal, Digram * pDigram,
		TreeNode * pOccurrence) {

	TreeNode * pParent = pOccurrence->getParent();

	TreeNode * pNewNode = new TreeNode(pNonterminal);
	pNewNode->setTransitiveRefCount(pOccurrence->getTransitiveRefCount());

	TreeNode * pChildOfOccurrence = TreeNode::getChild(dagGrammar, pOccurrence,
			pDigram->getChildIndex());

	bool childIsDagNonTeminal = false;
	Production * pProduction = 0;
	TreeNode * pRightHandSide = 0;

	if (dagGrammar.isNonTerminal(
			pOccurrence->getChild(pDigram->getChildIndex())->getSymbol())) {
		childIsDagNonTeminal = true;
		pProduction = dagGrammar.getProductionBySymbol(pOccurrence->getChild(
						pDigram->getChildIndex())->getSymbol());
		pRightHandSide = pProduction->getRightHandSide();
	}

	if (childIsDagNonTeminal) {
		// only create new productions if refCount of pProduction > 0 after decrement
		if (pProduction->getRefCount() > 1) {
			for (unsigned int i = 0; i < pRightHandSide->getSymbol()->getRank(); i++) {
				if (!dagGrammar.isNonTerminal(pRightHandSide->getChild(i)->getSymbol())) {
					Production * pNewProduction = createDagProduction(dagGrammar, pRightHandSide->getChild(i),
							pProduction->getTransitiveRefCount());
					TreeNode * pRefNode = new TreeNode(pNewProduction->getLeftHandSide());
					pRightHandSide->setChild(i, pRefNode);
					pNewProduction->addRefNode(pRefNode);
				}
			}
		}
	}

	for (unsigned int i = 0; i < pDigram->getChildIndex(); i++) {
		TreeNode * pTempChild = pOccurrence->getChild(i);
		pNewNode->addChild(pTempChild);
	}

	for (unsigned int i = 0; i < pChildOfOccurrence->getSymbol()->getRank(); i++) {
		if (childIsDagNonTeminal && pProduction->getRefCount() > 1) {
			Symbol * pNonTerminalSymbol =
					pChildOfOccurrence->getChild(i)->getSymbol();
#ifdef DEBUG
			if (!dagGrammar.isNonTerminal(pNonTerminalSymbol)) {
				cout << "Fatal error when updating tree. Exiting ..." << endl;
				exit(1);
			}
#endif
			TreeNode * pRefNode = new TreeNode(pNonTerminalSymbol);
			pNewNode->addChild(pRefNode);

			Production * pCurrentProduction = dagGrammar.getProductionBySymbol(pNonTerminalSymbol);
			pCurrentProduction->addRefNode(pRefNode);
			pCurrentProduction->incrementRefCount();
		} else {
			TreeNode * pTempChild = pChildOfOccurrence->getChild(i);
			pNewNode->addChild(pTempChild);

		}
	}

	for (unsigned int i = pDigram->getChildIndex() + 1; i
			< pOccurrence->getSymbol()->getRank(); i++) {
		TreeNode * pTempChild = pOccurrence->getChild(i);
		pNewNode->addChild(pTempChild);
	}


	if (childIsDagNonTeminal) {

		pProduction->decrementRefCount(1);
		pProduction->removeRefNode(pOccurrence->getChild(pDigram->getChildIndex()));

		pProduction->decrementTransitiveRefCount(pOccurrence->getTransitiveRefCount());
		pRightHandSide->setTransitiveRefCount(pProduction->getTransitiveRefCount());

		if (pProduction->getRefCount() > 0) {
			for (unsigned int i = 0; i < pRightHandSide->getSymbol()->getRank(); i++) {
				pDigramCounter->registerOccurrence(pRightHandSide, i);
			}
		} else {
			dagGrammar.removeProduction(pProduction);
			delete pProduction;
		}
	}

	if (pParent != 0) {
		int index = pOccurrence->getIndex();
		pParent->setChild(index, pNewNode);
	} else {
		Production * pProduction = dagGrammar.getProductionByRightHandSide(pOccurrence);
		if (pProduction != 0) {
			dagGrammar.updateRightHandSide(pProduction, pNewNode);
		}
	}

	return pNewNode;
}

void DigramReplacer::registerNewOccurrences(Digram * pDigram, TreeNode * pNode) {

	TreeNode * pParent = pNode->getParent();

	// digrams of pNode and its children
	pDigramCounter->registerOccurrences(pNode);

	if (pParent != 0) {
		pParent->setPreviousDigramOccurrence(pNode->getIndex(), 0);
		pParent->setNextDigramOccurrence(pNode->getIndex(), 0);

		// digram of pNode with its parent node
		pDigramCounter->registerOccurrence(pParent, pNode->getIndex());
	} else {
		Production * pProduction = dagGrammar.getProductionByRightHandSide(pNode);

		HASH_N * pReferencingNodes = pProduction->getReferencingNodes();
		for (HASH_N::iterator it = pReferencingNodes->begin(); it
				!= pReferencingNodes->end(); it++) {

			TreeNode * pRefNode = *it;
			TreeNode * pParentOfRefNode = pRefNode->getParent();
			pDigramCounter->registerOccurrence(pParentOfRefNode,
												pRefNode->getIndex());
		}
	}
}

void DigramReplacer::deregisterAbsorbedOccurrences(Digram * pDigram,
		TreeNode * pOccurrence) {

	TreeNode * pParent = pOccurrence->getParent();

	TreeNode * pChildOfOccurrence = TreeNode::getChild(dagGrammar, pOccurrence,
			pDigram->getChildIndex());

	// digram of pOccurrence with its parent node
	if (pParent != 0) {
		pDigramCounter->deregisterOccurrence(pParent, pOccurrence->getIndex(), pDigram);
	} else {
		Production * pProduction = dagGrammar.getProductionByRightHandSide(pOccurrence);

		HASH_N * pReferencingNodes = pProduction->getReferencingNodes();
		for (HASH_N::iterator it = pReferencingNodes->begin(); it
				!= pReferencingNodes->end(); it++) {

			TreeNode * pRefNode = *it;
			TreeNode * pParentOfRefNode = pRefNode->getParent();
			pDigramCounter->deregisterOccurrence(pParentOfRefNode,
											pRefNode->getIndex(), 0);
		}
	}

	// digrams of pOccurrence and its children (besides pChildOfOccurrence)
	for (unsigned int i = 0; i < pOccurrence->getSymbol()->getRank(); i++) {
		if (i == pDigram->getChildIndex()) {
			continue;
		}

		pDigramCounter->deregisterOccurrence(pOccurrence, i, pDigram);
	}

	// digrams of pChildOfOccurrence and its children
	for (unsigned int i = 0; i < pChildOfOccurrence->getSymbol()->getRank(); i++) {
		pDigramCounter->deregisterOccurrence(pChildOfOccurrence, i, pDigram);
	}
}

#ifdef DEBUG
unsigned int DigramReplacer::countOccurrences(Digram * pDigram)
{

	OccurrenceFinder * pFinder = new OccurrenceFinder(pDigram);
	TreeNode * pOccurrence = pFinder->getNextOccurrence(0);
	int counter = 0;

	while (pOccurrence != 0)
	{
		unsigned int refCount = pOccurrence->getTransitiveRefCount();
		counter += refCount;
		pOccurrence = pFinder->getNextOccurrence(pOccurrence);
	}

	return counter;
}

void DigramReplacer::checkProductions() {

	for (unsigned int i = 0; i < dagGrammar.size(); i++) {
		Production * pProduction = dagGrammar[i];
		TreeNode * pRightHandSide = pProduction->getRightHandSide();
		if (dagGrammar.isNonTerminal(pRightHandSide->getSymbol())) {
			cout << "Fatal error when checking productions. Exiting ..." << endl;
			exit(1);
		}
	}
}

void DigramReplacer::checkOccurrences(HASH_D_D * pDigramHashMap, bool showDebugInfo)
{

	if (showDebugInfo)
	{
		cout << "Checking occurrences of all digrams for errors ";
	}

	HASH_D_D::iterator it;
	Digram * pDigram = 0;
	for (it = pDigramHashMap->begin(); it != pDigramHashMap->end(); ++it)
	{
		pDigram = it->second;

		checkOccurrencesOfDigram(pDigram, showDebugInfo);

		/*unsigned int actualNumberOfOccurrences = countOccurrences(pDigram);
		 if (actualNumberOfOccurrences != pDigram->getNumberOfOccs()) {
		 cout << "Error when checking " << *pDigram << ": Actual number of occurrences (" << actualNumberOfOccurrences << ") differs from estimated quantity (" << pDigram->getNumberOfOccs() ")." << endl;
		 }*/
	}

	if (showDebugInfo)
	{
		cout << endl;
	}
}

void DigramReplacer::checkOccurrencesOfDigram(Digram * pDigram, bool showDebugInfo)
{
	TreeNode * pOccurrence = pDigram->getFirstOccurrence();
	TreeNode * pLastOccurrence = 0;
	OccurrenceFinder * pFinder = OccurrenceFinder::getNewOccurrenceFinder(pDigram);

	if (showDebugInfo)
	{
		cout << ".";
	}

	//	cout << "checking: ";
	//	Digram::print(cout, pDigram);
	//	cout << endl;

	// is very slow when not in dag mode
//	for (unsigned int i = 0; i < dagGrammar.size(); i++) {
//		UniversalTreeTraverser treeTraverser(0, dagGrammar[i]->getRightHandSide());
//	}

	while (pOccurrence != 0)
	{
		if (pLastOccurrence == pOccurrence)
		{
			cout << "Error: Found loop when checking occurrences of ";
			Digram::print(cout, pDigram);
			cout << endl;
			exit(1);
		}

		pOccurrence->setVisited(false);

		pLastOccurrence = pOccurrence;
		pOccurrence = pFinder->getNextOccurrence(pOccurrence);
	}

	unsigned int counter = 0;

	// check for duplicate occurrences?

	pLastOccurrence = 0;
	while (pOccurrence != 0)
	{

		/*if (pDigram->getNumberOfOccs() == 21674) {
		 cout << ", " << pOccurrence << ", ";
		 }*/

		unsigned int refCount = pOccurrence->getTransitiveRefCount();
		counter += refCount;
		// ++counter;

		if (pOccurrence->isVisited())
		{
			cout << "Error when checking ";
			Digram::print(cout, pDigram);
			cout
					<< ": Occurrences list contains at least one duplicate entry. Exiting ..."
					<< endl;
			exit(1);
		}
//		else if (pDigram->getParentSymbol() == pDigram->getChildSymbol()
//				&& pOccurrence->getParent() != 0 && pOccurrence->getParent()->isVisited()
//				&& pOccurrence->getParent()->getSymbol() == pOccurrence->getSymbol()
//				&& pDigram->getChildIndex() == pOccurrence->getIndex()) {
//
//			if (pOccurrence->getParent()->getOccurrence(pDigram->getChildIndex())) {
//				cout << "is occ" << endl;
//			}
//
//			cout << "Error when checking ";
//			Digram::print(cout, pDigram);
//			cout
//					<< ": Occurrences list contains overlapping entries. Exiting ..."
//					<< endl;
//			exit(1);
//		}

		pOccurrence->setVisited(true);

		//if (counter % 1000 == 0)
		//	cout << "\r" << counter;

		checkOccurrenceWithDigram(pOccurrence, pDigram, counter);

		pLastOccurrence = pOccurrence;
		pOccurrence = pFinder->getNextOccurrence(pOccurrence);

		if (pOccurrence == pLastOccurrence)
		{
			cout << "Error when checking occurrence number " << counter
					<< " of ";
			Digram::print(cout, pDigram);
			cout
					<< ": Following occurrence pointer points again to this occurrence."
					<< endl;
			exit(1);
		}
	}

	if (counter > pDigram->getNumberOfOccs())
	{
		cout << "Error when checking ";
		Digram::print(cout, pDigram);
		cout << ": Actual number of occurrences (" << counter
				<< ") is bigger than assumed (" << pDigram->getNumberOfOccs() << ").";
		// cout << pDigram->getFirstOccurrence()->getSymbol()->getName();
		cout << endl;
		exit(1);
	}

	//	if (pDigram->getParentSymbol()->getName() == 116 && pDigram->getChildSymbol()->getName() == 116
	//		&& pDigram->getChildIndex() == 7)
	//	{
	//		cout << "debug: digram has " << counter << " occurrences." << endl;
	//	}

	// cout << "\r";
}

void DigramReplacer::checkOccurrenceWithDigram(TreeNode * pOccurrence,
		Digram * pDigram, int counter)
{
	TreeNode * pChildOfOccurrence = TreeNode::getChild(dagGrammar,
			pOccurrence, pDigram->getChildIndex());

	if (pChildOfOccurrence == 0)
	{
		cout << "Error when checking occurrence number " << counter
				<< " of ";
		Digram::print(cout, pDigram);
		cout << ": Occurrence ";
		TreeNode::print(cout, pOccurrence, false);
		cout << " has no child with index " << pDigram->getChildIndex()
				<< "." << endl;
		exit(1);
	}

	if (pOccurrence->getSymbol() != pDigram->getParentSymbol())
	{
		cout << "Error when checking occurrence number " << counter
				<< " of ";
		Digram::print(cout, pDigram);
		cout << ": Occurrence ";
		TreeNode::print(cout, pOccurrence, false);
		cout << " has symbol " << Symbol::getElementTypeById(
				pOccurrence->getSymbol()->getId());
		cout << " instead of " << Symbol::getElementTypeById(
				pDigram->getParentSymbol()->getId()) << "." << endl;
		exit(1);
	}

	if (pChildOfOccurrence->getSymbol() != pDigram->getChildSymbol())
	{
		cout << "Error when checking occurrence number " << counter
				<< " of ";
		Digram::print(cout, pDigram);
		cout << ": Child of occurrence ";
		TreeNode::print(cout, pOccurrence, false);
		cout << " has symbol " << Symbol::getElementTypeById(
				pChildOfOccurrence->getSymbol()->getId());
		cout << " instead of " << Symbol::getElementTypeById(
				pDigram->getChildSymbol()->getId()) << "." << endl;
		exit(1);
	}
}
#endif
