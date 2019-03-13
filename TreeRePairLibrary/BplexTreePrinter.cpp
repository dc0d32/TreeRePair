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
#include "BplexTreePrinter.h"

#include "BplexGrammarWriter.h"
#include "TreeRePairMode.h"
#include "Helper.h"
#include "Production.h"

BplexTreePrinter::BplexTreePrinter(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, TreeNode * pRootPar,
		ostream & outPar) :
	dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar),
	pRoot(pRootPar), out(outPar) {

	traverseTree();

	if (out.rdbuf() == cout.rdbuf()) {
		out << endl;
	}
}

BplexTreePrinter::~BplexTreePrinter() {
}

void BplexTreePrinter::processNode(TreeNode * pParent, unsigned int index,
		int depth, bool closing) {

	TreeNode * pNode;
	if (index != UINT_MAX) {
		pNode = pParent->getChild(index);
	} else {
		pNode = pParent;
	}

	Symbol * pSymbol = pNode->getSymbol();

	if (dagGrammar.isNonTerminal(pSymbol) && !closing) {
		Production * pProduction = dagGrammar.getProductionBySymbol(pNode->getSymbol());
		BplexTreePrinter treePrinter(dagGrammar, mainGrammar, pProduction->getRightHandSide(), out);
		if (pParent != pNode && (index < pParent->getSymbol()->getRank() - 1)) {
			out << ", ";
		}
	} else {
		if (pNode->getSymbol()->getRank() > 0) {
			if (!closing) {
				out << BplexGrammarWriter::getNameForSymbol(mainGrammar, pSymbol) << "(";

				if (Symbol::isTerminalSymbol(pSymbol->getId())
						&& pSymbol->getRank() < 2) {
					if (!pSymbol->hasLeftChild()) {
						out << "A0, ";
					}
				}
			} else {
				if (Symbol::isTerminalSymbol(pSymbol->getId())
						&& pSymbol->getRank() < 2) {
					if (!pSymbol->hasRightChild()) {
						out << ", A0";
					}
				}
				out << ")";
				if (pParent != pNode && pParent != 0 && (index
						< pParent->getSymbol()->getRank() - 1)) {
					out << ", ";
				}
			}
		} else {
			if (!closing) {
				out << BplexGrammarWriter::getNameForSymbol(mainGrammar, pSymbol);
				if (Symbol::isTerminalSymbol(pSymbol->getId())) {
					out << "(A0, A0)";
				}

				if (pParent != pNode && (index < pParent->getSymbol()->getRank() - 1)) {
					out << ", ";
//				} else if (pParent == pNode && dagGrammar[0]->getRightHandSide() != pNode) {
//					out << ", ";
				}
			}
		}
	}
}

// traverse: root, then children from left to right
void BplexTreePrinter::traverseTree() {
	TreeNode * pCurrentNode = pRoot;
	unsigned int index = 0;
	int depth = 0;

	processNode(pCurrentNode, UINT_MAX, depth, false);
	depth++;

	while (true) {
		if (pCurrentNode->getSymbol()->getRank() > index) {
			processNode(pCurrentNode, index, depth, false);
			pCurrentNode = pCurrentNode->getChild(index);
			depth++;
			index = 0;
		} else {
			if (pCurrentNode->getParent() != 0) {
				index = pCurrentNode->getIndex() + 1;
				TreeNode * pTemp = pCurrentNode;
				pCurrentNode = pCurrentNode->getParent();
				depth--;
				processNode(pCurrentNode, pTemp->getIndex(), depth, true);
			} else {
				break;
			}
		}
	}

	depth--;
	processNode(pCurrentNode, UINT_MAX, depth, true);
}
