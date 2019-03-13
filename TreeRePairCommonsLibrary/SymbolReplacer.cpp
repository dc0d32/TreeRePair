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
#include "SymbolReplacer.h"

#include "TreeRePairMode.h"
#include "TreeRePairTypes.h"
#include "Production.h"
#include "TreeCopier.h"
#include "RefCountUpdater.h"
#include "Helper.h"

SymbolReplacer::SymbolReplacer(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar) :
	TreeTraversal(0), dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar)
{
}

SymbolReplacer::~SymbolReplacer(void)
{
}

void SymbolReplacer::replaceNonterminals(SymbolReplacer & symbolReplacer, Grammar & mainGrammar)
{
	cout << "Replacing symbols (this may take some time) ... " << endl;

	unsigned int counter = 0;
	Production * pCurrentProduction = 0;
	unsigned int size = mainGrammar.size();

	for (unsigned int i = size; i > 0; i--) {

		++counter;

		pCurrentProduction = mainGrammar[i - 1];

		if (TreeRePairMode::getInstance()->isShowProgress())
		{
			cout << Helper::stringify((size_t) counter) << ": Replacing "
					<< pCurrentProduction->getLeftHandSide()->getName()
					<< " with ";
//			cout << "(";
//			cout << pCurrentReplacement->getDigramReplaced()->getParentSymbol()->getName()
//					<< ", ";
//			cout << pCurrentReplacement->getDigramReplaced()->getChildSymbol()->getName();
//			cout << ", ";
//			cout << pCurrentReplacement->getDigramReplaced()->getChildIndex();
//			cout << ") ";
//			cout << "(" << pCurrentReplacement->getNumberOfOccurrences() << " occurrences)";
			cout << endl;
		}
		else if (counter % 10 == 0 || counter == size)
		{
//			cout << "Replacing symbols (this may take some time) ... "
//					<< counter << "\r" << flush;
			Helper::printProgress(counter, mainGrammar.size());
		}

		symbolReplacer.replaceNonterminal(pCurrentProduction);
	}

	cout << endl;
}

#ifdef DEBUG
void SymbolReplacer::replaceNonterminal(Production * pProduction) {

	pCurrentProduction = pProduction;

	pRoot = dagGrammar[0]->getRightHandSide();
	traverseTree();
	processQueue();
}
#else
void SymbolReplacer::replaceNonterminal(Production * pProduction) {

	pCurrentProduction = pProduction;

	if (TreeRePairMode::getInstance()->getAlgMode() == TreeRePairMode::DECOMPRESS) {

		pRoot = dagGrammar[0]->getRightHandSide();
		traverseTree();
		processQueue();

	} else { // COMPRESS MODE

		HASH_N * refNodesHashSet = pProduction->getReferencingNodes();
		for (HASH_N::iterator it = refNodesHashSet->begin(); it != refNodesHashSet->end(); it++) {
			TreeNode * pNode = *it;
			occurrencesQueue.push_back(pNode);
		}

		processQueue();
	}

}
#endif

void SymbolReplacer::processNode(TreeNode * pNode, unsigned int index,
		int depth, bool closing)
{

	if (closing == true)
	{
		return;
	}

	if (index != UINT_MAX)
	{
		pNode = pNode->getChild(index);
	}

	if (pNode->getSymbol() == pCurrentProduction->getLeftHandSide())
	{
		pair<set<TreeNode *>::iterator, bool> ret = occurrencesSet.insert(
				pNode);
		if (ret.second == true)
		{
			occurrencesQueue.push_back(pNode);
		}
	}
}

void SymbolReplacer::processQueue()
{
	TreeNode * pNode = 0;
//	size_t counter = 0;
//	size_t size = occurrencesQueue.size();

	while (!occurrencesQueue.empty())
	{

//		counter++;

//		if (TreeRePairMode::getInstance()->isShowProgress())
//		{
//			if (counter % 50 == 0 || counter == size)
//			{
//				Helper::printProgress(0, counter, size);
//			}
//		}

		pNode = occurrencesQueue.back();
		occurrencesQueue.pop_back();

		TreeNode * pUpperNode = replaceTreeNode(pNode);

		// if (pNode->getId() == pRoot->getId())
		if (pNode->getParent() == 0)
		{
			// pRoot = pUpperNode;
			Production * pProduction = dagGrammar.getProductionByRightHandSide(pNode);
			if (pProduction != 0) {
				dagGrammar.updateRightHandSide(pProduction, pUpperNode);
			} else {
				pProduction = mainGrammar.getProductionByRightHandSide(pNode);
				mainGrammar.updateRightHandSide(pProduction, pUpperNode);
			}
		} else {
			TreeNode * pParent = pNode->getParent();
			pParent->setChild(pNode->getIndex(), pUpperNode);
		}

		delete pNode;
	}

	occurrencesSet.clear();

//	if (TreeRePairMode::getInstance()->isShowProgress())
//	{
//		cout << endl;
//	}
}

TreeNode * SymbolReplacer::replaceTreeNode(TreeNode * pNode)
{
	Production * pProduction = pCurrentProduction->copy();
	RefCountUpdater updater(mainGrammar, pProduction->getRightHandSide(), -1);
	Grammar::applyProduction(pNode, pProduction);
	TreeNode * pRightHandSide = pProduction->getRightHandSide();
	delete pProduction;
	return pRightHandSide;
}
