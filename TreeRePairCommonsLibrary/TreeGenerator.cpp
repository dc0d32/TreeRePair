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
#include "TreeGenerator.h"

#include "Symbol.h"
#include "Production.h"
#include "TreeRePairConstants.h"
#include "Helper.h"

#define NUM_SYMBOLS 3

TreeGenerator::TreeGenerator(Grammar & dagGrammarPar) : dagGrammar(dagGrammarPar)
{
}

void TreeGenerator::createStartProduction(TreeNode * pNode) {

	Symbol * pStartSymbol = new Symbol();
	pStartSymbol->addSymbolToHashMaps(START_TAG);
	pStartSymbol->setRank(0);

	//Symbol * pRootSymbol = new Symbol(true, false);
	//Symbol::setTerminalSymbol(pRootSymbol->getId());
	//pRootSymbol->addSymbolToHashMaps("g");
	//pRootSymbol->setRank(1);

	//TreeNode * pRoot = new TreeNode(pRootSymbol);
	//pRoot->addChild(pNode);

	//Production * pProduction = new Production(pStartSymbol, pRoot);

	Production * pProduction = new Production(pStartSymbol, pNode);

	pProduction->incrementRefCount();
	pProduction->incrementTransitiveRefCount();
	dagGrammar.addProduction(pProduction);

}

DegeneratedTreeGenerator::DegeneratedTreeGenerator(Grammar & dagGrammarPar, unsigned int depth)
: TreeGenerator(dagGrammarPar) {

	pBinaryTreeInnerSymbol = new Symbol(true, true);
	Symbol::setTerminalSymbol(pBinaryTreeInnerSymbol->getId());
	pBinaryTreeInnerSymbol->addSymbolToHashMaps("f");
	pBinaryTreeInnerSymbol->setRank(2);

	string symbolNames [5] = { "a", "b", "c", "d", "e" };
	
	for (unsigned int i = 0; i < NUM_SYMBOLS; i++) {
		Symbol * pLeafSymbol = new Symbol(false, false);
		Symbol::setTerminalSymbol(pLeafSymbol->getId());
		pLeafSymbol->addSymbolToHashMaps(symbolNames[i]);
		pLeafSymbol->setRank(0);
		leafSymbols.push_back(pLeafSymbol);
	}

	TreeNode * pRoot = createTreeAux(0, depth);

	createStartProduction(pRoot);
}

TreeNode * DegeneratedTreeGenerator::createTreeAux(unsigned int curDepth, unsigned int maxDepth) {

	if (curDepth < maxDepth) {

		TreeNode * pNode = new TreeNode(pBinaryTreeInnerSymbol);
		pNode->setChild(0, new TreeNode(leafSymbols[curDepth % NUM_SYMBOLS]));
		pNode->setChild(1, createTreeAux(curDepth + 1, maxDepth));
		return pNode;

	} else {
		return new TreeNode(leafSymbols[curDepth % NUM_SYMBOLS]);
	}
}

BinaryTreeGenerator::BinaryTreeGenerator(Grammar & dagGrammarPar, int depth)
: TreeGenerator(dagGrammarPar) {

	pBinaryTreeInnerSymbol = new Symbol(true, true);
	Symbol::setTerminalSymbol(pBinaryTreeInnerSymbol->getId());
	pBinaryTreeInnerSymbol->addSymbolToHashMaps("f");
	pBinaryTreeInnerSymbol->setRank(2);
	
	pBinaryTreeLeftLeafSymbol = new Symbol(false, false);
	Symbol::setTerminalSymbol(pBinaryTreeLeftLeafSymbol->getId());
	pBinaryTreeLeftLeafSymbol->addSymbolToHashMaps("a");
	pBinaryTreeLeftLeafSymbol->setRank(0);

	pBinaryTreeRightLeafSymbol = new Symbol(false, false);
	Symbol::setTerminalSymbol(pBinaryTreeRightLeafSymbol->getId());
	pBinaryTreeRightLeafSymbol->addSymbolToHashMaps("b");
	pBinaryTreeRightLeafSymbol->setRank(0);

	TreeNode * pRoot = createBinaryTreeAux(0, depth);

	createStartProduction(pRoot);
}

TreeNode * BinaryTreeGenerator::createBinaryTreeAux(unsigned int curDepth, unsigned int maxDepth, bool left) {

	if (curDepth < maxDepth) {

		TreeNode * pNode = new TreeNode(pBinaryTreeInnerSymbol);
		pNode->setChild(0, createBinaryTreeAux(curDepth + 1, maxDepth, true));
		pNode->setChild(1, createBinaryTreeAux(curDepth + 1, maxDepth, false));
		return pNode;

	} else {

		if (left) {
			return new TreeNode(pBinaryTreeLeftLeafSymbol);
		} else {
			return new TreeNode(pBinaryTreeLeftLeafSymbol);
		}

	}
}

BinaryTreeGenerator2::BinaryTreeGenerator2(Grammar & dagGrammarPar, int depth)
: TreeGenerator(dagGrammarPar), counter(0) {

	pBinaryTreeInnerSymbol = new Symbol(true, true);
	Symbol::setTerminalSymbol(pBinaryTreeInnerSymbol->getId());
	pBinaryTreeInnerSymbol->addSymbolToHashMaps("f");
	pBinaryTreeInnerSymbol->setRank(2);

	TreeNode * pRoot = createBinaryTreeAux(0, depth);

	createStartProduction(pRoot);
}

TreeNode * BinaryTreeGenerator2::createBinaryTreeAux(unsigned int curDepth, unsigned int maxDepth) {

	if (curDepth < maxDepth) {

		TreeNode * pNode = new TreeNode(pBinaryTreeInnerSymbol);
		pNode->setChild(0, createBinaryTreeAux(curDepth + 1, maxDepth));
		pNode->setChild(1, createBinaryTreeAux(curDepth + 1, maxDepth));
		return pNode;

	} else {

		Symbol * pSymbol = new Symbol(false, false);
		Symbol::setTerminalSymbol(pSymbol->getId());
		string strC = "a" + Helper::stringify((size_t) counter++);
		pSymbol->addSymbolToHashMaps(strC);
		pSymbol->setRank(0);
		return new TreeNode(pSymbol);

	}
}
