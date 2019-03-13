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
#include "DefaultTreePrinter.h"

#include "TreeRePairMode.h"
#include "TreeRePairTypes.h"
#include "Production.h"

DefaultTreePrinter::DefaultTreePrinter(Grammar * grammarPar,
		ostream & out, bool rawModePar) : TreePrinter(out, rawModePar),
		grammar(grammarPar)
{
}

DefaultTreePrinter::~DefaultTreePrinter(void)
{
}

void DefaultTreePrinter::printTree(TreeNode * pNode) {
	TreePrinter::printTree(pNode);
}

void DefaultTreePrinter::processNode(TreeNode * pParent, unsigned int index, int depth, bool closing) {

	TreeNode * pNode;
	if (index != UINT_MAX) {
		pNode = pParent->getChild(index);
	} else {
		pNode = pParent;
	}

	// TODO implementation working for dag mode?
	if (grammar != 0 && grammar->isNonTerminal(pNode->getSymbol())) {
		Production * pProduction = grammar->getProductionBySymbol(pNode->getSymbol());
		DefaultTreePrinter treePrinter(grammar, *out, rawMode);
		treePrinter.printTree(pProduction->getRightHandSide());
	} else {
		if (pNode->getSymbol()->getRank() > 0) {
			if (!closing) {
				if (rawMode) {
					*out << " " << pNode->getSymbol()->getName();
				} else {
					*out << Symbol::getElementTypeById(pNode->getSymbol()->getId()) << "(";
				}
			} else {
				if (!rawMode) {
					*out << ")";
				}
			}
		} else {
			if (!closing) {
				if (rawMode) {
					*out << " " << pNode->getSymbol()->getName();
				} else {
					*out << Symbol::getElementTypeById(pNode->getSymbol()->getId());
				}
			}
		}
	}
}
