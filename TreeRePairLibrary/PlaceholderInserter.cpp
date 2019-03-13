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
#include "PlaceholderInserter.h"

#include "TreeRePairTypes.h"
#include "Production.h"

PlaceholderInserter::PlaceholderInserter(Grammar & dagGrammarPar) :
	TreeTraversal(0), dagGrammar(dagGrammarPar) // , justCount(false)
{
	HASH_ST_VS symbolsHashMap;

	for (unsigned int i = 0; i < Symbol::counter; i++) {
		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {
			HASH_ST_VS::iterator result = symbolsHashMap.find(Symbol::getElementTypeById(pSymbol->getId()));
			vector<Symbol*> * vec;
			if (result != symbolsHashMap.end()) {
				vec = result->second;
			} else {
				vec = new vector<Symbol*>();
				symbolsHashMap[Symbol::getElementTypeById(pSymbol->getId())] = vec;
			}
			vec->push_back(pSymbol);
		}
	}

	for (HASH_ST_VS::iterator it = symbolsHashMap.begin(); it != symbolsHashMap.end(); it++) {
		vector<Symbol*> * vec = it->second;

		if (vec->size() > 1) {
		// if (vec->size() == 2) {

			unsigned int maxRank = 0;

//			cout << it->first << endl;

			for (unsigned int j = 0; j < vec->size(); j++) {
				if (vec->at(j)->getRank() > maxRank) {
					maxRank = vec->at(j)->getRank();
					pNewSymbol = vec->at(j);
				} else if (maxRank == vec->at(j)->getRank()
						&& maxRank == 1) {
					// do nothing (maybe we should create a new symbol
					// with rank 2 and replace all others)
					maxRank = 1;
					pNewSymbol = 0;
				}
			}

			// if (pNewSymbol != 0 && pNewSymbol->getRank() == 2) {
			if (pNewSymbol != 0) {
				for (unsigned int i = 0; i < vec->size(); i++) {
					if (vec->at(i) == pNewSymbol) continue;
					pOldSymbol = vec->at(i);
					// cout << "From rank " << pOldSymbol->getRank() << " to rank " << pNewSymbol->getRank() << ": ";

//					transtiveCounter = 0;
//					counter = 0;
//
//					justCount = true;
//					for (unsigned int j = 0; j < dagGrammar.size(); j++) {
//						pRoot = dagGrammar[j]->getRightHandSide();
//						pCurrentRule = dagGrammar[j];
//						traverseTree();
//					}

//					if (counter < 20) {
//						justCount = false;
						for (unsigned int j = 0; j < dagGrammar.size(); j++) {
							pRoot = dagGrammar[j]->getRightHandSide();
							pCurrentProduction = dagGrammar[j];
							traverseTree();
						}
//					}

					// cout << transtiveCounter << " (" << counter << ") times." << endl;
				}
			}
		}
	}


}

PlaceholderInserter::~PlaceholderInserter(void)
{
}

void PlaceholderInserter::processNode(TreeNode * pParent, unsigned int index,
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

	if (!closing)
	{
		return;
	}

	if (pNode->getSymbol() == pOldSymbol) {

//		if (justCount) {
//			transtiveCounter += pCurrentProduction->getTransitiveRefCount();
//		} else {
			TreeNode * pNewNode = new TreeNode(pNewSymbol);
			TreeNode * pParent = pNode->getParent();

			if (pOldSymbol->getRank() == 0) {
				for (unsigned int i = 0; i < pNewSymbol->getRank(); i++) {
					pNewNode->addChild(new TreeNode(Symbol::getPlaceholderSymbol()));
				}
			} else if (pOldSymbol->getRank() == 1) {
				if (pOldSymbol->hasLeftChild()) {
					pNewNode->addChild(pNode->getChild(0));
				}
				pNewNode->addChild(new TreeNode(Symbol::getPlaceholderSymbol()));
				if (pOldSymbol->hasRightChild()) {
					pNewNode->addChild(pNode->getChild(0));
				}
			}

			if (pParent != 0) {
				pParent->setChild(index, pNewNode);
			} else {
				dagGrammar.updateRightHandSide(pCurrentProduction, pNewNode);
			}

			delete pNode;
//		}
	}
}

