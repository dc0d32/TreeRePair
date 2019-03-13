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
#include "Grammar.h"

#include "TreeNode.h"
#include "TreeDeleter.h"
#include "Production.h"

Grammar::Grammar(void)
{
//#if !defined(__GNUC__)
//		ruleBySymbolIdHashMap.set_empty_key(UINT_MAX - 1);
//		ruleBySymbolIdHashMap.set_deleted_key(UINT_MAX);
//#endif

//		ruleByRightHandSideHashMap.set_empty_key(NULL);
//		ruleByRightHandSideHashMap.set_deleted_key(new TreeNode());
}

Grammar::~Grammar(void)
{
	TreeDeleter treeDeleter;
	Production * pProduction = 0;
	for (unsigned int i = 0; i < productions.size(); i++) {
		pProduction = productions[i];
		treeDeleter.deleteTree(pProduction->getRightHandSide());
		delete pProduction;
	}

	productions.clear();
}

Grammar * Grammar::copy() {
	Grammar * pGrammarCopy = new Grammar();
	for (unsigned int i = 0; i < size(); i++) {
		Production * pProd = productions[i];
		Production * pProdCopy = pProd->copy();
		pGrammarCopy->addProduction(pProdCopy);
	}
	return pGrammarCopy;
}

Production * Grammar::operator[] (const int index)
{
	if (index >= 0 && (unsigned int)index < productions.size()) {
		return productions[index];
	} else {
#ifdef DEBUG
		cout << "Fatal error when retrieving production from grammar. Exiting ..." << endl;
		exit(1);
#endif
		return 0;
	}
}

void Grammar::updateRightHandSide(Production * pProduction, TreeNode * pNewRightHandSide) {

	if (pProduction->getRightHandSide() != 0) {
		productionByRightHandSideHashMap.erase(pProduction->getRightHandSide());
	}

	pProduction->setRightHandSide(pNewRightHandSide);

	if (pNewRightHandSide != 0) {
		productionByRightHandSideHashMap[pNewRightHandSide] = pProduction;
	}
}

void Grammar::addProduction(Production * pProduction) {
	productions.push_back(pProduction);

	productionBySymbolIdHashMap[pProduction->getLeftHandSide()->getId()] = pProduction;

	if (pProduction->getRightHandSide() != 0) {
		productionByRightHandSideHashMap[pProduction->getRightHandSide()] = pProduction;
	}
}

void Grammar::removeProduction(vector<Production *>::iterator it) {

#ifdef DEBUG
	if (it < productions.begin() || it > productions.end()) {
		cout << "Fatal error when removing rule. Exiting ..." << endl;
		exit(1);
	}
#endif

	Production * pProduction = *it;

//		for (unsigned int i = 0; i < pRule->getRightHandSide()->getCurrentChildrenCount(); i++) {
//			if (pRule->getRightHandSide()->getOccurrence(i)) {
//				cout << "Error." << endl;
//				exit(1);
//			}
//		}

	if (pProduction->getRightHandSide() != 0) {
		productionByRightHandSideHashMap.erase(pProduction->getRightHandSide());
	}

	if (pProduction->getLeftHandSide() != 0) {
		productionBySymbolIdHashMap.erase(pProduction->getLeftHandSide()->getId());
	}

//		HASH_N_R::iterator result = ruleByRightHandSideHashMap.find(pRule->getRightHandSide());
//		if (result != ruleByRightHandSideHashMap.end()) {
//			ruleByRightHandSideHashMap.erase(result);
//		}

	productions.erase(it);
}

void Grammar::removeProduction(Production * pProduction) {

	// TODO inefficient
	for (vector<Production * >::iterator it = productions.begin(); it != productions.end(); it++) {
		Production * pTempProduction = *it;
		if (pTempProduction->getLeftHandSide()->getId() == pProduction->getLeftHandSide()->getId()) {
			removeProduction(it);
			return;
		}
	}

	cout << "Fatal error: Unable to remove production from grammar. Exiting ..." << endl;
	exit(1);
	return;
}


Production * Grammar::getProductionBySymbol(Symbol * pSymbol)
{
	return productionBySymbolIdHashMap[pSymbol->getId()];
}

Production * Grammar::getProductionByRightHandSide(TreeNode * pRightHandSide)
{
	return productionByRightHandSideHashMap[pRightHandSide];
}

bool Grammar::isNonTerminal(Symbol * pSymbol) {
	HASH_I_P::iterator result = productionBySymbolIdHashMap.find(pSymbol->getId());

	// mysterious: sometimes we get result != ruleBySymbolIdHashMap.end()
	// even if the key was not added to the hash map;
	// so we test against != 0 too
	if (result != productionBySymbolIdHashMap.end() && result->second != 0) {
		return true;
	} else {
		return false;
	}
}

void Grammar::applyProduction(TreeNode * pNodeToBeReplaced, Production * pProduction) {

	for (unsigned int i = 0; i < pNodeToBeReplaced->getSymbol()->getRank(); i++) {
		TreeNode * pParameterNode = pProduction->getParameter(i);
		TreeNode * pParent = pParameterNode->getParent();
		pParent->setChild(pParameterNode->getIndex(), pNodeToBeReplaced->getChild(i));
		delete pParameterNode;
	}
}
